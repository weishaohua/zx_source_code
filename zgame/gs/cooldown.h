#ifndef __ONLINE_GAME_GS_COOLDOWN_H__
#define __ONLINE_GAME_GS_COOLDOWN_H__

#include <sys/time.h>
#include "staticmap.h"
#include "world.h"

class cd_manager
{
	struct cd_ent
	{
		timeval t;
		int	interval;
		cd_ent(){}
		cd_ent(const timeval & t1, int i):t(t1),interval(i){}
	};
	typedef abase::static_multimap<int,cd_ent, abase::fast_alloc<> >  COOLDOWN_LIST;
	COOLDOWN_LIST _list;
public:
	cd_manager()
	{
		_list.reserve(5);
	}

	void Swap(cd_manager & cd)
	{
		_list.swap(cd._list);
	}

	bool TestCoolDown(int id)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it != _list.end())
		{
			timeval tv;
			g_timer.get_systime(tv);
			timeval & ct = it->second.t;
			if(ct.tv_sec > tv.tv_sec) 
				return false;
			else if(ct.tv_sec == tv.tv_sec && ct.tv_usec > tv.tv_usec) 
				return false;
			_list.erase(it);
		}
		return true;
	}


	bool SetCoolDown(int id, int cooldown_msec)
	{
		if(cooldown_msec <= 0) return true;
		//������ȴ�ĵ���ʱ��
		timeval tv;
		g_timer.get_systime(tv);
		int sec = cooldown_msec / 1000;
		int usec = (cooldown_msec % 1000)*1000;
		tv.tv_sec += sec;
		if((tv.tv_usec += usec) > 1000000)
		{
			tv.tv_sec += 1;
			tv.tv_usec -= 1000000;
		}

		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it == _list.end())
		{
			_list[id] = cd_ent(tv,cooldown_msec);
		}
		else
		{
			it->second.t = tv;
			it->second.interval = cooldown_msec;
		}
		return true;
	}

	int ReduceCoolDown(int id, int cooldown_msec, archive &ar)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it == _list.end())
		{
			return -1;
		}
		else
		{
			timeval tv;
			g_timer.get_systime(tv);
			
			int remain_time = (it->second.t.tv_sec - tv.tv_sec) * 1000 + (it->second.t.tv_usec - tv.tv_usec)/1000;
			remain_time = remain_time - cooldown_msec; 
			if(remain_time <= 0)
			{
				_list.erase(it);
				return 0;
			}

			int sec = remain_time / 1000;
			int usec = (remain_time % 1000) * 1000;
			tv.tv_sec += sec;
			if((tv.tv_usec += usec) > 1000000)
			{
				tv.tv_sec += 1;
				tv.tv_usec -= 1000000;
			}

			it->second.t = tv;

			ar << (short)1;
			ar << (unsigned short)id;
		        ar << remain_time;
			ar << it->second.interval;	
			return remain_time;
		}
	}

	bool ClrCoolDown(int id)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it == _list.end()) return false;

		_list.erase(it);
		return true;
	}


	void ClearExpiredCoolDown()
	{
		timeval tv;
		g_timer.get_systime(tv);
		COOLDOWN_LIST::iterator it = _list.end();
		while(it != _list.begin())
		{
			--it;
			timeval & ct = it->second.t;
			if(ct.tv_sec > tv.tv_sec)  
				continue;
			else if(ct.tv_sec == tv.tv_sec && ct.tv_usec > tv.tv_usec) 
				continue;
			_list.erase(it);
		}
	}

	void Save(archive & ar)
	{
		ar << -1;
		ar << (size_t)_list.size();
		
		COOLDOWN_LIST::iterator it = _list.begin();
		while(it != _list.end())
		{
			ar << it->first;
			ar.push_back(&(it->second),sizeof(cd_ent));
			++it;
		}
	}

	void Load(archive & ar)
	{
		int version;
		ar >> version;
		if(version != -1) return;		//ֱ������,���ֻ��Ӱ�����ݿ�洢
		size_t count;
		ar >> count;
		
		for(size_t i = 0; i < count; i++)
		{
			int id;
			cd_ent ent;
			ar >> id;
			ar.pop_back(&ent,sizeof(ent));
			_list[id] = ent;
		}
	}

	void SaveForClient(archive &ar)
	{	
		unsigned short count = _list.size();
		ar << count;

		timeval tv;
		g_timer.get_systime(tv);
		COOLDOWN_LIST::iterator it = _list.begin();
		for(;it != _list.end();++it)
		{
			int t = 0;
			if(it->second.t.tv_sec < tv.tv_sec)
			{
			}
			else if(it->second.t.tv_sec == tv.tv_sec && it->second.t.tv_usec < tv.tv_usec) 
			{
			}
			else 
			{
				t = (it->second.t.tv_sec - tv.tv_sec)*1000 + (it->second.t.tv_usec - tv.tv_usec)/1000;
			}
			ar << (unsigned short)(it->first);
			ar << t;
			ar << it->second.interval;
		}
	}

	void GetCoolDownList(abase::vector<int> & list)
	{
		COOLDOWN_LIST::iterator it = _list.begin();
		for(;it != _list.end();++it)
		{
			list.push_back(it->first);
		}
	}

};


#endif

