#include "gmatrix.h"
#include "playerduel.h"
#include "world.h"
#include "player_imp.h"
#include "obj_interface.h"
#include "duel_filter.h"
#include "filter_man.h"
#include "sfilterdef.h"

void 
player_duel::PlayerDuelRequest(gplayer_imp * pImp,const XID & target)
{
	//����ǰ����������Ƿ�ս��ģʽ������ģʽ���ж�
	if(_invite.is_invite || _invite.is_agree_duel)
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_DUEL_TWICE);
		return ;
	}

	if(pImp->GetObjectLevel() < MIN_DUEL_LEVEL) 
	{
		return ;
	}

	if(!target.IsPlayer())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_TARGET);
		return;
	}
	
	if(IsDuelMode())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_DUEL_TWICE);
		return ;
	}
	
	_invite.duel_target = target;
	_invite.timeout = DUEL_INVITE_TIMEOUT;
	_invite.is_invite = true;
	_invite.is_agree_duel = false;
	pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_REQUEST,target,0);
	
}

void
player_duel::PlayerDuelReply(gplayer_imp * pImp,const XID & target,int param)
{
	if(!target.IsPlayer())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_TARGET);
		return;
	}

	if(_invite.is_agree_duel || param || pImp->GetObjectLevel() < MIN_DUEL_LEVEL) 
	{
		//����Ѿ�ͬ����� ���߾ܾ��˾���������Դ˴ξ�����Ϣ
		//�ܾ��˾���
		pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_REPLY,target,2);
		return;
	}
	
	if(IsDuelMode())
	{
		//����Ѿ���ʼ������ ��ܾ�����
		pImp->_runner->error_message(S2C::ERR_CANNOT_DUEL_TWICE);
		return ;
	}
	
	_invite.duel_target = target;
	_invite.timeout = 3;	//����ͼ��볬ʱ��
	_invite.is_invite = false;
	_invite.is_agree_duel = true;
	
	//ͬ���˾���,����ͬ�����Ϣ
	pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_REPLY,target,param);
}

void 
player_duel::MsgDuelRequest(gplayer_imp * pImp,const XID & target)
{
	__PRINTF("MsgDuelRequest\n");
	if(_invite.is_agree_duel || IsDuelMode() || pImp->IsCombatState())
	{
		//ֱ�Ӿܾ�����
		pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_REPLY,target,DUEL_REPLY_ERR_STATUS);
		return;
	}
	
	pImp->_runner->duel_recv_request(target);
}

void 
player_duel::MsgDuelReply(gplayer_imp * pImp,const XID & who,int param)
{
	__PRINTF("MsgDuelReply\n");
	if(IsDuelMode())
	{
		//����Ѿ���������������
		return;
	}
	
	if(param)
	{
		//���󱻾ܾ�
		if(_invite.is_invite && _invite.duel_target == who)
		{
			_invite.is_invite = false;
			pImp->_runner->duel_reject_request(who,param);
		}
		else if(_invite.is_agree_duel && _invite.duel_target == who)
		{
			_invite.is_agree_duel = false;
			pImp->_runner->error_message(S2C::ERR_CREATE_DUEL_FAILED);
		}
		return ;
	}
	
	if(!_invite.is_invite || _invite.duel_target != who || pImp->IsCombatState())
	{
		//��Ҳ����Ͼ���Ҫ�� ����һ������ʧ�ܵ����
		pImp->SendTo<0>(GM_MSG_ERROR_MESSAGE,who,S2C::ERR_CREATE_DUEL_FAILED);
		return ;
	}

	//���Կ�ʼ����
	//�����������ʱ״̬,��֪ͨ˫��
	_invite.is_agree_duel = false;
	_invite.is_invite = false;
	_duel_target = who;
	SetDuelPrepareMode();
	_duel_timeout = 3;	//3��Ԥ��

	//֪ͨ�Է���ʼ
	pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_PREPARE,who,0);
	pImp->_runner->duel_prepare(who,3);
}

void 
player_duel::MsgDuelPrepare(gplayer_imp * pImp,const XID & who)
{
	__PRINTF("MsgDuelPrepare\n");
	if(_invite.is_agree_duel && who == _invite.duel_target)
	{
		_invite.is_agree_duel = false;
		_invite.is_invite = false;
		_duel_target = who;
		SetDuelPrepareMode();
		_duel_timeout = 10;	//10��Ԥ��,��ԭ���ǵȴ�����һ���˷�����ʼ��������Ϣ

		pImp->_runner->duel_prepare(who,3);
	}
	else
	{
		//֪ͨ����ֹͣ����?
		pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_CANCEL,who,0);
	}
}

void 
player_duel::MsgDuelCancel(gplayer_imp * pImp, const XID & who)
{
	__PRINTF("MsgDuelCancel\n");
	if(!IsDuelMode() || _duel_target != who) return;
	ClearDuelMode(pImp);
	_duel_target = XID(-1,-1);
	pImp->_runner->duel_cancel(who);
}

void 
player_duel::MsgDuelStart(gplayer_imp * pImp,const XID & who)
{
	__PRINTF("MsgDuelStart\n");
	if(!(IsPrepareMode() && _duel_target == who))
	{
		//״̬����ȷ������һ��Cancel
		pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_CANCEL,who,0);
		return;
	}
	SetDuelStartMode(pImp);
}

void
player_duel::MsgDuelStop(gplayer_imp * pImp, const XID & who,int param)
{
	__PRINTF("MsgDuelStop\n");
	if(!IsDuelMode() || _duel_target != who) return;
	ClearDuelMode(pImp);
	_duel_target = XID(-1,-1);

	if (param == DUEL_RESULT_VICTORY) { // ����ʤ��
		// ���»�Ծ��
		pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_JUEDOU);		
	}
}

void 
player_duel::Heartbeat(gplayer_imp *pImp)
{
	if(IsDuelMode())
	{
		if(IsPrepareMode())
		{	
			__PRINTF("DuelPrepare Timer\n");
			//��ʱԤ������ģʽ
			_duel_timeout --;
			if(_duel_timeout <= 0)
			{
				//���;�����ʼ����Ϣ
				_duel_timeout = 3;
				XID list[2] =  {pImp->_parent->ID,_duel_target};
				MSG msg;
				BuildMessage(msg,GM_MSG_PLAYER_DUEL_START,list[1],
						list[0],pImp->_parent->pos, 0, &_duel_target, sizeof(_duel_target));
				gmatrix::SendMessage(list,list + 2, msg);
			}
		}
		else
		{
			__PRINTF("Duel Timer\n");
			//���ھ�����ģʽ
			_duel_timeout --;
			bool bRst = true;
			int val = 0;
			if(_duel_timeout <= 0)
			{
				bRst = false;
				val = DUEL_RESULT_TIMEOUT;
			}
			if(bRst)
			{
				//û�г�ʱ�����з�Χ��λ�õ��ж�
				world::object_info info;
				if(!pImp->_plane->QueryObject(_duel_target,info)
						|| (info.state & world::QUERY_OBJECT_STATE_ZOMBIE)
						|| (info.pos.squared_distance(pImp->_parent->pos)) >= 100.f*100.f)
				{
					val = DUEL_RESULT_DEUCE;
					bRst = false;
				}
			}

			if(!bRst)
			{
				//��ʱ,��������
				//���ͽ�����������Ϣ
				_duel_timeout = 3;
				XID list[2] =  {pImp->_parent->ID,_duel_target};
				MSG msg;
				BuildMessage(msg,GM_MSG_PLAYER_DUEL_STOP,list[1],
						list[0],pImp->_parent->pos,
						val,&_duel_target,sizeof(_duel_target));
				gmatrix::SendMessage(list,list + 2, msg);
				pImp->_runner->duel_result(_duel_target, false);
			}
		}
	}
	else
	{
		if(_invite.is_invite)
		{
			__PRINTF("Duel Invite Timer\n");
			_invite.timeout --;
			if(_invite.timeout <=0) 
			{
				//����ʧ��
				pImp->_runner->duel_reject_request(_invite.duel_target,DUEL_REPLY_TIMEOUT);
				_invite.is_invite = false;
			}
		}
		else
		if(_invite.is_agree_duel)
		{
			__PRINTF("Duel Agree Timer\n");
			_invite.timeout --;
			if(_invite.timeout <=0) 
			{
				//����ʧ��
				pImp->_runner->error_message(S2C::ERR_CREATE_DUEL_FAILED);
				_invite.is_agree_duel = false;
			}
		}
	}
}

void 
player_duel::OnDeath(gplayer_imp *pImp, bool duel_failed)
{
	if(duel_failed)
	{
		__PRINTF("Duel OnDeath \n");
		//����ʧ��
		if(IsDuelStarted())
		{
			//������Ϣ
			pImp->_runner->duel_result(_duel_target, true);

			//����������������Ϣ
			pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_STOP,_duel_target,DUEL_RESULT_VICTORY,
					&_duel_target,sizeof(_duel_target));
					
		}
		ClearDuelMode(pImp);
		_duel_target = XID(-1,-1);
	}
	else
	{
		//�Ǿ���ʧ��,Ҳ��Ҫ���н��������Ĳ���
		if(IsDuelStarted())
		{
			__PRINTF("Duel OnDeath \n");
			pImp->_runner->duel_result(_duel_target, false);

			//����������������Ϣ
			pImp->SendTo<0>(GM_MSG_PLAYER_DUEL_STOP,_duel_target,DUEL_RESULT_DEUCE,
					&_duel_target,sizeof(_duel_target));

			ClearDuelMode(pImp);
			_duel_target = XID(-1,-1);
		}
	}
}

void 
player_duel::SetDuelPrepareMode()
{
	_duel_mode = 1;
}

void 
player_duel::SetDuelStartMode(gplayer_imp * pImp)
{
	_duel_mode = 2;
	_duel_timeout = DUEL_TIME_LIMIT;
	//����һ��filter 
	object_interface oif(pImp);
	oif.AddFilter(new pvp_duel_filter(pImp,_duel_target,FILTER_INDEX_DUEL_FILTER));

}

void 
player_duel::ClearDuelMode(gplayer_imp * pImp)
{
	_duel_mode = 0;
	if(pImp->_filters.IsFilterExist(FILTER_INDEX_DUEL_FILTER))
	{
		//ɾ������Ҫ��filter
		//֪ͨ����Լ��뿪�˾���ģʽ ��filter ����ɰ� 

		//pImp->_filters.RemoveFilter(FILTER_INDEX_DUEL_FILTER);

		//�ټ���һ���µ�filter��������Ҫ����ʱ5����޵�
		//ASSERT(_duel_target.IsValid());
		pImp->_filters.ModifyFilter(FILTER_INDEX_DUEL_FILTER,FMID_DUEL_END,NULL,0);
	}
}

bool 
player_duel::IsPrepareMode()
{
	return _duel_mode == 1;
}

bool 
player_duel::IsDuelStarted()
{
	return _duel_mode == 2;
}
	

void 
mafia_duel::CheckBattleStart(gplayer_imp * pImp,int mafia_id)
{
	int counter = gmatrix::Instance()->GetBattleCtrl().GetBattleCounter();
	if(!counter)  return;
	CheckAndSetBattle(mafia_id);
}

void 
mafia_duel::CheckBattleEnd(gplayer_imp * pImp)
{
	int t = g_timer.get_systime();
	if(_end_time <= t)
	{
		ClearBattle();
	}
}

void mafia_duel::ClearBattle()
{
	_state = STATE_NORMAL;
	_enemy = 0;
	_end_time = 0;
	__PRINTF("ĳĳ���뿪ս��״̬\n");
}

void mafia_duel::CheckAndSetBattle(int mafia_id)
{
	int enemy = 0;
	int end_time = 0;
	if(!gmatrix::Instance()->GetBattleCtrl().GetMafiaBattle(mafia_id, &enemy, &end_time))
	{
		return ;
	}
	if(enemy && end_time)
	{
		__PRINTF("ĳĳ�����ս��״̬%d --> %d\n",mafia_id, enemy);
		//��ʼ��ս״̬
		_state = STATE_BATTLE;
		_enemy = enemy;
		_end_time = end_time;
	}
}

void mafia_duel::OnDeath(int mafia_id, const XID & self, const XID & killer)
{
	if(_state == STATE_BATTLE)
	{
		_deaths ++;
		GMSV::SendMafiaDuelDeath(mafia_id, self.id, killer.id);

	}
}

void faction_hostile_duel::Refresh( gplayer_imp* pImp, unsigned int faction_id, bool bForce )
{
	if( gmatrix::Instance()->GetFHManager().RefreshHostileInfo( faction_id, _refresh_seq, _hostiles, bForce ) )
	{       
		//__PRINTF( "��� %d �ж԰�����Ϣ������, faction_id=%d", pImp->_parent->ID.id, faction_id );
		//for( abase::static_set<unsigned int>::iterator it = _hostiles.begin(); it != _hostiles.end(); ++it )
		//{       
		//	__PRINTF( ", %d", *it );
		//}       
		//__PRINTF( "\n" ); 
	} 
}
