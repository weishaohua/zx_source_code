#ifndef __ONLINEGAME_GS_NPC_SESSION_H__
#define __ONLINEGAME_GS_NPC_SESSION_H__

#include "actsession.h"
#include "aipolicy.h"
#include "skillwrapper.h"
#include "pathfinding/chaseinfo.h"

typedef CChaseInfo chase_info;
class ai_task;
class session_npc_template 
{
protected:
	int	_ai_task_id;
public:
	session_npc_template():_ai_task_id(-1){}

	inline void SetAITask(int id)
	{
		_ai_task_id = id;
	}
};

enum
{
	NSRC_SUCCESS,
	NSRC_OUT_OF_RANGE,
	NSRC_TIMEOUT,
	NSRC_ERR_PATHFINDING,
};

class session_npc_attack : public session_normal_attack , public session_npc_template
{
	float _short_range;
	int _attack_times;
protected:
	session_npc_attack():_short_range(0),_attack_times(0){}
public:
	DECLARE_SUBSTANCE(session_npc_attack);
	explicit session_npc_attack(ai_object * obj)
				:session_normal_attack(obj->GetImpl()),_short_range(0),_attack_times(0)
	{
	}

	void inline SetShortRange(float sr)
	{
		_short_range = sr*sr;
	}

	void inline SetAttackTimes(int t)
	{
		_attack_times = t;
	}

	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual int  GetMask()		//ȡ���Լ���mask�������Լ��ڶ����е����
	{
		return 0;
	}
	
	virtual int  GetExclusiveMask()	//����������mask���������ų����������Щsession
	{
		return ~0;
	}
	virtual bool IsTimeSpent()
	{
		return true;
	}

	bool Save(archive & ar) 
	{
		session_normal_attack::Save(ar);
		ar << _ai_task_id << _short_range << _attack_times;
		return true;
	}

	bool Load(archive & ar) 
	{
		session_normal_attack::Load(ar);
		ar >> _ai_task_id >> _short_range >> _attack_times;
		return true;
	}
};

class session_npc_range_attack : public session_npc_attack 
{
	bool _auto_interrupt;
	float _short_range;
	float _attack_range;
	session_npc_range_attack():_short_range(0){}
public:
	DECLARE_SUBSTANCE(session_npc_range_attack);
	explicit session_npc_range_attack(ai_object * obj)
				:session_npc_attack(obj),_auto_interrupt(true),_short_range(0)
	{
	}
	void SetInterrupt(bool value)
	{
		_auto_interrupt = value;
	}
	void SetRange(float attack_range , float sr)
	{
		_short_range = sr*sr;
		_attack_range = attack_range * attack_range;
	}
	
	virtual bool RepeatSession();
	bool Save(archive & ar) 
	{
		session_normal_attack::Save(ar);
		ar << _ai_task_id <<_short_range << _attack_range << _auto_interrupt;
		return true;
	}

	bool Load(archive & ar) 
	{
		session_normal_attack::Load(ar);
		ar >> _ai_task_id >> _short_range >> _attack_range >>_auto_interrupt;
		return true;
	}
};

namespace path_finding
{
	class follow_target;
	class cruise;
	class keep_out;
        class follow_target_without_map;  // Youshuang add
        class keep_out_without_map;  // Youshuang add
};

class session_npc_keep_out : public act_timer_session, public session_npc_template
{
protected:
	XID   _target;
	float _range;
	int   _timeout;
	int _retcode;
	char _stop_flag;
	path_finding::keep_out * _agent;

	bool Run();
	session_npc_keep_out():_agent(0){}
public:
	DECLARE_SUBSTANCE(session_npc_keep_out);
	explicit session_npc_keep_out(ai_object *obj)
				:act_timer_session(obj->GetImpl()),
				_target(-1,-1),_range(5),_retcode(0),_stop_flag(1),_agent(0)
				{}
	~session_npc_keep_out();
	void SetTarget(const XID & target, float attack_range,int timeout = 8)
	{
		_target = target;
		_range = attack_range * 0.9f;
		_timeout = timeout;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();


	virtual int  GetMask() { return 0; }
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
	virtual float GetSpeed();

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _ai_task_id << _target 
		   << _range << _timeout << _stop_flag << _retcode ;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _ai_task_id >> _target
		   >> _range >> _timeout >> _stop_flag >> _retcode;
		return true;
	}
};

class session_npc_delay : public act_timer_session, public session_npc_template
{
	int   _timeout;
	session_npc_delay(){}
public:
	DECLARE_SUBSTANCE(session_npc_delay);
	explicit session_npc_delay(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_timeout(2)
				{}
	void SetTarget(int timeout = 2)
	{
		_timeout = timeout;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();


	virtual int  GetMask() { return 0; }
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _ai_task_id << _timeout;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _ai_task_id>> _timeout;
		return true;
	}
};


class session_npc_flee : public session_npc_keep_out 
{
protected:
	session_npc_flee(){}
public:
	DECLARE_SUBSTANCE(session_npc_flee);
	explicit session_npc_flee(ai_object *obj)
				:session_npc_keep_out(obj)
				{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();


	virtual int  GetMask() { return 0; }
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
};

class session_npc_silent_flee : public session_npc_flee
{
	session_npc_silent_flee(){}
public:
	DECLARE_SUBSTANCE(session_npc_silent_flee);
	explicit session_npc_silent_flee(ai_object *obj)
				:session_npc_flee(obj)
				{}
	virtual bool RepeatSession();
};


class session_npc_follow_target :  public act_timer_session, public session_npc_template
{
protected:
	float _range_min;
	float _range_max;
	float _range_target;
	XID _target;
	int _retcode;
	int _rand_dir;
	int _quadrant;
	unsigned short _speed;
	bool _stop_flag;
	char _reachable_count;
	int _timeout;
	int Run();
	path_finding::follow_target * _agent;
	chase_info  _chase_info;
	session_npc_follow_target():_agent(0){}
public:
	DECLARE_SUBSTANCE(session_npc_follow_target);
	explicit session_npc_follow_target(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_target(-1,-1),_retcode(0),_rand_dir(0),_quadrant(-1),_stop_flag(true),_reachable_count(0),_timeout(20),_agent(0)
	{
		memset(&_chase_info, 0,sizeof(_chase_info));
	}
	~session_npc_follow_target();

	void SetTarget(const XID & target, float range_min,float range_max,float range_target,int timeout=(int)(10/NPC_FOLLOW_TARGET_TIME))
	{
		_target = target;
		_range_max = range_max*range_max;
		_range_min = range_min*range_min;
		_range_target = range_target;
		_timeout = timeout;
	}

	void SetChaseInfo(chase_info * pInfo)
	{
		_chase_info = *pInfo;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual int  GetMask() { return 0; } 
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
	virtual float GetSpeed();

	void TrySendStop();
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _ai_task_id << _target << _timeout
		   << _range_min << _range_max << _range_target << _retcode << _rand_dir 
		   <<_quadrant << _speed << _stop_flag << _reachable_count;
		ar.push_back(&_chase_info,sizeof(_chase_info));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _ai_task_id >> _target >> _timeout
		   >> _range_min >> _range_max >> _range_target >> _retcode >> _rand_dir 
		   >>_quadrant >> _speed >> _stop_flag >> _reachable_count;
		ar.pop_back(&_chase_info,sizeof(_chase_info));
		return true;
	}
};

class session_npc_empty : public act_session
{
public:
	DECLARE_SUBSTANCE(session_npc_empty);
	session_npc_empty():act_session(NULL){}
	virtual bool StartSession(act_session * next_cmd = NULL) { return false;}
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual int  GetMask() { return 0; } 
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return false; }
	bool Save(archive & ar) { return true; }
	bool Load(archive & ar) { return true; }
};

class session_npc_cruise :  public act_timer_session, public session_npc_template
{
	A3DVECTOR _center;
	int _timeout;
	bool _stop_flag;
	float _range;
	bool Run();
	session_npc_cruise():_agent(0){}
	path_finding::cruise * _agent;
public:
	DECLARE_SUBSTANCE(session_npc_cruise);
	explicit session_npc_cruise(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_timeout(0),_stop_flag(false),_range(0),_agent(0)
	{}
	~session_npc_cruise();

	void SetTarget(const A3DVECTOR &center, int timeout,float range)
	{
		_center = center;
		_timeout = timeout;
		_range = range;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual int  GetMask() { return 0; } 
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _center << _timeout << _stop_flag << _range;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _center >> _timeout >> _stop_flag >> _range;
		return true;
	}
	virtual float GetSpeed();
};

class session_npc_skill : public act_timer_session, public session_npc_template
{
protected:
	session_npc_skill(){}
	unsigned int _skill_id;
	int _skill_level;
	XID  _target;
	bool _end_flag;
public:
	DECLARE_SUBSTANCE(session_npc_skill);
	explicit session_npc_skill(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_end_flag(false)
	{}
	void SetTarget(int skill_id,int skill_level,const XID & target)
	{
		_target = target;
		_skill_id = skill_id;
		_skill_level = skill_level;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force) { return EndSession(); }
	virtual bool OnAttacked();		// �������ʾҪ�ж�
	
	virtual int  GetMask() { return SS_MASK_ATTACK; } 
	virtual int  GetExclusiveMask() { return ~SS_MASK_MOVE; }
	virtual bool IsTimeSpent() { return true; }

	virtual void OnTimer(int index,int rtimes, bool & is_delete)
	{
		if(rtimes)
			SendForceRepeat(_self_id);
		else
			SendEndMsg(_self_id);
	}
	
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _skill_id << _skill_level <<_target << _end_flag;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _skill_id >> _skill_level >> _target >> _end_flag;
		return true;
	}
	
};

class session_npc_follow_master : public session_npc_follow_target
{
	session_npc_follow_master(){}
public:
	DECLARE_SUBSTANCE(session_npc_follow_master);
	explicit session_npc_follow_master(ai_object *obj):session_npc_follow_target(obj)
	{}

	virtual float GetSpeed();
};

class session_npc_regeneration : public act_timer_session, public session_npc_template
{
	int _timeout;
	bool _fast_regen;
	session_npc_regeneration():_timeout(10){}
public:
	DECLARE_SUBSTANCE(session_npc_regeneration);
	explicit session_npc_regeneration(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_timeout(10),_fast_regen(true)
				{}
	
	void SetFastRegen(bool val)
	{
		_fast_regen = val;
	}

	void SetTimeout(int t)
	{
		_timeout = t;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();

	virtual int  GetMask() { return 0; }
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
	virtual bool Save(archive & ar)
	{
		act_timer_session::Save(ar);
		ar << _timeout;
		return true;
	}
	virtual bool Load(archive & ar)
	{
		act_timer_session::Load(ar);
		ar >> _timeout;
		return true;
	}

	virtual void OnTimer(int index,int rtimes, bool & is_delete);
	
};

class patrol_agent;
class session_npc_patrol : public act_timer_session, public session_npc_template
{
protected:
	A3DVECTOR _target;
	int _retcode;
	bool _stop_flag;
	bool _is_run;
	char _reachable_count;
	bool _has_path_agent;
	bool _pathmap_avail;
	int _timeout;
	int Run();
	path_finding::follow_target * _agent;
	session_npc_patrol():_agent(0){}
public:
	DECLARE_SUBSTANCE(session_npc_patrol);
	explicit session_npc_patrol(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_retcode(1),_stop_flag(true),_is_run(false),_has_path_agent(false),_pathmap_avail(true),_agent(0)
	{}
	~session_npc_patrol();

	void SetTarget(const A3DVECTOR & target, int timeout,patrol_agent * agent = NULL, bool is_run = false)
	{
		_target = target;
		_timeout = timeout;
		_has_path_agent = agent ?true:false;
		_pathmap_avail = _has_path_agent && agent->IsPathmapAvail();
		_is_run = is_run;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual int  GetMask() { return 0; } 
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }
	virtual float GetSpeed();

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _ai_task_id  << _target << _stop_flag << _is_run << _reachable_count << _timeout << _retcode;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _ai_task_id >> _target >> _stop_flag >> _is_run >>  _reachable_count >> _timeout >> _retcode;
		return true;
	}
};

class session_npc_regen : public act_timer_session, public session_npc_template
{
protected:
	int _timeout;
	session_npc_regen() {}
public:
	DECLARE_SUBSTANCE(session_npc_regen);
	explicit session_npc_regen(ai_object *obj)
				:act_timer_session(obj->GetImpl()),_timeout(5)
	{}

	void SetTimeout(int timeout)
	{
		_timeout = timeout;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force) { return EndSession(); }
	virtual int  GetMask() { return 0; } 
	virtual int  GetExclusiveMask() { return ~0; }
	virtual bool IsTimeSpent() { return true; }

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _ai_task_id  << _timeout;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _ai_task_id  >> _timeout;
		return true;
	}
};

#endif

