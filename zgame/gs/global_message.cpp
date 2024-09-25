#include "global_manager.h"
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include <glog.h>

int
global_world_message_handler::HandleMessage(const MSG & msg)
{
	//��Щ��Ϣ�������ܻ�ȽϷ�ʱ�䣬�Ƿ���Կ���Task��ɣ��������̵߳Ļ���Ҫ����msg�����������ˡ�

	//���ǽ�һЩ�����ƶ���������
	switch(msg.message)
	{
		case GM_MSG_PLANE_SWITCH_REQUEST:
		//ȷ���л����� 
		//ֱ�ӷ��ط��� ������Ҫ����Ƿ���Դ��͵�
		//����Ƿ���ʣ��ռ�
		
		if((size_t)(gmatrix::GetPlayerAlloced() + 10) < gmatrix::GetMaxPlayerCount())
		{
			//��������Ƿ�����
			if(_plane->PosInWorld(msg.pos))
			{
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				gmatrix::SendMessage(nmsg);
			}
		}
		else
		{
			//֪ͨ�ͻ����޷�����
			MSG nmsg;
			BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,S2C::ERR_TOO_MANY_PLAYER_IN_INSTANCE);
			gmatrix::SendMessage(nmsg);
		}
		break;

		default:
			return world_message_handler::HandleMessage(msg);
	}
	return 0;
}

