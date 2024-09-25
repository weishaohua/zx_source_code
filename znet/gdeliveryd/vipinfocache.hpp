#ifndef __GNET_VIPInfoCache_HPP
#define __GNET_VIPInfoCache_HPP

#include <map.h>
#include "itimer.h"

namespace GNET
{

class VIPInfoCache : public IntervalTimer::Observer
{
public:
	friend class DebugCommand;
	static VIPInfoCache *GetInstance()
	{
		static VIPInfoCache instance;
		return &instance;
	}

	void Initialize();

	void UpdateVIPInfo(int userid, int currenttime, int starttime, int endtime, int viplevela, int status, int totalcash, int recenttotalcash, int infolack);

	void SendVIPInfoToGS(int userid, int roleid, int gsid) const;
	void SendVIPInfoToCentralDS(int userid) const;

	size_t GetOnlineVIPNum() const
	{
		return _vipinfos.size();
	}

	bool Update();
private:
	VIPInfoCache() : _now(0), _timebase(0), _seconds_perday(86400)
	{
	}

	~VIPInfoCache()
	{
	}

	bool IsAnotherPeriod()
	{
		_now = Timer::GetTime();
		// Ϊ��֤VIPϵͳ��Ϣ�Ѿ������꣬��Ϸ������ÿ��5����µ��ڵ�VIP��Ϣ
		return (_now - _timebase > _seconds_perday || _now < _timebase);
	}

	void SetTimeBase();
	void EraseOfflineVIPInfo();
	void GatherUserNeedToUpdate();
	void QueryVIPInfo();

	void SetIsOpen(int isopen)
	{
		_isopen = isopen;
	}

	enum
	{
		UPDATE_INTERVAL = 10,
		SECOND_TO_MICROSECOND = 1000000,
		QUERY_VIPINFO = 5,
		MAXNUM_PERUPDATE = 20,
	};

	struct VIPInfo
	{
		int currenttime;
		int starttime;
		int endtime;
		int viplevel;
		int deltatime; // delivery��VIP��������ʱ���
		int status;
		int totalcash;
		int recenttotalcash;
		int infolack;
	};

	typedef std::map<int/*userid*/, VIPInfo> VIPInfoMap;
	VIPInfoMap _vipinfos; // ��ǰ���ߵ�VIP��ҵ�VIP��Ϣ

	time_t _now;
	time_t _timebase; 	// ÿ���0�㣬���ڸ�������VIP��ҵ���Ϣ
	std::vector<int/*userid*/> _updating_vipinfos; 	// 0����Ҫ��AU������µ�VIP��ң����ڿ��ܸ��¶�Σ�_vipinfos�ڸ��¹����п��ܸı䣬��Ҫ�ȴ�����
	static bool _isopen; // VIP�����Ƿ���

	int _seconds_perday;
};

};
#endif
