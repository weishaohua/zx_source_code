#ifndef __ONLINE_GAME_GS_AI_TRIGGER_H__
#define __ONLINE_GAME_GS_AI_TRIGGER_H__

#include "substance.h"
#include <common/types.h>
#include <amemory.h>
#include <arandomgen.h>
#include <ASSERT.h>
#include <string.h>

class ai_object;
class ai_policy;
namespace ai_trigger
{
	class policy;
	class condition : public substance
	{
	public:
		enum 
		{
			TYPE_INVALID,
			TYPE_HEARTBEAT,
			TYPE_TIMER,
			TYPE_KILL_TARGET,
			TYPE_START_COMBAT,
			TYPE_START_DEATH,
			TYPE_PATH_END,
			TYPE_START_BORN,
			TYPE_SKILL_ATTACK,
		};
	public:
		virtual condition * Clone() const = 0;
		virtual ~condition() {}
		virtual bool Check(policy * self, int param = 0) = 0;	
		virtual int GetConditionType() = 0;		//�������ͣ� �������ͺʹ������� 
		virtual bool IsAutoDisable() = 0;
		virtual void Reset() {}
	};
	class expr
	{
	public:
		enum {
			EXPR_CONSTANT,
			EXPR_COMMON_DATA,
			EXPR_PLUS,
			EXPR_MINUS,
			EXPR_MULTIPLY,
			EXPR_DIVIDE,
			EXPR_RANK_LEVEL,
		};
		expr (){}
		virtual expr* Clone() const = 0;
		virtual ~expr () {}
		virtual int GetValue(policy *self) =0;
		virtual int GetExprType() =0;
		class Exception 
		{
		   std::string _desc;
		public:
		   Exception(const char *s):_desc(s) {}
		   const char *what() const{return _desc.c_str();}
		};
	};


	class target : public substance
	{
	public:
		virtual target * Clone() const = 0;
		virtual ~target() {}
		virtual void GetTarget(policy * self, XID & target) = 0;	
	};

	class operation : public substance
	{
	protected:
		target * 	_target;	
		operation * 	_next;
		bool 		_inbattle;
	public:
		virtual operation * Clone() const = 0;
		operation():_target(0),_next(0),_inbattle(true) {}
		operation(const operation & rhs)
		{
			if(rhs._target)
			{
				_target = rhs._target->Clone();
			}
			else
			{
				_target = 0;
			}
			_next = 0;
			_inbattle = rhs._inbattle;
		}
		void SetBattle(bool b)
		{
			_inbattle = b;
		}

		virtual ~operation() 
		{
			if(_target) delete _target;
		}

		void SetTarget(target * __tar) 
		{
			_target = __tar;
		}

		void SetNext(operation * next)
		{
			ASSERT(next == 0 || _next == 0);
			_next = next;
		}
		operation * GetNext() 
		{
			return _next;
		}
	public:
		virtual bool DoSomething(policy * self) = 0;
		virtual void Reset() {}
		virtual bool RequireTarget() = 0;
	};

	class trigger : public abase::ASmallObject
	{
	protected:
		int 		_id;
		condition * 	_cond;
		operation * 	_ops;
		bool 		_enable;
		bool		_default_enable;
		bool		_battle_trigger;
	public:
		trigger():_id(-1),_cond(0),_ops(0),_enable(false),_default_enable(false),_battle_trigger(true)
		{}
		~trigger()
		{
			delete _cond;
			operation * tmp = _ops;
			while(tmp)
			{
				operation *tmp2 = tmp->GetNext();
				delete tmp;
				tmp = tmp2;
			}
		}
		
		trigger(const trigger &rhs)
		{
			_enable = rhs._enable;
			_default_enable = rhs._default_enable;
			_battle_trigger = rhs._battle_trigger;
			_id = rhs._id;
			_cond = rhs._cond->Clone();
			if(rhs._ops)
			{
				operation *tmp = rhs._ops;
				operation *tmp2 = rhs._ops;
				_ops = tmp->Clone();
				tmp2 = _ops;
				while(tmp->GetNext())
				{
					tmp2->SetNext(0);
					tmp2->SetNext(tmp->GetNext()->Clone());
					tmp2= tmp2->GetNext();
					tmp = tmp->GetNext();
				}
				tmp2->SetNext(0);
			}
			else
			{
				_ops = 0;
			}
		}
		
		inline void SetData(int id , condition * cond)
		{
			_id = id;
			_cond = cond;
		}

		inline void AddOp(operation * op)
		{
			op->SetBattle(_battle_trigger);
			op->SetNext(0);
			if(_ops)
			{
				operation * tmp = _ops;
				while(tmp->GetNext())
				{
					tmp = tmp->GetNext();
				}
				tmp->SetNext(op);
				
			}
			else
			{
				_ops = op;
			}
		}
		
		inline bool IsEnable() 
		{
			return _enable;
		}

		inline bool IsBattleTrigger()
		{
			return _battle_trigger;
		}

		inline void EnableTrigger(bool enable)
		{
			_enable = enable;
		}

		inline void SetBattleEnable(bool battle_trigger)
		{
			_battle_trigger = battle_trigger;
		}

		inline void SetDefaultEnable(bool enable)
		{
			_default_enable = enable;
			_enable = enable;
		}

		inline int GetTriggerID()
		{
			return _id;
		}
		inline int GetCondType()
		{
			return _cond->GetConditionType();
		}
	public:
		inline bool TestTrigger(policy * self, int param = 0)
		{
			if(_cond->Check(self,param))
			{
				bool bRst = true;
				operation * tmp = _ops;
				while(tmp)
				{
					if(!tmp->DoSomething(self)) 
					{
						bRst = false;
						break;
					}
					tmp = tmp->GetNext();
				}
				if(_cond->IsAutoDisable())
				{
					EnableTrigger(false);
				}
				return bRst;
			}
			return true;
		}

		inline void Reset()
		{
			_cond->Reset();
			_ops->Reset();
			_enable = _default_enable;
		}
	};

	class policy : public abase::ASmallObject
	{
		struct timer
		{
			int id;
			int timeout;
			int period;
			int times;
			bool inbattle;
			timer(int id, int to , int p , int ts, bool b):id(id),timeout(to),period(p),times(ts),inbattle(b)
			{}
		};
		typedef abase::vector<trigger *,abase::fast_alloc<> > TRIGGER_LIST;	
		TRIGGER_LIST _trigger_list;	//�������ܱ�
		TRIGGER_LIST _hb_tri_list;	//������������Ѫ���жϵȣ�
		TRIGGER_LIST _tm_tri_list;	//��ʱ��������
		TRIGGER_LIST _st_bat_list;	//��ʼս��������
		TRIGGER_LIST _kl_ply_list;	//ɱ������
		TRIGGER_LIST _death_list;	//����
		TRIGGER_LIST _path_list;	//����·���յ�
		TRIGGER_LIST _born_list;	//�������
		TRIGGER_LIST _ski_atk_list;	//�ܵ����ܹ���

		abase::vector<timer>				_timer_list;
		int _timer_flag;	//���β����Ķ�ʱ��״̬	 0 ,�޲��� 0x01�ж�ʱ������,0x02,�ж�ʱ��Ҫ��ɾ��
		ai_object * _self;	//����ָ�����
		ai_policy * _aip;	//ai ����
		int _id;		//����id
		int _peace_trigger_count;
	public:
		void AddTrigger(trigger * pTrigger)
		{
			int t = pTrigger->GetCondType();
			switch(t)
			{
				case condition::TYPE_HEARTBEAT:
				_hb_tri_list.push_back(pTrigger);
				if(!pTrigger->IsBattleTrigger()) 
				{
					_peace_trigger_count ++;
				}
				break;
				case condition::TYPE_TIMER:
				_tm_tri_list.push_back(pTrigger);
				break;

				case condition::TYPE_KILL_TARGET:
				_kl_ply_list.push_back(pTrigger);
				break;

				case condition::TYPE_START_COMBAT:
				pTrigger->SetDefaultEnable(true);
				_st_bat_list.push_back(pTrigger);
				break;

				case condition::TYPE_START_DEATH:
				pTrigger->SetDefaultEnable(true);
				_death_list.push_back(pTrigger);
				break;

				case condition::TYPE_PATH_END:
				pTrigger->SetDefaultEnable(true);
				_path_list.push_back(pTrigger);
				break;

				case condition::TYPE_START_BORN:
				pTrigger->SetDefaultEnable(true);
				_born_list.push_back(pTrigger);
				break;

				case condition::TYPE_SKILL_ATTACK:
				pTrigger->SetDefaultEnable(true);
				_ski_atk_list.push_back(pTrigger);
				break;

				default:
					ASSERT(false);
				break;
			}
			_trigger_list.push_back(pTrigger);
		}
	public:
		policy(int id):_timer_flag(0),_self(NULL),_aip(NULL),_id(id),_peace_trigger_count(0)
		{}

		policy(const policy & rhs):_timer_flag(rhs._timer_flag),_self(NULL),_aip(NULL),_id(rhs._id)
		{
			_peace_trigger_count = rhs._peace_trigger_count;
			size_t count = rhs._trigger_list.size();
			for(size_t i = 0; i < count ; i ++)
			{
				trigger * pTri = new trigger(*rhs._trigger_list[i]);
				AddTrigger(pTri);
			}
		}

		~policy()
		{
			size_t count = _trigger_list.size();
			for(size_t i = 0; i < count ; i ++)
			{
				delete _trigger_list[i];
			}
		}
		
		void Reset()
		{
			size_t count = _trigger_list.size();
			for(size_t i = 0; i < count ; i ++)
			{
				if (_trigger_list[i]->IsBattleTrigger()) _trigger_list[i]->Reset();
			}

			_timer_flag = 0;
			for(size_t  i = _timer_list.size()  ; i > 0;--i)
			{
				size_t index = i - 1;
				if(_timer_list[index].inbattle)
				{
					_timer_list.erase(_timer_list.begin() + index);
				}
			}
		}

		void ResetAll()
		{
			size_t count = _trigger_list.size();
			for(size_t i = 0; i < count ; i ++)
			{
				_trigger_list[i]->Reset();
			}

			_timer_flag = 0;
			_timer_list.clear();
		}

		void SetParent(ai_object * self, ai_policy * aip)
		{
			_self = self;
			_aip = aip;
		}

		int GetID()
		{
			return _id;
		}
		

		ai_object * GetAIObject()
		{
			return _self;
		}

		ai_policy * GetAIPolicy()
		{
			return _aip;
		}

		void EnableTrigger(int id, bool enable)
		{
			for(size_t i = 0; i < _trigger_list.size(); i ++)
			{
				if(_trigger_list[i]->GetTriggerID() == id)
				{
					_trigger_list[i]->EnableTrigger(enable);
				}
			}
		}
		
		void CreateTimer(int id, int period, int times,bool bBattle)
		{
			for(size_t i = 0; i < _timer_list.size(); i ++)
			{
				if(_timer_list[i].id == id)
				{
					_timer_list[i].id = id;
					_timer_list[i].timeout = period;
					_timer_list[i].period = period;
					_timer_list[i].times = times;
					return;
				}
			}
			_timer_list.push_back(timer(id,period,period,times, bBattle));
		}

		void RefreshTimer()
		{
			_timer_flag = 0;
			for(size_t i = 0; i < _timer_list.size(); i ++)
			{
				timer & t = _timer_list[i];
				if(!t.timeout) t.timeout = t.period;
				t.timeout --;
				if(t.timeout == 0)
				{
					_timer_flag |= 0x01;
					if(t.times > 0)
					{
						t.times --;
						if(!t.times)
						{
							t.times = -1;
							_timer_flag |= 0x02;
						}
					}
				}
			}
		}

		void RemoveTimer(int id)
		{
			for(size_t i = 0; i < _timer_list.size(); i ++)
			{
				if(_timer_list[i].id == id)
				{
					_timer_list.erase(_timer_list.begin() + i);
					return;
				}
			}
		}

		void RemoveEmptyTimer()
		{
			_timer_flag = 0;
			for(size_t i = 0; i < _timer_list.size(); i ++)
			{
				timer & t = _timer_list[i];
				if(t.times < 0)
				{
					_timer_list.erase(_timer_list.begin() + i);
					i --;
				}
			}
		}
		
		bool CheckTimer(int id)
		{
			if(_timer_flag == 0) return false;
			for(size_t i = 0; i < _timer_list.size(); i ++)
			{
				timer & t = _timer_list[i];
				if(t.id != id) continue;
				return (t.timeout == 0);
			}
			return false;
		}

		void CheckTriggers(abase::vector<trigger *,abase::fast_alloc<> > & list, int param = 0)
		{
			for(size_t i = 0; i < list.size(); i ++)
			{
				if (!list[i]->IsEnable()) continue;
				if(!list[i]->TestTrigger(this,param))
				{
					return ;
				}
			}
		}

		void CheckPeaceTriggers(abase::vector<trigger *,abase::fast_alloc<> > & list)
		{
			for(size_t i = 0; i < list.size(); i ++)
			{
				if (list[i]->IsBattleTrigger()) continue;
				if (!list[i]->IsEnable()) continue;
				if(!list[i]->TestTrigger(this))
				{
					return ;
				}
			}
		}

		void CheckTriggersNoBreak(abase::vector<trigger *,abase::fast_alloc<> > & list)
		{
			for(size_t i = 0; i < list.size(); i ++)
			{
				if (!list[i]->IsEnable()) continue;
				list[i]->TestTrigger(this);
			}
		}

		void CheckTriggersNoTest(abase::vector<trigger *,abase::fast_alloc<> > & list)
		{
			for(size_t i = 0; i < list.size(); i ++)
			{
				if(!list[i]->TestTrigger(this))
				{
					return ;
				}
			}
		}

		void OnHeartbeat()
		{
			RefreshTimer();
			if(_timer_flag & 0x01)
			{
				CheckTriggersNoBreak(_tm_tri_list);
			}

			if(_timer_flag & 0x02)
			{
				RemoveEmptyTimer();
			}

			CheckTriggers(_hb_tri_list);
		}

		void OnPeaceHeartbeat()
		{
			RefreshTimer();
			if(_timer_flag & 0x01)
			{
				CheckTriggersNoBreak(_tm_tri_list);
			}

			if(_timer_flag & 0x02)
			{
				RemoveEmptyTimer();
			}

			if(!_peace_trigger_count) return;
			CheckPeaceTriggers(_hb_tri_list);
		}

		void OnPeaceHeartbeatInCombat()
		{
			if(!_peace_trigger_count) return;
			CheckPeaceTriggers(_hb_tri_list);
		}

		void StartCombat()
		{
			CheckTriggersNoTest(_st_bat_list);
		}

		void KillTarget(const XID & target)
		{
			//���ﻹ��Ҫ�������һЩ����
			CheckTriggers(_kl_ply_list);
		}

		void OnDeath()
		{
			CheckTriggers(_death_list);
		}

		void PathEnd(int path_id)
		{
			CheckTriggers(_path_list, path_id);
		}

		void OnBorn()
		{
			CheckTriggers(_born_list);
		}

		void OnSkillAttack(int skill_id)
		{
			CheckTriggers(_ski_atk_list, skill_id);
		}
	};

//�����Ƕ��������,Ŀ��ѡ��Ͳ���
//-------------���������������� -------- ����
	class cond_unary : public condition
	{
	protected:
		condition * _cond;
	public:
		
		cond_unary(condition * cond):_cond(cond)
		{
			ASSERT(_cond);
		}

		cond_unary(const cond_unary & rhs)
		{
			_cond = rhs._cond->Clone();
		}

		~cond_unary()
		{
			delete _cond;
		}
		
		virtual bool Check(policy * self,int p)
		{
			return !_cond->Check(self,p);
		}
		virtual int GetConditionType()
		{
			return _cond->GetConditionType();
		}
	};

	class cond_not : public cond_unary
	{
	public:
		
		virtual condition * Clone() const
		{
			return new cond_not(*this);
		}

		cond_not(condition * cond):cond_unary(cond)
		{
		}

		virtual bool Check(policy * self,int p)
		{
			return !_cond->Check(self,p);
		}
		virtual bool IsAutoDisable()
		{
			return _cond->IsAutoDisable();
		}
	};

	class cond_binary : public condition
	{
	protected:
		condition * _left;
		condition * _right;
	public:

		cond_binary(condition * left,condition *right):_left(left),_right(right)
		{
			ASSERT(_left && _right);
		}

		cond_binary(const cond_binary & rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}

		~cond_binary()
		{
			delete _left;
			delete _right;
		}
		
		virtual int GetConditionType()
		{
			return _left->GetConditionType();
		}
	};

	class cond_and : public cond_binary
	{
	public:
		
		virtual condition * Clone() const
		{
			return new cond_and(*this);
		}

		cond_and(condition * left,condition *right):cond_binary(left,right)
		{
		}

		virtual bool Check(policy * self, int p)
		{
			return _left->Check(self,p) && _right->Check(self,p);
		}
		virtual bool IsAutoDisable()
		{
			return _left->IsAutoDisable() && _right->IsAutoDisable();
		}

	};

	class cond_or : public cond_binary
	{
	public:
		
		virtual condition * Clone() const
		{
			return new cond_or(*this);
		}

		cond_or(condition * left,condition *right):cond_binary(left,right)
		{
		}

		virtual bool Check(policy * self, int p)
		{
			return _left->Check(self,p) || _right->Check(self,p);
		}
		virtual bool IsAutoDisable()
		{
			return _left->IsAutoDisable() || _right->IsAutoDisable();
		}

	};

//�����ǻ���Ԫ��
	class cond_hp_less : public condition
	{
		float _ratio;
	public:
		virtual condition * Clone() const
		{
			return new cond_hp_less(*this);
		}
		
		cond_hp_less(float ratio):_ratio(ratio)
		{}
		
		virtual bool Check(policy * self,int);
		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_timer : public condition
	{
		int _timer_id;
	public:
		virtual condition * Clone() const
		{
			return new cond_timer(*this);
		}
		
		cond_timer(int id ):_timer_id(id)
		{}
		
		virtual bool Check(policy * self,int);
		virtual int GetConditionType()
		{
			return TYPE_TIMER;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};
	
	class cond_start_combat : public condition
	{
	public:
		virtual condition * Clone() const
		{
			return new cond_start_combat(*this);
		}
		
		virtual bool Check(policy * self,int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_START_COMBAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_on_death : public condition
	{
	public:
		virtual condition * Clone() const
		{
			return new cond_on_death(*this);
		}
		
		virtual bool Check(policy * self,int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_START_DEATH;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_random : public condition
	{
		float _rate;
	public:
		virtual condition * Clone() const
		{
			return new cond_random(*this);
		}
		
		cond_random(float rate):_rate(rate)
		{}
		
		virtual bool Check(policy * self,int);
		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_kill_target : public condition
	{
	public:
		virtual condition * Clone() const
		{
			return new cond_kill_target(*this);
		}
		
		virtual bool Check(policy * self,int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_KILL_TARGET;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};


	class cond_path_end : public condition
	{
		int _path_id;
	public:
		cond_path_end(int path_id):_path_id(path_id)
		{
		}
		virtual condition * Clone() const
		{
			return new cond_path_end(*this);
		}
		
		virtual bool Check(policy * self,int path_id);

		virtual int GetConditionType()
		{
			return TYPE_PATH_END;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_aggro_count : public condition
	{
		int _count;
	public:
		cond_aggro_count(int count):_count(count)
		{
		}
		virtual condition * Clone() const
		{
			return new cond_aggro_count(*this);
		}

		virtual bool Check(policy * self,int path_id);

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_battle_range : public condition
	{
		float _squared_distance;
	public:
		cond_battle_range(float distance):_squared_distance(distance*distance)
		{
		}
		virtual condition * Clone() const
		{
			return new cond_battle_range(*this);
		}

		virtual bool Check(policy * self,int path_id);

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_on_born : public condition
	{
	public:
		virtual condition * Clone() const
		{
			return new cond_on_born(*this);
		}
		
		virtual bool Check(policy * self,int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_START_BORN;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};


	class cond_attack_by_skill : public condition
	{
		int _skill_id;
	public:
		cond_attack_by_skill(int id):_skill_id(id)
		{
		}

		virtual condition * Clone() const
		{
			return new cond_attack_by_skill(*this);
		}
		
		virtual bool Check(policy * self,int id)
		{
			return _skill_id == id;
		}
		virtual int GetConditionType()
		{
			return TYPE_SKILL_ATTACK;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};
	
//�±���Ȼ���������ж�

	class cond_expr : public condition	//���ʽΪ��, Ŀǰδ�õ�
	{
		expr * _e;
	public:
		cond_expr(expr *e):_e(e) {}

		virtual condition * Clone() const
		{
			return new cond_expr(*this);
		}
		cond_expr (const cond_expr& rhs)
		{
			_e = rhs._e->Clone();
		}
		~cond_expr ()
		{
			delete _e;
		}
		virtual bool Check(policy * self,int) 
		{
			return _e->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_compare_less : public condition	//С��
	{
		expr * _left;
		expr * _right;
	public:
		cond_compare_less (expr* l, expr* r):_left(l), _right(r) {}
		virtual condition * Clone() const
		{
			return new cond_compare_less(*this);
		}
		cond_compare_less (const cond_compare_less& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~cond_compare_less()
		{
			delete _left;
			delete _right;
		}
		virtual bool Check(policy * self,int) 
		{
			return _left->GetValue(self) < _right->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_compare_greater : public condition	//����
	{
		expr * _left;
		expr * _right;
	public:
		cond_compare_greater (expr* l, expr* r):_left(l), _right(r) {}

		virtual condition * Clone() const
		{
			return new cond_compare_greater(*this);
		}
		cond_compare_greater (const cond_compare_greater& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~cond_compare_greater()
		{
			delete _left;
			delete _right;
		}
		virtual bool Check(policy * self,int) 
		{
			return _left->GetValue(self) > _right->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_compare_equal : public condition	//����
	{
		expr * _left;
		expr * _right;
	public:
		cond_compare_equal (expr* l, expr* r):_left(l), _right(r) {}

		virtual condition * Clone() const
		{
			return new cond_compare_equal(*this);
		}
		cond_compare_equal (const cond_compare_equal& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~cond_compare_equal()
		{
			delete _left;
			delete _right;
		}
		virtual bool Check(policy * self,int) 
		{
			return _left->GetValue(self) == _right->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	//���ڵ��ڡ�С�ڵ��ںͲ����ڿ�����cond_not��ϳ�����

//�±������ֻ������ʽ
	class expr_constant: public expr	//����
	{
		int _value;
	public:
		expr_constant(const int value):_value(value) {}
		virtual expr* Clone () const
		{
			return new expr_constant(*this);
		}
		expr_constant (const expr_constant& rhs)
		{
			_value = rhs._value;
		}
		virtual int GetValue(policy*)
		{
			return _value;
		}
		virtual int GetExprType() 
		{
			return EXPR_CONSTANT;
		}
	};

	class expr_common_data : public expr	//��ȫ��������ȡ
	{
		int _key;
	public:
		expr_common_data (const int key):_key(key) {}
		virtual expr* Clone () const
		{
			return new expr_common_data(*this);
		}
		expr_common_data (const expr_common_data& rhs)
		{
			_key = rhs._key;
		}
		virtual int GetValue(policy*);
		virtual int GetExprType() 
		{
			return EXPR_COMMON_DATA;
		}
	};


//�±��ǻ������ʽ����������
	class expr_plus : public expr	//�ӷ�����
	{
		expr *_left;
		expr *_right;
	public:
		expr_plus (expr *a1, expr*a2): _left(a1), _right(a2) {}
		virtual expr* Clone () const
		{
			return new expr_plus(*this);
		}
		expr_plus (const expr_plus& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();	
		}
		~expr_plus ()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self) 
		{
			return _left->GetValue(self) + _right->GetValue(self);
		}
		virtual int GetExprType()
		{
			return EXPR_PLUS;
		}
	};
	class expr_minus : public expr	//��������
	{
		expr *_left;
		expr *_right;
	public:
		expr_minus (expr *a1, expr*a2): _left(a1),_right(a2) {}
		virtual expr* Clone () const
		{
			return new expr_minus(*this);
		}
		expr_minus (const expr_minus& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();	
		}
		~expr_minus ()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self) 
		{
			return _left->GetValue(self) - _right->GetValue(self);
		}
		virtual int GetExprType()
		{
			return EXPR_MINUS;
		}
	};
	class expr_multiply : public expr	//�˷�����
	{
		expr *_left;
		expr *_right;
	public:
		expr_multiply (expr *a1, expr*a2): _left(a1),_right(a2) {}
		virtual expr* Clone () const
		{
			return new expr_multiply(*this);
		}
		expr_multiply (const expr_multiply& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();	
		}
		~expr_multiply ()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self) 
		{
			return _left->GetValue(self) * _right->GetValue(self);
		}
		virtual int GetExprType()
		{
			return EXPR_MULTIPLY;
		}
	};
	class expr_divide : public expr	//��������
	{
		expr *_left;
		expr *_right;
	public:
		expr_divide (expr *a1, expr*a2): _left(a1),_right(a2) {}
		virtual expr* Clone () const
		{
			return new expr_divide(*this);
		}
		expr_divide (const expr_divide& rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();	
		}
		~expr_divide ()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self) 
		{
			int value = _right->GetValue(self);
			if (value ==0)
			{
				throw expr::Exception("�������");
			}
			//return _left->GetValue(self) / _right->GetValue(self);
			return _left->GetValue(self) / value;
		}
		virtual int GetExprType()
		{
			return EXPR_DIVIDE;
		}
	};

//�������а�ĳ��λ�õ���Ҽ���
	class expr_rank_level : public expr
	{
		int _index;
	public:
		expr_rank_level(const int index):_index(index){}
		virtual expr* Clone() const
		{
			return new expr_rank_level(*this);
		}
		expr_rank_level(const expr_rank_level &rhs){
			_index = rhs._index;	
		}
		virtual int GetValue(policy*);
		virtual int GetExprType()
		{
			return EXPR_RANK_LEVEL;
		}
	};

//------ ������ѡ���߼� --------
		
	class target_self : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_self(*this);
		}
		virtual void GetTarget(policy * self, XID & target);

	};

	class target_aggro_first : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_aggro_first(*this);
		}
		virtual void GetTarget(policy * self, XID & target);
	};

	class target_aggro_second : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_aggro_second(*this);
		}
		virtual void GetTarget(policy * self, XID & target);
	};

	class target_aggro_second_rand : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_aggro_second_rand(*this);
		}
		virtual void GetTarget(policy * self, XID & target);
	};

	class target_least_hp : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_least_hp(*this);
		}
		virtual void GetTarget(policy * self, XID & target);
	};

	class target_most_hp : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_most_hp(*this);
		}
		virtual void GetTarget(policy * self, XID & target);
	};

	class target_most_mp : public target
	{
	public:
		virtual target * Clone() const
		{
			return new target_most_mp(*this);
		}
		virtual void GetTarget(policy * self, XID & target);
	};

	class target_class_combo : public target
	{
		int _combo_state;
	public:
		target_class_combo(int combo):_combo_state(combo)
		{}

		virtual target * Clone() const
		{
			return new target_class_combo(*this);
		}
		virtual void GetTarget(policy * self, XID & target);

	};

//-----------�������еĲ���-----------------------	
	class op_attack : public operation
	{
		int _attack_strategy;	//�ⲫ ���������ܣ� ����
	public:
		op_attack(int strategy):_attack_strategy(strategy)
		{}

		virtual operation * Clone() const
		{
			return new op_attack(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return true;
		}
		
	};

	class op_skill : public operation
	{
		int _skill_id;
		int _skill_lvl;
	public:
		op_skill(int skill , int level):_skill_id(skill),_skill_lvl(level)
		{}

		virtual operation * Clone() const
		{
			return new op_skill(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return true;
		}
		
	};

	class op_flee : public operation
	{
	public:
		op_flee()
		{}

		virtual operation * Clone() const
		{
			return new op_flee(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return true;
		}
		
	};
	
	class op_create_timer : public operation
	{	
		int _timerid;
		int _interval;
		int _count;
	public:
		op_create_timer(int id, int interval, int count):_timerid(id),_interval(interval),_count(count)
		{}

		virtual operation * Clone() const
		{
			return new op_create_timer(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
		
	};

	class op_remove_timer : public operation
	{	
		int _timerid;
	public:
		op_remove_timer(int id):_timerid(id)
		{}

		virtual operation * Clone() const
		{
			return new op_remove_timer(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_enable_trigger : public operation
	{	
		int 	_trigger_id;
		bool 	_is_enable;
	public:
		op_enable_trigger(int id,bool is_enable):_trigger_id(id),_is_enable(is_enable)
		{}

		virtual operation * Clone() const
		{
			return new op_enable_trigger(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_exec_trigger : public operation
	{
		trigger * _trigger;
	public:
		op_exec_trigger(trigger * ptri):_trigger(ptri)
		{
		}

		op_exec_trigger(const op_exec_trigger &  rhs)
		{
			_trigger = new trigger(*rhs._trigger);
		}

		~op_exec_trigger()
		{
			if(_trigger) delete _trigger;
		}

		virtual operation * Clone() const
		{
			return new op_exec_trigger(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_set_common_data : public operation
	{
		int _key;
		int _set_value;
	public:
		op_set_common_data (int key, int set_value):
			_key (key), _set_value(set_value) {}
		virtual operation * Clone() const
		{
			return new op_set_common_data(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};

	class op_add_common_data : public operation
	{
		int _key;
		int _add_value;
	public:
		op_add_common_data (int key, int add_value):
			_key (key), _add_value(add_value) {}
		virtual operation * Clone() const
		{
			return new op_add_common_data(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};
	
	class op_assign_common_data : public operation
	{
		int _src;
		int _dest;
	public:
		op_assign_common_data (int src, int dest):
			_src (src), _dest(dest) {}
		virtual operation * Clone() const
		{
			return new op_assign_common_data(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};

	class op_summon_mineral : public operation
	{
		unsigned int _id;
		unsigned int _count;
		unsigned int _lifetime;
		float _range;
		bool _bBind;
	public:
		op_summon_mineral(unsigned int id, unsigned int count, unsigned int hp, float range, bool bBind):
			_id (id), _count(count), _lifetime(hp), _range(range), _bBind(bBind) {}
		virtual operation * Clone() const
		{
			return new op_summon_mineral(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};

	class op_drop_item : public operation
	{
		unsigned int _item_id;
		unsigned int _item_num;
		unsigned int _expire_date;
	public:
		op_drop_item(unsigned int id, unsigned int num, unsigned int date):
			_item_id (id), _item_num(num), _expire_date(date) {}
		virtual operation * Clone() const
		{
			return new op_drop_item(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};
	
	class op_change_aggro : public operation
	{
		int _change_aggro_value;
	public:
		op_change_aggro(int value): _change_aggro_value(value){}
		virtual operation * Clone() const
		{
			return new op_change_aggro(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return true;}
	};
	
	class op_start_event: public operation
	{
		int _event_id;
	public:
		op_start_event(int value): _event_id(value){}
		virtual operation * Clone() const
		{
			return new op_start_event(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};
	
	class op_stop_event: public operation
	{
		int _event_id;
	public:
		op_stop_event(int value): _event_id(value){}
		virtual operation * Clone() const
		{
			return new op_stop_event(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget() {return false;}
	};


	class message
	{
		void * _msg;
		size_t _size;

		void operator=(const message & );
	public:
		explicit message(const char * str)
		{
			size_t len = strlen(str);
			_size = len*2;
			_msg = abase::fastalloc(_size);
			for(size_t i = 0; i < len; i ++)
			{
				((char*)_msg)[i*2] = str[i];
				((char*)_msg)[i*2+1] = 0;
			}
		}

		message(const void * msg, size_t size)
		{
			if(size )
			{
				_msg = abase::fastalloc(size);
				_size = size;
				memcpy(_msg,msg,size);
			}
			else
			{
				_msg = NULL;
				_size = 0;
			}
		}


		message(const message & rhs)
		{
			_size = rhs._size;
			if(_size)
			{
				_msg = abase::fastalloc(_size);
				memcpy(_msg,rhs._msg,_size);
			}
			else
			{
				_msg = NULL;
			}
		}

		~message()
		{
			if(_size) abase::fastfree(_msg,_size);
		}

		inline void * data() { return _msg;}
		inline size_t size() { return _size;}

	};

	class op_say : public operation
	{
		message _msg;
	public:
		op_say(const char * str):_msg(str)
		{}

		op_say(const void * msg, size_t size):_msg(msg,size)
		{}


		virtual operation * Clone() const
		{
			return new op_say(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_reset_aggro : public operation
	{	
	public:
		op_reset_aggro()
		{}

		virtual operation * Clone() const
		{
			return new op_reset_aggro(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_swap_aggro : public operation
	{	
		size_t _index1;
		size_t _index2;
	public:
		op_swap_aggro(size_t index1,size_t index2):_index1(index1),_index2(index2)
		{}

		virtual operation * Clone() const
		{
			return new op_swap_aggro(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_be_taunted : public operation
	{
	public:
		op_be_taunted()
		{}

		virtual operation * Clone() const
		{
			return new op_be_taunted(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_fade_target : public operation
	{
	public:
		op_fade_target()
		{}

		virtual operation * Clone() const
		{
			return new op_fade_target(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_aggro_fade: public operation
	{
	public:
		op_aggro_fade()
		{}

		virtual operation * Clone() const
		{
			return new op_aggro_fade(*this);
		}
		virtual bool DoSomething(policy * self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_break : public operation
	{
	public:
		op_break()
		{}

		virtual operation * Clone() const
		{
			return new op_break(*this);
		}
		virtual bool DoSomething(policy * self)
		{
			return false;
		}
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_active_spawner : public operation
	{
		bool _is_active_spawner;
		int _ctrl_id;
	public:
		op_active_spawner(int id, bool bStop)
		{
			_ctrl_id = id;
			_is_active_spawner = !bStop;
		}

		virtual operation * Clone() const
		{
			return new op_active_spawner(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_create_minors : public operation
	{
		int _mob_id;
		int _vis_id;
		int _life;
		int _count;
		bool _has_leader;
		bool _die_with_leader;
		message _name;
		float _radius;

	public:
		op_create_minors(int id,int vis_id, int life, bool has_leader,  bool die_with_leader, int count, void * name, size_t name_len, float radius)
			: _mob_id(id), _vis_id(vis_id),_life(life),_count(count), _has_leader(has_leader),_die_with_leader(die_with_leader),_name(name, name_len),_radius(radius)
		{
			if(_count <= 0) _count = 1;
			if(_count > 300) count = 300;
			if(_radius < 0) _radius =0;
			if(_radius > 50) _radius = 50;
		}

		virtual operation * Clone() const
		{
			return new op_create_minors(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_task_trigger : public operation
	{
		int _event;
	public:
		op_task_trigger(int event):_event(event)
		{
		}
		
		virtual operation * Clone() const
		{
			return new op_task_trigger(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_disappear : public operation
	{
	public:
		op_disappear()
		{
		}
		
		virtual operation * Clone() const
		{
			return new op_disappear(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_switch_path : public operation
	{
		int _path_id;
		int _type;
	public:
		op_switch_path(int path_id, int type):_path_id(path_id),_type(type)
		{
		}
		virtual operation * Clone() const
		{
			return new op_switch_path(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_cajole_mobs: public operation
	{
		float _radius;
	public:
		op_cajole_mobs(float radius):_radius(radius)
		{
			if(_radius > 100.f) _radius = 100.f;
			if(_radius <= 0.f) _radius = 0.1f;
		}
		virtual operation * Clone() const
		{
			return new op_cajole_mobs(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_range_skill: public operation
	{
		float _radius;
		int   _skill;
		int   _level;
	public:
		op_range_skill(float radius, int skill, int level):_radius(radius),_skill(skill),_level(level)
		{
			if(_radius <= 0) _radius = 0.1f;
		}
		virtual operation * Clone() const
		{
			return new op_range_skill(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

	class op_combat_reset : public operation
	{
	public:
		op_combat_reset()
		{}

		virtual operation * Clone() const
		{
			return new op_combat_reset(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy * self);
	};

}

#endif

