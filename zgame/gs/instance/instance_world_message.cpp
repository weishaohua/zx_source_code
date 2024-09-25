#include "instance_manager.h"
#include "../world.h"
#include "../player_imp.h"
#include "../usermsg.h"
#include "../aei_filter.h"

/*
 *	����������Ϣ
 */
int
instance_world_message_handler::RecvExternMessage(int msg_tag, const MSG & msg)
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
			//�����ѯKEY�ķ�ʽ���Ǻ���ȷ�� �д����� $$$$$$$$$$
				if(msg.content_length < sizeof(instance_key)) return 0;
				instance_key * key = (instance_key*)msg.content;
				//��Ϣ��ͷ��������instance_key
				//ASSERT(key->target.key_level1 == msg.source.id);
				//���ﲻ�����ˣ���Ҫ��GM���д���Ϊ
				instance_hash_key hkey;
				_manager->TransformInstanceKey(key->target, hkey);
				int index = _manager->GetWorldByKey(hkey);
				if(index < 0) return 0;
				return 0;
				//return _manager->GetWorldByIndex(index)->DispatchMessage(msg); $$$$$$ BW
			}
			*/

		case GM_MSG_PLANE_SWITCH_REQUEST:
		//ȷ���л����� 
		//ֱ�ӷ��ط��� ������Ҫ����Ƿ���Խ�����������
		{
			//���ȼ���Ƿ���Խ������磬 ��manager��������μ���������������Ƿ�ﵽ����
			if(msg.content_length != sizeof(instance_key)) 
			{
				ASSERT(false);
				return 0;
			}
			instance_key * key = (instance_key*)msg.content;
			int rst;
			if((rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param)) == 0)
			{
				//���л�����Ϣ
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				gmatrix::SendMessage(nmsg);
			}
			else
			{
				MSG nmsg;
				BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
				gmatrix::SendMessage(nmsg);
			}
		}
		return 0;

		default:
		if(msg.target.type == GM_TYPE_PLAYER)
		{
			int index = _manager->GetPlayerWorldIdx(msg.target.id);
			if(index < 0) return 0;
			//$$$$$$$$BW
			//return _manager->GetWorldByIndex(index)->DispatchMessage(msg);
			return 0;
		}
		//��������Ϣ��δ���� ..........
	}
	return 0;
}


void
instance_world_message_handler::SetInstanceFilter(gplayer_imp * pImp,instance_key & ikey)
{
	//���븱��key���filter,���filter���л�������ʱ������б���ͻָ�
	pImp->_filters.AddFilter(new aei_filter(pImp,FILTER_CHECK_INSTANCE_KEY));
}

/**��ʱ�����û�������������ת��*/
int 
instance_world_message_handler::PlayerComeIn(instance_world_manager *pManager, world * pPlane,const MSG &msg)
{
	gplayer * pPlayer = pPlane->GetPlayerByID(msg.source.id);
	if(!pPlayer)
	{
		//���ת���Ѿ���ʱ
		__PRINTF("�û�����,�����û��Ѿ���ʱ\n");
		return 0;
	}
	spin_autolock keeper(pPlayer->spinlock);
	if(pPlayer->ID != msg.source || !pPlayer->IsActived() || pPlayer->login_state != gplayer::WAITING_SWITCH)
	{
		//player��״̬����ȷ
		__PRINTF("�û�����,�����û������״̬����ȷ�\n");
		return 0;
	}
	ASSERT(pPlayer->imp == NULL);
	//g_timer.remove_timer(pPlayer->base_info.faction,(void*)pPlayer->base_info.cls);

	//ɾ����ʱtimer

	raw_wrapper wrapper(msg.content,msg.content_length);
	wrapper.SetLimit(raw_wrapper::LOAD_ONLY);
	instance_key ikey;
	int source_tag;
	wrapper >> ikey >> source_tag;
	if(ikey.target.key_level1 != msg.param)
	{
		//key�Ļ����жϲ�����
		GLog::log(GLOG_ERR,"invalid instance key while switch %d",pPlayer->ID.id);
		pPlane->FreePlayer(pPlayer);
		//�ָ�����ʧ�ܻ��߲���ʧ��
		ASSERT(false);
		return 0;
	}

	pPlayer->Import(wrapper);
	ASSERT(msg.source == pPlayer->ID);

	int rst = RestoreObject(wrapper,pPlayer,pPlane);
	//�����µ����� ����Ҫ����ɵ�������������
	A3DVECTOR source_pos = pPlayer->pos;

	pManager->SetIncomingPlayerPos(pPlayer,msg.pos);
	
	//������Ĵ��ڹ�ϵ
	if(rst < 0 || pPlane->InsertPlayer(pPlayer) <0)
	{       
		pPlane->FreePlayer(pPlayer);
		//�ָ�����ʧ�ܻ��߲���ʧ��
		//��Ҫ����־�����NPC����
		//��Ҫ����������� ����������PlayerMap,manager��player world��Ӧ��ϵ�ȵ�
		ASSERT(false);
		return 0;
	} 
	pPlane->InsertPlayerToMan(pPlayer);
	
	//��ͼ���¸ö������
	world_manager::player_cid  cid;
	pManager->GetPlayerCid(cid);
	TrySwapPlayerData(pPlane,cid.cid,pPlayer);

	gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
	PlayerPreEnterServer(pPlayer, pImp,ikey);

	pImp->PlayerEnterMap();

	//�趨���̵�ʱ�̣� �ӿ��˴��̵��ٶ�
	pImp->_write_timer = abase::Rand(1, 20);

	//����ת�Ƴɹ�������
	GMSV::SendSwitchServerSuccess(pPlayer->cs_index,pPlayer->ID.id, pPlayer->cs_sid,_manager->GetWorldIndex());

	//���ø����ļ��filter
	SetInstanceFilter(pImp,ikey);

	//���ý��븱����һЩ����
	pImp->SetInstanceTimer(_manager->GetWorldTag());
	pImp->SetLastInstancePos(_manager->GetWorldTag(),pPlayer->pos, pPlane->w_create_timestamp);
	pImp->SetLastInstanceSourcePos(source_tag, source_pos);

	//���Լ���λ����Ϣ����һ��
	dispatcher * runner = pPlayer->imp->_runner;
	runner->notify_pos(pPlayer->pos);
	//���·����������
	runner->begin_transfer();
	runner->enter_world();
	runner->end_transfer();
	runner->server_config_data();

	GLog::log(GLOG_INFO,"�û�%d(%d,%d)ת�Ƶ�%d",pPlayer->ID.id, pPlayer->cs_index,pPlayer->cs_sid,_manager->GetWorldTag());

	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("%dת�Ʒ��������:%u.%u\n",pPlayer->ID.id,tv.tv_sec,tv.tv_usec);
	return 0;
}

int
instance_world_message_handler::HandleMessage(const MSG & msg)
{
	//��Щ��Ϣ�������ܻ�ȽϷ�ʱ�䣬�Ƿ���Կ���Task��ɣ��������̵߳Ļ���Ҫ����msg�����������ˡ�
	switch(msg.message)
	{

		case GM_MSG_PLANE_SWITCH_REQUEST:
		//ȷ���л����� 
		//ֱ�ӷ��ط��� ������Ҫ����Ƿ���Խ�����������
		{
			//���ȼ���Ƿ���Խ������磬 ��manager��������μ���������������Ƿ�ﵽ����
			if(msg.content_length != sizeof(instance_key)) 
			{
				ASSERT(false);
				return 0;
			}
			instance_key * key = (instance_key*)msg.content;
			int rst;
			if((rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param)) == 0)
			{
				//���л�����Ϣ
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				gmatrix::SendMessage(nmsg);
			}
			else
			{
				MSG nmsg;
				BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
				gmatrix::SendMessage(nmsg);
			}
		}
		return 0;
		default:
			return world_message_handler::HandleMessage(msg);

	}
	return 0;
}

