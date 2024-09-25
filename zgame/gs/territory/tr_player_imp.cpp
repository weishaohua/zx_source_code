#include <stdio.h>
#include "../clstab.h"
#include "../userlogin.h"
#include "tr_player_imp.h"
#include "tr_world_manager.h"
#include <factionlib.h>
#include "../task/taskman.h"

DEFINE_SUBSTANCE( tr_player_imp, gplayer_imp, CLS_TR_PLAYER_IMP )

void tr_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.is_invader = false;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void tr_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.is_invader= false;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}
	
int tr_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int tr_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

int tr_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_ATTACK:
		{
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
			XID attacker = ack_msg.ainfo.attacker;
			//��������������Һ�NPC
			//�Լ���������
			if(attacker.id == _parent->ID.id) return 0;
			if(!(ack_msg.target_faction  & (GetFaction())))
			{
				//��ϵ����ȷ�򷵻�
				return 0;
			}
			if( ack_msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return 0;
			ack_msg.is_invader =  false;
			//����ѡ�����
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(msg,&ack_msg);
			return 0;
		}
		break;
		case GM_MSG_ENCHANT:
		{
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if( !ech_msg.helpful )
			{
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
				if(!(ech_msg.target_faction  & (GetFaction())))
				{
					return 0;
				}
				if( ech_msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return 0;
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}
			else
			{
				if( !(ech_msg.attacker_faction & (GetFaction())))
				{
					return 0;
				}
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;
		case GM_MSG_ENCHANT_ZOMBIE:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if( !ech_msg.helpful )
			{
				return 0;
			}
			if( !(ech_msg.attacker_faction & (GetFaction())))
			{
				return 0;
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;
		
		case GM_MSG_TURRET_NOTIFY_LEADER:
		{
			//ÿ�յ�һ�δ���֪ͨ ��3����֮�ڶ����ܽ����ٴ��ٻ�
			_turret_counter = 3;
			return 0;
		}
		break;
	}
	return gplayer_imp::MessageHandler( msg );
}

void tr_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	if(_turret_counter) --_turret_counter;
	tr_world_manager* pManager = (tr_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// �Ѿ��������,�����˵ȴ�������
		return;
	}
	if( _timeout >= 0 )
	{
		// �Ѿ����߳�׼��״̬�£�_timeout��һ�����߳������������������ж�
		--_timeout;
		if( _timeout <= 0  && _is_leaving == false)
		{
			LeaveTerritory();
		}
		return;
	}

	if(_task_timeout)
	{
		--_task_timeout;
		if(_task_timeout == 0)
		{
			PlayerTaskInterface  task_if(this);
			OnTaskCheckDeliver(&task_if,21068,0);
			_task_timeout = 10*60; 
		}
	}

	if(_is_leaving)
	{
		++_kick_timeout;
		if(_kick_timeout == 30)
		{
			LeaveAbnormalState();
			PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			GLog::log(GLOG_INFO, "����ս����˳��쳣����ǿ���˳�: userid= %d, status = %d, kick_timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());

		}
		else if(_kick_timeout == 45)
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			GLog::log(GLOG_INFO, "����ս����˳��쳣����ǿ�ƶ���: userid= %d, status = %d, kick_timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
				_parent->ID.id, pManager->GetBattleStatus(), _kick_timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		}
		return;
	}

	
	// ���½����ж��Ƿ���Ҫ�߳����
	if( CheckGMPrivilege() )
	{
		// gm����
		return;
	}
	if( !pManager->IsBattleRunning() )
	{
		// �ж��Ƿ���Ҫ���ͽ��,������
		if(!_battle_result_sent) HandleBattleResult();

		// ������״̬
		// xx����߳�����ң�������Ϣ
		_timeout = abase::Rand( pManager->GetKickoutTimeMin(), pManager->GetKickoutTimeMax() );
		__PRINTF( "������ս��״̬%d, %d ����߳����\n", pManager->GetBattleStatus(), _timeout );
		GLog::log(GLOG_INFO, "����սս��������״̬, ��Ҵ�ս���˳�: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return;
	}

	// �ж��Ƿ���ս������Ϸ������
	if(!ValidPlayer()) return;
	
	// ������
	HandleDeath();
}

bool tr_player_imp::ValidPlayer()
{
	tr_world_manager* pManager = (tr_world_manager*)_plane->GetWorldManager();
	// ���ڼ�����жϣ����𲻷����߳�ȥ
	if( _basic.level < pManager->GetLevelMin() || _basic.level > pManager->GetLevelMax() )
	{
		// ������������ҽ���ս����
		// 3����߳�����ң�������Ϣ
		_timeout = 3;
		__PRINTF( "��Ҽ����ڷ�Χ�� %d ����߳���� %d, ����:%d, ���󼶱�Χ %d - %d\n", 
			_timeout, _parent->ID.id, _basic.level, pManager->GetLevelMin(), pManager->GetLevelMax() );
		GLog::log(GLOG_INFO, "���������սս����Ϊ�ȼ������ϱ��߳�ȥ��: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, _battle_id, pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}

	return true;

}

void tr_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{                       
	//���õǳ�ʹ�õ�ԭ����
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;


	//������ȷ�ĵ�¼����	
	tr_world_manager * pManager = (tr_world_manager*)_plane->GetWorldManager();
	if(OI_GetMafiaID() == pManager->GetAttackerMafiaID() && OI_GetMafiaID() != 0 && trole == tr_world_manager::BF_ATTACKER)
	{
		_battle_faction = tr_world_manager::BF_ATTACKER; 
	}
	else if(OI_GetMafiaID() == pManager->GetDefenderMafiaID() && OI_GetMafiaID() != 0 && trole == tr_world_manager::BF_DEFENDER)
	{
		_battle_faction = tr_world_manager::BF_DEFENDER; 
	}
	//����Ӷ���߼�
	else
	{
		for(size_t i = 0; i < _inventory.Size(); ++i)
		{
			item & it = _inventory[i];
			if(it.GetItemType() == item_body::ITEM_TYPE_ASSIST_CARD)
			{
				size_t len;
				const void* buf = it.GetContent(len);
				if(len != sizeof(int) * 2) continue;

				int item_mafia_id = *(int*)buf;
				int item_territory_id = *(int*)((char*)buf +4);

				if(trole == tr_world_manager::BF_ATTACKER && item_mafia_id == pManager->GetAttackerMafiaID() && 
						(item_territory_id == pManager->GetBattleID() || item_territory_id == 0))
				{
					_battle_faction = tr_world_manager::BF_ATTACKER;
					if(item_territory_id == 0) *(int*)((char*)buf +4) = pManager->GetBattleID(); 
					if(it.expire_date == 0)
					{
						it.expire_date = g_timer.get_systime() + 3600;
					}
					_is_assist = true;
					it.Bind();
					break;
				}
				else if(trole == tr_world_manager::BF_DEFENDER && item_mafia_id == pManager->GetDefenderMafiaID() && 
						(item_territory_id == pManager->GetBattleID() || item_territory_id == 0))
				{
					_battle_faction = tr_world_manager::BF_DEFENDER;
					if(item_territory_id == 0) *(int*)((char*)buf +4) = pManager->GetBattleID(); 
					if(it.expire_date == 0)
					{
						it.expire_date = g_timer.get_systime() + 3600;
					}
					_is_assist = true;
					it.Bind();
					break;
				}
			}
		}
	}

	if(_battle_faction != 0)
	{
		A3DVECTOR temp(0,0,0);
		pManager->GetRandomEntryPos(temp,_battle_faction);
		_parent->pos = temp;
	}
	else
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "���������սս����Ϊ����Ӫ���߳�: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
	}
}               

void tr_player_imp::GetLogoutPos(int& world_tag,A3DVECTOR& pos)
{
	world_tag = _logout_tag;
	pos = _logout_pos;
}

void tr_player_imp::PlayerLeaveTerritory()
{
	if(!_pstate.CanLogout() || IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}

	LeaveTerritory();

}

void tr_player_imp::LeaveTerritory()
{
	if(_is_leaving || _battle_id == 0) return;
	_is_leaving = true;
	GNET::SendTerritoryLeave(_parent->ID.id, _battle_id, _logout_tag, _logout_pos.x, _logout_pos.y, _logout_pos.z);
	_kick_timeout = 0;
}

void tr_player_imp::PlayerEnterWorld()
{
	RecycleBattleItem();
	__PRINTF("���: %d ��������սս��world %d ��!!!\n",_parent->ID.id,_plane->GetTag());
	tr_world_manager * pManager = (tr_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	if(_battle_faction == tr_world_manager::BF_ATTACKER)
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		_attack_faction = FACTION_BATTLEDEFENCE;
		_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
	}
	else if(_battle_faction == tr_world_manager::BF_DEFENDER)
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
		_attack_faction = FACTION_BATTLEOFFENSE;
		_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
	}

	_battle_id = pManager->GetBattleID(); 

	
	if(!_is_assist && pPlayer->rank_mafia == 2)
	{
		int timeout = g_timer.get_systime() - pManager->GetBattleStartstamp();
		if(timeout < 180)
		{
			_task_timeout = 180-timeout;
		}
		else
		{
			_task_timeout = 600 - ((timeout - 180) % 600);
		}
	}
	
	pManager->PlayerEnter(pPlayer, _battle_faction);
	EnableFreePVP(true);
	_runner->enter_territory( _battle_faction, (unsigned char)(pManager->GetBattleType()), _battle_id, pManager->GetBattleEndstamp() );
	gplayer_imp::PlayerEnterWorld();
	
	GLog::log(GLOG_INFO, "����ս��ҽ���ս��: roleid=%d, line_id=%d, battle_type=%d, battle_id=%d, battle_world_tag=%d",
			_parent->ID.id, gmatrix::Instance()->GetServerIndex(),pManager->GetBattleType(), _battle_id, pManager->GetWorldTag()); 
}

void tr_player_imp::PlayerLeaveWorld()
{
	RecycleBattleItem();
	__PRINTF("���: %d �뿪����սս��world %d ��!!!\n",_parent->ID.id,_plane->GetTag());
	EnableFreePVP( false );
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsBattleOffense() )
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		
	}
	else if( pPlayer->IsBattleDefence() ) 
	{
		pPlayer->ClrObjectState( gactive_object::STATE_BATTLE_DEFENCE );
	}
	
	tr_world_manager* pManager = (tr_world_manager*)_plane->GetWorldManager();
	pManager->PlayerLeave(pPlayer,_battle_faction);
	_runner->leave_territory(); 
	gplayer_imp::PlayerLeaveWorld();
}

void tr_player_imp::PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos)
{
	__PRINTF("����սս���в�Ӧ�ó����л�����\n");
	ASSERT(false);
}

void tr_player_imp::PlayerLeaveMap()
{
	__PRINTF("����սս���в�Ӧ�ó����л�����\n");
	ASSERT(false);
}

void tr_player_imp::OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time)
{
	gplayer_imp::OnDeath(lastattack,is_pariah,true);
	_resurrect_hp_recover = 1.0f;
}


int tr_player_imp::CheckUseTurretScroll()
{
	if(_turret_counter >0)
	{
		return S2C::ERR_CONTROL_TOO_MANY_TURRETS;
	}
	return 0;
}


bool tr_player_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//����ͨ�����ж�������
	amsg.force_attack = 0;
	return true;
}

bool tr_player_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void tr_player_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void tr_player_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

void tr_player_imp::PlayerChangeTerritoryResult(int result)
{
	tr_world_manager * pManager = (tr_world_manager*)_plane->GetWorldManager();
	pManager->ChangeBattleResult(result);
}

gactive_imp::attack_judge tr_player_imp::GetHookAttackJudge()
{
	return tr_player_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge tr_player_imp::GetHookEnchantJudge()
{
	return tr_player_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill tr_player_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill tr_player_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

