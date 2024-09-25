#ifndef __ONLINEGAME_GS_KINGDOM2_PLAYER_H
#define __ONLINEGAME_GS_KINGDOM2_PLAYER_H

#include "../player_imp.h"

//����ս�����ͼ2��ҵ�ʵ��
//��ͼ2�����ͨ����ɱ�������Ӱ���ͼ1�������������Ѫ��
class kingdom2_player_imp : public gplayer_imp
{
public:
	DECLARE_SUBSTANCE( kingdom2_player_imp )

	kingdom2_player_imp()
	{
		_timeout = 0;
		_kick_timeout = 0;
		_is_leaving = 0;
	}

	virtual void OnHeartbeat( size_t tick );

	virtual void PlayerEnterWorld();
	virtual void PlayerLeaveWorld();
	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();
	virtual int MessageHandler( const MSG& msg );
	virtual void PostLogin(const userlogin_t& user, char trole, char loginflag);
	virtual void GetLogoutPos(int& world_tag,A3DVECTOR& pos);
	virtual void PlayerLeaveKingdom();
	
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	

private:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	void LeaveKingdom();
	void UpdateKeyNPCInfo();

private:
	int _timeout;			//�߳�ս����timeout
	int _kick_timeout;
	bool _is_leaving;		//�Ƿ������˳�
};


#endif
