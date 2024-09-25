#include "tower_raid_player.h"
#include <glog.h>
#include "../clstab.h"
#include "../world.h"
#include "tower_raid_world_manager.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "raid_player.h"
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
#include "raid_world_manager.h"
#include "../faction.h"
#include <factionlib.h>
#include "../general_indexer_cfg.h"

DEFINE_SUBSTANCE( tower_raid_player_imp, raid_player_imp, CLS_TOWER_RAID_PLAYER_IMP)

tower_raid_player_imp::tower_raid_player_imp()
{
}

/*int tower_raid_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length)
			{
				int score = GetScoreByKillMonster(msg.param);
				if(score > 0)
				{
					bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
					pManager->OnKillMonster(this, score);
				}
				return 0;
			}	
		}
		break;
	}
	return raid_player_imp::MessageHandler(msg);	
}*/

struct cost_item
{
	int idx;
	int count;
};
bool tower_raid_player_imp::ValidPlayer()
{
	tower_raid_world_manager* pManager = (tower_raid_world_manager*)_plane->GetWorldManager();
	if(GetTowerLevel() > -1 && pManager->GetInitTowerLevel() > GetTowerLevel())
	{
		_timeout = 3;
		__PRINTF( "���ѡ����δͨ���Ĺؿ�%d ����߳���� %d, �����ͨ������=%d, ���ѡ�����%d\n", _timeout, _parent->ID.id, pManager->GetInitTowerLevel(), GetTowerLevel());
		GLog::log(GLOG_INFO, "���ѡ����δͨ���Ĺؿ�: userid= %d, status = %d, timeout = %d, raid_id=%d, raid_world_tag =%d, end_timestamp=%d, cur_time=%d, tower_level=%d, select_tower_level=%d", _parent->ID.id, pManager->GetRaidStatus(), _timeout, pManager->GetRaidID(), pManager->GetWorldTag(), pManager->GetRaidEndstamp(), (int)g_timer.get_systime(), pManager->GetInitTowerLevel(), GetTowerLevel());
		_runner->kickout_raid( pManager->GetRaidID(), _timeout );
		return false;
	}

	const raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(pManager->GetRaidID());
	if(!rwt)
	{
		return false;
	}
	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_CONFIG& raid_config = *(const TOWER_TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt->rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		printf("��Ч�ĸ���ģ��: %d\n", rwt->rwinfo.raid_template_id);
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
	if(GetRebornCount() < raid_config.renascence_count)
	{
		_timeout = 3;
		__PRINTF( "��ҷ����ȼ����� %d ����߳���� %d, �����ȼ�: %d, ��������ȼ�: %d", _timeout, _parent->ID.id, GetRebornCount(), raid_config.renascence_count);
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

	return true;
}

void tower_raid_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	raid_player_imp::OnDeath(lastattack, is_pariah, faction_battle, is_hostile_duel, time);
	bool kill_much = false;
	gplayer* pPlayer = GetParent();
	tower_raid_world_manager* pManager = (tower_raid_world_manager*)_plane->GetWorldManager();
	pManager->OnPlayerDeath(pPlayer, lastattack.id, pPlayer->ID.id, _battle_faction, kill_much);
}

void tower_raid_player_imp::PlayerStartRandomTowerMonster(char client_idx)
{
	tower_raid_world_manager* pManager = (tower_raid_world_manager*)_plane->GetWorldManager();
	pManager->GenLevelMonsters(GetParent(), pManager->GetCurrTowerLevel(), client_idx);
}

void tower_raid_player_imp::OnResurrect()
{
	FullHPAndMP();
	ResetTowerPlayerPet();
}
