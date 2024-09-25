#ifndef __GNET_CROSSBATTLEMANAGER_H
#define __GNET_CROSSBATTLEMANAGER_H

#include <map>
#include "itimer.h"

namespace GNET
{

enum
{
	NORMAL_BATTLE,
	INSTANCING_BATTLE,
	RAID_BATTLE,
	OPENBANQUET_BATTLE,
	CRSSVRTEAMS_BATTLE, // ���6v6
}; // ս���Ĵ����ͣ�ÿ�ִ��������о������ս�����ͣ������battlemanager.h��

struct CrossBattleInfo
{
	int _gsid;
	int _mapid;
	int _starttime;
	int _team;
	int _subbattle_type;

	CrossBattleInfo() : _gsid(0), _mapid(0), _starttime(0), _team(0), _subbattle_type(0)
	{
	}

	CrossBattleInfo(int gsid, int mapid, int starttime, int team, int subbattle_type) : _gsid(gsid), _mapid(mapid), _starttime(starttime), _team(team), _subbattle_type(subbattle_type)
	{
	}
};

struct FightingTag
{
	int _gsid;
	int _tag;
	int _team;
	int _endtime;
	int _subbattle_type;
	bool _isactive;
	time_t _active_time;

	FightingTag() : _gsid(0), _tag(0), _team(0), _endtime(0), _subbattle_type(0), _isactive(false), _active_time(0)
	{
	}

	FightingTag(int gsid, int tag, int team, int endtime, int subbattle_type, bool isactive = false) : _gsid(gsid), _tag(tag), _team(team), _endtime(endtime), _subbattle_type(subbattle_type), _isactive(isactive), _active_time(0)
	{
	}
};

class BattleEnter_Re;
class CrossBattleManager : public IntervalTimer::Observer
{
public:
	// ͬ�����ս��һ��ֻ�ܱ���һ����������battle_typeΪkey
	typedef std::map<int/*battle_type*/, CrossBattleInfo> CrossBattleMap;
	typedef std::map<int/*roleid*/, CrossBattleMap> CrossPlayerMap;

	static CrossBattleManager *GetInstance()
	{
		static CrossBattleManager instance;
		return &instance;
	}

	void JoinTeam(int battle, int subbattle, int gsid, int mapid, int roleid, int starttime, int team, int zoneid);
	void InsertFightingTag(int gsid, int roleid, int tag = 0, int team = 0, int endtime = 0, int subbattle_type = 0)
	{
		//LOG_TRACE("InsertFightingTag roleid=%d, gsid=%d, _role2tag.size=%d", roleid, gsid, _role2tag.size());
		_role2tag[roleid] = FightingTag(gsid, tag, team, endtime, subbattle_type);
	}
	const FightingTag *GetFightingTag(int roleid) const
	{
		std::map<int, FightingTag>::const_iterator it = _role2tag.find(roleid);
		return it == _role2tag.end() ? NULL : &(it->second);
	}
	void ActiveFightingTag(int roleid)
	{
		std::map<int, FightingTag>::iterator it = _role2tag.find(roleid);
		if(it == _role2tag.end())
		      return;

		LOG_TRACE("ActiveFightingTag roleid=%d", roleid);
		it->second._isactive = true;
		it->second._active_time = Timer::GetTime();
	}
	//�������������Ҫ������ܽ����ս����Ϣ������ս��ʧ�ܣ��뿪ս��������ԭ���������ս��
	void EraseFightingTag(int roleid)
	{
		LOG_TRACE("EraseFightingTag roleid=%d", roleid);
		_role2tag.erase(roleid);
	}
	void EnterBattle(int battle_type, BattleEnter_Re &res);
	
	bool IsRoleInTeam(int roleid, int battle_type) const
	{
		CrossPlayerMap::const_iterator cpit = _crossroles.find(roleid);
		if(cpit == _crossroles.end())
		      	return false;
		CrossBattleMap::const_iterator cbit = cpit->second.find(battle_type);
		return cbit == cpit->second.end() ? false : true;
	}

	const CrossBattleInfo* GetCrossBattleInfo(int roleid, int battle_type) const
	{
		CrossPlayerMap::const_iterator cpit = _crossroles.find(roleid);
		if(cpit == _crossroles.end())
		      	return NULL;
		CrossBattleMap::const_iterator cbit = cpit->second.find(battle_type);
		if(cbit == cpit->second.end())
			return NULL;
		return &cbit->second;
	}

	int GetSrcZoneID(int roleid) const
	{
		std::map<int, int>::const_iterator it = _role2zone.find(roleid);
		return it == _role2zone.end() ? 0 : it->second;
	}
	void InsertCrossPlayer(int battle, int subbattle, int gsid, int mapid, int roleid, int starttime, int team)
	{
		_crossroles[roleid][battle] = CrossBattleInfo(gsid, mapid, starttime, team, subbattle);
		
		//LOG_TRACE("InsertCrossPlayer battle=%d, roleid=%d, team=%d, _crossroles.size=%d", battle, roleid, team, _crossroles.size());
		CrossPlayerMap::iterator tmp = _crossroles.begin();
		/*
		for(;tmp != _crossroles.end();++tmp)
		{
			int troleid = tmp->first;
			LOG_TRACE("InsertCrossPlayer troleid=%d", troleid);
		}*/


	}
	// ����ս���ɹ�����Ҫ����ԭ�����������ȴ����֤������߹�����ͬ����ȴ�ͱ�������
	void SetRoleCoolDown(int battle_type, int subbattle_type, int roleid);
	void PlayerLogin(int roleid);
	void LeaveBattle(int battle_type, int roleid, int sid);
	void EraseCrossBattle(int roleid, int battle_type);
	void EnterBattle(int battle_type, int roleid);
	void EnterOpenBanquetBattle(int battle_type, int roleid);
	void Initialize()
	{
		IntervalTimer::Attach(this, 60*1000000/IntervalTimer::Resolution());
	}
	bool Update();
private:
	CrossBattleManager()
	{
	}
	~CrossBattleManager()
	{
	}

	CrossPlayerMap _crossroles; // ��¼��Щ��ɫ�����˿��ս����ͬ��/�ܾ�����ս�����������
	std::map<int/*roleid*/, int/*zoneid*/> _role2zone; // ��¼�������ս���Ľ�ɫ�������ĸ�������
	std::map<int/*roleid*/, FightingTag> _role2tag;  // ��¼��ɫ��Ҫ�μӵ�ս����Ϣ
};

}

#endif
