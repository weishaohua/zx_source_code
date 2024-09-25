#ifndef __ONLINEGAME_GS_COLLISION_RAID_PLAYER_H
#define __ONLINEGAME_GS_COLLISION_RAID_PLAYER_H

#include "raid_player.h"


/** 
   ��ײս�������޷��ܺõĽ��ͬ�����ⱻȡ��
   �������ײ��ش����������ƥ��ս��
   ��������߼�һ�£�������ײ(collision)��ص����ֶ�û���޸�
 */

class collision_raid_player_imp : public raid_player_imp
{
public:
	DECLARE_SUBSTANCE(collision_raid_player_imp);
	collision_raid_player_imp();

	enum MOVE_STATE
	{
		STATE_NORMAL,
		STATE_START_DROP,
		STATE_DROP,
	};
	
	virtual int MessageHandler( const MSG& msg );
	virtual void OnHeartbeat( size_t tick );
	virtual void PlayerEnterWorld();
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void HandleRaidResult();
	virtual void CheckSpecialMove();
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void OnResurrect();
	virtual bool NeedCheckMove(){return false;}

	//���ս��Ĭ�ϲ������ײ��ǿ�������ڵ��棬�����ͷ���ʱ����Ҫ���
	virtual bool StandOnGround() { return true;}
	virtual bool InFlying() { return false;}
	virtual void HandleDeath();
	virtual void HandleCheatInfo(bool is_cheat, int cheat_counter); 

	virtual void DebugChangeCollisionResult(int faction, int score);
	virtual bool CheckBanish(){return true;}
	/*
	virtual void PlayerGetCollisionPlayerPos(int roleid);
	virtual void StartSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp);
	virtual void SpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, unsigned short stamp, char collision_state, int timestamp);
	virtual void StopSpecialMove(unsigned char dir, A3DVECTOR & cur_pos, unsigned short stamp);
	*/

	void ResetPos(); 
private:
	char move_state;
	int _revive_counter;		//ǿ�Ƹ����ʱ��

	enum
	{
		REVIVE_MAX_TIMEOUT = 10,
	};

};	

class collision2_raid_player_imp : public collision_raid_player_imp
{
public:
	DECLARE_SUBSTANCE(collision2_raid_player_imp);
	collision2_raid_player_imp(){}

	virtual void HandleRaidResult();
	virtual void CheckSpecialMove(){}

};


#endif
