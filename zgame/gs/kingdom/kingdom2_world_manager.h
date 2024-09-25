#ifndef __ONLINEGAME_GS_KINGDOM2_WORLD_MANAGER_H
#define __ONLINEGAME_GS_KINGDOM2_WORLD_MANAGER_H 

#include "kingdom_common.h"
#include "../global_manager.h"
class kingdom2_world_manager : public global_world_manager
{
public:
	kingdom2_world_manager()
	{
		_kickout_time_min = 0;
		_kickout_time_max = 0;
		_kickout_close_time = 0;

		_cur_timestamp = 0;
		_start_timestamp = 0;
		_end_timestamp = 0;

		_battle_status = 0;
		_battle_lock = 0;
		
		_heartbeat_counter = 0;
		_attacker_npc_id = 0;
		_defender_npc_id = 0;

		memset(&_key_npc_info, 0, sizeof(_key_npc_info));
	}

	void Reset()
	{
		_cur_timestamp = 0;
		_start_timestamp = 0;
		_end_timestamp = 0;

		_heartbeat_counter = 0;

		memset(&_key_npc_info, 0, sizeof(_key_npc_info));
	}

	enum BATTLE_FACTION
	{       
		BF_NONE                 = 0,    //����Ӫ
		BF_ATTACKER             = 1,    //������
		BF_DEFENDER             = 2,    //���ط�
		BF_MELEE		= 3,	//��ս
	};  
	
	enum BATTLE_STATUS
	{
		BS_NONE 	= 0,		// ��
		BS_READY	= 1,		// ս��׼�����
		BS_RUNNING 	= 2,		// ս������
		BS_CLOSING 	= 3,		// ս���ر�
	};
	
	enum
	{

		DEFAULT_KICKOUT_TIME_MIN = 3,
		DEFAULT_KICKOUT_TIME_MAX = 15,
		DEFAULT_KICKOUT_CLOSE_TIME = 30,

		KINGDOM_TAG_ID = 150,
	};

	int Init( const char* gmconf_file, const char* servername, int tag, int index);

	int GetKickoutTimeMin() const {return _kickout_time_min;}
	int GetKickoutTimeMax() const {return _kickout_time_max;}
	int GetBattleStatus() const {return _battle_status;}
	int GetBattleStartstamp() const {return _start_timestamp;}
	int GetBattleEndstamp() const {return _end_timestamp;}
	bool IsBattleRunning() { return _battle_status ==  BS_RUNNING;}
	inline bool IsBattleWorld() { return true; }
	inline bool IsKingdomWorld() { return true; }
	

	virtual void PlayerEnter( gplayer* pPlayer, int faction); 
	virtual void PlayerLeave( gplayer* pPlayer, int faction);

	virtual void OnDeliveryConnected( std::vector<kingdom_field_info>& info );
	virtual bool CreateKingdom(char fieldtype, int tag_id, int defender, std::vector<int> attacker_list); 
	virtual void StopKingdom(char fieldtype, int tag_id);
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );

	virtual void Heartbeat();

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
	
	virtual int OnMobDeath( world * pPlane, int faction,int tid,const A3DVECTOR& pos, int attacker_id);

	void OnSyncKeyNPCInfo(int cur_hp, int max_hp, int hp_add, int hp_dec);
	void OnGetKeyNPCInfo(int & cur_hp, int & max_hp, int & hp_add, int & hp_dec);
private:
	void OnInit();	
	void OnReady();
	void OnCreate();
	void OnRunning();
	void OnRunning2();
	void OnClosing();

	void Close();
private:

	//��̬��Ϣ
	int _cur_timestamp;		// ����ʹ��ʱ���
	int _start_timestamp;		// ս������ʱ���
	int _end_timestamp;		// ս������ʱ���
	
	int _kickout_time_min;		// �߳���Сʱ��
	int _kickout_time_max;		// �߳����ʱ��
	int _kickout_close_time;	// ս���ر�ʱ��

	int _battle_status;
	int _battle_lock;

	int _attacker_npc_id;
	int _defender_npc_id;

	int _heartbeat_counter;		// ������ʱ
	
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
		std::vector<A3DVECTOR> town_list;             //�سǵ�
		std::vector<A3DVECTOR> entry_list;            //�����
		CtrlReviveMap revive_map;                     //�����
	};
	battle_data _attacker_data;
	battle_data _defender_data;

	struct key_npc_info
	{
		int cur_hp;
		int max_hp;
		int hp_add;
		int hp_dec;
	};

	key_npc_info _key_npc_info;

};	

#endif

