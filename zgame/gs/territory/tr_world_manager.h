#ifndef __ONLINEGAME_GS_TR_WORLD_MANAGER_H_
#define __ONLINEGAME_GS_TR_WORLD_MANAGER_H_

#include "../usermsg.h"
#include "../global_manager.h"

struct territory_field_info
{
	int battle_type;
	int battle_world_tag;
	int player_limit;
};

class tr_world_manager : public global_world_manager
{
private:
	//��̬��Ϣ
	int _max_player_limit;		// ˫����������������
	int _min_player_level;		// �����͵ȼ�
	int _max_player_level;		// �����ߵȼ�

	int _kickout_time_min;		// �߳���Сʱ��
	int _kickout_time_max;		// �߳����ʱ��
	int _kickout_close_time;	// ս���ر�ʱ��

	int _battle_type;		// ս������

	//��̬��Ϣ
	int _cur_timestamp;		// ����ʹ��ʱ���
	int _start_timestamp;		// ս������ʱ���
	int _end_timestamp;		// ս������ʱ���

	int _heartbeat_counter;		// ������ʱ
	int _battle_info_seq;		// ������Ϣ����
	int _update_info_seq;		// ������Ϣ���� 

	int _battle_id;			// ս����Ӧ������id
	int _battle_status;		// ս��״̬
	int _battle_result;		// ս�����
	int _battle_lock;		// ս����

	cs_user_map  _defender_list;	// �ط��б�
	cs_user_map  _attacker_list;	// �����б�
	cs_user_map  _all_list;

	struct player_battle_info
	{
		player_battle_info() : battle_faction(0), kill(0), death(0){}
		int battle_faction;
		int kill;
		int death;
	};
	typedef std::map<int,player_battle_info> PlayerInfoMap;
	typedef std::map<int,player_battle_info>::iterator PlayerInfoIt;
		
	struct revive_pos_t
	{
		A3DVECTOR pos;
		bool active;
	};
	typedef std::map<int,std::vector<revive_pos_t> > CtrlReviveMap;
	typedef std::map<int,std::vector<revive_pos_t> >::iterator CtrlReviveMapIt;

	struct battle_data
	{
		int mafia_id;
		PlayerInfoMap player_info_map;		      //�����Ϣ
		std::vector<A3DVECTOR> town_list;             //�سǵ�
		std::vector<A3DVECTOR> entry_list;            //�����
		CtrlReviveMap revive_map;                     //�����
	};
	battle_data _attacker_data;
	battle_data _defender_data;

	enum BATTLE_TYPE
	{
		BT_NONE 	= 0,
		BT_HIGH 	= 1,		//�߼�ս��
		BT_MIDDLE 	= 2,		//�м�ս��
		BT_LOW 		= 3,		//�ͼ�ս��
	};

	enum BATTLE_RESULT
	{
		BR_NONE		=	0,	// �޽��
		BR_ATTACKER_WIN =	1,	// ������ʤ
		BR_DEFENDER_WIN =	2,	// �ط���ʤ
	};


	enum BATTLE_STATUS
	{
		BS_NONE 	= 0,		// ��
		BS_READY	= 1,		// ս��׼�����
		BS_RUNNING 	= 2,		// ս������
		BS_CLOSING 	= 3,		// ս���ر�
	};

	enum BATTLE_FACTION
	{       
		BF_NONE                 = 0,    //����Ӫ
		BF_ATTACKER             = 1,    //������
		BF_DEFENDER             = 2,    //���ط�
	};  
	
	enum
	{
		DEFAULT_MAX_PLAYER_COUNT = 100,

		DEFAULT_KICKOUT_TIME_MIN = 3,
		DEFAULT_KICKOUT_TIME_MAX = 15,
		DEFAULT_KICKOUT_CLOSE_TIME = 30,

		TERRITORY_WORLD_TAG_BEGIN = 501,
		CTRL_ID_BATTLE_BASE =			90000,	// ��ֵ
		CTRL_CONDISION_ID_BATTLE_MIN =		80000,	// ��Сս��������id
		CTRL_CONDISION_ID_BATTLE_PVE_BEGIN =	80052,	// PVEս������
		CTRL_CONDISION_ID_BATTLE_PVP_BEGIN =	80053,	// PVPս������
		CTRL_CONDISION_ID_BATTLE_MAX =		81000,	// ���ս��������id
		CTRL_CONDISION_ID_BATTLE_OFFSET = CTRL_CONDISION_ID_BATTLE_MAX - CTRL_CONDISION_ID_BATTLE_MIN + 1,
	};

	friend class tr_player_imp;

public:
	tr_world_manager()
	{
		_max_player_limit = 0;
		_min_player_level = 0;
		_max_player_level = 0;
		_kickout_time_min = 0;
		_kickout_time_max = 0;
		_kickout_close_time = 0;
		_battle_type = 0;

		_cur_timestamp = 0;
		_start_timestamp = 0;
		_end_timestamp = 0;
		_heartbeat_counter = 0;
		_battle_info_seq = 0;
		_update_info_seq = 0;
		_battle_id = 0;
		_battle_status = 0;
		_battle_result = 0;
		_battle_lock = 0;
		
		_attacker_data.mafia_id = 0;
		_defender_data.mafia_id = 0;
	}

	~tr_world_manager()
	{
	}

	int Init( const char* gmconf_file, const char* servername, int tag, int index);

	void Reset()
	{
		_cur_timestamp = 0;
		_start_timestamp = 0;
		_end_timestamp = 0;
		_heartbeat_counter = 0;
		_battle_info_seq = 0;
		_update_info_seq = 0;
		_battle_id = 0;
		_battle_result = 0;
		_battle_lock = 0;

		_defender_list.clear();
		_attacker_list.clear();
		_all_list.clear();

		_attacker_data.mafia_id = 0;
		_defender_data.mafia_id = 0;

		_attacker_data.player_info_map.clear();
		_defender_data.player_info_map.clear();
		
	}

	inline bool IsBattleWorld() { return true; }
	int GetLevelMin() const { return _min_player_level;}
	int GetLevelMax() const { return _max_player_level;}
	int GetKickoutTimeMin() const {return _kickout_time_min;}
	int GetKickoutTimeMax() const {return _kickout_time_max;}
	int GetBattleType() const {return _battle_type;}
	int GetBattleID() const {return _battle_id;}
	int GetBattleStatus() const {return _battle_status;}
	int GetBattleResult() const {return _battle_result;}
	int GetAttackerMafiaID() const {return _attacker_data.mafia_id;}
	int GetDefenderMafiaID() const {return _defender_data.mafia_id;}
	int GetBattleStartstamp() const {return _start_timestamp;}
	int GetBattleEndstamp() const {return _end_timestamp;}
	int GetBattleCtrlID() const { return _defender_data.mafia_id == 0 ? CTRL_CONDISION_ID_BATTLE_PVE_BEGIN : CTRL_CONDISION_ID_BATTLE_PVP_BEGIN;}
	void SendBattleInfo() {++_battle_info_seq;}
	bool IsBattleRunning() { return _battle_status ==  BS_RUNNING;}
	void ChangeBattleResult(int result)
	{
		spin_autolock keeper( _battle_lock );
		_battle_result = result;
		SendBattleInfo();
	}	      
	
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
	
	//ע��سǵ�
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction);
	//ע�Ḵ���
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id);
	//ע������
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	//��ûسǵ� 
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	//��ø����
	virtual bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag);
	//�����������
	virtual void GetRandomEntryPos(A3DVECTOR& pos,int battle_faction);

	virtual bool CreateBattleTerritory(int battle_id, int tag_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp);
	
	virtual void Heartbeat();

	virtual void PlayerEnter( gplayer* pPlayer, int faction); 
	virtual void PlayerLeave( gplayer* pPlayer, int faction);
	
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );
	virtual void OnDeliveryConnected( std::vector<territory_field_info>& info );
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnActiveSpawn(int id,bool active);
	
	virtual void BattleFactionSay( int faction ,const void* buf, size_t size, int channel = 0, const void *aux_data = 0, size_t dsize =0, int char_emote =0, int self_id =0);
	virtual void BattleSay( const void* buf, size_t size ){ }
	virtual int TranslateCtrlID( int which );
	virtual void ActiveCtrlIDLog(int which, bool active);

	void DumpWorldMsg();

private:
	void OnInit();	
	void OnReady();
	void OnCreate();
	void OnRunning();
	void OnClosing();

	virtual int OnDestroyKeyBuilding( int faction);
	virtual void OnPlayerLeave( gplayer* pPlayer);

};




#endif //__ONLINEGAME_GS_TR_WORLD_MANAGER_H_
