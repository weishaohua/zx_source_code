#include "collision_raid_player.h"
#include "collision_raid_world_manager.h"
#include "../clstab.h"
#include <common/message.h>
#include <factionlib.h>
#include "../actsession.h"
#include "../invincible_filter.h"
#include "../cooldowncfg.h"


DEFINE_SUBSTANCE( collision_raid_player_imp, raid_player_imp, CLS_COLLISION_RAID_PLAYER_IMP)
DEFINE_SUBSTANCE( collision2_raid_player_imp, collision_raid_player_imp, CLS_COLLISION2_RAID_PLAYER_IMP)

collision_raid_player_imp::collision_raid_player_imp()
{
	move_state = STATE_NORMAL;
	_revive_counter = 0;
}

int collision_raid_player_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		//��ֹ�������	
		case GM_MSG_PLAYER_LINK_RIDE_INVITE: 
		case GM_MSG_PLAYER_LINK_RIDE_INV_REPLY:
		//��ֹ��������
		case GM_MSG_PLAYER_BIND_REQUEST:
		case GM_MSG_PLAYER_BIND_INVITE:
		case GM_MSG_PLAYER_BIND_REQ_REPLY:
		case GM_MSG_PLAYER_BIND_INV_REPLY:		
		{
			return 0;
		}
		break;


		case GM_MSG_CS6V6_CHEAT_INFO:
		{
			ASSERT(msg.content_length == sizeof(msg_cs6v6_cheat_info));
			msg_cs6v6_cheat_info * pMsg = (msg_cs6v6_cheat_info*)(msg.content);
			HandleCheatInfo(pMsg->is_cheat, pMsg->cheat_counter);
		}
		return 0;
	}

	return raid_player_imp::MessageHandler(msg);
}

void collision_raid_player_imp::OnHeartbeat(size_t tick)
{
	raid_player_imp::OnHeartbeat(tick);
}


void collision_raid_player_imp::PlayerEnterWorld()
{
	__PRINTF( "��� %d �ڸ������� %d ������!!!\n", _parent->ID.id, _plane->GetTag() );
	EnableFreePVP( true );
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	bool rst = ((collision_raid_world_manager*)pManager)->GetCollisionPlayerInfo(_parent->ID.id, _battle_faction); 
	if(rst)
	{
		if( RF_ATTACKER == _battle_faction )
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
			_attack_faction = FACTION_BATTLEDEFENCE;
			_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
		}
		else if( RF_DEFENDER == _battle_faction ) 
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
			_attack_faction = FACTION_BATTLEOFFENSE;
			_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		}
		else if( RF_VISITOR == _battle_faction ) 
		{
			pPlayer->SetObjectState( gactive_object::STATE_BATTLE_VISITOR);
			SetSilentSealMode(true);
			_attack_faction = 0;
			_defense_faction = 0;
		}

		gplayer* pPlayer = GetParent();
		
		pManager->PlayerEnter( pPlayer, _battle_faction, _reenter);
		pPlayer->pos.y += 0.2;
	}

	int timeout = g_timer.get_systime() - pManager->GetRaidStartstamp();
	if(timeout > 0 && timeout < 30)
	{
		_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, 30 - timeout)); 
	}

	gplayer_imp::PlayerEnterWorld();
		
	if(_success_entered)
	{
		GNET::SendRaidEnterSuccess(gmatrix::GetServerIndex(), _parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag());
	}

	GLog::log(GLOG_INFO, "��ҽ��븱��: roleid=%d, raid_id=%d, raid_world_tag=%d, difficulty=%d", 
			_parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());
	if(rst)
	{
		_runner->enter_collision_raid( _battle_faction, (unsigned char)(pManager->GetRaidType()), pManager->GetRaidID(), pManager->GetRaidEndstamp() );
	
	}
	else
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "�������ײ������Ϊ��Ϣ�쳣�߳�����: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( pManager->GetRaidID(), _timeout );
	}
}

void collision_raid_player_imp::PlayerEnter()
{
}

void collision_raid_player_imp::PlayerLeave()
{
	EnableFreePVP( false );
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsBattleOffense() )
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		
	}
	else if( pPlayer->IsBattleDefence() ) 
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_DEFENCE );
	}
	else if( pPlayer->IsBattleVisitor() ) 
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_VISITOR);
		SetSilentSealMode(false);
	}

	pManager->PlayerLeave( pPlayer, _battle_faction,_cond_kick);
	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF );
	// ֪ͨ�ͻ���
	_runner->leave_raid();
}

void collision_raid_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	gplayer_imp::OnDeath( lastattack, is_pariah, true, false, 0);
	_resurrect_hp_recover = 1.0f;
	bool kill_much = false;
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	pManager->OnPlayerDeath(pPlayer, lastattack.id, pPlayer->ID.id, _battle_faction, kill_much);
}

void collision_raid_player_imp::HandleDeath()
{
	if(IsDead())
	{
		++_revive_counter;
		if(_revive_counter >= REVIVE_MAX_TIMEOUT)
		{
			//ǿ�Ƹ���
			_revive_counter = 0;
			ClearSession();
			session_resurrect_in_town *pSession= new session_resurrect_in_town(this);
			pSession->SetExpReduce(0);
			AddStartSession(pSession);
		}

	}
	else
	{
		_revive_counter = 0;
	}
}

/*
���ּ������
A���� 
	1) ʤ����5�֣�ƽ��3�֣�ʧ�ܻ�2��
	2) ÿ����һ��A����������Ϊ5��

B����  
	1) ʤ����5�֣�ʧ�ܻ�2��, ƽ��3��
	2) ÿ����һ��B����������Ϊ15��
*/

void collision_raid_player_imp::HandleRaidResult()
{
	collision_raid_world_manager* pManager = (collision_raid_world_manager*)_plane->GetWorldManager();

	int max_killer_id = pManager->GetRaidMaxKillerID();
	int raid_result = pManager->GetRaidResult();

	gplayer* pPlayer = GetParent();

	char result = 0;
	int score_a = 0;
	int score_b = 0;
	int score = 0; 
	if((raid_result == RAID_ATTACKER_WIN && pPlayer->IsBattleOffense()) || (raid_result == RAID_DEFENDER_WIN && pPlayer->IsBattleDefence()))
	{
		result = 1;
		score_a = score_b = 5;
		IncCollisionWin();
		// ��ײ������ʤ��Ҳ��Ҫ���ӻ�Ծ��
//		if (!IsZoneServer()) {
			EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CB_ZHANCHANG_WIN);
//		}
	}
	else if((raid_result == RAID_DEFENDER_WIN &&  pPlayer->IsBattleOffense()) || (raid_result == RAID_ATTACKER_WIN && pPlayer->IsBattleDefence()))
	{
		result = 2;
		score_a = score_b = 2;
		IncCollisionLost();
	}
	else if(raid_result == RAID_DRAW)
	{
		result = 0;
		score_a = score_b = 3;
		IncCollisionDraw();
	}

	if(pPlayer->IsBattleOffense())
	{
		score = pManager->GetAttackerAwardScore();
	}
	else if(pPlayer->IsBattleDefence())
	{
		score = pManager->GetDefenderAwardScore();
	}

	ChangeCollisionScore(score); 

	//ɱ������
	if(max_killer_id == _parent->ID.id)
	{
		score_b += 3;
	}

	//�ж��ǲ��ǵ����һ��
        int cur_time = g_timer.get_systime();
	bool same_day = IsSameDay(cur_time, _collision_info.collision_timestamp);

	//�����һ��
	if(!same_day)
	{
		//��ʤ
		if(result == 1)
		{
			_collision_info.day_win = 1;
			score_b += 3;
		}
		else
		{
			_collision_info.day_win = 0;
		}

		_collision_info.collision_score_a += score_a;
		_collision_info.collision_score_b += score_b;

		_collision_info.day_score_a = score_a; 
		_collision_info.day_score_b = score_b; 
	}
	//�����N��
	else
	{
		if(result == 1)
		{
			//��ʤ
			if(_collision_info.day_win == 0)
			{
				score_b += 3;
			}
			++_collision_info.day_win;
		}

		//����A����
		if(_collision_info.day_score_a < 5)
		{
			if(_collision_info.day_score_a + score_a > 5)
			{
				score_a = 5 - _collision_info.day_score_a;
			}
			_collision_info.collision_score_a += score_a; 
			_collision_info.day_score_a += score_a; 
		}

		//����B����
		if(_collision_info.day_score_b < 15)
		{
			if(_collision_info.day_score_b + score_b > 15)
			{
				score_b = 15 - _collision_info.day_score_b;
			}
			_collision_info.collision_score_b += score_b; 
			_collision_info.day_score_b += score_b; 
		}
	}
	_collision_info.collision_timestamp = cur_time;

	_runner->collision_raid_result(result, score_a, score_b); 
	SendCollisionRaidInfo();

	ActiveCombatState(false);

	_commander->DenyCmd(controller::CMD_SKILL);
	_commander->DenyCmd(controller::CMD_ATTACK);

	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF | filter::FILTER_MASK_BUFF);

	GLog::log(GLOG_INFO, "�����ɸ���: roleid= %d, raid_id=%d, radi_world_tag =%d, difficulty=%d", 
			_parent->ID.id, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidDifficulty());

	GLog::log(GLOG_INFO, "����˳�6V6����: roleid=%d, result=%d, add_score_a=%d, add_score_b=%d, is_max_killer=%d",
			_parent->ID.id, result, score_a, score_b, _parent->ID.id == max_killer_id); 
}


/*
void collision_raid_player_imp::StartSpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, char collision_state, int timestamp)
{
	_runner->object_start_special_move(_parent->ID.id, velocity, acceleration, cur_pos, collision_state, timestamp);

	printf("start move: roleid=%d pos.x=%f, pos.y=%f, pos.z=%f\n", _parent->ID.id, _parent->pos.x, _parent->pos.y, _parent->pos.z); 
}

void collision_raid_player_imp::SpecialMove(A3DVECTOR & velocity, A3DVECTOR & acceleration, A3DVECTOR & cur_pos, unsigned short stamp, char collision_state, int timestamp)
{
	A3DVECTOR offset = cur_pos; 
	offset -= _parent->pos;
	StepMove(offset);
	_runner->object_special_move(_parent->ID.id, velocity, acceleration, cur_pos, collision_state, timestamp);
	printf("special move: roleid=%d pos.x=%f, pos.y=%f, pos.z=%f\n", _parent->ID.id, _parent->pos.x, _parent->pos.y, _parent->pos.z); 
	CheckSpecialMove();

	//���Э��ƽ��ÿ��ᷢ��10�����ϣ���Һ����׽����˱�״̬
	//ǿ�ƽ����ؼ�3 (ÿ��Э��ķ��ͻ��Զ�����3�㸺��
	((gplayer_controller*)_commander)->DecreaseLoadStat(3);
	
}

void collision_raid_player_imp::StopSpecialMove(unsigned char dir, A3DVECTOR & cur_pos, unsigned short stamp)
{
	A3DVECTOR offset = cur_pos; 
	offset -= _parent->pos;
	StepMove(offset);
	_parent->dir = dir;
	_runner->object_stop_special_move(_parent->ID.id, dir, cur_pos);

	printf("stop move: roleid=%d pos.x=%f, pos.y=%f, pos.z=%f\n", _parent->ID.id, _parent->pos.x, _parent->pos.y, _parent->pos.z); 
	CheckSpecialMove();
}

*/


//�������ƶ����ж��Ƿ����̨��
//̨�ӵĸ߶���220, ����220��ʱ��ʼ�ж��Ƿ����̨��
void collision_raid_player_imp::CheckSpecialMove()
{
	A3DVECTOR & cur_pos = _parent->pos;
	if(move_state == STATE_NORMAL)
	{
		if(cur_pos.y < 220 && cur_pos.y > 180)
		{
			move_state = STATE_START_DROP;
		}
	}
	else if(move_state == STATE_START_DROP)
	{
		if(cur_pos.y < 215 && cur_pos.y > 180)
		{
			move_state = STATE_DROP;
		
			ResetPos();
			gplayer* pPlayer = GetParent();
			raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
			if(pPlayer->IsBattleOffense())
			{
				((collision_raid_world_manager*)pManager)->AddDefenderScore(1);
			}
			else if(pPlayer->IsBattleDefence())
			{
				((collision_raid_world_manager*)pManager)->AddAttackerScore(1);
			}
		}
	}
	else if(move_state == STATE_DROP)
	{
		if(cur_pos.y > 220)
		{
			move_state = STATE_NORMAL;
		}
		//��ҿ���drop�Ժ�û�и��µ�normal״̬(û�з�λ����Ϣ��
		//���ʱ����ܻ������ٴ�Drop
		else if(cur_pos.y < 220 && cur_pos.y > 215)
		{
			move_state = STATE_START_DROP;
		}
		else if(cur_pos.y < 210)
		{
			ResetPos();
		}
	}
	printf("check move: roleid=%d, pos.x=%f, pos.y=%f, pos.z=%f state=%d\n", _parent->ID.id, _parent->pos.x, _parent->pos.y, _parent->pos.z, move_state);
}

#define PI 3.14159265

//������䵽̨�����棬������ҵ�λ��
void collision_raid_player_imp::ResetPos()
{
	A3DVECTOR center_pos(2.647, 221, -247.801); 

	float angel = abase::Rand(float(0.0), (float)(2*PI));
	float radius = abase::Rand(float(3.0), float(35.0));
	
	A3DVECTOR new_pos;
	new_pos.x = center_pos.x + radius * cos(angel); 
	new_pos.y = center_pos.y + 0.2;
	new_pos.z = center_pos.z + radius * sin(angel); 


	/*
	int retry_cnt = 20;
	while(retry_cnt--)
	{
		bool retry = false;
		new_pos.x = center_pos.x + abase::Rand(-radius, radius);
		new_pos.y = center_pos.y + 3;
		new_pos.z = center_pos.z + abase::Rand(-radius, radius);

		world::object_info info;
		for(size_t i = 0; i < id_list.size(); ++i)
		{
			if(id_list[i] == _parent->ID.id) continue;
			bool rst = _plane->QueryObject(XID(GM_TYPE_PLAYER, id_list[i]), info);
			if(!rst) continue;

			if(new_pos.horizontal_distance(info.pos) < 2) 
			{
				retry = true;
				break;
			}
		}
		if(!retry) break;
	}
	*/
	printf("reset old pos: role_id=%d,oldpos.x=%f,oldpos.y=%f,oldpos.z=%f\n", _parent->ID.id, _parent->pos.x, _parent->pos.y, _parent->pos.z);

	_parent->pos = new_pos;
	_runner->notify_pos(new_pos);

	_runner->begin_transfer();
	_runner->enter_world();
	_runner->end_transfer();

	printf("reset new pos: role_id=%d,newpos.x=%f,newpos.y=%f,newpos.z=%f\n", _parent->ID.id, new_pos.x, new_pos.y, new_pos.z);
}


/*
void collision_raid_player_imp::PlayerGetCollisionPlayerPos(int roleid)
{
	world::object_info info;
	XID id(GM_TYPE_PLAYER, roleid);

	if(_plane->QueryObject(id, info))
	{
		_runner->collision_player_pos(roleid, info.pos);

	}
}
*/


void collision_raid_player_imp::OnResurrect()
{
	session_logon_invincible *pSession = new session_logon_invincible(this, 5);
	AddStartSession(pSession);
}

void collision_raid_player_imp::HandleCheatInfo(bool is_cheat, int cheat_counter)
{
	_runner->cs6v6_cheat_info(is_cheat, cheat_counter); 
}

void collision_raid_player_imp::DebugChangeCollisionResult(int faction, int score)
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	//����
	if(faction == 1)
	{
		((collision_raid_world_manager*)pManager)->AddAttackerScore(score);
	}
	//�ط�
	else if(faction == 2)
	{
		((collision_raid_world_manager*)pManager)->AddDefenderScore(score);
	}
}

void collision2_raid_player_imp::HandleRaidResult()
{
	gplayer* pPlayer = GetParent();
	ActiveCombatState(false);

	_commander->DenyCmd(controller::CMD_SKILL);
	_commander->DenyCmd(controller::CMD_ATTACK);

	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	char result = 0;
	int raid_result = pManager->GetRaidResult();
	if((raid_result == RAID_ATTACKER_WIN && pPlayer->IsBattleOffense()) || (raid_result == RAID_DEFENDER_WIN && pPlayer->IsBattleDefence()))
	{
		result = 1;
	}
	else if((raid_result == RAID_DEFENDER_WIN &&  pPlayer->IsBattleOffense()) || (raid_result == RAID_ATTACKER_WIN && pPlayer->IsBattleDefence()))
	{
		result = 2;
	}
	else if(raid_result == RAID_DRAW)
	{
		result = 0;
	}

	_runner->collision_raid_result(result, 0, 0); 
	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF | filter::FILTER_MASK_BUFF);

	GLog::log(GLOG_INFO, "����˳����6V6����: roleid=%d", _parent->ID.id);
}



