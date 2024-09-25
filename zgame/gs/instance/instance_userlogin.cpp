#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../world.h"
#include "../player.h"
#include "../player_imp.h"
#include "../config.h"
#include "../userlogin.h"
#include "../usermsg.h"
#include "../clstab.h"
#include "../playertemplate.h"
#include "../aei_filter.h"

#include <deque>
#include <db_if.h>
#include "../task/taskman.h"
#include "instance_manager.h"
#include <base64.h>

extern int __allow_login_class_mask;
extern int __allow_login_class_mask1;
namespace {
class LoginTask :  public abase::ASmallObject , public GDB::Result
{
	int _uid;
	int _cs_index;
	int _cs_sid;
	void * _auth_data;
	size_t _auth_size;
	char _flag;
public:
	LoginTask(int uid,int cs_index, int cs_sid, const void * auth_data , size_t auth_size, char flag)
		:_uid(uid),_cs_index(cs_index),_cs_sid(cs_sid),_auth_data(NULL),_auth_size(auth_size),_flag(flag)
		{
			if(auth_size)
			{
				_auth_data = abase::fastalloc(auth_size);
				memcpy(_auth_data,auth_data,auth_size);
			}
		}
	~LoginTask()
	{
		if(_auth_data)
		{
			abase::fastfree(_auth_data,_auth_size);
		}
	}

	void Failed()
	{
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,-1,_flag);	// login failed
		delete this;
	}
public:
	virtual void OnTimeOut()
	{
		GLog::log(GLOG_ERR,"�û�%d�����ݿ�ȡ�����ݳ�ʱ",_uid);
		Failed();
	}
	
	virtual void OnFailed()
	{
		GLog::log(GLOG_ERR,"�û�%d�����ݿ�ȡ������ʧ��",_uid);
		Failed();
	}
	virtual void OnGetRole(int id,const GDB::base_info * pInfo, const GDB::vecdata * data,const GNET::GRoleDetail* pRole);
};


void 
LoginTask::OnGetRole(int id,const GDB::base_info * pInfo, const GDB::vecdata * data, const GNET::GRoleDetail * pRole)
{
	//����¼����  
	//if(!(__allow_login_class_mask & (1 << (pInfo->cls & 0x7F))))
	if(object_base_info::CheckCls(pInfo->cls, __allow_login_class_mask, __allow_login_class_mask1))
	{               
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,1,_flag);    // login failed
		//��ʱ���մ���������
		OnFailed();
		return;
	}

	if(!do_login_check_data(pInfo,data))
	{
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,1,_flag);    // login failed
		//��ʱ���մ���������
		GLog::log(GLOG_ERR,"�û�%d�����쳣���޷���¼",id);
		OnFailed();
		return; 
	}


	//�����ݿ��ȡ������
	char name_base64[64] ="δ֪";
	if(data->user_name.data)
	{
		size_t name_len = data->user_name.size;
		if(name_len > 32) name_len = 32;
		base64_encode((unsigned char*)(data->user_name.data),name_len,name_base64);
	}
	
	GLog::log(GLOG_INFO,"�û�%d�����ݿ�ȡ�����ݣ�ְҵ%d,����%d ����'%s'",_uid,pInfo->cls,pInfo->level,name_base64);

	//�����ݿ���ȡ��instance key
	// BW instance_world_manager * pManager = (instance_world_manager*)world_manager::GetInstance();
	instance_world_manager * pManager = NULL;
	instance_hash_key ikey;
	player_var_data::GetInstanceKey(data->var_data.data,data->var_data.size,ikey);


	//��������ķ���
	int world_index;
	world * pPlane = pManager->AllocWorld(ikey,world_index);
	if(!pPlane)
	{
		//��������ʧ���ˣ�Ӧ����û���㹻�ĸ���
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,5,_flag); 
		delete this;
		return;
	}
	gplayer * pPlayer = pPlane->AllocPlayer();
	if(pPlayer == NULL)
	{
		//����û������ռ��������Player����Ϣ
		__PRINTF("�û��ﵽ�����������ֵ\n");
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,4,_flag); 
		return;
	}
	GLog::log(GLOG_INFO,"�û�%d��%d��ʼ��¼",_uid,_cs_index);
	pPlayer->cs_sid = _cs_sid;
	pPlayer->cs_index = _cs_index;
	pPlayer->ID.id = _uid;
	pPlayer->ID.type = GM_TYPE_PLAYER;
	pPlayer->login_state = gplayer::WAITING_LOGIN;
	pPlayer->pPiece = NULL;
	
	if(!pPlane->MapPlayer(_uid,pPlane->GetPlayerIndex(pPlayer)))
	{
		//map player ʧ�ܣ���ʾ����һ˲�����˼���
		pPlane->FreePlayer(pPlayer);
		mutex_spinunlock(&pPlayer->spinlock);
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,4,_flag);       // login failed
		return;
	}
	ASSERT(pPlayer->imp == NULL);
	pPlayer->imp = NULL; 

	userlogin_t user;
	memset(&user,0,sizeof(user));
	user._player = pPlayer;
	user._plane = pPlane;
	user._uid = _uid;
	user._auth_data = _auth_data;
	user._auth_size = _auth_size;

	//���ø�����Ӧ�ı�־
	pManager->SetPlayerWorldIdx(_uid,world_index);

	//��������Ƿ�ʹ�������һ�£������һ�£����������õ���� 
	int last_instance_timestamp;
	int last_instance_tag;
	A3DVECTOR last_instance_pos;
	player_var_data::GetLastInstance<0>(data->var_data.data,data->var_data.size,last_instance_tag,last_instance_pos,last_instance_timestamp);

	A3DVECTOR login_pos = A3DVECTOR(pInfo->posx, pInfo->posy, pInfo->posz);

	if(last_instance_tag == pManager->GetWorldTag())
	{
		if(last_instance_timestamp > 0)
		{
			if(last_instance_timestamp == pPlane->w_create_timestamp)
			{	
				//���̸���������Ȼ���ڣ���������ת�� ʲô������Ҫ��
			}
			else
			{
				//��ǰ�����Ѿ������ڣ�ʹ�ô���λ��  ������ʱ���
				login_pos = last_instance_pos;
				last_instance_timestamp = pPlane->w_create_timestamp;
			}
		}
		else
		{
			last_instance_tag = -1;
		}
	}
	else
	{
		last_instance_tag = -1;
	}
	//���û�����λ�õı궨����ô��ҵ����ݽ����Ǵ���ģ���ʱ����Σ�����ʹ�������ļ�������Ĭ�����
	if(last_instance_tag <= 0)
	{
		//��ͼʹ�������ļ��ڿ��ܴ��ڵ�λ��
		const struct world_pos & point = pManager->GetSavePoint();
		if(point.tag == pManager->GetWorldTag())
		{
			//����tag����Ҫ�����ʹ�õ�ǰ������
			last_instance_tag = pManager->GetWorldTag();
			last_instance_pos = point.pos;

			//����¼λ��Ҳ��Ϊ����λ��
			login_pos = last_instance_pos;
		}
	}
	
	//��������ļ��빤��
	do_player_login(login_pos, pInfo,data,user);

	if(pPlayer->imp)
	{
		gplayer_imp *pImp =(gplayer_imp*)pPlayer->imp;
		pImp->_filters.AddFilter(new aei_filter(pImp,FILTER_CHECK_INSTANCE_KEY));

		//���ø����������
		pImp->SetLastInstancePos(last_instance_tag, last_instance_pos, last_instance_timestamp);
	}

	//�⿪��ҵ���
	//�ȴ���ҷ���EnterWorld
	mutex_spinunlock(&pPlayer->spinlock);

	//ɾ������
	delete this;
}

}

void	instance_user_login(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size)
{
	ASSERT(false);
	/*
	//����������ֱ�ӵ�¼
	int world_index;
	//BW if(world_manager::GetInstance()->FindPlayer(uid,world_index))
	if(gmatrix::FindPlayer(uid,world_index))
	{
		//�����Ѿ����˵�¼����Ϣ
		GMSV::SendLoginRe(cs_index,uid,cs_sid,3);	// login failed
		GLog::log(GLOG_WARNING,"�û�%d�Ѿ���¼(%d,%d)(������%d)",uid , cs_index,cs_sid,world_index);
		return ;
	}

	//�����ݿ�ȡ�����ݣ����������еĵ�¼����
	GDB::get_role(uid, new LoginTask(uid,cs_index,cs_sid,auth_data,auth_size));
	*/
}
