#ifndef __GNET_CRSSVRTEAMSBASE_MANAGER_H
#define __GNET_CRSSVRTEAMSBASE_MANAGER_H

#include "mapuser.h"
#include "gcrssvrteamsrole"
#include "gcrssvrteamsinfo"
#include "gcrssvrteamsdata"
#include "crssvrteamsrolelist_re.hpp" 
#include "crssvrteamsteambase_re.hpp"
#include "crssvrteamsrolenotify_re.hpp"
#include "synccrssvrteams.hrp"
#include "synccrssvrteamsrole.hrp"
#include "crssvrteamsinfo.h"
#include "gtoptable"

namespace GNET
{
	enum 
	{
		CT_STATUS_OPENING = 1, // ����һɲ��
		CT_STATUS_OPENED = 2, // ����������
		CT_STATUS_CLOSING = 3, // ����һɲ��
		CT_STATUS_CLOSED = 4, // ����������
	};
	class CrssvrTeamsBaseManager
	{
	public:
		//int mday[12] = { 31,28,31,30,31,30,31,31,30,31,30,31};
		//const static int SEASON_CLOSE_DAY = 26;
		//const static int SEASON_OPEN_DAY = 1;
		//const static int SEASON_REOPEN_DAY = 31;
		enum
		{
			UPDATE_INTERVAL    = 100, 
			STATUS_UPDATE_INTERVAL = 10,
		};

		typedef std::vector<GCrssvrTeamsRole> TCtRolesVec;
		struct STeamsTopPos
		{
			int pos;
			GTopItem topitem;
		};

		typedef std::map<int/*roleid*/, int/*pos*/> CrssvrTeamsTop; // ս�ӻ���������
		typedef std::map<int/*teamid*/, CrssvrTeams_Info*>  Map;
		typedef std::pair<int, int> PairPeriod;
		typedef std::map<PairPeriod, int> OpenPairPeriodsMap;
		typedef std::vector<PairPeriod> OpenPairPeriodsVec;
		typedef std::vector<int> OpenPeriodsVec;
	
	protected:
		Map teams; // ս�Ӽ���
		int servertype; //1:ԭ��,2���
		CrssvrTeamsTop lastweektop; // ����ս�ӻ�������
		int ct_act_weektoptime; //�����а���Ű�ʱ�� 
		CrssvrTeamsTop lastseasontop; // ������ս�ӻ�������
		int ct_act_seasontoptime; //����������Ű�ʱ��
		CrssvrTeamsTop dailytop; // ÿ��ս�ӻ�������
		int ct_act_dailytoptime; // �հ����ʱ��

		bool is_reward_week; // �Ƿ�����콱�ܽ�
		bool is_reward_season;// �Ƿ������������
		OpenPairPeriodsVec seasonperiod;// ������ʼ�ֶ�
		OpenPairPeriodsMap punishperiod;// �ͷ��ֶ�
		OpenPeriodsVec mon_seasonperiod;// �������·ݷֶ�
	
		time_t t_forged;// ����ʱ��
		int status;
		int t_status_update;
		int t_lastweektoptime; // ������߻��ָ���ʱ��
	public:
		CrssvrTeamsBaseManager() : /*open(false),*/ servertype(0), ct_act_weektoptime(0), ct_act_seasontoptime(0),ct_act_dailytoptime(0), 
			is_reward_week(false),is_reward_season(false),/*t_season_start(0),*/
			t_forged(0),status(0),t_status_update(0),t_lastweektoptime(0)
		{
		}  
		
		virtual ~CrssvrTeamsBaseManager() 
		{ 
			for(Map::iterator it=teams.begin(),ie=teams.end();it!=ie;++it)
				delete it->second;
			teams.clear();
		}

		bool Initialize();
		void Update(time_t now);
		int  GetWeekBeginTime(time_t now);
		int  GetTuesMorningTime(time_t now);
		void BuildLastWeekTopTime();
		bool IsOpenTime(time_t now, time_t & start_time);
		bool IsCloseTime(time_t now, time_t & close_time);
		int  CalSeasonInterval(time_t a, time_t b);
		//time_t GetSeasonCloseTime(time_t now);
		void BuildStatus();
		void OnOpened(time_t now); // ������ʼһɲ�ǵ���
		void OnClosed(time_t now); // ��������һɲ�ǵ���
		void CheckClose(time_t now);
		void CheckOpen(time_t now);	
		void UpdateStatus(time_t now);
		bool ClearTeamByRoleId(int roleid);
		bool ClearTeamByTeamId(int teamid);

		bool IsSeasonOpen() { return CT_STATUS_OPENED == status; }
		bool IsSeasonClose() { return CT_STATUS_CLOSED == status; }

		int GetPunishPeriod(int score)
		{
			OpenPairPeriodsMap::const_iterator it, ite = punishperiod.end();
			for (it = punishperiod.begin(); it != ite; ++it)
			{
				if (score >= it->first.first && score <= it->first.second)
				{
					//LOG_TRACE("GetPunishPeriod, score=%d, it->first.first=%d, it->first.second=%d, it->second=%d", score, it->first.first, it->first.second, it->second);
					return it->second;
				}
			}
			
			return 0;
		}

		time_t GetTime();
		void SetForgedTime(time_t forge);

		void SetServerType(int type){ servertype = type; }
		size_t Size() { return teams.size(); }
		int GetWeekTopTime() { return ct_act_weektoptime; } // �ܰ����ʱ��
		int GetLastWeekTopTime() { return t_lastweektoptime; }	// ���ܶ�����6�㣬����������������
		void SetWeekReward(bool reward) { is_reward_week = reward; }
		void SetSeasonReward(bool reward) { is_reward_season = reward; }
		bool IsWeekReward() { return is_reward_week; }
		bool IsSeasonReward() { return is_reward_season; }

		CrssvrTeams_Info * Find(int teamid)
		{
			Map::iterator it = teams.find(teamid);
			return (it == teams.end()) ? NULL : it->second;
		}

		int  UpdateRoleName(int roleid, const Octets new_name);
		int  UpdateRoleLvl(int roleid, int level);
		
		int  GetRoleList(int roleid, int teamid, CrssvrTeamsRoleList_Re& re);
		int  GetTeamBase(int roleid, int teamid, CrssvrTeamsTeamBase_Re& re);
		int  GetRoleNotify(int roleid, CrssvrTeamsRoleNotify_Re& re);

		bool GetCrssvrTeamsData(int teamid, GCrssvrTeamsData& info, TTeamRaidRoleBrief& rolebrief);
		bool GetCrssvrTeamsData(int teamid, const TeamRoleList& rolelist, GCrssvrTeamsData& info, TTeamRaidRoleBrief& rolebrief);

		int  GetMemberCount(int teamid);
		bool GetName(int teamid, Octets& name);
		int  GetTeamScore(int teamid);
		int  GetRoleTeamId(int roleid);
		int  GetRoleLastWeekMaxScore(int roleid);
		int  GetRoleJoinBattleTime(int roleid);
		int  GetRoleBattleTimes(int roleid);
		bool GetRoleDataGroup(int roleid, SCTDataGroup& data);
		bool SetRoleDataGroup(int roleid, int type, int value);
		int  GetGsPostScore(int roleid, int award_type, GCrssvrTeamsPostScore& re);
		int  GetRoleLeaveCooldown(int roleid);
		int  GetWeekTopPos(int roleid);
		int  GetSeasonTopPos(int roleid);
		int  GetDailyTopPos(int roleid, int teamid);
		int  GetDailyTopPosByRoleId(int roleid);

		bool FindMaster(int teamid, int &roleid);

		bool IsMaster(int teamid, int roleid);
		bool CheckRoleExsit(int teamid, int roleid);

		bool Broadcast(int teamid, Protocol& data, unsigned int &localsid);
		bool BroadcastGS(int teamid, Protocol& data, int & roleid);

		bool ValidName(const Octets& name);
		bool CheckJoinPolicy(int roleid, int rolelvl);

		// ���½�ɫ�����Ϣ�ص�
		void OnRolenameChange(int roleid, const Octets& newname);

		// ���а����
		void SendSeasonTopSave(int updatetime, const GTopTable & toptable, const CrssvrTeamsTop& pos_map); // ����������
		void LoadSeasonTop();// ����������
		virtual void OnTopUpdate(const GTopTable & toptable, int toptype) = 0;

		// �������
		int  CalcTeamScore(int src_teamid, int team_b_score, int result);
		int  CalcRoleScore(int src_teamid, std::vector<int>& team_roles, int team_b_score, int result);

		void GetPunishScore(int roleid, int week_times, int& score, bool isrole = false);
		void CalcRolePunish(int roleid); // ������˻��ֳͷ�
		void CalcTeamPunish(int roleid); // ���������ֳͷ�
		int  UpdateRoleScore(int roleid, int score);
		int  PunishRoleScore(int roleid, int score);

		virtual void Save2DB(){ assert(false); }

		// Debug�������
		void DebugClearCD(int roleid); // �������cd���뿪ս��cd
		void DebugRoleScore(int roleid, int score);// ������һ���Ϊxx
		void DebugRoleLastMaxScore(int roleid, int score);// ����������߻���
		void DebugRoleTeamScore(int roleid, int score); // ����ս�ӻ���Ϊxx 
		void DebugRolesScore(int roleid, int score); // ����ս�����г�Ա����Ϊxx
		void DebugTeamWin(int roleid, int times); // ����ս��ʤ������
		void DebugTeamPunish(int roleid, int times); // ����ս�ӳͷ�ʱ��ս�����κ�ʱ��
		void DebugRolePunish(int roleid, int times); // ����ս�ӳͷ�ʱ��ս�����κ�ʱ��
		void DebugRoleSeasonPos(int roleid, int pos); // ���������콱��λ��
		void DebugRoleWeekPos(int roleid, int pos); // ��������ս�ӻ��ְ�λ��


		// ��������ҳͷ�����ʹ��
		//void SyncRolePunishScore2DB(int roleid, int role_score, int times);
protected:
		void ConvertTopTableToMap(const GTopTable & toptable, int toptype); // ��toptableת����Map�洢, �콱ʱ��
		void ConvertDailyTableToMap(const GTopTable & toptable); // ��ս����teamid��pos��ʽ�洢�����ڱ���ս��λ��
		void UpdateTeamNewSeason(int teamid, CrssvrTeams_Info* detail);

		struct Packer
		{
			static void Unpack(TCtRolesVec& rolelist, int& zoneid, const Octets & src);
		};


	};

};
#endif

