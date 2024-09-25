#include "crssvrteamsmanager.h"
#include "maplinkserver.h"
#include "gproviderserver.hpp"
#include "centraldeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "matcher.h"
#include "localmacro.h"
#include "dbcrssvrteamsget.hrp"
#include "crosscrssvrteamsgettoptable.hrp"

namespace GNET
{
void CrssvrTeamsSaver::DoSave()
{
	if (!isWork)
		return;
	CrssvrTeamsManager::Map& teamsmap = CrssvrTeamsManager::Instance()->teams;
	CrssvrTeamsManager::Map::iterator it = teamsmap.upper_bound(curcor);
	if(it != teamsmap.end())
	{
		curcor = it->first;
		it->second->Save2DB(true);
		return;
	}
	isWork = false;
}


/////////////////////////////////////////////////////////////////////////////////
bool CrssvrTeamsManager::Initialize()
{
	CrssvrTeamsBaseManager::Initialize();
	IntervalTimer::Attach(this,500000/IntervalTimer::Resolution());
	SetServerType(ECT_SERVER);

	return true;
}

bool CrssvrTeamsManager::Update()
{
	time_t now = GetTime();
	CrssvrTeamsBaseManager::Update(now);
	// UpdateStatus����Ƶ��Ҫ��
	if(t_status_update < now)
	{
		UpdateStatus(now);
		UpdateTopTable();
		t_status_update = now + STATUS_UPDATE_INTERVAL;
	}

	Map::iterator it = teams.upper_bound(cursor);
	if(it==teams.end())
	{
		//LOG_TRACE("CrssvrTeamsManager : update, teamid=%d, mapsize=%d, cursor=%d, upper_bound", cursor, teams.size(), cursor);
		cursor = 0;
		return true;
	}
	
	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return true;

	cursor = detail->info.teamid;
	if(now - detail->updatetime > UPDATE_INTERVAL)
	{
		//LOG_TRACE("CrssvrTeamsManager : update, teamid=%d, mapsize=%d, cursor=%d, update", cursor, teams.size(), cursor);
		detail->Update();
		detail->updatetime = now;
		if(!detail->IsRoleSrcOnline() && !detail->IsRoleCrossOnline()) // ԭ���Ϳ���������ߣ��ſ����建��
		{
			ClearTeamByTeamId(cursor);
			return true;
		}

		UpdateTeamNewSeason(cursor, detail);
	}

	return true;
}

int CrssvrTeamsManager::OnLoad(int roleid, GCrssvrTeamsData& info, bool isUpdateStatus)
{
	Map::iterator it = teams.find(info.info.teamid);
	if (it != teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnLoad team had load, roleid=%d, teamid=%d", roleid, info.info.teamid);
		return ERR_SUCCESS;
	}
	
	CrssvrTeams_Info*  detail = new CrssvrTeams_Info(ECT_SERVER, info, this);
	teams[info.info.teamid] = detail;

	detail->SetLoadFinish(true);
	if(isUpdateStatus)
	{
		detail->UpdateRolesOnline(CRSSVRTEAMS_OFFLINE_STATUS);
		detail->UpdateRoleOnline(roleid, CRSSVRTEAMS_SRC_ONLINE_STATUS);
	}
	detail->SendTeamBaseRe(ERR_SUCCESS);
	detail->SendRoleListRe(ERR_SUCCESS);

	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnCreate(int roleid, GCrssvrTeamsData& info)
{
	//LOG_TRACE("CrssvrTeamsManager : OnCreate teamid=%d", info.info.teamid);
	CrssvrTeams_Info*  detail = new CrssvrTeams_Info(ECT_SERVER, info, this);
	teams[info.info.teamid] = detail;

	detail->UpdateRoleOnline(roleid, CRSSVRTEAMS_SRC_ONLINE_STATUS);
	detail->SetLoadFinish(true);
	detail->SendTeamBaseRe(ERR_SUCCESS);
	detail->SendRoleListRe(ERR_SUCCESS);
	
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnDeleteRole(int teamid, int roleid)
{
	LOG_TRACE("CrssvrTeamsManager : OnLeave teamid=%d, roleid=%d", teamid, roleid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnLeave teamid=%d, roleid=%d team not find", teamid, roleid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}	

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;
	
	detail->DeleteRole(roleid);
	detail->SendRoleListRe(ERR_SUCCESS);
	
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnJoin(int teamid, GCrssvrTeamsRole& roleinfo)
{
	LOG_TRACE("CrssvrTeamsManager : OnJoin teamid=%d, invitee=%d", teamid, roleinfo.roleid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnJoin teamid=%d, invitee=%d team not find", teamid, roleinfo.roleid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->Join(roleinfo);
	detail->UpdateRoleOnline(roleinfo.roleid, CRSSVRTEAMS_SRC_ONLINE_STATUS);
	detail->SendTeamBaseRe(ERR_SUCCESS);
	detail->SendRoleListRe(ERR_SUCCESS);

	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnDismiss(int teamid)
{
	LOG_TRACE("CrssvrTeamsManager : OnDelete teamid=%d", teamid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnDelete teamid=%d team not find", teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = ic->second;
	
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->Dismiss();
	delete detail;
	
	teams.erase(ic);
	
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnGradeChg(int teamid, int roleid, int level)
{
	LOG_TRACE("CrssvrTeamsManager : OnGradeChg roleid=%d, teamid=%d", roleid, teamid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnGradeChg roleid=%d, teamid=%d team not find", roleid, teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}
	
	CrssvrTeams_Info* detail = ic->second;

	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->UpdateRoleLvl(roleid, level);

	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnNameChange(int teamid, int changetime, const Octets & newname)
{
	LOG_TRACE("CrssvrTeamsManager : OnNameChange teamid=%d", teamid);

	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnNameChange teamid=%d team not find", teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}
	
	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->info.name = newname;
	detail->info.changenametime = GetTime();
	detail->SendTeamBaseRe(ERR_SUCCESS);
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnChgCaptain(int teamid, int master_old, int master_new)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return ERR_CRSSVRTEAMS_NOTEXIST;
	if(ic->second->GetMasterID()!=master_old)
		return ERR_CRSSVRTEAMS_NOT_MASTER;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	if(!detail->IsMemberExsit(master_new))
	{
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}

	detail->ChgCaptain(master_new);
	detail->SendTeamBaseRe(ERR_SUCCESS);

	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnLogin(int roleid, int teamid, bool isfromcentral)
{
	int role_teamid = (teamid != -1) ? teamid : GetRoleTeamId(roleid);
	if( role_teamid <= 0)
	{
		//LOG_TRACE("CrssvrTeamsManager::OnLogin, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}

	LOG_TRACE("CrssvrTeamsManager::OnLogin, roleid=%d, teamid=%d", roleid, role_teamid); 
	Map::iterator ic = teams.find(role_teamid);
	if(ic==teams.end())  // not found
	{
		SendTeamGetReq(roleid, role_teamid, isfromcentral);
		return ERR_SUCCESS;
	}
	else if (!ic->second->IsLoadFinish()) // found but in loading 
	{
		LOG_TRACE("CrssvrTeamsManager::OnLogin, roleid=%d, teamid=%d found but in loading", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_DATA_IN_LOADING;
	}
	else // found and load finish
	{
		// �ͷ�ֻ����ԭ����������һ���Ӷ��ڿ������ͷ����ˣ���������	
		CalcRolePunish(roleid);
		CalcTeamPunish(roleid);
		//LOG_TRACE("CrssvrTeamsManager::OnLogin, roleid=%d, teamid=%d found and load finish", roleid, role_teamid); 
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->UpdateRoleOnline(roleid, CRSSVRTEAMS_SRC_ONLINE_STATUS);
	detail->UpdateRoleLastMaxScore(roleid);

	return ERR_SUCCESS;
}

int CrssvrTeamsManager::OnLogout(int roleid, int level, unsigned char occupation)
{
	int role_teamid = GetRoleTeamId(roleid);
	if( role_teamid <= 0)
	{
		LOG_TRACE("CrssvrTeamsManager::OnLogin, roleid=%d, teamid=%d ERR_CRSSVRTEAMS_ROLE_NOT_IN", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}


	Map::iterator it = teams.find(role_teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : OnLogout roleid=%d, teamid=%d team not find", roleid, role_teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}

	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->UpdateRoleOnline(roleid, CRSSVRTEAMS_OFFLINE_STATUS);
	detail->UpdateRoleLvl(roleid, level);
	detail->UpdateRoleOccup(roleid, occupation);
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::CanDismiss(int teamid, int master)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return ERR_CRSSVRTEAMS_NOTEXIST;

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	if(detail->GetMasterID()!=master)
		return ERR_CRSSVRTEAMS_NOT_MASTER;
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::CanKick(int teamid, int master)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return ERR_CRSSVRTEAMS_NOTEXIST;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	if(detail->GetMasterID()!=master)
		return ERR_CRSSVRTEAMS_NOT_MASTER;
	return ERR_SUCCESS;
}


int CrssvrTeamsManager::CanChgCaptain(int teamid, int master_old, int master_new)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return ERR_CRSSVRTEAMS_NOTEXIST;
	if(ic->second->GetMasterID()!=master_old)
		return ERR_CRSSVRTEAMS_NOT_MASTER;
	
	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	if(!detail->IsMemberExsit(master_new))
	{
		return ERR_CRSSVRTEAMS_ROLE_NOT_IN;
	}

	return ERR_SUCCESS;
}

int CrssvrTeamsManager::CanChangeName(int teamid, int roleid, const Octets & newname)
{
	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
		return ERR_CRSSVRTEAMS_NOTEXIST;
	if (it->second->info.master != roleid)
		return ERR_FC_NO_PRIVILEGE;
	if (GetTime()-it->second->info.changenametime < 604800)
		return ERR_CRSSVRTEAMS_CHANGENAME_CD;
	if (newname == it->second->info.name)
		return ERR_CRSSVRTEAMS_CREATE_DUP;
	if (!ValidName(newname))
		return ERR_CRSSVRTEAMS_INVALIDNAME;
	return ERR_SUCCESS;
}

int CrssvrTeamsManager::CanRecruit(int teamid, int inviter, int invitee, int& invitee_linksid, int& invitee_localsid)
{
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
		return ERR_CRSSVRTEAMS_NOTEXIST;

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	// ���������Ƿ�����
	Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(invitee);
	if (NULL==pinfo )
	{
		LOG_TRACE("CanRecruit, invitee=%d not online", invitee);
		return ERR_CRSSVRTEAMS_OFFLINE;
	}
	invitee_linksid = pinfo->linksid;
	invitee_localsid = pinfo->localsid;
	// ���������Ƿ��Ѿ��ж���
	int invitee_teamid = GetRoleTeamId(invitee);
	if(invitee_teamid > 0)
	{
		LOG_TRACE("CanRecruit, invitee=%d,invitee_teamid=%d in team", invitee, invitee_teamid);
		return ERR_CRSSVRTEAMS_DB_INTEAMS;
	}

	// �������˵���ȴ��֤
	int invitee_leavetime = GetRoleLeaveCooldown(invitee);
	if (invitee_leavetime != -1 && GetTime() - invitee_leavetime <= 24 * 3600) // 1 day
	{
		LOG_TRACE("CanRecruit, now=%d, invitee_leavetime=%d", GetTime(), invitee_leavetime);
		return ERR_CRSSVRTEAMS_LEAVE_COOLDOWN;	
	}

	// ��������δ����
	if(!CheckJoinPolicy(invitee, pinfo->level))
	{
		LOG_TRACE("CanRecruit, invitee=%d role_lvl =%d", invitee, pinfo->level);
		return ERR_CRSSVRTEAMS_JOIN_NOT_REBORN;
	}

	// ����ְҵ����
	/*if(detail->GetOccupCount(pinfo->occupation) >= OCCUPATION_PLAYER_LIMIT)
	{
		LOG_TRACE("CanRecruit, invitee=%d occupt=%d, occupcount=%d", invitee, pinfo->occupation, detail->GetOccupCount(pinfo->occupation));
		return ERR_CRSSVRTEAMS_JOIN_OCCUP_CONT;
	}*/

	return ERR_SUCCESS;
}

int  CrssvrTeamsManager::CanCreate(int master)
{
	//LOG_TRACE("CanCreate, master=%d", master);

	// ���������Ƿ�����
	Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(master);
	if (NULL==pinfo )
	{
		LOG_TRACE("CanCreate, master=%d not online", master);
		return ERR_CRSSVRTEAMS_OFFLINE;
	}

	if(!CheckJoinPolicy(master, pinfo->level))
	{
		LOG_TRACE("CanCreate, master=%d role_lvl =%d", master, pinfo->level);
		return ERR_CRSSVRTEAMS_JOIN_NOT_REBORN;
	}

	return ERR_SUCCESS;
}

// ԭ��ʹ��
void CrssvrTeamsManager::UpdateSeasonPos(int updatetime, const CrssvrTeamsTop& pos_map)
{
	if(updatetime == 0)
	{
		// �����ݿ����ʱ�����Ϊ�հ�Ҫ�������󣬿��Ҳ��һ���У�
		return;
	}

	ct_act_seasontoptime = updatetime;
	lastseasontop = pos_map;
	LOG_TRACE("CrssvrTeamsManager::UpdateSeasonPos lastseasontop.size=%d, ct_act_seasontoptime=%d", lastseasontop.size(), ct_act_seasontoptime);
}

void CrssvrTeamsManager::OnTopUpdate(const GTopTable & toptable, int toptype)
{
	//�����Ƿ���������ʱ Load ���а����ݴ���
	//Ҳ�������ܶ������а����ʱ����
	if(toptype == CRSSVRTEAMS_WEEKLY_TOP_TYPE && toptable.time != ct_act_weektoptime)
	{
		LOG_TRACE("CrssvrTeamsManager : OnTopUpdate ConvertTopTableToMap toptype=%d, table.size=%d", toptype, toptable.maxsize);
		ConvertTopTableToMap(toptable, toptype);
	}
	else if(toptype == CRSSVRTEAMS_DAILY_TOP_TYPE && toptable.time != ct_act_dailytoptime)
	{
		LOG_TRACE("CrssvrTeamsManager : OnTopUpdate ConvertDailyTableToMap toptype=%d, table.size=%d", toptype, toptable.maxsize);
		// �հ�
		ConvertDailyTableToMap(toptable);
	}
	else if(toptype == CRSSVRTEAMS_SEASON_TOP_TYPE && toptable.time != ct_act_seasontoptime)
	{
		time_t season_close_time = 0;
		IsCloseTime(GetTime(), season_close_time);

		LOG_TRACE("CrssvrTeamsManager : OnTopUpdate IsSeasonClose=%d, ct_act_seasontoptime=%d, season_close_time=%d", 
				IsSeasonClose(), ct_act_seasontoptime, season_close_time);
		// ������
		if(IsSeasonClose() && /*ct_act_seasontoptime == 0 &&*/ toptable.time >= season_close_time)
		{
			// ����������� && ct_act_seasontoptime == 0����ʾ�������а�δ��ֵ
			LOG_TRACE("CrssvrTeamsManager : OnTopUpdate update the season top");
			ConvertTopTableToMap(toptable, CRSSVRTEAMS_SEASON_TOP_TYPE);
			SendSeasonTopSave(toptable.time, toptable, lastseasontop);
		}
		else
		{
			// ���������л����Ѿ������˱�������
			LOG_TRACE("CrssvrTeamsManager : OnTopUpdate neet't update the season top");
			return;
		}

	}
}

void CrssvrTeamsManager::Save2DB()
{
	saver.Reset();
	saver.Begin();
}

void CrssvrTeamsManager::OnSync(int teamid) { saver.OnSync(teamid);}

int CrssvrTeamsManager::SyncRoleInfo2Central(int roleid, int teamid)
{
	LOG_TRACE("CrssvrTeamsManager : SyncRoleInfo2Central roleid=%d, teamid=%d", roleid, teamid);
	Map::iterator it = teams.find(teamid);
	if (it == teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : SyncRoleInfo2Central roleid=%d, teamid=%d team not find", roleid, teamid);
		return ERR_CRSSVRTEAMS_NOTEXIST;
	}
	
	CrssvrTeams_Info* detail = it->second;
	if(!detail)
		return ERR_CRSSVRTEAMS_NOTEXIST;

	detail->SyncRoleInfo2Central(roleid);
	return ERR_SUCCESS;
}


bool CrssvrTeamsManager::UpdateRoleOnline(int teamid, int roleid, char status)
{
	LOG_TRACE("CrssvrTeamsManager : UpdateRoleOnline roleid=%d, teamid=%d, status=%d", roleid, teamid, status);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : UpdateRoleOnline roleid=%d, teamid=%d not find", roleid, teamid);
		return false;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	detail->UpdateRoleOnline(roleid, status);
	return true;
}

bool CrssvrTeamsManager::UpdateRolesOnline(int teamid, char status)
{
	LOG_TRACE("CrssvrTeamsManager : UpdateRolesOnline teamid=%d, status=%d", teamid, status);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : UpdateRolesOnline teamid=%d not find", teamid);
		return false;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	detail->UpdateRolesOnline(status);
	return true;
}

bool CrssvrTeamsManager::IsRoleCrossOnline(int teamid)
{
	//LOG_TRACE("CrssvrTeamsManager : IsRoleCrossOnline teamid=%d", teamid);
	Map::iterator ic = teams.find(teamid);
	if(ic==teams.end())
	{
		LOG_TRACE("CrssvrTeamsManager : IsRoleCrossOnline teamid=%d not find", teamid);
		return false;
	}

	CrssvrTeams_Info* detail = ic->second;
	if(!detail)
		return false;

	return detail->IsRoleCrossOnline();
}

void CrssvrTeamsManager::SendTeamGetReq(int roleid, int teamid, bool isfromcentral)
{
	time_t start_time = 0;
	time_t now = GetTime();
	IsOpenTime(now, start_time);
	DBCrssvrTeamsGet* rpc = (DBCrssvrTeamsGet*) Rpc::Call( RPC_DBCRSSVRTEAMSGET,TeamId(teamid,start_time));
	rpc->roleid = roleid;
	rpc->isfromcentral = isfromcentral;
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void CrssvrTeamsManager::UpdateTopTable()
{
	if(ct_act_weektoptime == 0) // �ܰ�û�е�
	{
		CrossCrssvrTeamsGetTopTableArg cross_arg;
		cross_arg.zoneid = GDeliveryServer::GetInstance()->zoneid;
		cross_arg.toptype = CRSSVRTEAMS_WEEKLY_TOP_TYPE;		
		CrossCrssvrTeamsGetTopTable* cross_rpc = (CrossCrssvrTeamsGetTopTable*) Rpc::Call( RPC_CROSSCRSSVRTEAMSGETTOPTABLE,cross_arg);
		CentralDeliveryClient::GetInstance()->SendProtocol(cross_rpc);
	}

	if(ct_act_dailytoptime == 0) // �հ�û�е�
	{
		CrossCrssvrTeamsGetTopTableArg cross_arg;
		cross_arg.zoneid = GDeliveryServer::GetInstance()->zoneid;
		cross_arg.toptype = CRSSVRTEAMS_DAILY_TOP_TYPE;		
		CrossCrssvrTeamsGetTopTable* cross_rpc = (CrossCrssvrTeamsGetTopTable*) Rpc::Call( RPC_CROSSCRSSVRTEAMSGETTOPTABLE,cross_arg);
		CentralDeliveryClient::GetInstance()->SendProtocol(cross_rpc);
	}

}

};

