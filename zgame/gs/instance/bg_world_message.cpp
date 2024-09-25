#include "battleground_manager.h"
#include "../world.h"
#include "../player_imp.h"
#include "../usermsg.h"
#include "../aei_filter.h"
#include "battleground_ctrl.h"

/*
 *	����������Ϣ
 */
int
battleground_world_message_handler::RecvExternMessage(int msg_tag, const MSG & msg)
{
	//���ڸ�����ֻ���ܸ���ҵ���Ϣ
	if(msg.target.type != GM_TYPE_PLAYER && msg.target.type != GM_TYPE_MANAGER ) return 0;
	if(msg_tag != _manager->GetWorldTag())
	{
		//����ĳЩ��Ϣ.......
	}

	//����ҪҪ����ĳЩ��Ϣ
	//���ﻹӦ��ֱ�Ӵ���ĳЩ��Ϣ
	//����ת����Ҫ�����ж�

	//��Щ��Ϣ��Ҫ��������ת��
	switch(msg.message)
	{
		/*
		case GM_MSG_SWITCH_USER_DATA:
			{
				if(msg.content_length < sizeof(instance_key)) return 0;
				instance_key * key = (instance_key*)msg.content;
				//��Ϣ��ͷ��������instance_key
				//���渽�����������
				ASSERT(key->target.key_level1 == msg.source.id);
				instance_hash_key hkey;
				_manager->TransformInstanceKey(key->target, hkey);
				int index = _manager->GetWorldByKey(hkey);
				if(index < 0) return 0;
				return 0;
				//return _manager->GetWorldByIndex(index)->DispatchMessage(msg); $$$$$ BW
			}
			*/

		case GM_MSG_PLANE_SWITCH_REQUEST:
		//ȷ���л����� 
		//��鸱�������Ƿ���ڣ���������ڣ������ȴ��б�
		//����Ѿ����ڣ���ˢ��һ�·�������ʱ���־�������سɹ���־
		//�������Ҫ��������������ɾ������ 
		{
			if(msg.content_length != sizeof(instance_key)) 
			{
				ASSERT(false);
				return 0;
			}
			instance_key * key = (instance_key*)msg.content;
			int rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param);
			if(rst == 0)
			{
				//���л�����Ϣ
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				gmatrix::SendMessage(nmsg);
			}
			else if(rst > 0)
			{
				MSG nmsg;
				BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
				gmatrix::SendMessage(nmsg);
			}
			//���rstС��0��������ǰ�޷�ȷ���Ƿ��ܹ��������磬��Ҫ�ȴ� ���Է���ʲô������
		}
		return 0;

		case GM_MSG_CREATE_BATTLEGROUND:
		{
			//battle_ground_param &param = *(battle_ground_param*) msg.content;
			//_manager->CreateBattleGround(param);
		}
		return 0;

		default:
		if(msg.target.type == GM_TYPE_PLAYER)
		{
			int index = _manager->GetPlayerWorldIdx(msg.target.id);
			if(index < 0) return 0;
			return 0;
			//return _manager->GetWorldByIndex(index)->DispatchMessage(msg);
		}
		//��������Ϣ��δ���� ..........
	}
	return 0;
}

void
battleground_world_message_handler::SetInstanceFilter(gplayer_imp * pImp,instance_key & ikey)
{
	//���븱��key���filter,���filter���л�������ʱ������б���ͻָ�
	pImp->_filters.AddFilter(new aebf_filter(pImp,FILTER_CHECK_INSTANCE_KEY,ikey.target.key_level3));
}

void 
battleground_world_message_handler::PlayerPreEnterServer(gplayer * pPlayer, gplayer_imp * pImp,instance_key & ikey)
{	
	world * pPlane = pImp->_plane;
	
	//������ҵİ����趨�������ط�
	//���������Ĳ����� ��ҵ�EnterServer���������
	battleground_ctrl * pCtrl = (battleground_ctrl*)(pPlane->w_ctrl);

	int id = pPlayer->id_mafia;
	if(id)
	{
		if(id == pCtrl->_data.faction_attacker)
		{
			//����
			pPlayer->SetBattleOffense();
			//����������� ע�����������������player_battle���PlayerLeaveWorld������
			if(!pCtrl->AddAttacker())
			{
				//��������,�������İ�������
				ikey.target.key_level3 = -1;

				//���ս���ı�־(��������)
				pPlayer->ClrBattleMode();
			}
		}
		else
		if(id == pCtrl->_data.faction_defender)
		{	
			//�ط�
			pPlayer->SetBattleDefence();
			//����������� ע�����������������player_battle���PlayerLeaveWorld������
			if(!pCtrl->AddDefender())
			{
				//��������,�������İ�������
				ikey.target.key_level3 = -1;

				//���ս���ı�־(��������)
				pPlayer->ClrBattleMode();
			}
		}
	}
}

int
battleground_world_message_handler::HandleMessage(const MSG & msg)
{
	//��Щ��Ϣ�������ܻ�ȽϷ�ʱ�䣬�Ƿ���Կ���Task��ɣ��������̵߳Ļ���Ҫ����msg�����������ˡ�

	switch(msg.message)
	{

		case GM_MSG_PLANE_SWITCH_REQUEST:
			//ȷ���л����� 
			//��鸱�������Ƿ���ڣ���������ڣ������ȴ��б�
			//����Ѿ����ڣ���ˢ��һ�·�������ʱ���־�������سɹ���־
			//�������Ҫ��������������ɾ������ 
			{
				if(msg.content_length != sizeof(instance_key)) 
				{
					ASSERT(false);
					return 0;
				}
				instance_key * key = (instance_key*)msg.content;
				int rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param);
				if(rst == 0)
				{
					//���л�����Ϣ
					MSG nmsg = msg;
					nmsg.target = msg.source;
					nmsg.source = msg.target;
					nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
					gmatrix::SendMessage(nmsg);
				}
				else if(rst > 0)
				{
					MSG nmsg;
					BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
					gmatrix::SendMessage(nmsg);
				}
				//���rstС��0��������ǰ�޷�ȷ���Ƿ��ܹ��������磬��Ҫ�ȴ� ���Է���ʲô������
			}
			return 0;
		default:
			return world_message_handler::HandleMessage(msg);

	}
	return 0;
}

