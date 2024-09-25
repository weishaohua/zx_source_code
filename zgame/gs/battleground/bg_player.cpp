#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "bg_player.h"
#include "../usermsg.h"
#include "../actsession.h"
#include "../userlogin.h"
#include "../playertemplate.h"
#include "../serviceprovider.h"
#include <common/protocol_imp.h>
#include "../task/taskman.h"
#include "../playerstall.h"
#include "../pvplimit_filter.h"
#include <glog.h>
#include "../pathfinding/pathfinding.h"
#include "../player_mode.h"
#include "../cooldowncfg.h"
#include "../template/globaldataman.h"
#include "../petnpc.h"
#include "../item_manager.h"
#include "../netmsg.h"
#include "../mount_filter.h"
#include "bg_world_manager.h"
#include "../faction.h"
#include "../invincible_filter.h"

DEFINE_SUBSTANCE( bg_player_imp, gplayer_imp, CLS_BG_PLAYER_IMP )

void bg_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void bg_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}
	
int bg_player_imp::MessageHandler( const MSG& msg )
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
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			return 0;
		}
		break;
		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			return 0;
		}
		break;
		case GM_MSG_BATTLE_INFO_CHANGE:
		{
			return 0;
		}
		break;
	}
	return gplayer_imp::MessageHandler( msg );
}

void bg_player_imp::OnTimeOutKickout()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	int world_tag = -1;
	A3DVECTOR pos(0,0,0);
	pManager->GetLogoutPos(this,world_tag,pos);
	LongJump(pos,world_tag);
}

void bg_player_imp::OnHeartbeat( size_t tick )
{
	gplayer_imp::OnHeartbeat( tick );
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if( _parent->b_disconnect )
	{
		// �Ѿ��������,�����˵ȴ�������
		return;
	}
	if( _timeout >= 0 )
	{
		// �Ѿ����߳�׼��״̬�£�_timeout��һ�����߳������������������ж�
		--_timeout;
		if( _timeout <= 0 )
		{
			if(_pstate.IsMarketState())
			{
				CancelPersonalMarket();
			}
			else if(_pstate.IsBindState())
			{
				LeaveBindState();
			}
			_old_pos.y += 0.5f;
			if(!_parent->b_disconnect)
			{
				if(_pstate.IsTradeState())
				{
					LostConnection();
				}
				else
				{
					OnTimeOutKickout();
				}
			}
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
		GLog::log(GLOG_INFO, "������ս��״̬, �����ս�����߳�ȥ��: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return;
	}

	// �ж��Ƿ���ս������Ϸ������
	if(!ValidPlayer()) return;
	
	// ������
	HandleDeath();
}


bool bg_player_imp::ValidPlayer()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// ���ڼ�����жϣ����𲻷����߳�ȥ
	if( _basic.level < pManager->GetLevelMin() || _basic.level > pManager->GetLevelMax() )
	{
		// ������������ҽ���ս����
		// 3����߳�����ң�������Ϣ
		_timeout = 3;
		__PRINTF( "��Ҽ����ڷ�Χ�� %d ����߳���� %d, ����:%d, ���󼶱�Χ %d - %d\n", 
			_timeout, _parent->ID.id, _basic.level, pManager->GetLevelMin(), pManager->GetLevelMax() );
		GLog::log(GLOG_INFO, "�����ս����Ϊ�ȼ������ϱ��߳�ȥ��: userid= %d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}

	int rebornCnt = GetRebornCount();
	//������Ҳ��ܲμӵ�ս��
	if(pManager->IsRebornBattle() && rebornCnt != 1)
	{
		_timeout = 3;
		//��ͨ��Ҳ��ܽ������ս��
		GLog::log(GLOG_INFO, "��ͨ����ڷ���ս�����߳�: userid=%d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id,	pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}
	//��ͨ��Ҳ��ܲμӵ�ս��
	else if(pManager->IsNormalBattle()&& rebornCnt != 0)
	{
		//������Ҳ��ܽ�����ͨս��
		_timeout = 3;
		GLog::log(GLOG_INFO, "�����������ͨս�����߳�ȥ��: userid=%d, status = %d, timeout = %d, battle_id=%d, battle_world_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id,	pManager->GetBattleStatus(), _timeout, pManager->GetBattleID(), pManager->GetWorldTag(), pManager->GetBattleEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_instance( _timeout );
		return false;
	}
	return true;

}

void bg_player_imp::PostLogin(const userlogin_t & user, char trole, char loginflag)
{
	//ֻ�п���ϵ�ս����player����ֱ��login����
	ASSERT(gmatrix::IsZoneServer());
	gplayer_imp::PostLogin(user, trole, loginflag);
	_battle_faction = trole;
}

void bg_player_imp::PlayerEnterWorld()
{
	if(gmatrix::IsZoneServer())
	{
		__PRINTF( "��� %d ���ս�� %d ������!!!\n", _parent->ID.id, _plane->GetTag() );
		gplayer* pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);

		PlayerEnter();
		gplayer_imp::PlayerEnterWorld();

		PostPlayerEnterWorld();

		_old_world_tag = 615;
		A3DVECTOR pos(-398, 175, 138);
		_old_pos = pos;

		bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
		_runner->enter_battleground( _battle_faction, (unsigned char)(pManager->GetBattleType()), pManager->GetBattleID(), pManager->GetBattleEndstamp() );

		return;
	}
	// ս���в�Ӧ�ó���ֱ�ӽ�������
	ASSERT( false );
}

void bg_player_imp::PlayerLeaveWorld()
{
	__PRINTF( "��� %d ��ս����ͼ %d ������!!!\n", _parent->ID.id, _plane->GetTag() );
	PlayerLeave();
	gplayer_imp::PlayerLeaveWorld();
}

void bg_player_imp::PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos )
{
	__PRINTF( "��� %d ����ս����ͼ %d ��!!!\n", _parent->ID.id, _plane->GetTag() );

	if(gmatrix::IsZoneServer())
	{
		_old_world_tag = 615;
		A3DVECTOR pos(-398, 175, 138);
		_old_pos = pos;

		gplayer* pPlayer = GetParent();
		pPlayer->SetExtraState(gplayer::STATE_CROSSZONE);
	}
	else
	{
		//����п��ܴ�һЩ�����ͼ����ս�����������ᣬ��᱾���ܴ���
		world_manager * manager = gmatrix::Instance()->FindWorld(old_tag);
		if(manager->GetWorldLimit().savepoint && manager->GetSavePoint().tag > 0)
		{
			_old_world_tag = manager->GetSavePoint().tag;
			_old_pos = manager->GetSavePoint().pos;
		}
		else
		{
			_old_world_tag = old_tag;
			_old_pos = old_pos;
		}
	}

	// ���һ��Ҫ��gplayer_imp::PlayerEnterMap();֮ǰ���
	// Ҫ��PlayerEnterMap֮ǰȷ�������
	gplayer_imp::PlayerEnterMap();
	//PlayerEnter()Ҫ��PlayerEnterMap֮����ã���ΪPlayerEnterMap����session���imp�����µ�
	PlayerEnter();
}

void bg_player_imp::PlayerLeaveMap()
{
	__PRINTF( "��� %d �뿪ս����ͼ %d ��!!!\n", _parent->ID.id, _plane->GetTag() );
	PlayerLeave();
	gplayer_imp::PlayerLeaveMap();
}

int bg_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int bg_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

void bg_player_imp::PlayerEnter()
{
	EnableFreePVP( true );
	//����ս��ר����Ʒ
	RecycleBattleItem();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	if( BF_ATTACKER == _battle_faction )
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		_attack_faction = FACTION_BATTLEDEFENCE;
		_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
	}
	else if( BF_DEFENDER == _battle_faction ) 
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
		_attack_faction = FACTION_BATTLEOFFENSE;
		_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
	}
	else if( BF_VISITOR == _battle_faction ) 
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_VISITOR);
		SetMeleeSealMode(true);
		SetDietSealMode(true);
		SetSilentSealMode(true);
		_attack_faction = 0;
		_defense_faction = 0;
		_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, 7200));
	}
	else if( BF_MELEE == _battle_faction ) 
	{
		_attack_faction = 0;
		_defense_faction = 0xFFFFFFFF;
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}

	A3DVECTOR temp(0,0,0);
	pManager->GetRandomEntryPos(temp,_battle_faction);
	temp.y += 3.0f;
	_parent->pos = temp;
	pManager->PlayerEnter( pPlayer, _battle_faction );
	__PRINTF("��ҵ�½ս�����꣺ world_tag=%d, pos(%f, %f, %f)\n", pManager->GetWorldTag(), temp.x, temp.y, temp.z);
	// ֪ͨ�ͻ���
	_runner->enter_battleground( _battle_faction, (unsigned char)(pManager->GetBattleType()), pManager->GetBattleID(), pManager->GetBattleEndstamp() );
}


void bg_player_imp::PlayerLeave()
{
	EnableFreePVP( false );
	//����ս��ר����Ʒ
	RecycleBattleItem();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
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
		_filters.RemoveFilter(FILTER_BATTLEGROUND_INVINCIBLE);
		SetMeleeSealMode(false);
		SetDietSealMode(false);
		SetSilentSealMode(false);
	}
	pManager->PlayerLeave( pPlayer, _battle_faction,_cond_kick, IsLogoutDisconnected());
	_filters.ClearSpecFilter( filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BATTLE_BUFF | filter::FILTER_MASK_BUFF);
	// ֪ͨ�ͻ���
	_runner->leave_battleground();
}


const A3DVECTOR& bg_player_imp::GetLogoutPos( int &world_tag )
{
	world_tag = _old_world_tag;
	return _old_pos;
}

bool bg_player_imp::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//����ͨ�����ж�������
	amsg.force_attack = 0;
	return true;
}

bool bg_player_imp::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	emsg.force_attack = 0;
	return true;
}

void bg_player_imp::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void bg_player_imp::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

gactive_imp::attack_judge bg_player_imp::GetHookAttackJudge()
{
	return bg_player_imp::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge bg_player_imp::GetHookEnchantJudge()
{
	return bg_player_imp::__GetHookEnchantJudge;
}

gactive_imp::attack_fill bg_player_imp::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill bg_player_imp::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}


bool bg_player_imp::PlayerUseItem(int where, size_t inv_index, int item_type,size_t count)
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return false;
	}

	if( pManager->IsForbiddenItem(item_type) )
	{
		return false;
	}

	
	return gplayer_imp::PlayerUseItem(where,inv_index,item_type,count);
}

bool bg_player_imp::PlayerUseItemWithArg(int where,size_t inv_index,int item_type,size_t count, const char * buf, size_t buf_size)
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return false;
	}

	if( pManager->IsForbiddenItem(item_type) )
	{
		return false;
	}
	
	return gplayer_imp::PlayerUseItemWithArg(where,inv_index,item_type,count,buf,buf_size);
}
