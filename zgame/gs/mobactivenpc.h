#ifndef __ONLINE_GAME_GS_MOB_ACTIVE_NPC_H__
#define __ONLINE_GAME_GS_MOB_ACTIVE_NPC_H__

#include "npc.h"
#include "aipolicy.h"
#include <map>

struct mob_active_statement;

class mob_active_imp: public gnpc_imp
{
	friend class mob_active_policy;

private:
	char _active_mode; // �������: 0 - ��Ʒ,1 - ����
	unsigned char _active_point_num; //�ܹҵ���
	char _active_times_max; //�ɻ�����������
	bool _active_no_disappear; //���������������Ƿ���ʧ
	int _active_point[6]; // �ҵ�ģ��
	int  _need_equipment;
	int _task_in;
	int _level;
	int _require_gender;
	int64_t _character_combo_id;
	int64_t _character_combo_id2;
	int _eliminate_equipment;
	int _active_ready_time;
	int _active_running_time;
	int _active_ending_time;
	int _path_id;
	int _end_new_mob_active_id;
	int _move_can_active; //�ƶ�ʱ�ܷ񻥶�
	bool _path_end_finish_active; //����·���յ��������

	char _active_counter; //����������ʱ��
	bool _lock;
	int _lock_id;
	int _lock_time_out;
	std::map<int,int> _player_map; //id->pos
	std::map<int,A3DVECTOR> _player_pos_map; //
	bool _pos[6];

	int _mob_active_move_status;
	bool _mob_active_can_reclaim;

	enum
	{
		MOBACTIVE_ACTIVE = 0,
		PLAYER_ACTIVE = 1,
	};

	enum
	{
		MOBACTIVE_CANNOT_MOVE = 0, // �����ƶ���һ�㶼��_path_id = 0
		MOBACTIVE_MOVE,
		MOBACTIVE_STOP_MOVE,
	};

public:
	int _mob_active_life;

	DECLARE_SUBSTANCE(mob_active_imp);

	mob_active_imp();

	void SetParam(mob_active_statement* mob_active_data);
	virtual int MessageHandler(const MSG & msg);
	virtual void OnHeartbeat(size_t tick);
	virtual bool StepMove(const A3DVECTOR& offset);

	int findpos()
	{
		int emptypos = -1;
		for (int i = 0; i < _active_point_num; ++i)
		{
			if (_pos[i] == false) return i;
		}
		return emptypos;
	}

	void addplayer(int playerid,A3DVECTOR pos)
	{
		//����Ѿ��ڻ����б���
		if (_player_map.find(playerid) != _player_map.end()) return;

		int emptypos = findpos();
		if (emptypos == -1)
		{
			ASSERT(false && "ѡλ��ʧ��");
		}
		_player_map.insert(std::make_pair(playerid,emptypos));
		_player_pos_map.insert(std::make_pair(playerid,pos));
		_pos[emptypos] = true;
	}

	void delplayer(int playerid)
	{
		_pos[_player_map[playerid]] = false;
		_player_map.erase(playerid);
		_player_pos_map.erase(playerid);
	}

	void SendErrorMessage(const XID & who, int message)
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_ERROR_MESSAGE,who,_parent->ID,_parent->pos,message);
		gmatrix::SendMessage(msg);
	}

	bool BecomeNewMobActive(int npc_tid);

	void MobActiveStartMove();

	void MobActiveStopMove();

	void CreateMobActive();
};

class mob_active_policy : public ai_policy
{

public:
	DECLARE_SUBSTANCE(mob_active_policy);

	mob_active_policy()
	{
	}

	virtual void Init(const ai_object & self, const ai_param & aip);

	virtual void OnHeartbeat();

	virtual int GetPathID();

	virtual bool GetNextWaypoint(A3DVECTOR & pos);

};

#endif
