#ifndef __GNET_OPEN_BANQUET_H
#define __GNET_OPEN_BANQUET_H

#include "mapuser.h"
#include "domaindataman.h"
#include "gopenbanquetinfo"
#include "openbanquetserver"
#include "battlefieldinfo"
#include "gopenbanquetinfo"
#include "battlefieldinfo"
#include "battlemanager.h"
#include "gopenbanquetroleinfo"

namespace GNET
{
	class PlayerInfo;

	enum EOpenBanquetFaction
	{
		EOBF_NONE = 0,
		EOBF_ATTACKER = 1,
		EOBF_DEFENDER = 2,
	};

	enum EOpenBanquetInfo
	{
		//EOBI_MAX_ROLES_NUM = 150,	// �����������
		//EOBI_MAX_HALF_ROLES_NUM = 75, 	// һ���������
		EOBI_MAX_WAITING = 100,		// �ȴ�����ʱ��s���ͻ�����30s�������ӳٺͼ�����Դ
		EOBI_MAX_DISCONNET_TIME = 300,  // ���߱���ʱ��

	};

	struct SOpenBanquetID
	{
		int gs_id;
		int map_tag;// ��ͼid��gs�е�world_tag
	};

	enum EOpenBanquetRoleStatus
	{
		EOBRS_NONE = 0, 
		EOBRS_QUEUE = 1, // �Ŷ���״̬
		EOBRS_WAITING = 2, // �Ѿ������ˣ��ȴ��������
		EOBRS_FIGHTING = 3, // ����ս��״̬
		EOBRS_DISCONNECT = 4, // ���ߵ���ʱ״̬
		EOBRS_ACCEPT = 5, // ��ҵ��ȷ��
	};

	struct SOpenBanquetRoleInfo
	{
		int roleid;
		int old_gs;
		int faction;
		Octets name;
		int src_zoneid;
		int status;
	};

	const static int ENTER_OPENBANQUT_REASON=3;//playerchangegs reason, normal = 0, vote kick =1, 2 = 6v6, 3 = openbanquet
	
	typedef std::map<int/*roleid*/, SOpenBanquetRoleInfo> TRolesInfoMap;
	typedef std::set<int> TRolesMap;
	typedef std::vector<int> TRolesVec;
	typedef std::deque<TRolesVec> TWaitQueue;
	typedef std::map<int/*roleid*/, int/*endtime*/> TTimeMap;
	typedef std::multimap<int/*disconnect time*/, SOpenBanquetRoleInfo> TDisMap;

	class OpenBanquetInfo
	{
	public:
		unsigned int EOBI_MAX_HALF_ROLES_NUM;
	public:
		SOpenBanquetID id;
		int sid;
		int map_id;
		int start_time; // ��ʼʱ��
		int end_time; // ����ʱ��
		int last_time; //����ʱ��
		int status; // ״̬
		int timeout;// �ӳ�ʱ��
		TRolesInfoMap roleinfo_map;// ���в�����һ�����Ϣ
		TRolesMap attack_map;
		TTimeMap attack_wait; // �������� �Ѿ������ˣ��ȴ��������

		TRolesMap defend_map;
		TTimeMap defend_wait; // ���ط� ���Ѿ������ˣ��ȴ��������

		TWaitQueue wait_queue;// �ŶӶ���
		TDisMap disconnect_map;// ���ߵ���ʱ����

		OpenBanquetInfo(SOpenBanquetID& tid, int tsid, const BattleFieldInfo& tinfo)
		{
			Reset();
			sid = tsid;
			id = tid;
			map_id = tinfo.map_id;
			start_time = 0;// ��ʱ
			end_time = 0;// ��ʼʱ����
			last_time = tinfo.fighting_time;
			EOBI_MAX_HALF_ROLES_NUM = 75;
			//attack_map.insert(28845992);
			//defend_map.insert(28845993);
		}

		void Reset()
		{
			start_time = 0;
			end_time = 0;
			status = 0;
			timeout= 0;
			roleinfo_map.clear();
			attack_map.clear();
			defend_map.clear();
			attack_wait.clear();
			defend_wait.clear();
			disconnect_map.clear();
		}
		
		SOpenBanquetRoleInfo* GetRoleInfo(int roleid)
		{
			TRolesInfoMap::iterator it = roleinfo_map.find(roleid);
			if(it == roleinfo_map.end())
			{
				return NULL;
			}

			return &it->second;
		}
		void ClearRoleInfo(int roleid)
		{
			roleinfo_map.erase(roleid);
		}

		void InsertRoleInfo(int roleid, SOpenBanquetRoleInfo& info) 
		{ 
			roleinfo_map[roleid] = info; 
		}

		// �������״̬
		bool SetRoleStatus(int roleid, int status)
		{
			TRolesInfoMap::iterator it = roleinfo_map.find(roleid);
			if(it == roleinfo_map.end())
			{
				return false;
			}

			it->second.status = status;

			return true;
		}
		
		// �õ����״̬
		int GetRoleStatus(int roleid)
		{
			TRolesInfoMap::iterator it = roleinfo_map.find(roleid);
			if(it == roleinfo_map.end())
			{
				return -1;
			}

			return it->second.status;
		}

		bool IsRolesFull();
		int  GetRolesCount();
		bool FindRole(int roleid, char& faction);
		void SetTimeOut(int roleid, char faction, time_t time);
		void RemoveTimeOut(int roleid);

		void UpdateTimeOut(time_t time);
		bool CheckFreeInfo(int& free_faction, int roles_num);
		bool PutWaitMap(int free_faction, TRolesVec& roles, time_t now);
		bool PushWaitQueue(TRolesVec& roles, int gsid);
		bool PopWaitQueue();
		void WaitMapClear(time_t now); // ɾ���ȴ�������е�ʱ�����
		void EraseCrossBattle(int roleid);
		void BattleMapClear(int roleid); // ս�������ɾ��
		void Leave(int roleid);
		bool GetEnterFaction(int roleid, char& tfaction);
		bool Enter(int roleid);
		void Login(int roleid);
		void QueueCancel(int roleid, char isqueue);
		int  GetQueueCount();
		int  GetQueuePos(int roleid);
		bool GetRoleField(std::vector<GOpenBanquetRoleInfo>& attack_field, std::vector<GOpenBanquetRoleInfo>& defend_field);
		void SendJoinWaitMsg(TRolesVec& roles, int free_faction);
		void SendJoinMsg(int roleid, int jointype);
		void SendJoinMsg(TRolesVec& roles, int jointype);
		void SendChangeGsMsg(int roleid);
		void SendRoleDelMsg(int roleid, char faction);
	};
	
	inline bool operator == ( const SOpenBanquetID& lhs, const SOpenBanquetID& rhs )
	{
			return lhs.gs_id == rhs.gs_id && lhs.map_tag == rhs.map_tag;
	}

	inline bool operator < ( const SOpenBanquetID& lhs, const SOpenBanquetID& rhs )
	{
		return lhs.gs_id < rhs.gs_id ? true : (lhs.gs_id == rhs.gs_id ? lhs.map_tag < rhs.map_tag : false);
	}


	class OpenBanquetManager : public IntervalTimer::Observer
	{
	public:
		enum OPENBANQUET_STATUS //��������״̬
		{
			OBS_INIT	  = 0x0001,  // ��ʼ����ʼ
			OBS_WAITING	  = 0x0002,  // waiting
			OBS_FIGHTING      = 0x0004,  // ս��������
			OBS_SENDSTART     = 0x0008,  // ��ʼ�����Ѿ�����
			OBS_CANCEL        = 0x0010,  // ȡ��
			OBS_OPEN	  = 0x0020,  // ս����
			OBS_CLOSE	  = 0x0040,  // ս���ر�
		};
		enum
		{
			UPDATE_INTERVAL   		= 2, 		// ��tick���ʱ��
			STATUS_UPDATE_INTERVAL   	= 35, 		// ״̬���¼��ʱ��
			SENDSTART_TIMEOUT		= 60,		// BattleStart Msg timeout
			CLOSE_CHECK_ROLE_SIZE		= 140,  	// 140��
			BATTLE_START_TIME 		= 68400, 	// 19:00 ÿ��ս������ʱ��
			BATTLE_END_TIME 		= 75600,	// 21:00 ÿ��ս���ر�ʱ��
			BATTLE_DELEY_TIME		= 1800,    	// ս���ӳ�ʱ�� 30����
			BATTLE_CLOSE_CHECK_TIME 	= 600, 		// �ر�ǰ10����ս���ڻ���140�ˣ�������ʱ30����
			BATTLE_DELEY_MAX_TIME 		= 86300, 	// ս������ӵ�24��,��1����
			BATTLE_LVL_NEED			= 135,		// �ȼ�Ҫ��
		};

		typedef std::map<SOpenBanquetID, OpenBanquetInfo> ServerMap;
		typedef std::map<int/*roleid*/, int/*�������*/> TRolesPerdayMap;// ÿ��������Ƽ�¼
		typedef std::map<int/*roleid*/, SOpenBanquetID/*��ˮϯID*/> TRolesServerMap;// ��ұ�������ˮϯ��¼


	public:
		time_t BattleBeginTime() { return t_base + t_start;}
		time_t BattleEndTime() { return t_base + t_close; }
		time_t BattleCloseCheck() { return t_base + t_close_check; }
		time_t BidFinalEndTime() { return t_base + BATTLE_DELEY_MAX_TIME;}
	private:	
		ServerMap servers;
		TRolesPerdayMap perday_map; // ÿ��������Ƽ�¼
		TRolesServerMap roles_svr;// ��ұ�������ˮϯ��¼
		//int status;
		time_t t_base; 
		time_t t_forged;// ����ʱ��
		time_t t_status_update; // ״̬����ʱ��

		bool b_open;// ս���Ƿ���
		time_t t_start;// ս������ʱ��
		time_t t_close_check;// ս���رռ��ʱ��
		time_t t_close;// ս���ر�ʱ��

		OpenBanquetManager()
		{
			//status = 0;
			t_base = 0;
			t_forged = 0;
			t_status_update = 0;
			b_open = false;
			t_start = BATTLE_START_TIME;
			t_close_check = BATTLE_END_TIME - BATTLE_CLOSE_CHECK_TIME;
			t_close = BATTLE_END_TIME;
		}  
	public:
		~OpenBanquetManager() { }
		void ReSet()
		{
			t_start = BATTLE_START_TIME;
			t_close_check = BATTLE_END_TIME - BATTLE_CLOSE_CHECK_TIME;
			t_close = BATTLE_END_TIME;

			perday_map.clear();
			roles_svr.clear();
		}
		static OpenBanquetManager* GetInstance() { static OpenBanquetManager instance; return &instance;}
		int GetFieldType() { return BATTLEFIELD_TYPE_FLOW_CRSSVR; }

		void SetMaxHalfRoleNum(int roleid, unsigned int num);
		void SetHalfRoleNum(int roleid);

		void InsertRolesServer(int roleid, SOpenBanquetID& id)
		{
			roles_svr.insert(std::make_pair(roleid, id));
		}
		SOpenBanquetID* FindRolesServer(int roleid)
		{
			TRolesServerMap::iterator it = roles_svr.find(roleid);
			if(it == roles_svr.end())
			{
				return NULL;
			}

			return &it->second;
		}
		void ClearRolesServer(int roleid)
		{
			roles_svr.erase(roleid);
		}

		void InsertPerdayLimit(int roleid) 
		{ 
			//LOG_TRACE("InsertPerdayLimit roleid=%d, perday_map.size=%d", roleid, perday_map.size());
			TRolesPerdayMap::iterator it = perday_map.find(roleid);
			if(it != perday_map.end())
			{
				it->second++;
			}
			else
			{
				perday_map.insert(std::make_pair(roleid, 1)); 
			}
		}
		bool IsPerdayLimit(int roleid, int max_count)
		{
			//LOG_TRACE("IsPerdayLimit roleid=%d, perday_map=%d", roleid, perday_map.size());
			TRolesPerdayMap::iterator it = perday_map.find(roleid);
			if(it != perday_map.end())
			{
				if(it->second >= max_count)
				{
					return false;
				}
			}
			return true;
		}
		void ClearPerdayLimit(int roleid) { perday_map.erase(roleid); }
		
		OpenBanquetInfo* FindFreeServer()
		{
			SOpenBanquetID pos;
			int min = INT_MAX;
			ServerMap::iterator it=servers.begin();
			ServerMap::iterator ite=servers.end();
			for (; it!=ite; ++it)
			{
				if(it->second.GetQueueCount() < min)
				{
					pos = it->first;
				}
			}

			it = servers.find(pos);
			if(it != ite)
			{
				return &it->second;
			}

			return NULL;
		}

		OpenBanquetInfo* FindMapInfo(int gs_id, int map_tag)
		{	
			SOpenBanquetID uid;
			uid.gs_id = gs_id;
			uid.map_tag = map_tag;

			ServerMap::iterator it = servers.find(uid);
			if (it == servers.end())
				return NULL;
			else
				return &(it->second);
		}

		bool IsOpenBanquetServer(int gs_id, int map_tag)
		{
			//LOG_TRACE("IsOpenBanquetServer gs_id=%d, map_tag=%d, ",  gs_id, map_tag);
			SOpenBanquetID uid;
			uid.gs_id = gs_id;
			uid.map_tag = map_tag;

			if (gs_id == 0 || gs_id == -1)
				return false;
			ServerMap::const_iterator it = servers.find(uid);
			if (it == servers.end())
				return false;
			else if (it->second.id == uid)
			{

				//LOG_TRACE("IsOpenBanquetServer id.gs_id=%d, id.map_tag=%d ",  it->second.id.gs_id, it->second.id.map_tag);
				return true;
			}
			else
			{

				//LOG_TRACE("IsOpenBanquetServer id.gs_id=%d, id.map_tag=%d ",  it->second.id.gs_id, it->second.id.map_tag);
				return false;
			}
		}
		
		bool CheckJoinPolicy(int role_level, int role_reborn);
		void StartAnnounce();
		bool Initialize();
		bool RegisterServer(int sid, int gs_id, const std::vector<BattleFieldInfo>& info);
		time_t GetTime();
		int  GetQueuePos(int roleid, int gs_id, int map_tag);// �õ��Ŷ�����
		bool GetRoleField(int roleid, int gs_id, int map_tag, int localsid, std::vector<GOpenBanquetRoleInfo>& attack_field, std::vector<GOpenBanquetRoleInfo>& defend_field);
		void SetForgedTime(time_t forge);
		void SetStartTime(time_t start);
		time_t UpdateTime();
		void UpdateOpenBanquet(time_t now);
		void UpdateStatus(time_t now);
		bool Update();
	
		void SendNotifyMsg(int roleid, int result);
		int  OnJoin(std::vector<int>& roles, char is_team, int gsid);//����
		void OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map);
		void OnLogout(int roleid, int gsid, int map);
		void OnRealEnter(int roleid, int new_gs, int new_map);
		void OnQueue(int roleid, int new_gs, int new_map);

		void LogOpenBanquetList();
		int  OnOpenBanquetEnd(int gs_id, int map_tag, int result);
		int  OnOpenBanquetStart(int gs_id, int map_tag, int retcode);
		void OnDisconnect(int gsid);
		int  OnBattleLeave(int roleid, int gs_id, int map_tag);
		int  OnTryEnter(int roleid, int gs_id, int map_tag, int & endtime, char &battle_type, char iscross, bool b_gm);
		bool ReEnter(int roleid, int& gs_id, int& map_tag, char& battle_type);// ��������
		int  OnCancel(int roleid, int gs_id, int map_tag, char isqueue);

		int  TryAbnormalOffline(int roleid, int gs_id, int map_tag);
	};
};
#endif

