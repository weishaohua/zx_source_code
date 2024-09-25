#ifndef __NETIO_H
#define __NETIO_H

#include <unistd.h>
#include <errno.h>
#include <string>

#if defined _REENTRANT
#ifndef __USE_UNIX98 
#define __USE_UNIX98
#endif
#include <pthread.h>
#endif

#include "octets.h"
#include "pollio.h"
#define USE_OLD_NETIO

namespace ONET
{

class NetIO;
class NetSession
{
public:
	virtual std::string Identification () const = 0;
	virtual void OnRecv(NetIO *, Octets &) { }
	virtual void OnSend(NetIO *, Octets &) { }
	virtual void OnOpen(NetIO *) { }
	virtual void OnClose(const NetIO *) { }
	virtual void OnAbort(const NetIO *) { }
	virtual	NetSession* Clone() const = 0;
	virtual void Destroy() { delete this; }
	virtual ~NetSession () { }
};

class NetIO : public PollIO
{
	Octets	ibuf;
	Octets	obuf;
	bool	closing;
	NetSession *session;
	int 	_event;
#if defined _REENTRANT
	pthread_rwlock_t locker_ibuf;
	pthread_rwlock_t locker_obuf;
#endif

	enum { MAXIOBUF = 65536 ,
		AUTO_FLUSH_SIZE = 256
		};

	void PollIn()
	{
#if defined _REENTRANT
		pthread_rwlock_wrlock(&locker_ibuf);
#endif
		ibuf.reserve(ibuf.size() + MAXIOBUF);
		int recv_bytes;
		do
		{
			if ((recv_bytes = read(fd, ibuf.end(), MAXIOBUF)) > 0)
			{
				ibuf.truncate((char*)ibuf.end() + recv_bytes);
				session->OnRecv(this, ibuf);
#if defined _REENTRANT
				pthread_rwlock_unlock(&locker_ibuf);
#endif
				return;
			}
		} while (recv_bytes == -1 && errno == EINTR);
		obuf.clear();
#if defined _REENTRANT
		pthread_rwlock_unlock(&locker_ibuf);
#endif
		closing = true;
	}

	void PollOut()
	{
		int send_bytes;
#if defined _REENTRANT
		pthread_rwlock_wrlock(&locker_obuf);
#endif
		do
		{
			if ((send_bytes = write(fd, obuf.begin(), obuf.size())) > 0)
			{
				obuf.erase(obuf.begin(), (char*)obuf.begin() + send_bytes);
				session->OnSend(this, obuf);
#if defined _REENTRANT
				pthread_rwlock_unlock(&locker_obuf);
#endif
				return;
			}
			if(send_bytes == -1 && errno == EAGAIN)
			{
#if defined _REENTRANT
				pthread_rwlock_unlock(&locker_obuf);
#endif
				return ;
			}
		} while (send_bytes == -1 && errno == EINTR);
		obuf.clear();
#if defined _REENTRANT
		pthread_rwlock_unlock(&locker_obuf);
#endif
		closing = true;
	}

	int UpdateEvent()
	{
		if (closing) return -1;
		int event = 0;
#if defined _REENTRANT
		pthread_rwlock_rdlock(&locker_obuf);
#endif
		size_t osize = obuf.size();
#if defined _REENTRANT
		pthread_rwlock_unlock(&locker_obuf);
		pthread_rwlock_rdlock(&locker_ibuf);
#endif
		size_t isize = ibuf.size();
#if defined _REENTRANT
		pthread_rwlock_unlock(&locker_ibuf);
#endif

		if (osize) event = POLLOUT;
		if (isize < MAXIOBUF)
			event |= POLLIN;
		else
		{
#if defined _REENTRANT
			pthread_rwlock_wrlock(&locker_ibuf);
#endif
			session->OnRecv(this, ibuf);
#if defined _REENTRANT
			pthread_rwlock_unlock(&locker_ibuf);
#endif
		}
		return _event = event;
	}

public:
	~NetIO()
	{
		session->OnClose(this);
		session->Destroy();
		close(fd);
#if defined _REENTRANT
		pthread_rwlock_destroy(&locker_obuf);
		pthread_rwlock_destroy(&locker_ibuf);
#endif
	}

	NetIO(int fd, NetSession *s) : PollIO(fd), closing(false), session(s),_event(0)
	{
#if defined _REENTRANT
		pthread_rwlock_init(&locker_ibuf, NULL);
		pthread_rwlock_init(&locker_obuf, NULL);
#endif
		session->OnOpen(this);
	}

	void Recv(Octets &data)
	{
		size_t size = data.size();
#if defined _REENTRANT
		pthread_rwlock_wrlock(&locker_ibuf);
#endif
		if (size >= ibuf.size())
		{
			ibuf.swap(data);
			ibuf.clear();
		} else {
			data.replace(ibuf.begin(), size);
			ibuf.erase(ibuf.begin(), (char*)ibuf.begin() + size);
		}
#if defined _REENTRANT
		pthread_rwlock_unlock(&locker_ibuf);
#endif
	}

	bool Send(const Octets &data)
	{
		if (obuf.size() >= MAXIOBUF) return false;
#if defined _REENTRANT
		pthread_rwlock_wrlock(&locker_obuf);
#if defined USE_OLD_NETIO
		obuf.insert(obuf.end(), data.begin(), data.end());
#else
		TestAndSend(data.begin(),data.size());
#endif
		pthread_rwlock_unlock(&locker_obuf);
#else
		obuf.insert(obuf.end(), data.begin(), data.end());
#endif
		return true;
	}
	
	bool Send(const void *data, size_t len) 
	{ 
		if (obuf.size() >= MAXIOBUF) return false;
#if defined _REENTRANT
		pthread_rwlock_wrlock(&locker_obuf);
#if defined USE_OLD_NETIO
		obuf.insert(obuf.end(), data, len); 
#else
		TestAndSend(data,len);
#endif
		/*
		 * �������˸Ľ�������ķ������ݣ�ʹ���ӳټ�С
		 * �����ǣ����������io�Ļ����޷�������ȷ�ķ�����
		 * ���⣬������̷��͵Ļ���Ҫע�������pollout ��ʧ�ܶ��ر����ӣ�Ҫ����һЩ����
		 */
		pthread_rwlock_unlock(&locker_obuf);
#else
		obuf.insert(obuf.end(), data, len); 
#endif
		return true;
	}

	void Close() { closing = true; }
protected:
	inline void TestAndSend(const void * data, size_t len)
	{
		obuf.insert(obuf.end(), data, len);
		if(!(_event & POLLOUT) && obuf.size() >= AUTO_FLUSH_SIZE)	//��С�����ݲ������̷���
		{
			//����ڵȴ����ͣ����Բ����̷���
			int send_bytes;
			size_t size = obuf.size();
			if ((send_bytes = write(fd, obuf.begin(),size)) > 0)
			{
				obuf.erase(obuf.begin(), (char*)obuf.begin() + send_bytes);
				session->OnSend(this, obuf);
				if((size_t)send_bytes < size)
				{
					_event |= POLLOUT;
				}
				return;
			}
			else
			{
				if( send_bytes == -1 && (errno == EINTR || errno == EAGAIN))
				{
					_event |= POLLOUT;
				}
				else
				{
					obuf.clear();
					closing = true;
				}
			}
		}
	}
};

};

#endif
