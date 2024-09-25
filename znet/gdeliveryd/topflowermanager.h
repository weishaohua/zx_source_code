#ifndef __GNET_TOPFLOWERMGR_H
#define __GNET_TOPFLOWERMGR_H 

#include <vector>
#include <map>

#include "gtopflowerdata"
#include "localmacro.h"
//#include <sys/time.h>
#include "timer.h"
#include "thread.h"
#include "flowertoprecord"

namespace GNET
{

#define TOPFLOWER_CLIENT_COUNT 500 // �ͻ��˰���
#define TOPFLOWER_ALL_COUNT 600 // �ܰ񵥰���,���100����ֹ��ɫ��ɾ
#define ONE_PAGE_COUNT 20 // ÿһҳ20��

struct STopFlowerID
{
	int count;// ��������
	int stamp;// ʱ���
};

inline bool operator > ( const STopFlowerID& lhs, const STopFlowerID& rhs )
{
	return lhs.count > rhs.count ? true : (lhs.count == rhs.count ? lhs.stamp < rhs.stamp : false);
}


class TopFlowerManager : public IntervalTimer::Observer
{
	typedef std::multimap<STopFlowerID, GTopFlowerData, std::greater<STopFlowerID> > TMultiMap;
	typedef std::map<int, GTopFlowerData> TMap; // roleid is key

	TopFlowerManager() : locker("topflowermanager::locker_top"),id_load(0),status(TOPFLOWER_NOTREADY){}  
public:
	Thread::RWLock & GetLocker() { return locker; }
public:
	Thread::RWLock locker;
	int id_load;
	ETopFlowerStatus status;

	TMultiMap m_RecvTopMap;// ���հ�
	TMultiMap m_SendTopMap; // ǰ500���ͻ���

	TMap m_RecvAllMap;// ���ջ� δ��񼯺�
	TMap m_SendAllMap;// δ��񵥼���
	
	~TopFlowerManager() {}

	inline ETopFlowerStatus GetStatus() { return status; }

	bool	Initialize()
	{
		IntervalTimer::Attach(this,(10*1000000)/IntervalTimer::Resolution());
		return true;
	}

	static TopFlowerManager* Instance() { static TopFlowerManager instance; return &instance;}
	bool InitTop(bool isRecv, std::vector<GTopFlowerData>& data);
	bool InitAddTop(bool isRecv, std::vector<GTopFlowerData>& data);
	void OnDBConnect();
	void Open();
	ETopFlowerStatus UpdateTop(bool isRecv, int roleid, int userid, int add_count, int gender, const Octets& rolename);
	bool GetTop(bool isRecv, int page, std::vector<GTopFlowerData>& data, int& all_count);
	bool GetRolePos(bool isRecv, int roleid, int& pos, int& count);
	bool IsGiftTake(bool isRecv, int roleid); // �Ƿ������ȡ����
	bool UpdateGiftTake(bool isRecv, int roleid); // ����Ѿ���ȡ����
	bool DelRoleTop(bool isRecv, int roleid);// ɾ����ɫ����
	bool OnRolenameChange(bool isRecv, int roleid , const Octets& oldname, const Octets& newname); // �����ֵ���	
	int GetTopForWeb(int type, std::vector<FlowerTopRecord> & top);	// ����վ�Ľӿ�

	bool Update();
	
	bool CheckGetTop();// ��ȡ���а���
	bool CheckOpenTop();// �ͺͽ��ܷ���
	bool CheckOpenGift();// ��ȡ�������ܷ���

protected:
	int _GetTopMinCount(bool isRecv); // ���а���Сֵ 
	GTopFlowerData* _GetKey(bool isRecv, int roleid); // �õ�key��Map��
	bool _UpdateTop(bool isRecv, GTopFlowerData& oldData, GTopFlowerData& newData); // ���°���̭������
	inline bool _CheckTime(struct tm& start, struct tm& end)
	{
		time_t curr_sec;
		time(&curr_sec);
		//struct tm dt;
		//localtime_r(&curr_sec, &dt);
		time_t start_sec = mktime(&start);
		time_t end_sec = mktime(&end);
		LOG_TRACE("Start_sec: %d, end_sec: %d, curr_sec: %d\n", (int)start_sec, (int)end_sec, (int)curr_sec);
		return curr_sec >= start_sec && curr_sec <= end_sec;

	}
	inline int _Local_MillSecond()
	{
		/*struct timeval _tstart;
		gettimeofday(&_tstart, NULL);
		return (int)(_tstart.tv_sec*1000 + _tstart.tv_usec/1000);*/
		time_t curr_sec;
		time(&curr_sec);
		//localtime(&curr_sec);
		return curr_sec;
	}
};

};
#endif

