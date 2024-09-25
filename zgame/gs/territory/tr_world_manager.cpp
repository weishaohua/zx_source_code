#include <stdio.h>
#include <conf.h>
#include "../faction.h"
#include "../player_imp.h"
#include <factionlib.h>

#include "tr_world_manager.h"
#include "tr_world_message_handler.h"

int tr_world_manager::Init( const char* gmconf_file, const char* servername, int tag, int index)
{
	__PRINTF( "����սս����%s ��ʼ��\n", servername );
	int rst = global_world_manager::Init( gmconf_file, servername, tag, index);
	if(rst != 0) return rst;

	if(_message_handler != NULL) delete _message_handler;
	_message_handler = new tr_world_message_handler( this, &_plane );

	ONET::Conf *conf = ONET::Conf::GetInstance();
	ONET::Conf::section_type section = "World_";
	
	section += servername;
	_battle_type = atoi( conf->find( section, "battle_type" ).c_str() );
	if(_battle_type > BT_LOW || _battle_type < BT_HIGH)
	{
		__PRINTF( "_battle_type=%d,����ȷ\n", _battle_type );
		return -10000;
	}
	
	_max_player_limit = atoi( conf->find( section, "battle_max_player_count" ).c_str() );
	if( _max_player_limit <= 0 )
	{
		_max_player_limit = DEFAULT_MAX_PLAYER_COUNT;
	}
	__PRINTF( "ս��ÿ����������Ϊ��%d\n", _max_player_limit);
	
	_kickout_time_min = atoi( conf->find( section, "battle_kickout_time_min" ).c_str() );
	if( _kickout_time_min <= 0 )
	{
		_kickout_time_min = DEFAULT_KICKOUT_TIME_MIN;
	}
	__PRINTF( "�߳����ʱ��min:%d\n", _kickout_time_min );

	_kickout_time_max = atoi( conf->find( section, "battle_kickout_time_max" ).c_str() );
	if( _kickout_time_max <= 0 )
	{
		_kickout_time_max = DEFAULT_KICKOUT_TIME_MAX;
	}
	__PRINTF( "�߳����ʱ��max:%d\n", _kickout_time_max );

	_kickout_close_time = atoi( conf->find( section, "battle_kickout_close_time" ).c_str() );
	if( _kickout_close_time <= 0 )
	{
		_kickout_close_time = DEFAULT_KICKOUT_CLOSE_TIME;
	}
	__PRINTF( "�ر��ӳ�ʱ��max:%d\n", _kickout_close_time );
	
	_min_player_level = atoi( conf->find( section, "battle_need_level_min" ).c_str() );
	if( _min_player_level < 0 || _min_player_level > 200 )
	{
		__PRINTF( "����������ȷ battle_need_level_min=%d\n", _min_player_level);
		return -10001;
	}
	__PRINTF( "battle_need_level_min=%d\n", _min_player_level);
	
	_max_player_level = atoi( conf->find( section, "battle_need_level_max" ).c_str() );
	if( _max_player_level <= 0 || _max_player_level > 200  || _max_player_level < _min_player_level)
	{
		__PRINTF( "����������ȷ battle_need_level_max=%d\n", _max_player_level);
		return -10002;
	}

	OnInit();
	return 0;
}

bool tr_world_manager::CreateBattleTerritory(int battle_id, int tag_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp)
{
	spin_autolock keeper( _battle_lock );

	GLog::log(GLOG_INFO, "����սս����������: battle_id=%d, tag_id=%d, defender_id=%d, attacker_id=%d\n",
			battle_id, tag_id, defender_id, attacker_id);
	__PRINTF("����սս����������: battle_id=%d, tag_id=%d, defender_id=%d, attacker_id=%d\n",
			battle_id, tag_id, defender_id, attacker_id);
	
	if(_battle_status != BS_READY)
	{
		__PRINTF( "��������սս��ʧ�ܣ���ǰ״̬��ΪBS_READY, _battle_status=%d\n", _battle_status );
		GLog::log( GLOG_INFO, "����սս������ʧ��,ԭ��ս��״̬��ΪBS_READY"
				"tag_id=%d, battle_id=%d ,end_timestemp=%d,_status=%d\n",
				tag_id, battle_id, end_timestamp, _battle_status );
		return false;
	}

	// �ж�ʱ���
	int t = end_timestamp - g_timer.get_systime();
	if( t < 0)
	{
		__PRINTF( "end_timestemp�������󣬳���ʱ��Ϊ�� t=%d\n", t );
		GLog::log( GLOG_INFO, "ս������ʧ��,ԭ�򣺳���ʱ��Ϊ����"
				"tag_id=%d, battle_id=%d, end_time=%d,time=%d\n",
				tag_id, battle_id, end_timestamp, t );
		return false;
	}

	_battle_id = battle_id;

	_start_timestamp = g_timer.get_systime();
	_end_timestamp = end_timestamp;
	_cur_timestamp = _end_timestamp;

	_defender_data.mafia_id = defender_id;
	_attacker_data.mafia_id = attacker_id;

	OnCreate();

	__PRINTF( "����սս�������ɹ�: battle_id=%d, tag_id=%d\n", _battle_id, GetWorldTag()); 
	GLog::log( GLOG_INFO, "����սս�������ɹ�: tag_id=%d, battle_id=%d, end_time=%d\n",
			tag_id, battle_id, end_timestamp );
	return true;
}


void tr_world_manager::PlayerEnter( gplayer* pPlayer, int faction)
{
	__PRINTF( "tr_world_manager::PlayerEnter, pPlayer=%p, id=%d, faction=%d\n", pPlayer, pPlayer->ID.id, faction);
	spin_autolock keeper( _battle_lock );
	AddMapNode( _all_list, pPlayer );
	player_battle_info tempinfo;
	memset( &tempinfo, 0, sizeof(player_battle_info) );
	tempinfo.battle_faction = faction;	

	if( faction == BF_ATTACKER )
	{
		_attacker_data.player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode( _attacker_list, pPlayer );
	}
	else if( faction == BF_DEFENDER )
	{
		_defender_data.player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode( _defender_list, pPlayer );
	}
	SendBattleInfo();
}

void tr_world_manager::PlayerLeave( gplayer* pPlayer, int faction)
{
	__PRINTF( "tr_world_manager::PlayerLeave, pPlayer=%p, id=%d, faction=%d\n", pPlayer, pPlayer->ID.id, faction);
	spin_autolock keeper( _battle_lock );
	DelMapNode( _all_list, pPlayer );
	PlayerInfoIt it;
	if( faction == BF_ATTACKER )
	{
		DelMapNode( _attacker_list, pPlayer );
		it = _attacker_data.player_info_map.find( pPlayer->ID.id );
		if( it != _attacker_data.player_info_map.end() )
		{
			_attacker_data.player_info_map.erase( it );
		}
	}
	else if( faction == BF_DEFENDER )
	{
		DelMapNode( _defender_list, pPlayer );
		it = _defender_data.player_info_map.find( pPlayer->ID.id );
		if( it != _defender_data.player_info_map.end() )
		{
			_defender_data.player_info_map.erase( it );
		}
	}
	OnPlayerLeave(pPlayer);
	SendBattleInfo();
}


void tr_world_manager::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("ע��سǵ�: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_attacker_data.town_list.push_back(pos);
	}       
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.town_list.push_back(pos);
	}       
	else
	{
		ASSERT(false && "ע��سǵ� faction���� ");
	}
}

void tr_world_manager::RecordRebornPos(const A3DVECTOR& pos,int faction,int ctrl_id)
{
	__PRINTF("ע�Ḵ���: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		_attacker_data.revive_map[ctrl_id].push_back(temp);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.revive_map[ctrl_id].push_back(temp);
	}
	else
	{
		ASSERT(false && "ע�Ḵ��� faction���� ");
	}
}

bool tr_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	//�����������
	int battle_faction = pImp->GetBattleFaction();
	spin_autolock keeper(_battle_lock);
	std::vector<A3DVECTOR> revive_pos_list;
	CtrlReviveMapIt it;
	if(BF_ATTACKER == battle_faction)
	{
		for(it = _attacker_data.revive_map.begin();it != _attacker_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{
				if(it->second[i].active)
				{
					revive_pos_list.push_back(it->second[i].pos);
				}
			}
		}
	}
	else if(BF_DEFENDER == battle_faction)
	{
		for(it = _defender_data.revive_map.begin();it != _defender_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{       
				if(it->second[i].active)
				{
					revive_pos_list.push_back(it->second[i].pos);
				}
			}               
		}
	}
	else
	{
		return false;
	}
	size_t size = revive_pos_list.size();
	if(0 == size) return false;
	pos = revive_pos_list[abase::Rand(0,revive_pos_list.size() - 1)];
	world_tag = GetWorldTag();
	return true;
}

void tr_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{       
	__PRINTF("ע������: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	if(faction & FACTION_OFFENSE_FRIEND)
	{       
		//���ڻ��޷����ֹ����͹���������
		_attacker_data.entry_list.push_back(pos);
	}       
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.entry_list.push_back(pos);
	}
	else
	{
		ASSERT(false && "ע������ no faction");
	}       
}      


bool tr_world_manager::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag)
{
	return false;
}       

void tr_world_manager::GetRandomEntryPos(A3DVECTOR& pos,int battle_faction)
{
	pos = A3DVECTOR(0,0,0);
	spin_autolock keeper(_battle_lock);
	if(BF_ATTACKER == battle_faction)
	{       
		if(0 == _attacker_data.entry_list.size()) return;
		pos = _attacker_data.entry_list[abase::Rand(0,_attacker_data.entry_list.size() - 1)];
	}
	else if(BF_DEFENDER == battle_faction)
	{       
		if(0 == _defender_data.entry_list.size()) return;
		pos = _defender_data.entry_list[abase::Rand(0,_defender_data.entry_list.size() - 1)];
	}
	//gm�����
	else
	{
		if(0 == _defender_data.entry_list.size()) return;
		pos = _defender_data.entry_list[abase::Rand(0,_defender_data.entry_list.size() - 1)];
	}
} 

void tr_world_manager::BattleFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int emote_id, int self_id)
{
	if( faction & (FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND) )
	{
		multi_send_chat_msg( _attacker_list, buf, size, channel, emote_id, aux_data, dsize, self_id);
	}       
	else if( faction & (FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND) )
	{
		multi_send_chat_msg( _defender_list, buf, size, channel, emote_id, aux_data, dsize, self_id);
	}
}

int tr_world_manager::TranslateCtrlID( int which )
{
	if( which >= CTRL_CONDISION_ID_BATTLE_MIN && which <= CTRL_CONDISION_ID_BATTLE_MAX )
	{
		return CTRL_ID_BATTLE_BASE +
			( GetWorldTag() - TERRITORY_WORLD_TAG_BEGIN ) * CTRL_CONDISION_ID_BATTLE_OFFSET +
			( which - CTRL_CONDISION_ID_BATTLE_MIN );
	}
	return which;
}

void tr_world_manager::ActiveCtrlIDLog(int which, bool active)
{
	GLog::log(GLOG_INFO, "����ս����������: ��ͼid=%d ������id=%d ״̬=%d", GetWorldTag(), which, (int)active);
}

	
void tr_world_manager::Heartbeat()
{
	global_world_manager::Heartbeat();
	++_heartbeat_counter; 
	if( (_heartbeat_counter) % 20 == 0 )
	{
		spin_autolock keeper( _battle_lock );
		switch ( _battle_status )
		{
			case BS_READY:
			{
				OnReady();
			}
			break;
			
			case BS_RUNNING:
			{
				OnRunning();
			}
			break;

			case BS_CLOSING:
			{
				OnClosing();
			}
			break;

			default:
			{
				ASSERT( false );
			}
		};
	}
	// ÿ30�����һ����Ϣ
	if( (_heartbeat_counter) % ( 30 * 20 ) == 0 )
	{
		SendBattleInfo();
		_heartbeat_counter = 0;
	}
}

void tr_world_manager::OnInit()
{
	_battle_status = BS_READY;
}

void tr_world_manager::OnReady()
{
}

void tr_world_manager::OnCreate()
{
	_battle_status = BS_RUNNING;
	ActiveSpawn( this, GetBattleCtrlID(), true );
}

void tr_world_manager::OnRunning()
{
	// ս���߼�,���ʱ�䵽���߲��������
	if( _cur_timestamp < g_timer.get_systime() || 0 != _battle_result )
	{
		if(_battle_result == 0) _battle_result = BR_DEFENDER_WIN; 
		_battle_status = BS_CLOSING;
		_cur_timestamp = g_timer.get_systime() + _kickout_close_time;
		ActiveSpawn( this, GetBattleCtrlID(), false);
		
		GNET::SendTerritoryEnd(_battle_id, _battle_result); 

		// ս������ʱ����һ����Ϣ
		SendBattleInfo();
		
		// ɱ�����й���
		MSG msg;	
		BuildMessage(msg,GM_MSG_NPC_DISAPPEAR,XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
		std::vector<exclude_target> empty;
		_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);
		

		// ȥ�����п���
		MSG msg2;	
		BuildMessage(msg2,GM_MSG_MINE_DISAPPEAR,XID(GM_TYPE_MATTER,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
		empty.clear();
		_plane.BroadcastSphereMessage(msg2,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF, empty);

		__PRINTF( "����ս�������������ʼ׼������: tag=%d, battle_id=%d, result=%d, min=%d, max=%d, close=%d\n", 
				GetWorldTag(), _battle_id, _battle_result, _kickout_time_min, _kickout_time_max, _kickout_close_time );
		GLog::log( GLOG_INFO, "����ս������ʼ����,gs_id=%d, battle_world_tag=%d, result=%d, battle_id=%d, end_time=%d,",
				gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_result, _battle_id, _end_timestamp );
	}
}	      

void tr_world_manager::OnClosing()
{
	// ����ڼ��߳����
	// �����heartbeat�߼�����ɹ涨ʱ���ڵ�����
	if( _cur_timestamp < g_timer.get_systime() )
	{
		GLog::log( GLOG_INFO, "����ս����,gs_id=%d, battle_world_tag=%d, battle_id=%d, result=%d, end_time=%d,",
				gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _battle_id, _battle_result, _end_timestamp );



		// ����֮ǰ��Ҫȷ����gm����Ѿ����߳�
		_battle_status = BS_READY; 
		Reset();

	}
}

void tr_world_manager::OnDeliveryConnected( std::vector<territory_field_info>& info )
{
	__PRINTF( "�ռ�ս��ע����Ϣline_id=%d, battle_type=%d, battle_world_tag=%d, max_player_limit=%d\n",
		gmatrix::Instance()->GetServerIndex(), _battle_type, GetWorldTag(), _max_player_limit);
	GLog::log( GLOG_INFO, "�ռ�ս��ע����Ϣline_id=%d, battle_type=%d, battle_world_tag=%d, max_player_limit=%dn",
		gmatrix::Instance()->GetServerIndex(), _battle_type, GetWorldTag(), _max_player_limit);
	territory_field_info temp;
	temp.battle_type = _battle_type;
	temp.battle_world_tag = GetWorldTag();
	temp.player_limit = _max_player_limit; 
	// ����ֻ�ռ���Ϣ,������ͳһ��һ��Э���
	info.push_back( temp );
}

int tr_world_manager::OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm )
{
	if(_battle_status != BS_RUNNING && !is_gm)
	{
		GLog::log(GLOG_INFO, "����ս����޷�����ս��,ս��δ����:  roleid=%d, line_id=%d, battle_type=%d, battle_id=%d, battle_world_tag=%d, battle_status=%d",
				pInfo->id, gmatrix::Instance()->GetServerIndex(),_battle_type,_battle_id,GetWorldTag(),_battle_status);
		return -1001;
	}

	if((int)pInfo->factionid == _attacker_data.mafia_id)
	{
		if(_attacker_data.player_info_map.size() >= (size_t)_max_player_limit) return -1002;
	}
	else if((int)pInfo->factionid == _defender_data.mafia_id)
	{
		if(_defender_data.player_info_map.size() >= (size_t)_max_player_limit) return -1002;
	}

	return 0;
}

void tr_world_manager::OnPlayerLeave(gplayer* pPlayer)
{
}

int tr_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	npc_template * pTemplate = npc_stubs_manager::Get( tid );
	if( !pTemplate ) return 0;
	if( ROLE_IN_WAR_BATTLE_KEY_BUILDING == pTemplate->role_in_war )
	{
		OnDestroyKeyBuilding( faction );
	}
	return 0; 
}

int tr_world_manager::OnDestroyKeyBuilding( int faction )
{
	spin_autolock keeper( _battle_lock );
	if( _battle_result || BS_RUNNING != _battle_status ) return -1;
	if( faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
	{
		_battle_result = BR_DEFENDER_WIN;
	}
	else
	{
		_battle_result = BR_ATTACKER_WIN;
	}

	GLog::log(GLOG_INFO, "����ս�ؼ��������ݻ�: battle_id=%d, battle_world_tag=%d,battle_type=%d, _battle_result=%d",
			_battle_id,GetWorldTag(),_battle_type,_battle_result);
	SendBattleInfo();
	return 0;
}

void tr_world_manager::OnActiveSpawn(int id,bool active)
{
	if(_attacker_data.revive_map.find(id) != _attacker_data.revive_map.end())
	{
		size_t size = _attacker_data.revive_map[id].size();
		for(size_t i = 0;i < size;++i)
		{
			_attacker_data.revive_map[id][i].active = active;
		}
	}
	if(_defender_data.revive_map.find(id) != _defender_data.revive_map.end())
	{
		size_t size = _defender_data.revive_map[id].size();
		for(size_t i = 0;i < size;++i)
		{
			_defender_data.revive_map[id][i].active = active;
		}
	}
	global_world_manager::OnActiveSpawn(id,active);

}


void tr_world_manager::DumpWorldMsg()
{
	__PRINTF( "-------------------- world message dump --------------------\n" );
	__PRINTF( "battle_id=%d, tag=%d, status=%d, result=%d\n", _battle_id, GetWorldTag(), _battle_status, _battle_result );
	__PRINTF( "������:%d, ����������:%d, ���ط�����:%d\n",
		_attacker_data.player_info_map.size() + _defender_data.player_info_map.size(),
		_attacker_data.player_info_map.size(), _defender_data.player_info_map.size() ); 

	__PRINTF( "�����Ϣ:\n" );
	PlayerInfoIt it;
	for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
	{
		__PRINTF( "\tid=%d,battle_faction=%d,kill=%d,death=%d\n",
			it->first, it->second.battle_faction,  it->second.kill, it->second.death );
	}
	for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
	{
		__PRINTF( "\tid=%d,battle_faction=%d,kill=%d,death=%d\n",
			it->first, it->second.battle_faction,  it->second.kill, it->second.death );
	}
	__PRINTF( "\n" );
}

