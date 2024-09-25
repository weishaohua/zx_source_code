/*
 * FILE: kf_player.h 
 *
 * DESCRIPTION: killing field player implementation 
 *
 * CREATED BY: lianshuming, 2009/04/13
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_KF_PLAYER_H__
#define __ONLINEGAME_GS_KF_PLAYER_H__

#include "bg_player.h"

class kf_player_imp : public bg_player_imp
{
private:
	char _rank;			//����
	int _con_kill;			//������ɱ����	
	int _postion;			//����
	int _revive_counter;		//ǿ�Ƹ����ʱ��
	int _revive_timeout; 		//����ʱ�䵹��ʱ, ���ͻ����õ�
	int _revive_punish_counter;   	//����ͷ�ʱ��
	int _revive_clean_punish;	//�������ͷ�
	int _update_info_counter;	//��ʱ������Ϣ������
		
	enum
	{
		REVIVE_MIN_TIMEOUT = 10,  	 //�����ʱ����Сֵ
		REVIVE_MAX_TIMEOUT = 50,  	 //�����ʱ�����ֵ
		REVIVE_FORCE_TIME  = 90,  	 //ǿ�Ƹ���ʱ��
		REVIVE_PUNISH_COUNTER = 60,	 //�����ͷ��ļ�ʱ��
		REVIVE_CLEAN_PUNISH = 180,	 //�������ͷ��ļ�ʱ��
	};
public:
		
	DECLARE_SUBSTANCE( kf_player_imp )
	kf_player_imp()
	{
		_rank = 1; 	
		_con_kill = 0;	
		_postion = 0;
		_revive_counter = 0;
	       	_revive_timeout = REVIVE_MIN_TIMEOUT;
		_revive_punish_counter = 0;	
		_revive_clean_punish = 0;
		_update_info_counter = 0;
	}
	
	virtual int MessageHandler( const MSG& msg );
	
	virtual void OnHeartbeat( size_t tick );
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	
	virtual void GetBattleInfo();
	virtual void SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data, size_t dsize);
	virtual void ChangeKillingfieldScore (int value);
	virtual void ResetKillingfieldScore();

protected:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void HandleBattleResult();
	virtual void HandleDeath();
	
	virtual void DumpBattleSelfInfo();
	virtual void DumpBattleInfo();
	virtual void DebugIncBattleScore(int inc);

private:
	inline char GetRank() const { return _rank;}
	inline size_t GetScore() const { return _killingfield_score; }
	size_t GetBattleScore() { return _killingfield_score; }
	inline int GetConKill() const { return _con_kill; }
	inline int GetDeathCount() const { return _death_count;}

	inline void UpdateScore(int offset, int sync_msg = GM_MSG_SYNC_BATTLE_INFO , int sync_param =0)
	{
		_killingfield_score += offset;  
		UpdateRank();
		
		//ͬ����Ϣ���ͻ��˺�ս��
		SendClientSelfInfo();
		SyncBattleInfo(sync_msg, sync_param);
	}
	
	inline void UpdateRank()
	{
		int map_id = _plane->GetWorldIndex();
		int rank =  player_template::GetRankByScore(map_id, _killingfield_score); 
		if(rank != -1 && rank != _rank)
		{	
			gplayer * pPlayer = GetParent();
			_rank = rank;
			pPlayer->rank = _rank;
			_runner->rank_change(_rank);
		}
	}

	inline int GetKillScoreByRank(int rank)
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetKillScoreByRank(map_id, rank);
	}
	
	inline int GetExtScoreByConKill(int _con_kill)
	{
		if(_con_kill < 3) return 0;
		int map_id = _plane->GetWorldIndex();
		return player_template::GetConKillScore(map_id, _con_kill);
	}
	
	inline int GetWinExtScore()
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetBattleWinScore(map_id);
	}

	inline int GetScoreByKillMonster(int id)
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetKillMonsterScore(map_id, id);
	}
	
	inline int GetDeathDropItemID()
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetDeathDropItemID(map_id, _rank);
	}

	inline int GetTaskIDByRank()
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetTaskIDByRank(map_id, _rank);
	}
	
	void HandleDeathDrop();
	void HandleDeathRobDrop();
	
	//��ȡ��ǰ������
	int GetPostion();

	//ͬ���Լ���ս����Ϣ��manager
	void SyncBattleInfo(int msg_id, int param);
	
	void SendClientSelfInfo();
	void GetKillingFieldInfo();

};

#endif
