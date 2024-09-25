#include <stdio.h>
#include <sys/types.h>
#include "../clstab.h"
#include "../task/taskman.h"
#include "bf_player_imp.h"
#include "bf_world_manager.h"
#include "../userlogin.h"
#include "../usermsg.h"

DEFINE_SUBSTANCE(bf_player_imp,gplayer_imp,CLS_BF_PLAYER_IMP)

bf_player_imp::bf_player_imp()
{
	_attack_faction = 0;
	_defense_faction = 0;
	_old_world_tag = 0;
	_old_pos = A3DVECTOR(0,0,0);
	_timeout = -1;
	_kickout = 0;
	_battle_result_sent = false;
	_turret_counter = 0;
	_battle_info_seq = 0;
}

void bf_player_imp::FillAttackMsg(const XID& target,attack_msg& attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void bf_player_imp::FillEnchantMsg(const XID& target,enchant_msg& enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}

int bf_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int bf_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

int bf_player_imp::MessageHandler(const MSG& msg)
{
	switch(msg.message)
	{
		case GM_MSG_TURRET_NOTIFY_LEADER:
		{
			//ÿ�յ�һ�δ���֪ͨ ��3����֮�ڶ����ܽ����ٴ��ٻ�
			_turret_counter = 3;
			return 0;
		}
		break;
		
		case GM_MSG_ATTACK:
		{
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
			XID attacker = ack_msg.ainfo.attacker;
			//��������������Һ�NPC
			//�Լ���������
			if(attacker.id == _parent->ID.id) return 0;
			if(!(ack_msg.target_faction & (GetFaction())))
			{
				//��ϵ����ȷ�򷵻�
				return 0;
			}
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
			if(!ech_msg.helpful )
			{       
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
				if(!(ech_msg.target_faction & (GetFaction())))
				{       
					return 0;
				}
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}
			else
			{       
				if(!(ech_msg.attacker_faction & (GetFaction())))
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
			if(!ech_msg.helpful )
			{
				return 0;
			}
			if(!(ech_msg.attacker_faction & (GetFaction())))
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
			//���������delivery��
		}
		return 0;
	}
	return gplayer_imp::MessageHandler(msg);
}

void bf_player_imp::OnHeartbeat(size_t tick)
{
	gplayer_imp::OnHeartbeat(tick);
	if(_turret_counter) --_turret_counter;
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	if(GetParent()->b_disconnect)
	{
		//�Ѿ��������,�����˵ȴ�������
		return;
	}
	if(_timeout >= 0)
	{
		//�Ѿ����߳�׼��״̬��,_timeout��һ�����߳�,���������������ж�
		//�����˸�������,һ����Ҫ�߳�
		--_timeout;
		if(_timeout <= 0)
		{
			++_kickout;
		}
		if(_kickout && _timeout <= 0)
		{
			if(_kickout > 5)
			{
				LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			}
			else
			{
				LeaveAbnormalState();
				_timeout = 3;
				PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			}
		}
		return;
	}
	//���½����ж��Ƿ���Ҫ�߳����
	if(CheckGMPrivilege())
	{
		//gm����
		return;
	}
	if(!pManager->IsBattleStart())
	{
		//������״̬
		//xx����߳������,������Ϣ
		_timeout = abase::Rand(pManager->GetKickoutTimeMin(),pManager->GetKickoutTimeMax());
		__PRINTF("������ս��״̬: %d,%d ����߳����\n",pManager->GetBattleStatus(),_timeout);
		_runner->kickout_instance(_timeout);
		return;
	}
	if(pManager->IsBattlePreparing())
	{
		//׼��״̬ʱ���λ��
		gplayer* pPlayer = GetParent();
		if(!pManager->CheckValidPreparePos(_battle_faction,pPlayer->pos))
		{
			_timeout = DEFAULT_KICKOUT_TIME;
			_runner->kickout_instance(_timeout);
			return;
		}
	} 
	//ȫ�ּ��λ��,�����
	gplayer* pPlayer = GetParent();
	if(pManager->CheckInvalidPos(pPlayer->pos))
	{
		_timeout = DEFAULT_KICKOUT_TIME;
		_runner->kickout_instance(_timeout);
		return;
	}
	if(OI_GetMafiaID() != pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER)
		&& OI_GetMafiaID() != pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER)
		&& OI_GetMafiaID() != pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT) )
	{
		_timeout = DEFAULT_KICKOUT_TIME;
		__PRINTF("���: %d ������Ϣ����: %d ����߳�\n",_parent->ID.id, _timeout );
		_runner->kickout_instance(_timeout);
		return;
	}
        if(pManager->IsBattleInfoRefresh(_battle_info_seq))
        {       
		pManager->SendBattlefieldInfo(this);
        } 
}

void bf_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{                       
	//���õǳ�ʹ�õ�ԭ����
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;
}               

void bf_player_imp::PlayerEnterWorld()
{
	__PRINTF("���: %d ����ս��world %d ��!!!\n",_parent->ID.id,_plane->GetTag());
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	if(OI_GetMafiaID() == pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER))
	{
		_battle_faction = bf_world_manager_base::BF_ATTACKER;
		_attack_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		_defense_faction = FACTION_BATTLEOFFENSE;
		//Э��
		_runner->enter_battlefield(pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER),
			pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER),
			pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT),
			pManager->GetBattleID(),pManager->GetBattleEndTimestamp());
		pManager->PlayerEnter(pPlayer,bf_world_manager_base::BF_ATTACKER);
	}
	else if(OI_GetMafiaID() == pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER))
	{
		_battle_faction = bf_world_manager_base::BF_DEFENDER;
		_attack_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
		_defense_faction = FACTION_BATTLEDEFENCE;
		_runner->enter_battlefield(pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER),
			pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER),
			pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT),
			pManager->GetBattleID(),pManager->GetBattleEndTimestamp());
		pManager->PlayerEnter(pPlayer,bf_world_manager_base::BF_DEFENDER);
	}
	else if(OI_GetMafiaID() == pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT))
	{
		_battle_faction = bf_world_manager_base::BF_ATTACKER_ASSISTANT;
		_attack_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		_defense_faction = FACTION_BATTLEOFFENSE;
		_runner->enter_battlefield(pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER),
			pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER),
			pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT),
			pManager->GetBattleID(),pManager->GetBattleEndTimestamp());
		pManager->PlayerEnter(pPlayer,bf_world_manager_base::BF_ATTACKER_ASSISTANT);
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}
	pManager->SendBattlefieldInfo(this);
	EnableFreePVP(true);
	gplayer_imp::PlayerEnterWorld();
}

void bf_player_imp::PlayerLeaveWorld()
{
	__PRINTF("���: %d �뿪ս��world %d ��!!!\n",_parent->ID.id,_plane->GetTag());
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	pManager->PlayerLeave(pPlayer,_battle_faction);
	_runner->leave_battlefield();
	gplayer_imp::PlayerLeaveWorld();
}

void bf_player_imp::PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos)
{
	__PRINTF("ս���в�Ӧ�ó����л�����\n");
	ASSERT(false);
}

void bf_player_imp::PlayerLeaveMap()
{
	__PRINTF("ս���в�Ӧ�ó����л�����\n");
	ASSERT(false);
}

void bf_player_imp::OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time)
{
	__PRINTF("ս������������Ϣ: ��� %d �� %d ɱ����\n",_parent->ID.id,lastattack.id);
	gplayer* pPlayer = GetParent();
	if(pPlayer->IsZombie()) return;
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	//ɱ�˱�ɱ��¼�߼�
	if(lastattack.IsPlayerClass() && pManager->IsBattleStart())
	{
		//����Ǳ���ɱ��,����ս���ڽ�����,����delivery˭��˭ɱ��
		bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
                GMSV::SendBattlefieldDeath(OI_GetMafiaID(),pPlayer->ID.id,lastattack.id,pManager->GetBattleID());
	}
	gplayer_imp::OnDeath(lastattack,false,true);
	_resurrect_hp_recover = 1.0f;
}

void bf_player_imp::GetLogoutPos(int& world_tag,A3DVECTOR& pos)
{
	world_tag = _old_world_tag;
	pos = _old_pos;
}

void bf_player_imp::SendNormalChat(char channel,const void* buf,size_t len)
{
	if(GMSV::CHAT_CHANNEL_TRADE != channel && GMSV::CHAT_CHANNEL_LOCAL != channel)
	{
		return;
	}
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	pManager->BattleChat(GetFaction(),buf,len,channel,GetParent()->ID.id);
}

bool bf_player_imp::QueryBattlefieldConstructInfo(int type,int page)
{
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	return pManager->QueryBattlefieldConstructInfo(this,type,page);
}

bool bf_player_imp::BattlefieldContribute(int res_type)
{
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	return pManager->BattlefieldContribute(this,res_type);
}

bool bf_player_imp::BattlefieldConstruct(int key,int type,int cur_level)
{
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	return pManager->BattlefieldConstruct(this,key,type,cur_level);
}

int bf_player_imp::CheckUseTurretScroll()
{
	if(_turret_counter >0)
	{
		return S2C::ERR_CONTROL_TOO_MANY_TURRETS;
	}
	return 0;
}

