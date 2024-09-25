#ifndef __ONLINEGAME_GS_BATTLEGROUND_MANAGER_H__
#define __ONLINEGAME_GS_BATTLEGROUND_MANAGER_H__

#include "instance_manager.h"
#include "battleground_ctrl.h"

struct battle_ground_param
{
	int battle_id;
	int attacker;		//����������
	int defender;		//���ط�����  �ط����ɿ���Ϊ0 ��ʱ��Ҫ�����ض��Ĺ���ֲ�����

	int player_count;	//ÿ��������Ƶ�����    �������������������2  GM���벻�������������
	
	int end_timestamp;	//����ʱ�� ��ʱ֮��δ����ս�������չ���ʤ������
};

/*------------------------ս����������-------------------------------*/
class battleground_world_manager : public instance_world_manager 
{
	virtual void UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size, char flag);
	virtual void GetLogoutPos(gplayer_imp * pImp, int & world_tag ,A3DVECTOR & pos);
	virtual void FinalInit(const char * servername);
	virtual void PreInit(const char * servername);
	virtual void OnDeliveryConnected();
	struct town_entry
	{
		int faction;
		A3DVECTOR target_pos;
	};
	
	abase::vector<town_entry> _town_list;
	int _win_condition;
	int _player_count_limit;

	bool GetTown(int faction, A3DVECTOR &pos, int & tag);
public:
	typedef battleground_ctrl::map_data map_data;

protected:

	map_data _defence_data;
	map_data _offense_data;
public:
	battleground_world_manager():instance_world_manager()
	{
		//ս������Ӧ���ǹ̶�ʱ�����
		_idle_time = 300;
		memset(&_defence_data,0,sizeof(map_data));
		memset(&_offense_data,0,sizeof(map_data));
	}
	virtual void TransformInstanceKey(const instance_key::key_essence & key, instance_hash_key & hkey)
	{
		hkey.key1 = key.key_level4;
		hkey.key2 = 0;
	}

	virtual int CheckPlayerSwitchRequest(const XID & who,const instance_key * key,const A3DVECTOR & pos,int ins_timer);
	virtual void Heartbeat();
	virtual bool CreateBattleGround(const battle_ground_param &);
	virtual world * CreateWorldTemplate();
	virtual world_message_handler * CreateMessageHandler();
	world * GetWorldInSwitch(const instance_hash_key & ikey,int & world_index);
	virtual bool GetTownPosition(gplayer_imp *pImp, const A3DVECTOR &opos, A3DVECTOR &pos, int & tag);
	virtual void RecordTownPos(const A3DVECTOR &pos,int faction);
	virtual void SetIncomingPlayerPos(gplayer * pPlayer, const A3DVECTOR & origin_pos);
	virtual void RecordMob(int type, int tid, const A3DVECTOR &pos,int faction,int count); 
	virtual int OnMobDeath(world * pPlane, int faction,  int tid, int attacker_id);
};

class battleground_world_message_handler : public instance_world_message_handler
{
protected:
	virtual ~battleground_world_message_handler(){}
	
	virtual void SetInstanceFilter(gplayer_imp * pImp,instance_key &  ikey);//���ø�����Ҫ��filter
	virtual void PlayerPreEnterServer(gplayer * pPlayer, gplayer_imp * pimp,instance_key &  ikey);//�ڵ���EnterWorld֮ǰ�Ĵ���
public:
	battleground_world_message_handler(instance_world_manager * man):instance_world_message_handler(man) {}
	virtual int HandleMessage(const MSG& msg);
	virtual int RecvExternMessage(int msg_tag,const MSG & msg);
};

#endif

