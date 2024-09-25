#include "mafia_battle_raid_player.h"
#include "mafia_battle_raid_world_manager.h"
#include "../clstab.h"
#include <factionlib.h>

DEFINE_SUBSTANCE( mafia_battle_raid_player_imp, raid_player_imp, CLS_MAFIA_BATTLE_RAID_PLAYER_IMP)


mafia_battle_raid_player_imp::mafia_battle_raid_player_imp()
{}

mafia_battle_raid_player_imp::~mafia_battle_raid_player_imp()
{}

void mafia_battle_raid_player_imp::PlayerEnter()
{
	EnableFreePVP( true );
	//���ո���ר����Ʒ
	RecycleBattleItem();
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

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
		SetMeleeSealMode(true);
		SetDietSealMode(true);
		SetSilentSealMode(true);
		_attack_faction = 0;
		_defense_faction = 0;
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}

	if(_battle_faction != 0)
	{
		A3DVECTOR temp(0,0,0);
		pManager->GetRandomEntryPos(temp,_raid_faction);
		if(temp.x != 0 || temp.y != 0 || temp.z != 0)
		{
			_parent->pos = temp;
		}
		__PRINTF("��Ҹ�����½λ��: x=%3f, y=%3f, z=%3f\n", _parent->pos.x, _parent->pos.y, _parent->pos.z);
	}
	else
	{
		_timeout = 3;
		GLog::log(GLOG_INFO, "����ڸ�����Ϊ����Ӫ���߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_tag =%d, end_timestamp=%d, cur_time=%d",
			_parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( pManager->GetRaidID(), _timeout );
	}

	bool first_time = ((mafia_battle_raid_world_manager*)(pManager))->IsFirstTimeEnter(pPlayer);
	if(first_time && ( _battle_faction == RF_ATTACKER|| _battle_faction == RF_DEFENDER )) 
	{
		DeliverBattleItem();
	}

	pManager->PlayerEnter( pPlayer, _battle_faction, _reenter);
	// ֪ͨ�ͻ���
	_runner->enter_raid( _battle_faction, (unsigned char)(pManager->GetRaidType()), pManager->GetRaidID(), pManager->GetRaidEndstamp() );
}

#define MAFIA_BATTLE_AWARD_LIFE_POTION 54871 
#define MAFIA_BATTLE_AWARD_MANA_POTION 54872
void mafia_battle_raid_player_imp::DeliverBattleItem()
{
	if(!_pstate.IsNormalState() && !_pstate.IsBindState()) return;

	if(_inventory.GetEmptySlotCount() >= 2)
	{
		DeliverItem(MAFIA_BATTLE_AWARD_LIFE_POTION, 1, true, 3600, 0);
		DeliverItem(MAFIA_BATTLE_AWARD_MANA_POTION, 1, true, 3600, 0);
	}
	else if(_inventory.GetEmptySlotCount() == 1)
	{
		DeliverItem(MAFIA_BATTLE_AWARD_LIFE_POTION, 1, true, 3600, 0);

		/*
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * data = gmatrix::GetDataMan().generate_item(MAFIA_BATTLE_AWARD_MANA_POTION,&tag,sizeof(tag));
		if(data)
		{
			data->expire_date = g_timer.get_systime() + 3600;
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0);
		}
		*/
	}
	/*
	else if(_inventory.GetEmptySlotCount() == 0)
	{
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * data = gmatrix::GetDataMan().generate_item(MAFIA_BATTLE_AWARD_LIFE_POTION,&tag,sizeof(tag));
		if(data)
		{
			data->expire_date = g_timer.get_systime() + 3600;
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0);
		}

		element_data::item_tag_t tag2 = {element_data::IMT_CREATE,0};
		item_data * data2 = gmatrix::GetDataMan().generate_item(MAFIA_BATTLE_AWARD_MANA_POTION,&tag2,sizeof(tag2));
		if(data)
		{
			data2->expire_date = g_timer.get_systime() + 3600;
			DropItemData(_plane,_parent->pos,data2,_parent->ID,0,0);
		}
	}
	*/

	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	GLog::log(GLOG_INFO, "��ҽ����ս������ô���������, roleid=%d, raid_id=%d, raid_tag=%d, empty_slot=%d", 
			_parent->ID.id, pManager->GetRaidID(),pManager->GetWorldTag(),_inventory.GetEmptySlotCount() );

}


struct cost_item
{
	int idx;
	int count;
};

bool mafia_battle_raid_player_imp::ValidPlayer()
{
	mafia_battle_raid_world_manager* pManager = dynamic_cast<mafia_battle_raid_world_manager*>(_plane->GetWorldManager());
	const raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(pManager->GetRaidID());
	if(!rwt)
	{
		return false;
	}
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt->rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		printf("��Ч�ĸ���ģ��: %d\n", rwt->rwinfo.raid_template_id);
		return false;
	}	

	if(RF_NONE == _battle_faction)
	{
		__PRINTF("\n\nERR: ��ҵ�½��ս��������Ӫ�������\n\n");
		return false;
	}

	//�����븱����Ʒ
	int joinItemId = raid_config.required_item_id;
	int joinItemCnt = raid_config.required_item_count;
	std::vector<cost_item> costItems;
	item_list& inv = GetInventory();
	
	if(joinItemId > 0 && joinItemCnt > 0)
	{
		bool flag = false;
		for(size_t i = 0; i < inv.Size(); i ++)
		{
			if(inv[i].type == joinItemId && joinItemCnt > 0)
			{
				int invCnt = inv[i].count;
				cost_item ci;
				ci.idx = i;
				ci.count = joinItemCnt <= invCnt ? joinItemCnt : invCnt;
				costItems.push_back(ci);
				joinItemCnt -= invCnt;
				if(joinItemCnt <= 0)
				{
					flag = true;
					break;
				}
			}
		}
		if(!flag)
		{
			_timeout = 3;
			__PRINTF( "���û�н��븱��������Ʒ %d ����߳���� %d, ��ƷID=%d, ����%d\n", _timeout, _parent->ID.id, raid_config.required_item_id, raid_config.required_item_count);
			GLog::log(GLOG_INFO, "���û�н��븱��������Ʒ���߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
			_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
			return false;
		}
	}

	// ���ڼ�����жϣ����𲻷����߳�ȥ
	if( _basic.level < raid_config.player_min_level || _basic.level > raid_config.player_max_level)
	{
		// ������������ҽ��븱����
		// 3����߳�����ң�������Ϣ
		_timeout = 3;
		__PRINTF( "��Ҽ����ڷ�Χ�� %d ����߳���� %d, ����:%d, ���󼶱�Χ %d - %d\n", _timeout, _parent->ID.id, _basic.level, pManager->GetLevelMin(), pManager->GetLevelMax() );
		GLog::log(GLOG_INFO, "����ڸ�����Ϊ�ȼ������ϱ��߳�ȥ��: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}

	short cls = GetParent()->GetClass();
	if(!object_base_info::CheckCls(cls, raid_config.character_combo_id, raid_config.character_combo_id2))
	{
		_timeout = 3;
		__PRINTF( "���ְҵ���ڷ�Χ�� %d ����߳���� %d, ְҵ:%d, ְҵ����1:0x%llx, ְҵ����2:0x%lld\n", _timeout, _parent->ID.id, cls, raid_config.character_combo_id, raid_config.character_combo_id2);
		GLog::log(GLOG_INFO, "���ְҵ�����ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.required_race == 0 && !(cls <= 0x1F || (cls > 0x3F && cls <= 0x5F)))
	{
		_timeout = 3;
		__PRINTF( "������岻�� %d ����߳���� %d, ְҵ: %d\n", _timeout, _parent->ID.id, cls);
		GLog::log(GLOG_INFO, "������岻���ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.required_race == 1 && !((cls > 0x1F && cls <= 0x3F) || (cls > 0x5F && cls <= 0x7F)))
	{
		_timeout = 3;
		__PRINTF( "������岻�� %d ����߳���� %d, ְҵ: %d\n", _timeout, _parent->ID.id, cls);
		GLog::log(GLOG_INFO, "������岻���ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d\n", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(_basic.sec_level < raid_config.renascence_count)
	{
		_timeout = 3;
		__PRINTF( "��ҷ����ȼ����� %d ����߳���� %d, �����ȼ�: %d, ��������ȼ�: %d", _timeout, _parent->ID.id, _basic.sec_level, raid_config.renascence_count);
		GLog::log(GLOG_INFO, "��ҷ����ȼ������ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.god_devil_mask && !(GetCultivation() & raid_config.god_devil_mask))
	{
		_timeout = 3;
		__PRINTF( "����컯���� %d ����߳���� %d, ����컯: %d, �����컯: %d\n", _timeout, _parent->ID.id, GetCultivation(), raid_config.god_devil_mask);
		GLog::log(GLOG_INFO, "����컯�����ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}
	if(raid_config.required_money > 0 && GetMoney() < (size_t)raid_config.required_money)
	{
		__PRINTF( "��ҽ�Ǯ���� %d ����߳���� %d, ��ҽ�Ǯ: %d, �����Ǯ: %d\n", _timeout, _parent->ID.id, GetMoney(), raid_config.required_money);
		GLog::log(GLOG_INFO, "��ҽ�Ǯ�����ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}

	if(RF_VISITOR == _battle_faction && MAFIA_BATTLE_RAID_VISITOR_COST > 0 && GetMoney() < (size_t)MAFIA_BATTLE_RAID_VISITOR_COST)
	{
		__PRINTF( "��ս�����������Ϊ��ս�߽�Ǯ���� %d ����߳���� %d, ��ҽ�Ǯ: %d, �����Ǯ: %d\n", _timeout, _parent->ID.id, GetMoney(), MAFIA_BATTLE_RAID_VISITOR_COST);
		GLog::log(GLOG_INFO, "��ս�����������Ϊ��ս�߽�Ǯ�����ϱ��߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
		_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
		return false;
	}

	for(int i = 0; i < 4; i ++)
	{
		int id = raid_config.required_reputation[i].reputation_type;
		int value = raid_config.required_reputation[i].reputation_value; 
		if(id >= 0 && value > 0 && GetRegionReputation(id) < value)
		{
			__PRINTF( "����������� %d ����߳���� %d, �������: %d, ��������ID: %d, ��������ֵ%d\n", _timeout, _parent->ID.id, GetRegionReputation(id), id, value);
			GLog::log(GLOG_INFO, "����������㱻�߳�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime());
			_runner->kickout_raid( rwt->rwinfo.raid_id, _timeout );
			return false;
		}
	}	

	for(size_t i = 0; i < costItems.size(); i ++)
	{
		cost_item& ci = costItems[i];
		UseItemLog(inv, ci.idx, ci.count);
		inv.DecAmount(ci.idx, ci.count);
		_runner->player_drop_item(IL_INVENTORY, ci.idx, joinItemId, ci.count ,S2C::DROP_TYPE_USE);
	}

	if(raid_config.required_money > 0)
	{
		SpendMoney(raid_config.required_money);
		_runner->spend_money(raid_config.required_money);
	}

	if(RF_VISITOR == _battle_faction && MAFIA_BATTLE_RAID_VISITOR_COST > 0)
	{
		//��Ǯ����
		if(GetMoney() < (size_t)MAFIA_BATTLE_RAID_VISITOR_COST)
		{
			return false;
		}
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=7:hint=%d",GetParent()->ID.id,GetUserID(),MAFIA_BATTLE_RAID_VISITOR_COST,pManager->GetWorldTag());
		SpendMoney(MAFIA_BATTLE_RAID_VISITOR_COST);
		GNET::SendGFactionPkRaidDeposit(pManager->GetRaidID(), pManager->GetWorldTag(), (unsigned int)MAFIA_BATTLE_RAID_VISITOR_COST);
		_runner->spend_money(MAFIA_BATTLE_RAID_VISITOR_COST);
	}

	return true;

}

void mafia_battle_raid_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	raid_player_imp::OnDeath(lastattack, is_pariah, faction_battle, is_hostile_duel, time);
	_resurrect_hp_recover = 1.0f;
	bool kill_much = false;
	gplayer* pPlayer = GetParent();
	raid_world_manager * pManager = (raid_world_manager *)_plane->GetWorldManager();
	pManager->OnPlayerDeath(pPlayer, lastattack.id, pPlayer->ID.id, _battle_faction, kill_much);

	//��delivery�Ʒ�
	if(lastattack.IsPlayerClass() && pManager->IsRaidRunning())
	{
		GNET::SendGFactionPkRaidKill(pManager->GetRaidID(), pManager->GetWorldTag(), lastattack.id, GetParent()->ID.id, GetParent()->id_mafia);
	}
}

void mafia_battle_raid_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{
	//raid_player_imp::PostLogin������Ӫ���������_battle_faction��������
	_battle_faction = trole;
	raid_player_imp::PostLogin(user, trole, loginflag);
	_battle_faction = trole;
}

