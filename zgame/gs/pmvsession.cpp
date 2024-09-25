#include "actobject.h"
#include "pmvsession.h"
#include "world.h"
#include "gmatrix.h"
#include "npc.h"
#include "ainpc.h"
#include "aipolicy.h"
#include "clstab.h"
#include <common/protocol.h>
#include "npcgenerator.h"
#include "player_imp.h"
#include "skill_filter.h"
#include "sfilterdef.h"
#include <gsp_if.h>
#include "invincible_filter.h"
#include <typeinfo>

DEFINE_SUBSTANCE(session_p_start_move, act_session , CLS_SESSION_P_START_MOVE);
DEFINE_SUBSTANCE(session_p_move, act_session ,       CLS_SESSION_P_MOVE);
DEFINE_SUBSTANCE(session_p_stop_move, act_session ,  CLS_SESSION_P_STOP_MOVE);

move_control & session_p_start_move::GetControl()
{
	return ((gplayer_imp *)_imp)->_mcontrol;
}

session_p_start_move::session_p_start_move(gplayer_imp * imp):act_timer_session(imp),_base_time(0)
{
}

bool session_p_start_move::StartSession(act_session * next_cmd) 
{
	move_control & ctrl = GetControl();
	gplayer_imp *pImp = (gplayer_imp *) _imp;
	g_timer.get_systime(ctrl.val_start);

	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = pImp->GetNextSessionID();
	_self_id = pImp->_parent->ID;

	ctrl.start_seq = pImp->_commander->GetCurMoveSeq();
	ctrl.session_id =  _session_id;
	int tick = MILLISEC_TO_TICK(STD_MOVE_USE_TIME);	
	//�����ʱ����Ϊ��ռ��һ��ʱ���session���� ʵ�ʺ����������ܻᳬ����ʱ�䣬����Ҳ���������
	if(next_cmd)
	{
		//������������Ҫʱ�������,��ֱ�������ʱ�������ӳ�ʱ��
		int next_time = next_cmd->GetMoveTime();
		if(next_time > 0)
		{
			//Ҫע���ֹ�ͻ��˵���ƭ
			tick = MILLISEC_TO_TICK(next_time);
			if(tick <= 0) tick = 1;
		}
	}
	SetTimer(g_timer,tick,1);
	return true;
}

bool session_p_start_move::RepeatSession()
{
	return false;
}

bool session_p_start_move::EndSession()
{
	if(_session_id != -1)
	{
		g_timer.get_systime(GetControl().val_end);
#ifdef __TEST_ATTACK_DELAY__
		timeval tv4;
		gettimeofday(&tv4,NULL);
		GLog::log(GLOG_INFO, "%d session_p_start_move EndSession %d -- %ld.%06ld",_self_id.id, GetControl().GetMilliRange(),tv4.tv_sec,tv4.tv_usec);
#endif
	}
	return true;
}

bool session_p_start_move::Mutable(act_session * next_cmd)
{
	if(next_cmd == NULL) return false;	//���ԵĻ�ֱ�ӷ���
	move_control & ctrl = GetControl();
	if(ctrl.start_seq < 0) return false;

	ASSERT(_session_id != -1);
	int next_time = next_cmd->GetMoveTime();
	if(next_time > 0)
	{
		//����һ���Ƿ���Ҫֹͣ
		timeval end;
		g_timer.get_systime(end);
		int t = move_control::MilliRange(ctrl.val_start, end) + _base_time;
		int t2 = next_time - t;
		if(t2 < FAST_MOVE_SKIP_TIME)
		{
			//���ʱ���������Χ֮�� ��ֱ�ӷ�Χ ,����һ������������ʱ���ӳ� 
#ifdef __TEST_ATTACK_DELAY__
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d %s Mutable end -- %ld.%06ld",_self_id.id,typeid(*this).name(),tv4.tv_sec ,tv4.tv_usec);
#endif
			return true;
		}

		//���¿����µĶ�ʱ��
		RemoveTimer();
		//��һ��Session ID��������Ϣ��ͻ
		_session_id = _imp->GetNextSessionID();
		ctrl.session_id =  _session_id;
		SetTimer(g_timer,MILLISEC_TO_TICK(t2),1);

#ifdef __TEST_ATTACK_DELAY__
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d %s Mutable restart %d (%d %d) -- %ld.%06ld",_self_id.id, typeid(*this).name(), MILLISEC_TO_TICK(t2), t, next_time, tv4.tv_sec ,tv4.tv_usec);
		}
#endif
		return false;
	}
	return true;
}


bool session_p_move::StartSession(act_session * next_cmd)
{
	if(!CheckCmdSeq<0>()) 
	{
		return false;
	}

	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;


	gplayer_imp *pImp = (gplayer_imp *) _imp;
	A3DVECTOR predict = _target;
	if(!_on_carrier)
	{
		_target -= _imp->_parent->pos;
	}
	else
	{
		_target -= pImp->GetParent()->rpos;
	}

	if(_use_time < MIN_MOVE_USE_TIME ) _use_time = MIN_MOVE_USE_TIME;
	if(!_on_carrier && !CheckPlayerMove(_imp,predict,_target,_move_mode,_use_time,_seq))
	{
		//������
		//������
		return false;
	}

	//Add by Houjun 2011-07-05, ����ƶ���������
	if(!_imp->CheckMoveArea(predict))
	{
		printf("������������ƶ��ķ�Χ������������Ŷ��������������\n");
		_imp->UpdateStopMove(_move_mode);

		int dir = _imp->_parent->dir;
		if(!_on_carrier)
		{
			_imp->_runner->stop_move(predict, _speed, dir, _move_mode);
		}
		else
		{
			_imp->_runner->player_stop_move_on_carrier(predict,_speed,dir,_move_mode);
		}
		return false;
	}

	_imp->UpdateMoveMode(_move_mode);

	//�����Լ��ƶ�����Ϣ��������Ҫ��һ���ļ򻯲���
	//����Ծ���Զ��λ�ü��ٲ��ŵ�Ƶ�ʵȵ�

	if(!_on_carrier)
	{
		_imp->_runner->move(predict,_use_time,_speed,_move_mode);
	}
	else
	{
		_imp->_runner->player_move_on_carrier(predict,_use_time,_speed,_move_mode);
	}
	_imp->StepMove(_target);
	__PRINTF("MMMM MMMMM MMM:%f %f %f ---- %f %f %f\n",_target.x,_target.y,_target.z,
			_imp->_parent->pos.x,_imp->_parent->pos.y,_imp->_parent->pos.z);

	//�����ӳ��ٶ�		
	move_control & ctrl = GetControl();
	if(ctrl.IsInValid(_cur_seq,_session_id))
	{
#ifdef __TEST_ATTACK_DELAY__
		GLog::log(GLOG_INFO, "%d session_p_move NormalSession",_self_id.id);
#endif
		//����������start_moveģʽ�� ʹ��ͨ���ķ�ʽ
		ctrl.start_seq = -1;
		_cur_seq = -1;
		//�����ӳ��ٶ�
		int tick = MILLISEC_TO_TICK(_use_time) - 1;
		if(tick <= 8) tick = 8;
		SetTimer(g_timer,tick,1);
	}
	else
	{
		//�µĵ��ӳٷ�ʽ
		timeval end;
		g_timer.get_systime(end);
		int t = move_control::MilliRange(GetControl().val_start, end);
		t = t - _use_time;
		if(t > 0)
		{
			//���пռ����ƺ͵���
			//ǰ���ʱ������ſ��õ�����
			//���ʹ����100ms�Ķ��
			if(t > 100) t = 100;
			_base_time = t;
		}
		else
		{
			//ǰ��session��ʱ�䲻�㣬������Ҫ�����ʱ����ʹ�õ�
			//���ǵ�
			if(t < -MAX_MOVE_USE_TIME) t = -MAX_MOVE_USE_TIME;
			_base_time = t;
		}
		ctrl.val_start = end;	//�����ڿ�ʼ��ʱ  ǰ��Ľ���Ѿ�����base_time
		ctrl.session_id = _session_id;
		ctrl.start_seq = _imp->_commander->GetCurMoveSeq();

		//����base_time���趨�ӳ�ʱ��
		int tick = -_base_time;
		//base_timeʱ��󣬺��������Ϳ���׼��ִ����

		//������ں���������Ѿ����ﲢ�ҿ��Ի�ø������ִ��ʱ��
		//��ô�������ʱ���������ӳ�(����ӳپ��Ƕ���Ժ�ִ����һ������)
		//�����������û�е����ô��һ�����Ƶ�ʱ�䣬 ʹ���ƶ���ƽ��ʱ������Ϊִ��ʱ�䡣
		int next_tick = STD_MOVE_USE_TIME; //���޺���������ձ�׼ʱ��Ԥ��
		if(next_cmd)
		{
			//���Ǻ����������Ƿ���Ҫʱ��
			int next_time = next_cmd->GetMoveTime();
			if(next_time > 0) next_tick = next_time;	//���������Ϊ�˱���ͻ�����ƭ
		}

		tick += next_tick;
		tick = MILLISEC_TO_TICK(tick);
		if(tick <= 0) tick = 1;

		SetTimer(g_timer,tick,1);

	}
	return true;
}

bool session_p_move::RepeatSession()
{
	return false;
}

bool session_p_move::EndSession()
{
	if(_session_id != -1)
	{
		move_control & ctrl = GetControl();
		if(ctrl.start_seq >= 0)
		{
			ctrl.val_start.tv_usec -= _base_time*1000;
			g_timer.get_systime(ctrl.val_end);
#ifdef __TEST_ATTACK_DELAY__
			{
				timeval tv4;
				gettimeofday(&tv4,NULL);
				GLog::log(GLOG_INFO, "%d session_p_move EndSession %d(%d) usetime:%d -- %ld.%06ld", _self_id.id, ctrl.GetMilliRange(),_base_time, _use_time , tv4.tv_sec ,tv4.tv_usec);
			}
#endif
		}
	}
	return true;
}


bool session_p_stop_move::StartSession(act_session * next_cmd)
{
	if(!CheckCmdSeq<0>()) 
	{
		return false;
	}

	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	gplayer_imp *pImp = (gplayer_imp *) _imp;
	A3DVECTOR pos = _target;
	if(!_on_carrier)
	{
		_target -= _imp->_parent->pos;
	}
	else
	{
		_target -= pImp->GetParent()->rpos;
	}
	//ÿ��stopmove ��Ҫ��¼����
	if(_use_time < MIN_MOVE_USE_TIME ) _use_time = MIN_MOVE_USE_TIME;
	if(!_on_carrier && !CheckPlayerMove(_imp,pos,_target,_move_mode,_use_time,_seq))
	{	
		//������
		//������
		return false;
	}

	//Add by Houjun 2011-07-05, ����ƶ���������
	if(!_imp->CheckMoveArea(pos))
	{
		printf("Stop Move ������������ƶ��ķ�Χ������������Ŷ��������������\n");
		_imp->UpdateStopMove(_move_mode);

		if(!_on_carrier)
		{
			_imp->_runner->stop_move(pos, _speed, _dir, _move_mode);
		}
		else
		{
			_imp->_runner->player_stop_move_on_carrier(pos,_speed,_dir,_move_mode);
		}
		return false;
	}

	_imp->UpdateStopMove(_move_mode);

	if(!_on_carrier)
	{
		_imp->_runner->stop_move(pos,_speed,_dir,_move_mode);
	}
	else
	{
		_imp->_runner->player_stop_move_on_carrier(pos,_speed,_dir,_move_mode);
	}
	//����ֹͣ�ƶ�����Ϣ
	
	//�����������ƶ�
	_imp->StepMove(_target);
	__PRINTF("MMMM SSSSS MMM:%f %f %f ---- %f %f %f\n",_target.x,_target.y,_target.z,
			_imp->_parent->pos.x,_imp->_parent->pos.y,_imp->_parent->pos.z);

	move_control & ctrl = GetControl();
	if(ctrl.IsInValid(_cur_seq,_session_id))
	{
#ifdef __TEST_ATTACK_DELAY__
		GLog::log(GLOG_INFO, "%d session_p_stop_move NormalSession",_self_id.id);
#endif
		//����������start_moveģʽ�� ʹ��ͨ���ķ�ʽ
		ctrl.start_seq = -1;
		_cur_seq = -1;
		//�����ӳ��ٶ�
		int t = MILLISEC_TO_TICK(_use_time);
		if(t < 8) t = 8;
		SetTimer(g_timer,t,1);
		return true;
	}
	else
	{
		//�µĵ��ӳٷ�ʽ
		timeval end;
		g_timer.get_systime(end);
		int t = move_control::MilliRange(GetControl().val_start, end);
		t = t - _use_time;
		ctrl.start_seq = -1;
		_cur_seq = -1;
		
#ifdef __TEST_ATTACK_DELAY__
		int t1 = t;
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d session_p_stop_move t:%d usetime:%d -- %ld.%06ld", _self_id.id, t1,_use_time , tv4.tv_sec ,tv4.tv_usec);
		}
#endif

		if(t > 0) return false;
		if(t < -MAX_MOVE_USE_TIME) t = -MAX_MOVE_USE_TIME;

		//����base_time���趨�ӳ�ʱ��
		int tick = -t;
#ifdef __TEST_ATTACK_DELAY__
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d session_p_stop_move t1:%d tick %d -- %ld.%06ld", _self_id.id, t, MILLISEC_TO_TICK(tick), tv4.tv_sec ,tv4.tv_usec);
		}
#endif
		tick = MILLISEC_TO_TICK(tick);
		if(tick <= 0) return false;

		SetTimer(g_timer,tick,1);

		//���ñ�������Ҫ����tick������ʱ�ж�֮��
		_tick = tick;
		return true;

	}
	return false;
}

int 
session_p_stop_move::GetLimitTime()
{
	return _tick;
}


bool 
session_p_stop_move::EndSession()
{
	if(_session_id != -1)
	{
		move_control & ctrl = GetControl();
		ctrl.start_seq = -1;
		ctrl.session_id = -1;
	}
	return true;
}

void 
session_p_stop_move::DoTimeoutLog()
{
	std::string str;
	if(DumpContent(str))
	{
		GLog::log(GLOG_ERR, "%s", str.c_str());
	}
}

