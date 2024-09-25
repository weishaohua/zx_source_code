#include "crossbattlemanager.hpp"
#include "battlemanager.h"
#include "centraldeliveryserver.hpp"
#include "centraldeliveryclient.hpp"
#include "gdeliveryserver.hpp"
#include "crossbattlejointeam.hpp"
#include "battlejoin_re.hpp"
#include "battlegetfield.hpp"
#include "battleleave_re.hpp"
#include "battleenter.hpp"
#include "battleenter_re.hpp"
#include "openbanquetenter.hpp"
#include "copenbanquetjoin.hpp"

namespace GNET
{

void CrossBattleManager::JoinTeam(int battle, int subbattle, int gsid, int mapid, int roleid, int starttime, int team, int zoneid)
{
	InsertCrossPlayer(battle, subbattle, gsid, mapid, roleid, starttime, team);
	_role2zone[roleid] = zoneid;
	// �����˿��ս����Ҫ֪ͨԭ��������������ԭ���ٱ���ս��
	// ����ԭ������ս������Ҫ֪ͨ�������Ϊԭ��ս��Logout�ͻ����
	CrossBattleJoinTeam jointeam(roleid, battle, subbattle, gsid, mapid, starttime, team);
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, jointeam); 
}

void CrossBattleManager::PlayerLogin(int roleid)
{

	LOG_TRACE("CrossBattleManager::PlayerLogin  roleid=%d, _crossroles.size=%d",  roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	/*
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EraseCrossBattle troleid=%d", troleid);
	}*/


	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	PlayerInfo *info = UserContainer::GetInstance().FindRole(roleid);
	if(info == NULL)
	      	return;

	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	bool iscentralds = dsm->IsCentralDS();
	int now = Timer::GetTime();
	int gsid = 0, mapid = 0, subbattle = 0, starttime = 0, team = 0, zoneid = 0, retcode = 0;
	CrossBattleMap &battles = cpit->second;
	for(CrossBattleMap::iterator cbit = battles.begin(); cbit != battles.end();)
	{
		// ��ɫ��½��֪ͨ�䶼�μ�����Щ���ս�����Ա�ͻ�����ʾս��ͼ��
		gsid = cbit->second._gsid;
		mapid = cbit->second._mapid;
		subbattle = cbit->second._subbattle_type;
		starttime = cbit->second._starttime;
		team = cbit->second._team;
		if(now > starttime) // ����Ѿ���ʼ���������ΪĳЩԭ��ʧ����
		{
			battles.erase(cbit++);
		}
		else
		{
			// �����ʱ��������߿�����ս����ť����ս����������֪ͨ
			if(!iscentralds || gsid == info->gameid) 
			{
				if(subbattle != OPENBANQUET_BATTLE) 
				{
					dsm->Send(info->linksid, BattleJoin_Re(ERR_BATTLE_CROSSBATTLE, roleid, gsid, mapid, subbattle, !iscentralds, team, starttime, info->localsid));
					LOG_TRACE("CrossBattleManager::PlayerLogin roleid=%d,gsid=%d,mapid=%d,subbattle=%d",roleid, gsid, mapid, subbattle);
					++cbit;
				}
				else
				{
					// ��ˮϯս��������ʱ�˳������Ҫ��ԭ����ʾ�Ŷ���Ϣ
					COpenBanquetJoin join_msg;
					join_msg.roleid = roleid;
					CentralDeliveryClient::GetInstance()->SendProtocol(join_msg);
					++cbit;
				}
			}
			else if(iscentralds && gsid != info->gameid) // ����绻������Ϊ�˳����ս��, ��ˮϯ���⣬��Ϊ��ˮϯ���Ի���
			{
				if(subbattle != OPENBANQUET_BATTLE) // ��ˮϯ���⣬��Ϊ��ˮϯ����֮�󣬿���ȥ������
				{
					battles.erase(cbit++);
					retcode = BattleManager::GetInstance()->LeaveTeam(roleid, gsid);
					zoneid = GetSrcZoneID(roleid);
					CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, BattleLeave_Re(retcode, roleid, 0, true));
				}
				else
				{
					// ��ˮϯҪ���Ƿ��ڱ���״̬��
					//OpenBanquetManager::GetInstance()->OnQueue(roleid, gsid, mapid);
					++cbit;
				}
			}
			else
			{
				++cbit;
			}
		}
	}
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}

void CrossBattleManager::LeaveBattle(int battle_type, int roleid, int sid)
{
	/*
	LOG_TRACE("LeaveBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("LeaveBattle troleid=%d", troleid);
	}*/

	_role2tag.erase(roleid);
	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	CrossBattleInfo &info = cbit->second;
	int retcode = 0;
	int zoneid = GetSrcZoneID(roleid);
	GDeliveryServer *dsm = GDeliveryServer::GetInstance();
	if(dsm->IsCentralDS())
	{
		retcode = BattleManager::GetInstance()->LeaveTeam(roleid, info._gsid);
		CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, BattleLeave_Re(retcode, roleid, 0, true));
	}
	dsm->Send(sid, BattleLeave_Re(retcode, roleid, 0, false));

	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}

void CrossBattleManager::EraseCrossBattle(int roleid, int battle_type)
{
	/*
	LOG_TRACE("EraseCrossBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EraseCrossBattle troleid=%d", troleid);
	}*/

	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
		return;
	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	     	return;
	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		//_role2zone.erase(roleid);
	}
}

void CrossBattleManager::EnterBattle(int battle_type, int roleid)
{
	/*
	LOG_TRACE("EnterBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EnterBattle troleid=%d", troleid);
	}*/

	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	CrossBattleInfo &info = cbit->second;
	CentralDeliveryClient::GetInstance()->SendProtocol(BattleEnter(roleid, info._gsid, info._mapid, battle_type, 0, true));
	InsertFightingTag(info._gsid, roleid); // Ϊ�˿��ʱ�ܽ���ָ����GS

	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}

void CrossBattleManager::EnterOpenBanquetBattle(int battle_type, int roleid)
{
	/*
	LOG_TRACE("EnterOpenBanquetBattle battle=%d, roleid=%d, _crossroles.size=%d", battle_type, roleid,  _crossroles.size());
	CrossPlayerMap::iterator tmp = _crossroles.begin();
	for(;tmp != _crossroles.end();++tmp)
	{
		int troleid = tmp->first;
		LOG_TRACE("EnterOpenBanquetBattle troleid=%d", troleid);
	}
	*/
	CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	CrossBattleInfo &info = cbit->second;
	CentralDeliveryClient::GetInstance()->SendProtocol(OpenBanquetEnter(roleid, info._gsid, info._mapid, 0, true));
	InsertFightingTag(info._gsid, roleid); // Ϊ�˿��ʱ�ܽ���ָ����GS

	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}
}


void CrossBattleManager::EnterBattle(int battle_type, BattleEnter_Re &res)
{
	int roleid = res.roleid;
	int zoneid = _role2zone[roleid];
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, res);
	/*CrossPlayerMap::iterator cpit = _crossroles.find(roleid);
	if(cpit == _crossroles.end())
	      	return;

	CrossBattleMap::iterator cbit = cpit->second.find(battle_type);
	if(cbit == cpit->second.end())
	      	return;

	res.battle_type = cbit->second._subbattle_type;
	cpit->second.erase(battle_type);
	if(_crossroles[roleid].size() == 0)
	{
		_crossroles.erase(roleid);
		_role2zone.erase(roleid);
	}*/
}

void CrossBattleManager::SetRoleCoolDown(int battle_type, int subbattle_type, int roleid)
{
	if(battle_type == NORMAL_BATTLE)
	{
		BattleManager::GetInstance()->SetRoleCoolDown(subbattle_type, roleid);
	}
}

bool CrossBattleManager::Update()
{
	time_t now = Timer::GetTime();
	std::map<int, FightingTag>::iterator it, ite = _role2tag.end();
	for (it = _role2tag.begin(); it != ite; ++it)
	{
		//active ֮�� �ͻ��˴����Ĺ����п���ʧ�� �˴�ͨ����ʱ�ָ�
		if (it->second._isactive && it->second._active_time > 0
				&& now-it->second._active_time > 60)
		{
			LOG_TRACE("inactive fighting tag, roleid %d tag %d", it->first,
					it->second._tag);
			it->second._isactive = false;
			it->second._active_time = 0;
		}
	}
	return true;
}
}
