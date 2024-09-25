#ifndef __ONLINEGAME_GS_BATTLEGROUND_CONTROL_H__
#define __ONLINEGAME_GS_BATTLEGROUND_CONTROL_H__

#include "../world.h"
#include <interlocked.h>
#include <vector.h>
#include "../usermsg.h"

class battleground_ctrl : public world_data_ctrl
{
public:
	struct 
	{
		int battle_id;
		int faction_attacker;
		int faction_defender;
		int attacker_count;
		int defender_count;

		int player_count_limit;

		int end_timestamp;	//����ʱ��
	}_data;

	struct map_data
	{
		int key_building;
		int mobs;
		int war_mobs;
	};

	map_data _defence_data;
	map_data _offense_data;
	map_data _defence_init_data;
	map_data _offense_init_data;
	int	 _battle_result;
	int 	 _win_condition;		//0Ŀ�����Ľ��� 1 ��ͨ����

	enum
	{
		BR_NULL = 0,
		BR_WINNER_OFFENSE,
		BR_WINNER_DEFENCE,
		BR_TIMEOUT,
	};

	cs_user_map  _attacker_list;
	cs_user_map  _defender_list;
	cs_user_map  _all_list;
	int _user_list_lock;

	inline void AddMapNode(cs_user_map & map, gplayer * pPlayer)
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val(pPlayer->ID.id,pPlayer->cs_sid);
		if(cs_index >= 0 && val.first >= 0)
		{
			map[cs_index].push_back(val);
		}
	}

	inline void DelMapNode(cs_user_map & map, gplayer * pPlayer)
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val(pPlayer->ID.id,pPlayer->cs_sid);
		if(cs_index >= 0 && val.first >= 0)
		{
			cs_user_list & list = map[cs_index];
			int id = pPlayer->ID.id;
			for(size_t i = 0; i < list.size(); i ++)
			{
				if(list[i].first == id)
				{
					list.erase(list.begin() + i);
					i --;
				}
			}
		}
	}

public:
	bool AddAttacker()
	{
		if(_data.attacker_count >= _data.player_count_limit) return false;
		int p = interlocked_increment(&_data.attacker_count);
		if(p > _data.player_count_limit)
		{
			interlocked_decrement(&_data.attacker_count);
			return false;
		}
		else
		{
			return true;
		}
	}
	
	bool AddDefender()
	{
		if(_data.defender_count >= _data.player_count_limit) return false;
		int p = interlocked_increment(&_data.defender_count);
		if(p > _data.player_count_limit)
		{
			interlocked_decrement(&_data.defender_count);
			return false;
		}
		else
		{
			return true;
		}
	}

	void DelAttacker()
	{
		interlocked_decrement(&_data.attacker_count);
	}
	
	void DelDefender()
	{
		interlocked_decrement(&_data.defender_count);
	}
	
	void PlayerEnter(gplayer * pPlayer,int mask); 	//MASK: 1 attacker, 2 defneder
	void PlayerLeave(gplayer * pPlayer,int mask); 	//MASK: 1 attacker, 2 defneder

public:
	void DestroyKeyBuilding(int faction);
	void DestroyMobs(int faction);
	void CheckBattleResult(world * pPlane);
	void BattleFactionSay(int faction ,const void * buf, size_t size);
	void BattleSay(const void * buf, size_t size);

protected:
	int GetGoal(const map_data & data);
	void BattleEnd(world * pPlane);
public:

	battleground_ctrl():_battle_result(0), _attacker_list(), _defender_list(), _all_list()
	{
		memset(&_data,0,sizeof(_data));
		memset(&_defence_data,0,sizeof(_defence_data));
		memset(&_offense_data,0,sizeof(_offense_data));
	}
	
	virtual world_data_ctrl * Clone()
	{
		return new battleground_ctrl(*this);
	}

	virtual void Reset()
	{
		memset(&_data,0,sizeof(_data));
		memset(&_defence_data,0,sizeof(_defence_data));
		memset(&_offense_data,0,sizeof(_offense_data));
		_battle_result = 0;
	}

	virtual void Tick(world * pPlane);
};

#endif

