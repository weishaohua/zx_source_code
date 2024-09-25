#ifndef __ONLINEGAME_GS_BG_PLAYER_H
#define __ONLINEGAME_GS_BG_PLAYER_H

#include "../player_imp.h"

class bg_player_imp: public gplayer_imp
{
private:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);

protected:
	int _attack_faction;		//������Ӫ
	int _defense_faction;		//�ط���Ӫ
	int _timeout;			//�߳�ս����timeout
	int _battle_info_seq;		//ս��info���к�
	int _old_world_tag;		//����ս��ǰ��tag	
	int _kill_count;		//ɱ����
	int _death_count;		//��ɱ��
	bool _battle_result_sent;	//ս��������ͱ�־
	bool _cond_kick;		//���������߳���־
	A3DVECTOR _old_pos;		//����ս��ǰ��λ��

public:
	DECLARE_SUBSTANCE( bg_player_imp )
	bg_player_imp()
	{
		_battle_faction = 0;
		_attack_faction = 0;
		_defense_faction = 0;
		_timeout = -1;
		_battle_info_seq = -1;
		_old_world_tag = 0;
		_kill_count = 0;
		_death_count = 0;
		_battle_result_sent = false;
		_old_pos = A3DVECTOR(0,0,0);
		_cond_kick = false;
	}
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int MessageHandler( const MSG& msg );
	virtual void OnHeartbeat( size_t tick );
	virtual void PlayerEnterWorld();
	virtual void PlayerLeaveWorld();
	virtual void PostPlayerEnterWorld() {}
	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();
	virtual int GetFaction();
	virtual int GetEnemyFaction();
	virtual const A3DVECTOR& GetLogoutPos( int &world_tag );
	virtual bool PlayerUseItem(int where, size_t inv_index, int item_type,size_t count);
	virtual bool PlayerUseItemWithArg(int where,size_t inv_index,int item_type,size_t count, const char * buf, size_t buf_size);

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time ){}
	virtual void GetBattleInfo(){}
	
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();

	virtual void PostLogin(const userlogin_t & user, char trole, char loginflag);
	virtual bool IsLogoutDisconnected() { return false; }

	virtual void OnTimeOutKickout();

protected:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual bool ValidPlayer();
	
	virtual void HandleBattleResult(){}
	virtual void HandleDeath(){}

	virtual void DumpBattleSelfInfo(){}
	virtual void DumpBattleInfo(){}
	virtual void DebugIncBattleScore(int inc){}
	
};

#endif

