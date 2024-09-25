#include <stdio.h>
#include "clstab.h"
#include "skillwrapper.h"
#include <amemory.h>
#include "npcsession.h"
#include "aipolicy.h"
#include "obj_interface.h"
#include <common/message.h>
#include <arandomgen.h>
#include "patrol_agent.h"
#include "aitrigger.h"

extern abase::timer g_timer;
DEFINE_SUBSTANCE(ai_policy,substance, CLS_NPC_AI_POLICY_BASE)
DEFINE_SUBSTANCE(group_master_policy,ai_policy,CLS_NPC_AI_POLICY_MASTER)
DEFINE_SUBSTANCE(group_minor_policy, ai_policy,CLS_NPC_AI_POLICY_MINOR)
DEFINE_SUBSTANCE(group_boss_policy, ai_policy,CLS_NPC_AI_POLICY_BOSS)
DEFINE_SUBSTANCE(ai_task,substance, CLS_NPC_AI_TASK)
DEFINE_SUBSTANCE(ai_melee_task,ai_task,CLS_NPC_AI_MELEE_TASK)
DEFINE_SUBSTANCE(ai_range_task,ai_task,CLS_NPC_AI_RANGE_TASK)
DEFINE_SUBSTANCE(ai_magic_task,ai_task,CLS_NPC_AI_MAGIC_TASK)
DEFINE_SUBSTANCE(ai_magic_melee_task,ai_task,CLS_NPC_AI_MAGIC_MELEE_TASK)
DEFINE_SUBSTANCE(ai_follow_master,ai_task,CLS_NPC_AI_FOLLOW_MASTER_TASK)
DEFINE_SUBSTANCE(ai_pet_follow_master,ai_task,CLS_NPC_AI_PET_FOLLOW_MASTER_TASK)
DEFINE_SUBSTANCE(ai_follow_target,ai_task,CLS_NPC_AI_FOLLOW_TARGET_TASK)
DEFINE_SUBSTANCE(ai_runaway_task,ai_task,CLS_NPC_AI_RUNAWAY_TASK)
DEFINE_SUBSTANCE(ai_skill_task,ai_task, CLS_NPC_AI_SKILL_TASK)
DEFINE_SUBSTANCE(ai_skill_task_2,ai_task, CLS_NPC_AI_SKILL_TASK_2)
DEFINE_SUBSTANCE(ai_pet_skill_task,ai_task, CLS_NPC_AI_PET_SKILL_TASK)
DEFINE_SUBSTANCE(ai_silent_runaway_task,ai_runaway_task, CLS_NPC_AI_SILIENT_RUNAWAY_TASK)
DEFINE_SUBSTANCE(ai_rest_task,ai_task, CLS_NPC_AI_REST_TASK)
DEFINE_SUBSTANCE(ai_regeneration_task,ai_task, CLS_NPC_AI_REGENERATION_TASK)
DEFINE_SUBSTANCE(ai_patrol_task,ai_task, CLS_NPC_AI_PATROL_TASK)
DEFINE_SUBSTANCE(ai_fix_melee_task,ai_task,CLS_NPC_AI_FIX_MELEE_TASK)
DEFINE_SUBSTANCE(ai_silent_task,ai_task,CLS_NPC_AI_SILIENT_TASK)
DEFINE_SUBSTANCE(ai_returnhome_task,ai_task,CLS_NPC_AI_RETURNHOME_TASK)
DEFINE_SUBSTANCE(ai_fix_magic_task,ai_task,CLS_NPC_AI_FIX_MAGIC_TASK)
DEFINE_SUBSTANCE(ai_summon_follow_master,ai_pet_follow_master,CLS_NPC_AI_SUMMON_FOLLOW_MASTER_TASK)
DEFINE_SUBSTANCE(ai_clone_follow_master,ai_pet_follow_master,CLS_NPC_AI_CLONE_FOLLOW_MASTER_TASK)
DEFINE_SUBSTANCE(ai_mob_active_patrol_task,ai_task,CLS_NPC_AI_MOB_ACTIVE_PATROL_TASK)

DEFINE_SUBSTANCE_ABSTRACT(patrol_agent,substance,CLS_NPC_PATROL_AGENT)

ai_policy::~ai_policy()
{
	ClearTask();
	if(_self) delete _self;
	if(_path_agent) delete _path_agent;
	if(_mob_active_path_agent) delete _mob_active_path_agent;
	if(_at_policy) delete _at_policy;
}

void 
ai_policy::Init(const ai_object & self, const ai_param & aip)
{
	int primary_strategy = aip.primary_strategy;
	ASSERT(primary_strategy >=STRATEGY_MELEE && primary_strategy < STRATEGY_MAX);
	_primary_strategy = primary_strategy;
	_self = self.Clone();

	_attack_skills.count = aip.skills.as_count;
	_bless_skills.count = aip.skills.bs_count;
	_curse_skills.count = aip.skills.cs_count;
	for(size_t i = 0; i < 8; i ++)
	{
		_attack_skills.skill[i]= aip.skills.attack_skills[i].id;
		_attack_skills.level[i]= aip.skills.attack_skills[i].level;

		_bless_skills.skill[i]= aip.skills.bless_skills[i].id;
		_bless_skills.level[i]= aip.skills.bless_skills[i].level;

		_curse_skills.skill[i]= aip.skills.curse_skills[i].id;
		_curse_skills.level[i]= aip.skills.curse_skills[i].level;
	}

	_patrol_mode = (bool)aip.patrol_mode;
	_speed_flag = aip.speed_flag;
	ASSERT(_cur_task == NULL);
	ASSERT(_task_list.empty());
	ChangePath(aip.path_id,aip.path_type);
}

void    
ai_policy::ChangePath(int path_id, int path_type)
{                       
	if(_path_agent) 
	{       
		delete _path_agent;
		_path_agent = NULL;
	}
	if(path_id > 0)
	{
		_path_agent = new base_patrol_agent();
		if(!_path_agent->Init(path_id,_self->GetWorldManager(), path_type))
		{
			delete _path_agent;
			_path_agent = NULL;
		}
	}
}

bool 
ai_policy::DetermineTarget(XID & target)
{
	return _self->GetFirstAggro(target);
}

void ai_policy::OnAggro()
{
	if(IsTowerStrategy())
	{
		return;
	}

	//�����е��˵ı��
	if(_idle_mode && InCombat())
	{
		return ;
	}
	
	//����ǵ�һ�γ���aggro����ô�����̽��з�Ӧ
	XID target;
	if(!DetermineTarget(target))
	{
		//�޷�ȷ��Ŀ��
		return ;
	}

/*��ʱ����*/
/*
{
	ai_object::target_info info;
	int target_state;
	target_state = _self->QueryTarget(target,info);
	if(target_state == ai_object::TARGET_STATE_NORMAL  && (_self->GetEnemyFaction() & info.faction == 0) )
	{
		ASSERT(false);
	}
}
*/

	_self->ActiveCombatState(true);
	EnableCombat(true);

	if(_cur_task)
	{
		_cur_task->OnAggro();
		return ;
	}



	//�������б���ֻ��һ���� ���̷���һ��HATE
	if(target.type != GM_TYPE_NPC) _self->HateTarget(target);

	//ȷ������
	DeterminePolicy(target);
}

void 
ai_policy::KillTarget(const XID & target)
{
	if(_at_policy)
	{
		_at_policy->KillTarget(target);
	}
}


void ai_policy::DeterminePolicy(const XID & target)
{
	if(target.id != -1)
	{
		AddPrimaryTask(target, _primary_strategy);
		if(_cur_task ) return;
	}

//��Ŀ�� ���ߴ���ԭĿ��ʧ��
	int count = 3;
	XID old_target(-1,-1);
	while(_cur_task == NULL && _self->GetAggroCount() && count > 0)
	{
		XID new_target;
		if(!DetermineTarget(new_target))
		{
			break;
		}
		AddPrimaryTask(new_target, _primary_strategy);
		if(old_target == new_target) break;
		old_target = new_target;
	}
}

void
ai_policy::RollBack()
{
	//����б����,���ָ�����
	_self->ActiveCombatState(false);
	EnableCombat(false);
	_self->ClearDamageList();
	_policy_flag = 0;
	ClearTask();

	if(_path_agent)
	{
		A3DVECTOR pos;
		_path_agent->GetCurWayPoint(pos);
		//_self->ReturnHome(pos,0.0f);
		AddPosTask<ai_returnhome_task>(pos);
	}
	else
	{
		object_interface oi(_self->GetImpl());
		oi.SetInvincibleFilter(true,8,false);
		//�ж��Ƿ�ص�������
		A3DVECTOR pos;
		if(_self->IsReturnHome(pos,GetReturnHomeRange()))
		{
			AddPosTask<ai_returnhome_task>(pos);
		}
		else
		{
			_self->GetPos(pos);
			AddPosTask<ai_returnhome_task>(pos);
		}
	}
}

//void TTTTT(gactive_imp *);

void 
ai_policy::HaveRest()
{
	//�����Ƿ����ж���
	if(_patrol_mode && !_cur_task && _self->CanRest())
	{
		//������Ϣ����
		AddTask<ai_rest_task>();
	}
}

void 
ai_policy::SetAITrigger(int policy_id)
{
	ASSERT(_at_policy == NULL);

	if(policy_id)
	{
		_at_policy = ai_object::CreatePolicy(policy_id);
		ASSERT(_at_policy);
		if(_at_policy)
		{
			_at_policy->SetParent(_self,this);
		}
	}

}

void ai_policy::OnHeartbeat()
{
	if(_self->GetState() & ai_object::STATE_ZOMBIE) return;
	//�ж�����
	if(_life > 0)
	{
		if(_life < g_timer.get_systime())
		{
			//���Լ�ǿ������
			XID id;
			_self->GetID(id);
			if(_self->IsDisappearLifeExpire())
			{
				_self->SendMessage(id,GM_MSG_NPC_DISAPPEAR);
			}
			else
			{
				_self->SendMessage(id,GM_MSG_DEATH);
			}
			return ;
		}
	}

	if(_idle_mode) return;
	if(_at_policy)
	{
		if(InCombat())
		{
			_at_policy->OnHeartbeat();
			_at_policy->OnPeaceHeartbeatInCombat();
		}
		else
		{
			_at_policy->OnPeaceHeartbeat();
		}
	}
	if(_cur_task) 
	{
		//��������ǰ������
		_cur_task->OnHeartbeat();
		if(InCombat() && _self->GetAggroCount() == 0)
		{
			RollBack();
		}
	}
	else
	{
		if(!InCombat() || (InCombat() && IsTowerStrategy()))
		{
			//����ƽʱ�ͻ᲻ͣ�ĵ��ã������ںķ�?? 
			if(_path_agent)
			{
				A3DVECTOR pos;
				bool first_end = true;
				if(_path_agent->GetNextWayPoint(pos,first_end))
				{
					if(first_end && _at_policy)
					{
						_at_policy->PathEnd(_path_agent->GetPathID());
					}
					else
					{
						A3DVECTOR selfpos;
						_self->GetPos(selfpos);
						if(pos.squared_distance(selfpos) > 1e-3)
						{
							//����һ��׷��������task
							AddPosTask<ai_patrol_task>(pos);
						}
					}
				}
				return;
			}
			HaveRest();
			return;
		}

		if(_self->GetAggroCount())
		{
			DeterminePolicy();
		}
		else
		{
			RollBack();
			return;
		}
	}

	_tick ++;
}

void ai_policy::OnBorn()
{
	if(_at_policy)
	{
		_at_policy->OnBorn();
	}
}


void ai_policy::OnDeath(const XID & attacker)
{
	EnableCombat(false);
	
	if(_at_policy)
	{
		_at_policy->OnDeath();
		_at_policy->ResetAll();
	}
	ClearTask();
}

void ai_policy::OnSkillAttack(int id)
{
	if(_at_policy)
	{
		_at_policy->OnSkillAttack(id);
	}
}


void ai_policy::EnableCombat(bool is_combat) 
{ 
	if(!is_combat)
	{
		if(InCombat() && _at_policy)
		{
			_at_policy->Reset();
		}
		_self->SetAggroWatch(true);
	}
	else
	{
		if(!InCombat())
		{
			_self->GetPos(_last_combat_pos);
		}
		if(!InCombat() && _at_policy)
		{
			_at_policy->StartCombat();
		}
		_self->SetAggroWatch(false);
	}
	_in_combat_mode = is_combat;
}

/* --------------------------------------------------------------------- */


bool
ai_task::DetermineTarget(XID & target)
{
	return _self->GetFirstAggro(target);
}

bool 
ai_task::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}

bool 
ai_target_task::StartTask()
{
	ASSERT(_target.id != -1);
	Execute();
	return true;
}

void
ai_target_task::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	//��ǰsessoin����,
	//���»ص�ִ�н���
	switch(reason)
	{
		case NSRC_ERR_PATHFINDING:
		//Ѱ·ʧ�� 
			if(_target.IsPlayerClass())
			{
				_self->ClearAggro();
				_self->ClearDamageList();
				_apolicy->ClearNextTask();
			//	session_npc_regeneration * pSession = new session_npc_regeneration(_self);
			//	pSession->SetFastRegen(true);
			//	pSession->SetAITask(-1);
			//	_self->AddSession(pSession);
				EndTask();
				return ;
			}	
	}

	_session_id = -1;		//���ظ�ִ��ʱ�����session���ܻᱣ����һ��sessioin��ֵ���������Ӧ�����

	if(_apolicy->HasNextTask())
	{
		EndTask();
	}
	else
	{
		Execute();
	}
}

bool 
ai_target_task::EndTask()
{
	return ai_task::EndTask();
}

bool 
ai_target_task::ChangeTarget(const XID & target)
{
	ASSERT(target.type != -1);
	if(_target == target) return true;
	_target = target;
	_self->AddSession(new session_npc_empty());
	Execute();
	return true;
}

void 
ai_target_task::OnHeartbeat()
{
	if(_apolicy->HasNextTask())
	{
		if(!_self->HasSession())
		{
			EndTask();
		}
		else
		{
			_self->AddSession(new session_npc_empty());
		}
		return;
	}
	
	XID  target;
	if(_self->GetFirstAggro(target))
	{
		if(target != _target || !_self->HasSession())
		{
			_target = XID(-1,-1);
			_session_id = -1;
			ChangeTarget(target);
		}
	}
	else
	{
		//������ζ�����һ����session
		_self->AddSession(new session_npc_empty());
		if(_session_id == -1)
		{
			//Ӧ�ý���
			EndTask(); 
		}
		else
		{
			_target = XID(-1,-1);
		}
	}
	
}

/************************************************/

void
ai_melee_task::Execute()
{
	if(_session_id != -1) return;
	if(_target.type == -1) 
	{
		EndTask();
		return ;
	}
	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	//Ŀ�괦�ڹ�����Χ�� 
	float body_size = _self->GetBodySize();
	float pure_attack_range = _self->GetAttackRange() - body_size;
	float attack_range = pure_attack_range*0.8f + body_size + info.body_size;
	float short_range = body_size + info.body_size;
	if(range > attack_range * attack_range)//�����Ŀ��ľ��볬���˵�ǰmelee��������ֵ�����Ƚ�����·״̬
	{
		session_npc_follow_target *pSession = new session_npc_follow_target(_self);
		pSession->SetTarget(_target,attack_range,_self->GetIgnoreRange(),pure_attack_range*0.6f+_self->GetBodySize() + info.body_size);
		pSession->SetChaseInfo(&_chase_info);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}
	else
	{
		if (range < short_range*short_range)
		{
			//���������Ҫ���һ��
			session_npc_keep_out *pSession = new session_npc_keep_out(_self);
			pSession->SetTarget(_target,attack_range,4);
			pSession->SetAITask(_apolicy->GetTaskID());
			_self->AddSession(pSession);
			ClearChaseInfo();
			return ;
		}
	}

	//ֱ�ӽ��빥��״̬
	session_npc_attack *attack = new session_npc_attack (_self);
	attack->SetTarget(_target,false);
	attack->SetShortRange(short_range);
	attack->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(attack);
	ClearChaseInfo();
}

void
ai_fix_melee_task::Execute()
{
	if(_session_id != -1) return;
	if(_target.type == -1) 
	{
		EndTask();
		return ;
	}
	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	//Ŀ�괦�ڹ�����Χ�� 
	float body_size = _self->GetBodySize();
	float pure_attack_range = _self->GetAttackRange() - body_size;
	float attack_range = pure_attack_range*0.8f + body_size + info.body_size;
	if(range < attack_range * attack_range)//�����Ŀ��ľ��볬���˵�ǰmelee��������ֵ�����Ƚ�����·״̬
	{
		//ֱ�ӽ��빥��״̬
		session_npc_attack *attack = new session_npc_attack (_self);
		attack->SetTarget(_target,false);
		attack->SetShortRange(0);	//��short range
		attack->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(attack);
	}
	else
	{
		//����Ӧ�ù���������
		_self->FadeTarget(_target);
		EndTask();
		return;
	}
}

void
ai_fix_magic_task::Execute()
{
	if(_session_id != -1) return;
	if(_target.type == -1) 
	{
		EndTask();
		return ;
	}
	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	//Ŀ�괦�ڹ�����Χ�� 
	int skill = 0;
	int skill_level;
	_apolicy->GetPrimarySkill(skill,skill_level);
	if(skill <=0)
	{
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	float body_size = _self->GetBodySize();
	float skill_range = _self->GetMagicRange(skill,skill_level) + body_size;

	if(range < skill_range * skill_range)
	{
		//ֱ�ӽ��빥��״̬
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(skill,skill_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
	}
	else
	{
		//����Ӧ�ù���������
		_self->FadeTarget(_target);
		EndTask();
		return;
	}
}

void
ai_range_task::Execute()
{
	if(_session_id != -1) return;
	if(_target.type == -1)
	{
		EndTask();
		return ;
	}
	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	//�ж�Ŀ�괦�ڹ�����Χ�� 
	float attack_range = _self->GetAttackRange()  + info.body_size; 
	float sa = attack_range * attack_range;
	if(range > sa)		//�����Ŀ��ľ��볬���˵�ǰrange��������ֵ�����Ƚ���׷��
	{
		session_npc_follow_target *pSession = new session_npc_follow_target(_self);
		pSession->SetTarget(_target,attack_range,_self->GetIgnoreRange(),attack_range * 0.8f);
		pSession->SetChaseInfo(&_chase_info);
		//pImp->_cur_prop.attack_range,cmd->_ignore_range);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}

	//����ǵ�һ�ε��ã����Ƚ��빥��״̬
	float short_range = sa * (0.3f*0.3f);
	if(short_range < 4.f) short_range = 4.f;
	float close_range = info.body_size + _self->GetBodySize() + 0.3f;
	if((range < close_range * close_range || (range < sa * (0.6f*0.6f)  && _ko_count > 0 ) )
			&&  _state != 1 )
	{
		_state = 1;
		_ko_count --;
		//����̵�0.6����Χ�ڣ�������״̬
		session_npc_keep_out *pSession = new session_npc_keep_out(_self);
		pSession->SetTarget(_target,attack_range,5);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		ClearChaseInfo();
		return;
	}
	//ֱ�ӽ��빥��״̬
	_state = 2;	//��ǰ�ǹ���״̬
	session_npc_range_attack *attack = new session_npc_range_attack (_self);
	attack->SetTarget(_target,false);
	attack->SetInterrupt(range > short_range);
	attack->SetRange(attack_range, close_range);
	attack->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(attack);
	ClearChaseInfo();
}
#define FLEE_RANGE 30.f

bool 
ai_runaway_task::StartTask()
{
	ai_object::target_info info;
	A3DVECTOR selfpos;
	int target_state;
	float srange;

	_self->GetPos(selfpos);
	srange = _self->GetIgnoreRange();
	target_state = _self->QueryTarget(_target,info);
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| info.pos.squared_distance(selfpos) >= srange * srange)
	{
		//Ŀ������Զ���߲����ڣ�����ɾ�����������
		_self->RemoveAggroEntry(_target);
		return false;
	}

	session_npc_flee *pSession = new session_npc_flee(_self);
	pSession->SetTarget(_target,FLEE_RANGE + _self->GetBodySize(),8);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return true;
}

void
ai_runaway_task::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	_session_id = -1;
	EndTask();
}

bool 
ai_runaway_task::ChangeTarget(const XID & target)
{
	_target = target;
	return true;
}

bool
ai_silent_runaway_task::StartTask()
{
	session_npc_silent_flee *pSession = new session_npc_silent_flee(_self);
	pSession->SetTarget(_target,FLEE_RANGE,8);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return true;
}

bool
ai_magic_task::StartTask()
{
	//��ü��ܹ������� 
	_skill_type = _apolicy->GetPrimarySkill(_skill,_skill_level);
	if(_skill <=0) return false;
	_skill_range = _self->GetMagicRange(_skill,_skill_level) + _self->GetBodySize();
	return ai_target_task::StartTask();
}

void
ai_magic_task::Execute()
{
	if(_session_id != -1) return;
	if(_target.type == -1)
	{
		EndTask();
		return ;
	}
	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	if(!_skill_type)
	{
		//�����ף�����ܣ���ֱ��ʹ�ü���
		//ʹ�÷���
		_state = STATE_MAGIC;
		//use magic
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(_skill,_skill_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);

		//���ȡ����һ�μ���
		_skill_type = _apolicy->GetPrimarySkill(_skill,_skill_level);
		_skill_range = _self->GetMagicRange(_skill,_skill_level) + _self->GetBodySize();
		return ;
	}

	float magic_attack_range = _skill_range + info.body_size; 
	float sa = magic_attack_range * magic_attack_range;
	if(range > sa*0.81f)	//�����ǰ�������볬����ħ����ʹ�����ֵ�������׷��
	{
		_state = STATE_TRACE;
		session_npc_follow_target *pSession = new session_npc_follow_target(_self);
		pSession->SetTarget(_target,magic_attack_range*0.9f,_self->GetIgnoreRange(),magic_attack_range * 0.8f);
		pSession->SetChaseInfo(&_chase_info);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}

	//bool keep_range = _apolicy->KeepMagicCastRange();
	float body_size = _self->GetBodySize();
	float short_range = body_size + info.body_size;
	//if(range < sa * (0.5f * 0.5f) && _ko_count > 0 && _state != STATE_DODGE && keep_range)
	if(range < short_range * short_range)
	{
		_state = STATE_DODGE;
		_ko_count --;
		//��ʱӦ�ö��
		session_npc_keep_out *pSession = new session_npc_keep_out(_self);
		pSession->SetTarget(_target,magic_attack_range,4);	//���趼��2�볬ʱ
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		ClearChaseInfo();
		return ;
	}
	else
	{
		//ʹ�÷���
		_state = STATE_MAGIC;
		//use magic
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(_skill,_skill_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		
		ClearChaseInfo();
		//���ȡ����һ�μ���
		_skill_type = _apolicy->GetPrimarySkill(_skill,_skill_level);
		_skill_range = _self->GetMagicRange(_skill,_skill_level) + _self->GetBodySize();
		return ;
	}
}

bool
ai_magic_melee_task::StartTask()
{
	//��ü��ܹ������� 
	_skill_type = _apolicy->GetPrimarySkill(_skill,_skill_level);
	if(_skill <=0) return false;
	_skill_range = _self->GetMagicRange(_skill,_skill_level) + _self->GetBodySize();
	return ai_target_task::StartTask();
}

void
ai_magic_melee_task::Execute()
{
	if(_session_id != -1) return;
	if(_target.type == -1) 
	{
		EndTask();
		return ;
	}
	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
		|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	if(!_skill_type)
	{
		//�����ף�����ܣ���ֱ��ʹ�ü���
		//ʹ�÷���
		_state = STATE_MAGIC;
		//use magic
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(_skill,_skill_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);

		//���ȡ����һ�μ���
		_skill_type = _apolicy->GetPrimarySkill(_skill,_skill_level);
		_skill_range = _self->GetMagicRange(_skill,_skill_level) + _self->GetBodySize();
		return ;
	}


	float magic_attack_range = _skill_range + info.body_size; 
	float sa = magic_attack_range * magic_attack_range;
	
	if(range > sa*0.81f)	//�����ǰ�������볬����ħ����ʹ�����ֵ�������׷��
	{
		_state = STATE_TRACE;
		session_npc_follow_target *pSession = new session_npc_follow_target(_self);
		pSession->SetTarget(_target,magic_attack_range*0.9f,_self->GetIgnoreRange(),magic_attack_range*0.8f);
		pSession->SetChaseInfo(&_chase_info);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}

	float body_size = _self->GetBodySize();
	float pure_attack_range = _self->GetAttackRange() - body_size;
	float attack_range = pure_attack_range * 0.8f + body_size  + info.body_size; 
	sa = attack_range * attack_range;
	float short_range =  body_size + info.body_size + 0.0f;

	if(range > sa || (_state == STATE_PHYSC))
	{
		//ʹ�÷���
		_state = STATE_MAGIC;
		//use magic
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(_skill,_skill_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);

		ClearChaseInfo();
		//���ȡ����һ�μ���
		_skill_type = _apolicy->GetPrimarySkill(_skill,_skill_level);
		_skill_range = _self->GetMagicRange(_skill,_skill_level) + _self->GetBodySize();
		return ;
	}
	else
	{
		if (range < short_range*short_range && _state != STATE_DODGE)
		{
			_state = STATE_DODGE;
			//���������Ҫ���һ��
			session_npc_keep_out *pSession = new session_npc_keep_out(_self);
			pSession->SetTarget(_target,attack_range,4);
			pSession->SetAITask(_apolicy->GetTaskID());
			_self->AddSession(pSession);
			ClearChaseInfo();
			return ;
		}
	}

	//����������
	_state = STATE_PHYSC;
	//ֱ�ӽ��빥��״̬
	session_npc_attack *attack = new session_npc_attack (_self);
	attack->SetTarget(_target,false);
	attack->SetAITask(_apolicy->GetTaskID());
	attack->SetShortRange(short_range);
	attack->SetAttackTimes( (195 + abase::Rand(10,20)) / (_self->GetAttackSpeed() + 1));
	_self->AddSession(attack);
	ClearChaseInfo();
}



void 
ai_rest_task::OnAggro()
{
	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_rest_task::StartTask()
{
	Execute();
	return true;
}

bool 
ai_rest_task::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}

void
ai_rest_task::OnHeartbeat()
{
	if(_apolicy->InCombat()) 
	{
		ai_policy * policy = _apolicy;
		EndTask();
		policy->DeterminePolicy(XID(-1,-1));
		return;
	}
	if(--_timeout < 0)
	{
		EndTask();
		return;
	}
}

void 
ai_rest_task::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	//��ǰsessoin����,
	//���»ص�ִ�н���

	_session_id = -1;		//���ظ�ִ��ʱ�����session���ܻᱣ����һ��sessioin��ֵ���������Ӧ�����

	if(_apolicy->HasNextTask())
	{
		EndTask();
	}
	else
	{
		if(abase::Rand(0.f,1.f) < 0.1f)
			Execute();
		else
			EndTask();
	}
}

void 
ai_rest_task::Execute()
{
	if(_session_id != -1) return;
	//�����session�У��򲻴���

	//���в�����ִ�У����ȶ�λһ��λ��
	A3DVECTOR pos;
	_self->GetPatrolPos(pos);

	session_npc_cruise *pSession = new session_npc_cruise(_self);
	pSession->SetTarget(pos,8,3.2f);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return ;
}


bool 
ai_skill_task::StartTask()
{
	session_npc_skill *pSession = new session_npc_skill(_self);
	pSession->SetTarget(_skill,_level,_target);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return true;
}

void
ai_skill_task::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	_session_id = -1;
	EndTask();
}

bool 
ai_skill_task::ChangeTarget(const XID & target)
{
	_target = target;
	return ai_target_task::ChangeTarget(target);
}

bool ai_pet_skill_task::StartTask()
{
	_skill_range = _self->GetMagicRange(_skill,_level) + _self->GetBodySize();
	Execute();
	return true;
}

bool
ai_skill_task_2::StartTask()
{
	_skill_range_type = GNET::SkillWrapper::RangeType(_skill);
	_skill_range = _self->GetMagicRange(_skill,_level) + _self->GetBodySize();
	Execute();
	return true;
}

void
ai_skill_task_2::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	_session_id = -1;
	if(_state == STATE_TRACE)
	{
		//��׷��ʱ������
		if(reason == NSRC_ERR_PATHFINDING)
		{
			_self->ClearAggro();
			_self->ClearDamageList();
			_apolicy->ClearNextTask();
			session_npc_regeneration * pSession = new session_npc_regeneration(_self);
			pSession->SetFastRegen(true);
			pSession->SetAITask(-1);
			_self->AddSession(pSession);
			EndTask();
			return ;
		}
		else
		{
			Execute();
			return;
		}
	}
	EndTask();
}

bool 
ai_skill_task_2::ChangeTarget(const XID & target)
{       
	_target = target;
	return ai_target_task::ChangeTarget(target);
}

void
ai_skill_task_2::Execute()
{
	if(_session_id != -1 || (_target.type == -1 && _skill_range_type != 2 && _skill_range_type != 5))
	{
		EndTask();
		return ;
	}

	//��������������������͵ļ��ܣ���ֱ��ʹ�� �����Ŀ��״̬������
	if(_skill_range_type == 2 || _skill_range_type == 5)
	{
		//����������������������ֱ��ʹ�� ��� �����ǹ̶����͵Ĳ��ԣ�Ҳֱ����ͼʹ��
		_state = STATE_MAGIC;
		//use magic
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(_skill,_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}

	//�����session�У��򲻴���
	//status A
	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	float srange = _self->GetIgnoreRange();
	if(target_state != ai_object::TARGET_STATE_NORMAL 
			|| (range = info.pos.squared_distance(selfpos)) >= srange * srange)
	{       
		//Ŀ���Ѿ��޷��ٱ�����,�������
		_self->RemoveAggroEntry(_target);
		EndTask();
		return;
	}

	float magic_attack_range = _skill_range + info.body_size;
	float sa = magic_attack_range * magic_attack_range;
	if(_apolicy->IsFixStrategy() || (_trace_count <= 0 && range < sa * 1.21))
	{
		//�����ǹ̶����͵Ĳ��ԣ�Ҳֱ����ͼʹ��  ���׷���ﵽָ�����������Ҿ��볬Խ�Ĳ���Զ,��Ҳֱ��ʹ�ü���
		_state = STATE_MAGIC;
		//use magic
		session_npc_skill *pSession = new session_npc_skill(_self);
		pSession->SetTarget(_skill,_level,_target);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}

	_trace_count --;
	if(range > sa*0.81f)    //�����ǰ�������볬����ħ����ʹ�����ֵ�������׷��
	{
		if(_trace_count < -3) 
		{
			//���׷���������࣬����׷��
			EndTask();
			return;
		}
		_state = STATE_TRACE;
		session_npc_follow_target *pSession = new session_npc_follow_target(_self);
		pSession->SetTarget(_target,magic_attack_range*0.9f,_self->GetIgnoreRange(),magic_attack_range * 0.8f
				);
		pSession->SetChaseInfo(&_chase_info);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		return ;
	}

	//ʹ�÷���
	_state = STATE_MAGIC;
	//use magic
	session_npc_skill *pSession = new session_npc_skill(_self);
	pSession->SetTarget(_skill,_level,_target);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
		
	ClearChaseInfo();
	return ;
}

void 
ai_skill_task_2::OnHeartbeat()
{       
	if(_apolicy->HasNextTask())
	{       
		//_self->AddSession(new session_npc_empty());
		//�����Ƿ��ս�����
		return;
	}
	if(!_self->HasSession() || _self->GetAggroCount() == 0)
	{       
		EndTask();
	}
	//������������������
}


bool 
ai_silent_task::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}

void 
ai_silent_task::OnHeartbeat()
{
	if(--_timeout <= 0 ) 
	{
		EndTask();
		return;
	}
}

void 
ai_regeneration_task::OnAggro()
{
	//������aggro״̬�����֮
	_self->ClearAggro();
	_self->ActiveCombatState(false);
	_self->ClearDamageList();
}
bool 
ai_regeneration_task::StartTask()
{
	session_npc_regeneration * pSession = new session_npc_regeneration(_self);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return true;
}

bool 
ai_regeneration_task::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}

void 
ai_regeneration_task::OnSessionEnd(int session_id, int reason)
{
	_session_id = -1;
	EndTask();
}

void 
ai_regeneration_task::OnHeartbeat()
{
	if(--_timer <= 0 ) 
	{
		EndTask();
		return;
	}
}

//-------------------
void 
ai_returnhome_task::OnAggro()
{

	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	_state = 1;
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_returnhome_task::StartTask()
{
/*
	session_npc_regen *pSession = new session_npc_regen(_self);
	pSession->SetTimeout(4);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return true;
	*/
	session_npc_patrol *pSession = new session_npc_patrol(_self);
	pSession->SetTarget(_target,20,NULL,true);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return true;
}

bool 
ai_returnhome_task::EndTask()
{
	A3DVECTOR pos;
	_self->GetPos(pos);
	float r = _apolicy->GetReturnHomeRange();
	if(_target.squared_distance(pos) > r)
	{
		//�����Ȼ��Ҫ����ǿ�лؼ�
		_self->ReturnHome(_target,0.f);
	}
	_apolicy->TaskEnd();
	return true;
}

void 
ai_returnhome_task::OnSessionEnd(int session_id, int reason)
{
	_session_id = -1;
	if(_state == 0)
	{
		session_npc_patrol *pSession = new session_npc_patrol(_self);
		pSession->SetTarget(_target,20,NULL,true);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		_state = 1;
	}
	else
	{
		EndTask();
	}
}

void 
ai_returnhome_task::OnHeartbeat()
{
	if(--_timeout <= 0 ) 
	{
		EndTask();
		return;
	}
}


void  
group_minor_policy::HaveRest()
{
	XID leader_id = _self->GetLeaderID();
	//�����Ƿ����ж���
	//�������master����
	if(leader_id.IsActive())
	{
		AddTargetTask<ai_follow_master>(leader_id);
	}
	else
	{
		ai_policy::HaveRest();
	}
	return;
	
}

float 
group_minor_policy::GetReturnHomeRange() 
{
	XID leader_id = _self->GetLeaderID();
	if(leader_id.IsActive()) 
		return 1e20;
	else
		return 10.f*10.f;
}

void 
group_minor_policy::OnHeartbeat()
{

	if(_die_with_leader)
	{
		XID leader_id = _self->GetLeaderID();
		if(leader_id.IsActive())
		{
			ai_object::target_info info;
			if(_self->QueryTarget(leader_id,info) != ai_object::TARGET_STATE_NORMAL)
			{
				//�ӳ��Ѿ������� �򲻴���
				//���Լ�ǿ������
				XID id;
				_self->GetID(id);
				_self->SendMessage(id,GM_MSG_DEATH);
				_life = g_timer.get_systime() - 1;
				return ;
			}
		}
	}

	ai_policy::OnHeartbeat();
}

void
ai_follow_master::Execute()
{
}

void
ai_follow_master::OnHeartbeat()
{
	//ÿ������������Ƿ�̫�ķ��� ?? 
	if(_session_id != -1) return;
	if(_apolicy->InCombat()) 
	{
		ai_policy * policy = _apolicy;
		EndTask();
		policy->DeterminePolicy(XID(-1,-1));
		return;
	}

	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	if(target_state != ai_object::TARGET_STATE_NORMAL)
	{
		//�ӳ��Ѿ������� �򲻴���
		//���Բ�����ʲô����
		return ;
	}

	range = info.pos.horizontal_distance(selfpos);
	if(range >= MAX_MASTER_MINOR_RANGE)
	{
		//ִ�лع����
		_self->ReturnHome(info.pos,7.0f);

		return ;
	}
	if(range  >= 8.f*8.f)
	{
		//ִ�и������
		session_npc_follow_target *pSession = new session_npc_follow_target(_self);
		pSession->SetTarget(_target,7.0f,20.f,7.0f);
		pSession->SetChaseInfo(&_chase_info);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
	}
	else if (_self->CanRest())
	{
		//������ִ�����в���
		session_npc_cruise *pSession = new session_npc_cruise(_self);
		pSession->SetTarget(info.pos,6,1.f);
		pSession->SetAITask(_apolicy->GetTaskID());
		_self->AddSession(pSession);
		ClearChaseInfo();
	}
}

void 
ai_follow_master::OnAggro()
{
	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_follow_master::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}


void 
ai_follow_master::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	//��ǰsessoin����,
	//���»ص�ִ�н���

	_session_id = -1;		//���ظ�ִ��ʱ�����session���ܻᱣ����һ��sessioin��ֵ���������Ӧ�����

	if(_apolicy->HasNextTask())
	{
		EndTask();
	}
	else
	{
		if(abase::Rand(0.f,1.f) < 0.1f)
			Execute();
		else
			EndTask();
	}
}

void 
group_boss_policy::OnHeartbeat()
{
	ai_policy::OnHeartbeat();
	TryForwardAggro();
}

void 
ai_patrol_task::OnAggro()
{
	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_patrol_task::StartTask()
{
	_self->GetPos(_start);
	Execute();
	return true;
}

bool 
ai_patrol_task::EndTask()
{
	A3DVECTOR pos;
	_self->GetPos(pos);
	if(_start.squared_distance(pos) < 1e-3)
	{
		//�޷��ƶ���ֱ���߹�ȥ 
		_self->ReturnHome(_target,0.0f);
	}
	_apolicy->TaskEnd();
	return true;
}

void
ai_patrol_task::OnHeartbeat()
{
	if(_apolicy->InCombat()) 
	{
		ai_policy * policy = _apolicy;
		EndTask();
		policy->DeterminePolicy(XID(-1,-1));
		return;
	}
}

void 
ai_patrol_task::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;

	//������ʱ���ʧ�ܣ�Ӧ��ǿ���ƶ�npc��Ŀ���
	_session_id = -1;
	EndTask();
}

void 
ai_patrol_task::Execute()
{
	if(_session_id != -1) return;
	//�����session�У��򲻴���

	//���в�����ִ�У����ȶ�λһ��λ��
	session_npc_patrol *pSession = new session_npc_patrol(_self);
	pSession->SetTarget(_target,120,_apolicy->GetPathAgent(),_apolicy->GetSpeedFlag());
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return ;
}

ai_follow_target::ai_follow_target()
{}

ai_follow_target::ai_follow_target(const XID & target):ai_target_task(target)
{}

ai_follow_target::~ai_follow_target()
{
}

bool 
ai_follow_target::StartTask()
{
	Execute();
	return true;
}

void
ai_follow_target::Execute()
{
	session_npc_follow_target *pSession = new session_npc_follow_target(_self);
	pSession->SetTarget(_target,5.0f,60.f,5.0f);
	pSession->SetChaseInfo(&_chase_info);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
}

void
ai_follow_target::OnHeartbeat()
{
	//ÿ������������Ƿ�̫�ķ��� ?? 
	if(_session_id != -1) return;
	if(_apolicy->InCombat()) 
	{
		ai_policy * policy = _apolicy;
		EndTask();
		policy->DeterminePolicy(XID(-1,-1));
		return;
	}
}

void 
ai_follow_target::OnAggro()
{
	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_follow_target::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}

void 
ai_follow_target::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	//��ǰsessoin����,
	//���»ص�ִ�н���

	_session_id = -1;		//���ظ�ִ��ʱ�����session���ܻᱣ����һ��sessioin��ֵ���������Ӧ�����

	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	range = info.pos.horizontal_distance(selfpos);
	if(target_state == ai_object::TARGET_STATE_NORMAL && range < 60.f*60.f)
	{
		if(range  >= 6.f*6.f)
		{
			//ִ�и������
			Execute();
			return;
		}
	}
	EndTask();
}

bool 
ai_pet_follow_master::StartTask()
{
	Execute();
	return true;
}

void
ai_pet_follow_master::Execute()
{
	session_npc_follow_target *pSession = new session_npc_follow_target(_self);
	pSession->SetTarget(_target,4.0f,62.f,5.0f);
	pSession->SetChaseInfo(&_chase_info);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
}

void
ai_pet_follow_master::OnHeartbeat()
{
	if(_session_id != -1) return;
	if(_apolicy->InCombat()) 
	{
		ai_policy * policy = _apolicy;
		EndTask();
		policy->DeterminePolicy(XID(-1,-1));
		return;
	}
}

void 
ai_pet_follow_master::OnAggro()
{
	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_pet_follow_master::EndTask()
{
	_apolicy->TaskEnd();
	return true;
}


void 
ai_pet_follow_master::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;
	//��ǰsessoin����,
	//���»ص�ִ�н���

	_session_id = -1;		//���ظ�ִ��ʱ�����session���ܻᱣ����һ��sessioin��ֵ���������Ӧ�����

	switch(reason)
	{
		case NSRC_OUT_OF_RANGE:
		case NSRC_TIMEOUT:
		case NSRC_ERR_PATHFINDING:
			_apolicy->UpdateChaseInfo(&_chase_info);
			_apolicy->FollowMasterResult(1);
			EndTask();
			return ;
	}

	ai_object::target_info info;
	int target_state;
	float range;
	A3DVECTOR selfpos;

	_self->GetPos(selfpos);
	target_state = _self->QueryTarget(_target,info);
	range = info.pos.horizontal_distance(selfpos);
	if(target_state == ai_object::TARGET_STATE_NORMAL && range < 60.f*60.f)
	{
		if(range  >= 6.f*6.f)
		{
			//ִ�и������
			Execute();
			return;
		}
	}

	_apolicy->UpdateChaseInfo(&_chase_info);
	_apolicy->FollowMasterResult(0);
	EndTask();
}

void
ai_clone_follow_master::Execute()
{
	session_npc_follow_target *pSession = new session_npc_follow_target(_self);
	pSession->SetTarget(_target,1.0f,62.f,1.0f);
	pSession->SetChaseInfo(&_chase_info);
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
}


void 
ai_mob_active_patrol_task::OnAggro()
{
	//ȷ�����½��в�ѯ����
	ai_policy * policy = _apolicy;
	//�жϵ�ǰ��session �����Լ� ��������Ѱ����
	if(_session_id != -1)
		_self->ClearSession();
	else
		EndTask(); // ���� ClearSession�����EndTask���Բ����ٴε�����
	policy->DeterminePolicy(XID(-1,-1));
}

bool 
ai_mob_active_patrol_task::StartTask()
{
	_self->GetPos(_start);
	Execute();
	return true;
}

bool 
ai_mob_active_patrol_task::EndTask()
{
	A3DVECTOR pos;
	_self->GetPos(pos);
//	if(_start.squared_distance(pos) < 1e-3)
//	{
		//�޷��ƶ���ֱ���߹�ȥ 
//		_self->ReturnHome(_target,0.0f);
//	}
	_apolicy->TaskEnd();
	return true;
}

void
ai_mob_active_patrol_task::OnHeartbeat()
{
	if(_apolicy->InCombat()) 
	{
		ai_policy * policy = _apolicy;
		EndTask();
		policy->DeterminePolicy(XID(-1,-1));
		return;
	}
}

void 
ai_mob_active_patrol_task::OnSessionEnd(int session_id, int reason)
{
	//sessoin_id���ܻ᲻һ�µģ��������ڻ��ж��session
	if(session_id < _session_id) return;

	//������ʱ���ʧ�ܣ�Ӧ��ǿ���ƶ�npc��Ŀ���
	_session_id = -1;
	EndTask();
}

void 
ai_mob_active_patrol_task::Execute()
{
	if(_session_id != -1) return;
	//�����session�У��򲻴���

	//���в�����ִ�У����ȶ�λһ��λ��
	session_npc_patrol *pSession = new session_npc_patrol(_self);
	pSession->SetTarget(_target,120,_apolicy->GetMobActivePathAgent(),_apolicy->GetSpeedFlag());
	pSession->SetAITask(_apolicy->GetTaskID());
	_self->AddSession(pSession);
	return ;
}

void
ai_policy::SetLife(int life)
{
	if(life > 0)
	{
		_life = g_timer.get_systime() + life;
	}
	else
	{
		_life = 0;
	}
}

