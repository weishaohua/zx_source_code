#ifndef __ONLINEGAME_GS_TR_PLAYER_IMP_H
#define __ONLINEGAME_GS_TR_PLAYER_IMP_H

#include "../player_imp.h"

class tr_player_imp : public gplayer_imp
{
private:
	int _attack_faction;		//������Ӫ
	int _defense_faction;		//�ط���Ӫ
	int _timeout;			//�߳�ս����timeout
	int _battle_info_seq;		//ս��info���к�
	int _old_world_tag;		//����ս��ǰ��tag	
	int _kill_count;		//ɱ����
	int _death_count;		//��ɱ��
	int _turret_counter;		
	int _kick_timeout;
	int _battle_id;			//����id
	int _task_timeout;		//���������timeout
	bool _battle_result_sent;	//ս��������ͱ�־
	bool _is_assist;		//�Ƿ���Ӷ��
	bool _is_leaving;		//�Ƿ������˳�
	A3DVECTOR _old_pos;		//����ս��ǰ��λ��

public:
	DECLARE_SUBSTANCE( tr_player_imp )

	tr_player_imp()
	{
		_attack_faction = 0;
		_defense_faction = 0;
		_timeout = -1;
		_battle_info_seq = -1;
		_old_world_tag = 0;
		_kill_count = 0;
		_death_count = 0;
		_turret_counter = 0;
		_kick_timeout = 0;
		_battle_id = 0;
		_task_timeout = 0;
		_battle_result_sent = false;
		_is_assist = false;
		_is_leaving = false;
		_old_pos = A3DVECTOR(0,0,0);
	}
	
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int MessageHandler( const MSG& msg );
	virtual int GetFaction();
	virtual int GetEnemyFaction();

	virtual void OnHeartbeat( size_t tick );

	virtual void PlayerEnterWorld();
	virtual void PlayerLeaveWorld();
	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();
	virtual void PostLogin(const userlogin_t& user, char trole, char loginflag);
	virtual void GetLogoutPos(int& world_tag,A3DVECTOR& pos);
	virtual void PlayerLeaveTerritory();

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void GetBattleInfo(){}
	
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();

	virtual void PlayerChangeTerritoryResult(int result);
	virtual int CheckUseTurretScroll();

private:
	virtual bool ValidPlayer();
	virtual void HandleBattleResult(){}
	virtual void HandleDeath(){}

	void LeaveTerritory();


	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);
};


#endif
