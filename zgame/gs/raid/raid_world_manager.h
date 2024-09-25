#ifndef __ONLINEGAME_GS_RAID_WORLD_MANAGER_H__ 
#define __ONLINEGAME_GS_RAID_WORLD_MANAGER_H__

#include "../global_manager.h"
#include "../world.h"
#include "../usermsg.h"
#include "../raid_controller.h"
#include <vector>
#include <set>

enum
{
	CHAT_RAID = 27,
};

// ������������
enum
{
	// ʹ�������ֵ�������ʵ����ս����ʹ�õ�id
	CTRL_ID_RAID_BASE 							= 80000,	// ��ֵ
	
	CTRL_CONDISION_ID_RAID_MIN 					= 70204,	// ��Сս��������id
	CTRL_CONDISION_ID_RAID_BEGIN 				= 70204,	// ս��������ˢ����ͨ������

	CTRL_CONDISION_ID_RAID_ATTACKER_KEY 		= 70225,	// ˢ������key building
	CTRL_CONDISION_ID_RAID_DEFENDER_KEY 		= 70226,	// ˢ���ط�key building

	CTRL_CONDISION_ID_RAID_TEAM_CRSSVR_BEGIN 	= 70251, 	//���С��PK,��һС��
	CTRL_CONDISION_ID_RAID_TEAM_CRSSVR_END   	= 70255, 	//���С��PK,���һ��С��	
	CTRL_CONDISION_ID_RAID_MAX					= 70700,	// ���ս��������id

	CTRL_CONDISION_ID_RAID_OFFSET = CTRL_CONDISION_ID_RAID_MAX - CTRL_CONDISION_ID_RAID_MIN + 1,
}; 

enum RAID_RESULT
{
	RAID_ATTACKER_WIN 	=	1,	// ������ʤ
	RAID_DEFENDER_WIN 	=	2,	// �ط���ʤ
	RAID_DRAW 			=	3,	// ƽ��
};

enum RAID_FACTION
{       
	RF_NONE                 = 0,    //����Ӫ
	RF_ATTACKER             = 1,    //������
	RF_DEFENDER             = 2,    //���ط�
	RF_VISITOR				= 3,    //�۲��� 
};  

enum RAID_TYPE
{
	RT_COMMON				= 0,	//��ͨ����
	RT_TOWER,
	RT_COLLISION,
	RT_MAFIA_BATTLE,
	RT_LEVEL,
	RT_COLLISION2,
	RT_TRANSFORM,
	RT_STEP,
	RT_SEEK,
	RT_CAPTURE,
};

struct player_raid_info
{
	player_raid_info(): cls(0),level(0),reborn_cnt(0),timestamp(0)
	{
	}
	player_raid_info( int c, int l, int sl):
		cls(c), level(l), reborn_cnt(sl)
	{
	}
	player_raid_info( int c, int l, int sl, int ti):
		cls(c), level(l), reborn_cnt(sl), timestamp(ti) 
	{
	}

	short cls;
	short level;
	short reborn_cnt;	
	int timestamp;
};

typedef S2C::CMD::raid_info::player_info_in_raid player_info_raid;

// Youshuang add
enum LEVEL_OP
{
	LEVEL_NONE = -1,
	LEVEL_OR,
	LEVEL_AND,
	LEVEL_ALL,
};

struct raid_boss_info
{
	raid_boss_info() : tid( -1 ), is_killed( 0 ), achievement( 0 ), cur_num(0), max_num(0){}
	raid_boss_info( int id, unsigned char killed, unsigned char achieve ) 
		: tid( id ), is_killed( killed ), achievement( achieve ), cur_num(0), max_num(0){}
	int tid;
	int level;
	unsigned char is_killed;
	unsigned char achievement;
	unsigned char cur_num;
	unsigned char max_num;
};

struct raid_level_info
{
	int howto;
	std::vector<raid_boss_info*> boss;
};
// end

class raid_world_manager: public global_world_manager
{
protected:
	int _raid_id;			// ս��id,����world��Ӧһ��id,��֤�л�ʹ��
	//int _raid_type;			// ս������
	int _raid_result;
	int _raid_lock;

	int _max_player_count;		// ����������, ÿ��
	int _need_level_min;		// ������ҵĵȼ����ƣ���С����
	int _need_level_max;		// ������ҵĵȼ����ƣ���󼶱�
	
	int _raid_time;         	// ����ʱ��,0Ϊ����ʱ��
	int _invincible_time;		// ��Ҵ��븱�����޵еȴ�ʱ��
	int _close_raid_time;		// �رո�����ʱ��
	int _player_leave_time;		// ����ڸ����رպ���ͣ����ʱ��

	int _win_ctrl_id;			// ����ʤ������������ID�������������������
	int _win_task_id;			// ����ʤ����������id
	int _forbidden_items_id[10];// ������ƷID�б�
	int _forbidden_skill_id[10];// ���ü���ID�б�
	
	int _cur_timestamp;			// ����ʹ��ʱ���
	int _kill_monster_timestamp;// ɱ�����й����ʱ��
	int _end_timestamp;			// ��������ʱ���
	int _start_timestamp;		// ��������ʱ���

	int _heartbeat_counter;		// ������ʱ
	int _raid_info_seq;			// ������Ϣ����
	int _update_info_seq;		// ������Ϣ���� 
	int _apply_count;			// ��������

	int _status;				// ս��״̬
	int _raid_winner_id;		// ����ʤ����ID
	int _max_killer_id;			// ɱ��������

	int _attacker_score;		// ��������
	int _defender_score;		// �ط�����

	int _raidroom_id;			//deliveryʹ�õķ���id��delivery����ѯ��
	int _difficulty;
	int _difficulty_ctrl_id;		//�����Ѷȿ�����id
	
	cs_user_map  _defender_list;
	cs_user_map  _attacker_list;
	cs_user_map  _all_list;
	
	std::vector<int> _raid_member_list;	//Delivery�������ø�����ԱID�����ڽ�����
	// Youshuang add
	std::vector<raid_level_info> _level_info;
	std::multimap<int, raid_boss_info*> _tid_2_bossinfo;
	// end
	int _seek_heartbeat_counter; //��������

	enum
	{
		RS_READY 		= 0,	// ������׼�����
		RS_PREPARING 	= 1,	// ս��׼��״̬(��ʱ����)
		RS_RUNNING 		= 2,	// ս����ʼ״̬
		RS_CLOSING 		= 3,	// ս���رգ����������������
		RS_IDLE	  		= 4,	// ������������ڿ���״̬���ɱ��µĸ���ʵ����ʹ��
	};

	struct building_data
	{
		int normal_building_init;	// ��ʼ������
		int key_building_init;		// ��ʼ������
		int normal_building_left;	// ʣ����ͨ���������
	};

	typedef std::map<int,player_raid_info> PlayerInfoMap;
	typedef std::map<int,player_raid_info>::iterator PlayerInfoIt;

	// ��������
	struct competitor_data
	{
		PlayerInfoMap player_info_map;
		// ����������
		building_data buildings;
		// �״λ�ɱ��¼
		bool first_kill;
		struct revive_pos_t
		{
			A3DVECTOR pos;
			bool active;
		};
		typedef std::map<int,std::vector<revive_pos_t> > CtrlReviveMap;
		typedef std::map<int,std::vector<revive_pos_t> >::iterator CtrlReviveMapIt;
		std::vector<A3DVECTOR> town_list;      //�سǵ�
		std::vector<A3DVECTOR> entry_list;     //�����
		CtrlReviveMap revive_map;              //�����
		int kill_count;

		void Clear()
		{
			player_info_map.clear();
			memset(&buildings, 0, sizeof(building_data));
			first_kill = false;
			town_list.clear();
			entry_list.clear();
			revive_map.clear();
			kill_count = 0;
		}
	};

	competitor_data _attacker_data;
	competitor_data _defender_data;
	bool _all_monster_killed;

	std::vector<int> _global_var_list;
	
public:
	void Heartbeat();
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );
	void PostPlayerLogin( gplayer* pPlayer ) {}
	void RecordRaidBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building = false );

public:
	raid_world_manager();
	virtual ~raid_world_manager();

	inline void AddMapNode( cs_user_map& map, gplayer* pPlayer )
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val( pPlayer->ID.id, pPlayer->cs_sid );
		if( cs_index >= 0 && val.first >= 0 )
		{
			map[cs_index].push_back( val );
		}
	}
	inline void DelMapNode( cs_user_map& map, gplayer* pPlayer )
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val( pPlayer->ID.id, pPlayer->cs_sid );
		if( cs_index >= 0 && val.first >= 0 )
		{
			cs_user_list& list = map[cs_index];
			int id =  pPlayer->ID.id;
			size_t i = 0;
			for( i = 0; i < list.size(); ++i )
			{
				if( id == list[i].first )
				{
					list.erase( list.begin() + i );
					i --;
				}
			}
		}
	}

	// ������ͼ����true
	inline bool IsRaidWorld() { return true; }	
	inline bool IsRaidRunning() { return ( RS_RUNNING == _status ); }
	inline int GetRaidID() { return _raid_id; }
	inline int GetLevelMin() { return _need_level_min; }
	inline int GetLevelMax() { return _need_level_max; }
	inline int GetRaidResult() { return _raid_result; }
	inline int GetRaidMaxKillerID() { return _max_killer_id; }
	inline int GetRaidWinnerID(){return _raid_winner_id; }
	inline int GetPlayerLeaveTime() { return _player_leave_time; }
	inline int GetRaidStatus() { return _status; }
	inline int GetRaidStartstamp() { return _start_timestamp; }
	inline int GetRaidEndstamp() { return _end_timestamp; }
	inline int GetWinTaskID() { return _win_task_id; }
	inline int GetRaidRoomID() { return _raidroom_id; }
	inline int GetRaidDifficulty() { return _difficulty;}

	void SetRaidResult(int result); 

public:
 	virtual int GetRaidType() { return RT_COMMON;}
	
	//return 0, ���Դ�����-1��world_manager��ǰ��ռ��, ѡ����һ�����õ�WM��-2������data������ʧ��
	virtual int TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	void ForceCloseRaid();
	void Release();			//�ͷŸ�����Դ�����ø���״̬����Ҫֱ�ӵ��ã��رո�������CloseRaid
	virtual void Reset();

	// ͳһ����
	void HandleModifyRaidDeath( int player_id, int death_count, size_t content_length, const void * content);
	void HandleModifyRaidKill( int player_id, int kill_count, size_t content_length, const void * content);
	void HandleSyncRaidInfo( int player_id, int reserver, size_t content_length, const void * content);
	virtual void HandleTransformRaid(int player_id) {}
	virtual void HandleCaptureRaidSyncPos(char faction, const A3DVECTOR& pos) {}

	//����Ƶ��
	// type 0 ��һ��Ѫ, 1 ɱ��, 2 ��ֹ
	void BroadcastRaidMsg( unsigned char type, unsigned char raid_faction, int battle_id, int gs_id, int killer, int deadman );
	void RaidFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int char_emote, int self_id);
	void RaidSay( const void* buf, size_t size ){}
	int TranslateCtrlID( int which );
	//int GetPostion(int id, int score);

	bool GetRaidInfo( int& info_seq, std::vector<player_info_raid>& info, bool bForce = false);
	void GetRaidUseGlobalVars(std::vector<int>& var_list);
	
 	size_t GetRaidUseGlobalVarCnt();
	int GetRaidUseGlobalVar(size_t index);


public:
	virtual int GetClientTag() { return _raid_id; } //������ raid_id ���ǿͻ������õ�ͼ��
	virtual bool SaveCtrlSpecial() { return true; } //�Ƿ�Ϊ��ͬʵ���������������״̬ �����Ͱ��ɻ��ط��� true
	virtual void PlayerEnter( gplayer* pPlayer, int faction, bool reenter); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer
	virtual void PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick);
	virtual void PlayerEnterWorld( gplayer* pPlayer, int faction, bool reenter);

	//ע��سǵ�
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction);
	//ע�Ḵ���
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id);
	//ע������
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	//��ûسǵ� 
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	//��ø����
	virtual bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag) { return false; }
	//�����������
	virtual void GetRandomEntryPos(A3DVECTOR& pos,int raid_faction);

	virtual int GetRaidTotalScore()
	{
		return _attacker_score + _defender_score;
	}

	virtual int GetAttackerScore() { return _attacker_score;}
	virtual int GetDefenderScore() { return _defender_score;}
	
	// Youshuang
	virtual std::vector<raid_boss_info> GetKilledBossList() const;
	raid_boss_info* GetBossByID( int tid, unsigned char killed = 1 ) const;
	virtual void OnMobReborn( int tid );
	int NotifyBossBeenKilled();
	// end
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void OnKillMonster(gplayer* pPlayer, int monster_tid, int faction){}
	virtual void OnUsePotion(gplayer* pPlayer, char type, int value) {}
	virtual void OnGatherMatter(gplayer* pPlayer, int matter_tid, int cnt) {}

	virtual int  GetRoundStartstamp() { return 0; }
	virtual bool IsRoundRunning() { return false; }
	virtual void UpdateRoundScore(int raid_faction, int change) {}
	virtual bool IsLimitSkillInWorld(int skill_id);
	virtual bool IsForbiddenItem(int item_type);

	virtual bool IsIdle() const { return _status == RS_IDLE; } 
	virtual bool CanEnterWorld() const { return _status == RS_RUNNING; }
	virtual void OnActiveSpawn(int id,bool active);
/*
	virtual INIT_RES OnInitMoveMap(path_finding::PathfindMap* & mm);
	virtual INIT_RES OnInitCityRegion(city_region* & cr);
	virtual INIT_RES OnInitBuffArea(buff_area* & ba); 
	virtual INIT_RES OnInitPathMan(path_manager* & pm); 
	*/
	virtual INIT_RES OnInitMoveMap();
	virtual bool OnReleaseMoveMap() { return true; }
	virtual INIT_RES OnInitCityRegion();
	virtual bool OnReleaseCityRegion() { return true; }
	virtual INIT_RES OnInitBuffArea();
	virtual bool OnReleaseBuffArea() { return true; }
	virtual INIT_RES OnInitPathMan();
	virtual bool OnReleasePathMan() { return true; }

	virtual bool OnTraceManAttach();
	virtual void OnDisconnect(); //�� gdeliveryd �Ͽ�����ʱ�Ĵ���
protected:
	virtual bool OnInit(){return true;}		
	virtual void OnCreate();
	virtual void OnRunning();
	virtual void OnClosing();
	virtual void OnReady();
	virtual void OnClose();  //״̬����Running����������
	
	virtual int OnDestroyKeyBuilding( int faction);
	virtual int OnDestroyNormalBuilding( int faction){return 0;}
	virtual void OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction) {}
	virtual void OnPlayerEnterWorld(gplayer* pPlayer, bool reenter, int faction) {}
	virtual void OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction) {}
	virtual void OnRaidInfoChange(){}
	virtual void KillAllMonsters();
	
protected:
	void SendRaidInfo();
	void DumpWorldMsg();
	void UpdatePlayerInfo(PlayerInfoIt &it, size_t content_length, const void * content);	
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//��ͨ�����߼�
////////////////////////////////////////////////////////////////////////////////////////////////////
class cr_world_manager : public raid_world_manager
{       
protected:
	virtual bool OnInit();
	virtual void OnCreate();
	virtual int OnDestroyNormalBuilding(int faction);
	virtual void OnClosing();

	// Youshuang add
public:
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	// end
};
#endif

