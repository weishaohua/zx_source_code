/*
 * FILE: PlayerLink.cpp
 *
 * DESCRIPTION: Link multi players together
 *
 * CREATED BY: lianshuming, 2008/12/29
 *
 * HISTORY:
 *
 * Copyright (c) 2008 Archosaur Studio, All Rights Reserved.
 */

#include "playerlink.h"
#include "player_imp.h"
#include "vector.h"
#include <common/message.h>
#include <common/message.h>


extern abase::timer	g_timer;

//
//���Player��ǰ״̬�Ƿ���Խ���Link
bool player_link::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	//���Player״̬
	if(!pImp->CheckPlayerBindInvite() || !target.IsPlayer())
	{
		SendErrorMessage(pImp, ERR_LINK_INVALID_REQUEST);
		return false;
	}

	//����ͼ����
	if(pImp->GetWorldManager()->GetWorldLimit().nobind)
	{
		SendErrorMessage(pImp, ERR_LINK_CANNOT_HERE);
		return false;
	}
	return true;
}

//
//���Player��ǰ״̬�Ƿ���Խ���Link
bool player_link::CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	//���Player״̬
	if(!pImp->CheckPlayerBindRequest() || !target.IsPlayer())
	{
		SendErrorMessage(pImp, ERR_LINK_INVALID_REQUEST);
		return false;
	}

	//����ͼ����
	if(pImp->GetWorldManager()->GetWorldLimit().nobind)
	{
		SendErrorMessage(pImp, ERR_LINK_CANNOT_HERE);
		return false;
	}
	return true;
}

//
//������Ϣ��gs
void player_link::SendMessage(gplayer_imp *pImp, int message, const XID &target, int param)
{
	int msg = -1;
	if ((msg = GetMapMessage(message)) == -1){
		return;
	}
	if (!target.IsPlayer()){
		GLog::log(GLOG_ERR,"������Ϣ���������ʱ��id����ȷ��������=%d",pImp->_parent->ID.id);
		return;
	}

	pImp->SendTo<0>(msg,target,param);
}

//
//Leader������Ϣ������Member
void player_link::SendGroupMessage(gplayer_imp *pImp, int message, int param )
{
	int msg = -1;
	if ((msg = GetMapMessage(message)) == -1){
		return;
	}

	MEMBER_MAP::iterator iter = members.begin();
	XID target;
	for(; iter != members.end(); iter++){
		target = (*iter).second.id;
		if(!target.IsPlayer()) return;
		pImp->SendTo<0>(msg, target, param);
	}
}


//
//���ʹ�����Ϣ��Client
void player_link::SendErrorMessage(gplayer_imp *pImp, int message)
{
	int err_msg = -1;
	if ((err_msg = GetMapErrorMessage(message)) == -1){
		return;
	}
	pImp->_runner->error_message(err_msg);
}


void player_link::ClearAllState(gplayer_imp *pImp)
{
	ClsInviteMap();
	ClsAppliedMap();
	ClsMemberMap();
	team_seq.ResetAll();
	_leader = XID(-1, -1);
	_mode = MODE_NORMAL;
	_seq = 0;
	_timeout = 0;
	last_invite_time = 0;
	spinlock = 0;
	pImp->ReturnBindNormalState(2);
}

//
//�����󶨵�����
void player_link::PlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	//���Player״̬�͵�ͼ����
	if(!CheckPlayerLinkInvite(pImp, target)){
		return;
	}

	//�Ƿ��������
	if(invite_map.size() >= (GetMaxInviteCount()) || members.size() == GetMaxInviteCount() ){
		SendErrorMessage(pImp, ERR_LINK_INVITE_FULL);
		return;
	}

	//�Ƿ��Ѿ�������
	if(invite_map.find(target.id) != invite_map.end()){
		SendErrorMessage(pImp, ERR_LINK_ALREADY_INVITE);		
		return;
	}

	if(members.find(target.id) != members.end()){
		SendErrorMessage(pImp, ERR_LINK_ALREADY_EXIST);		
		return;
	}


	if (_mode == MODE_NORMAL){
		_mode = MODE_INVITE;
	}
	_timeout = NORMAL_TIMEOUT;

	invite_t it;
	it.timestamp = g_timer.get_systime() + LINK_INVITE_TIMEOUT;
	it.id = target;
	invite_map[target.id] = it;

	SendMessage(pImp, GM_MSG_PLAYER_LINK_INVITE, target);
}

//
//�����󶨵�����
void player_link::PlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	//���Player״̬�͵�ͼ����
	if(!CheckPlayerLinkRequest(pImp, target)){
		return;
	}

	//���mode
	if(_mode){
		SendErrorMessage(pImp, ERR_LINK_INVALID_REQUEST);
		return;
	}

	//�Ƿ��������
	if(applied_map.size() >= GetMaxRequestCount()){
		SendErrorMessage(pImp, ERR_LINK_REQUEST_FULL);		
		return;
	}

	//�Ƿ��Ѿ�������
	if(applied_map.find(target.id) != applied_map.end()){
		SendErrorMessage(pImp, ERR_LINK_ALREADY_REQUEST);				
		return;
	}

	//����״̬
	_mode = MODE_REQUEST;
	_timeout = NORMAL_TIMEOUT;

	invite_t it;
	it.timestamp = g_timer.get_systime() + LINK_INVITE_TIMEOUT;
	it.id = target;
	applied_map[target.id] = it;

	SendMessage(pImp, GM_MSG_PLAYER_LINK_REQUEST, target);
}

//
//����ͻ��˶�����Ļ�Ӧ
void player_link::PlayerLinkInvReply(gplayer_imp * pImp, const XID & target,int param)
{
	//���Player״̬�͵�ͼ����
	if(!CheckPlayerLinkRequest(pImp, target)){
		return;
	}

	//�ж��Ƿ�ʱ
	if(last_invite_time < g_timer.get_systime()){
		SendErrorMessage(pImp, ERR_LINK_INVITE_TIMEOUT);
		return;						
	}

	//ͬ������������״̬
	if(param == ERR_SUCCESS && _mode != MODE_LEADER){
		_mode = MODE_PREPARE;
		_timeout = PREPARE_TIMEOUT;
		_leader = target;
	}

	SendMessage(pImp, GM_MSG_PLAYER_LINK_INV_REPLY, target, param);	
}

//
//����ͻ��˶�����Ļ�Ӧ
void player_link::PlayerLinkReqReply(gplayer_imp * pImp, const XID & target,int param)
{
	//���Player״̬�͵�ͼ����
	if(!CheckPlayerLinkRequest(pImp, target)){
		return;
	}

	//ͬ������������״̬
	if(param == ERR_SUCCESS && _mode != MODE_LEADER){
		_mode = MODE_PREPARE;
		_timeout = PREPARE_TIMEOUT;
	}

	SendMessage(pImp, GM_MSG_PLAYER_LINK_REQ_REPLY, target, param);	
}

//
//�����뿪���߽�ɢ����
void player_link::PlayerLinkCancel(gplayer_imp *pImp)
{
	if(_mode != MODE_LEADER && _mode != MODE_MEMBER){
		return;
	}
	if(_mode == MODE_MEMBER && _leader != XID(-1, -1)){
		//����ǳ�Աֻ�ȷ�����Ϣ��leader
		SendMessage(pImp, GM_MSG_PLAYER_LINK_LEAVE_REQUEST, _leader);	
		CliMemberLeave(pImp, pImp->_parent->ID);
	}	

	else if(_mode == MODE_LEADER){
		SendGroupMessage(pImp, GM_MSG_PLAYER_LINK_LEADER_LEAVE);
		CliLeaderLeave(pImp);
	}
}

//
//���������߼�
void player_link::PlayerLinkKick(gplayer_imp *pImp, char pos)
{
	if(_mode != MODE_LEADER){
		return;
	}

	if(pos <= 0 || pos >5 ){
		return;
	}

	XID target;
	if((target=FindMemberByPos(pos)) == XID(-1, -1)){
		return;
	}

	if(!RemoveMember(target)){
		return;
	}

	SendMessage(pImp, GM_MSG_PLAYER_LINK_KICK_MEMBER, target);
	if(members.empty()){
		CliLeaderLeave(pImp);
	}
}
//
//����������Ϣ
void player_link::MsgInvite(gplayer_imp * pImp, const XID & target)
{
	//���Player��mode
	if(_mode == MODE_PREPARE || _mode == MODE_LEADER || _mode == MODE_MEMBER)
	{
		SendMessage(pImp, GM_MSG_PLAYER_LINK_INV_REPLY, target, ERR_CANNOT_BIND);
		return;
	}

	last_invite_time = g_timer.get_systime() + LINK_INVITE_TIMEOUT2;

	//֪ͨ��Ϣ���ͻ���
	CliMsgInvite(pImp, target);
}


//
//����������Ϣ
void player_link::MsgRequest(gplayer_imp * pImp, const XID & target)
{
	//���Player��mode
	if(_mode == MODE_PREPARE || _mode == MODE_LEADER || _mode == MODE_MEMBER)
	{
		//ֱ�ӷ���һ���ܾ���Ϣ����
		SendMessage(pImp, GM_MSG_PLAYER_LINK_REQ_REPLY, target, ERR_CANNOT_BIND);		
		return;
	}

	//֪ͨ��Ϣ���ͻ���
	CliMsgRequest(pImp, target);

}

//
//����ظ���������Ϣ
void player_link::MsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	INVITE_MAP::iterator it = invite_map.find(target.id);
	if (it == invite_map.end()){
		//����һ����Ϣ �öԷ�����ȴ�״̬
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);				
		return;
	}
	invite_map.erase(it);

	if(_mode == MODE_NORMAL || !CheckPlayerLinkInvite(pImp, target)){
		//����һ����Ϣ �öԷ�����ȴ�״̬
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}

	//������������Ǿܾ���Ϣ���������ǰ״̬�����б�Ҫ�Ĳ���
	if(param)
	{
		//�˲���Ҫ���ظ��ͻ���
		CliMsgInviteReply(pImp, target, param);
		return;
	}

	int seq;
	//��ӳ�Ա
	if((seq =AddMember(target)) == 0){
		SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}
	if (_mode != MODE_LEADER){
		if(!EnterBindLeaderState(pImp, target, GetLinkLeaderType()))
		{
			//����״̬ʧ�� ��ֱ�ӷ���
			return ;
		}
		_mode = MODE_LEADER;
	}

	//֪ͨ�Է���ʼ��
	SendMessage(pImp, GM_MSG_PLAYER_LINK_START, target, seq);
}

//
//����ظ���������Ϣ
void player_link::MsgRequestReply(gplayer_imp * pImp, const XID & target, int param)
{
	INVITE_MAP::iterator it = applied_map.find(target.id);
	if(it == applied_map.end()){
		//����һ����Ϣ �öԷ�����ȴ�״̬
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}

	applied_map.erase(it);

	if(_mode == MODE_NORMAL || !CheckPlayerLinkRequest(pImp,target)){
		//����һ����Ϣ �öԷ�����ȴ�״̬
		if(!param) SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		return;
	}

	//������������Ǿܾ���Ϣ���������ǰ״̬�����б�Ҫ�Ĳ���
	if(param)
	{
		//�˲���Ҫ���ظ��ͻ���
		CliMsgRequestReply(pImp, target, param);
		_mode = MODE_NORMAL;
		return;
	}

	_mode = MODE_PREPARE;
	_timeout = PREPARE_TIMEOUT;
	_leader = target;

	//���ص��ǳɹ���Ϣ,��ͨ��֮,����֪ͨ�Է�������Խ����״̬
	SendMessage(pImp, GM_MSG_PLAYER_LINK_PREPARE, target);	
}

void player_link::MsgPrepare(gplayer_imp *pImp, const XID & target)
{

	if((_mode != MODE_PREPARE && _mode != MODE_LEADER) || !CheckPlayerLinkInvite(pImp,target))
	{
		//״̬��ƥ�����Ŀ�겻ƥ�������
		return;
	}

	char seq;
	//��ӳ�Ա
	if((seq = AddMember(target)) == 0){
		return;
	}

	if(_mode != MODE_LEADER){
		//�Է�ͬ�⿪ʼ����,ֱ�ӽ����״̬
		if(!EnterBindLeaderState(pImp, target, GetLinkLeaderType()))
		{
			//����״̬ʧ�� ��ֱ�ӷ���
			return ;
		}
		//״̬һ��,����link��״̬
		_mode = MODE_LEADER;
	}
	//�ɶԷ����Ͱ󶨿�ʼ����Ϣ
	SendMessage(pImp, GM_MSG_PLAYER_LINK_START, target, seq);	

}

//
//������������˿�ʼ��
void player_link::MsgBeLinked(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos, const char seq)
{
	if(_mode != MODE_PREPARE || target != _leader || !CheckPlayerLinkRequest(pImp,target))
	{
		//���Ƿ�Ԥ�ڵ�״̬�򲻴���
		//����һ���Լ���״̬�Ƿ���� (�Ƿ��ڿ��Խ����ƶ���״̬,���״̬�������״̬һ��)
		//�Ƿ񷵻�һ����Ϣ?
		SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);
		ClearAllState(pImp);			
		return;
	}

	if(!EnterBindMemberState(pImp, target, GetLinkMemberType(), seq))
	{
		//����״̬ʧ����ֱ�ӷ���
		return;
	}

	_mode = MODE_MEMBER;
	_seq = seq;

	CliMemberJoin(pImp, target, pos);
}

//
//Member��ʼFollow Leader����
void player_link::MsgFollowOther(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	//������ҷ�����Ҫ��������Ϣ
	if(_mode != MODE_MEMBER|| target != _leader)
	{
		//���账��
		//���߿��Ƿ���һ��ֹͣlink
		if(target != _leader || _mode != MODE_PREPARE)
		{
			//���ǵ�����Ϣ˳�����,�������е�״̬�����ش�����Ϣ
			SendMessage(pImp, GM_MSG_PLAYER_LINK_STOP, target);		
		}
		return ;
	}

	CliMemberFollow(pImp, target, pos);
}


//
//Leader����Member�뿪������
void player_link::MsgMemberLeaveRequest(gplayer_imp *pImp, const XID & target)
{
	if(_mode != MODE_LEADER){
		return;
	}

	SendGroupMessage(pImp, GM_MSG_PLAYER_LINK_MEMBER_LEAVE, target.id);

	if(!RemoveMember(target)){
		return;
	}

	if(members.empty()){
		CliLeaderLeave(pImp);
	}
}

//
//Member�����Leader�����յ���Member�뿪����Ϣ
void player_link::MsgMemberLeave(gplayer_imp *pImp, const XID& leader, const XID & target)
{
	if(_mode != MODE_MEMBER || leader != _leader){
		return;
	}

	/*	if(!RemoveMember(target)){
		return;
		}
	 */
	//������Լ�����������
	if(target == pImp->_parent->ID){
		CliMemberLeave(pImp, target);
		return;
	}
	else{
		CliNotifyMemberLeave(pImp, target);
		return;
	}
}

//
//Member�յ�Leader������Ҫ�뿪����Ϣ�����ʱ�����������ɢ
void player_link::MsgLeaderLeave(gplayer_imp *pImp, const XID & target)
{
	if(_mode != MODE_MEMBER || _leader != target){
		return;
	}

	//Leader�뿪��Member��ȻҲ��Ҫ�뿪
	CliMemberLeave(pImp, target);
}

//
//Member�յ�leader�ߵ������Ϣ
void player_link::MsgKickMember(gplayer_imp *pImp, const XID & target)
{
	if(_mode != MODE_MEMBER || _leader != target){
		return;
	}

	CliKickMember(pImp, target, _seq);
}
//
//����link stop��Ϣ
void player_link::MsgStopLinked(gplayer_imp * pImp, const XID & target)
{
	if(_mode == MODE_LEADER)
	{
		INVITE_MAP::iterator iit = invite_map.find(target.id);
		if (iit != invite_map.end()) {
			invite_map.erase(iit);	
		}

		MEMBER_MAP::iterator mit = members.find(target.id);
		if (mit != members.end()){
			members.erase(mit);
		}
		if(members.empty()){
			CliLeaderLeave(pImp);
		}		
		return ;
	}
	if (target == _leader)
	{
		if(_mode == MODE_MEMBER) 
			CliMemberLeave(pImp, target);
		else if (_mode == MODE_PREPARE)
			ClearAllState(pImp);
	}
}

void player_link::CheckInviteMap(INVITE_MAP& _map)
{
	INVITE_MAP::iterator iter;

	iter = _map.begin();
	for(; iter != _map.end();){
		if((iter->second).timestamp < g_timer.get_systime()){
			_map.erase(iter++);
		}
		else{
			++iter;
		}
	}
}

bool player_link::EnterBindLeaderState(gplayer_imp * pImp, const XID & target, char type)
{
	return pImp->EnterBindMoveState(target, type);
}

bool player_link::EnterBindMemberState(gplayer_imp * pImp, const XID & target, char type, const char seq)
{
	return pImp->EnterBindFollowState(target, type, seq);
}

bool player_link::IsValidPartner(gplayer_imp * self, const XID & partner)
{
	world::object_info info;
	return 	self->_plane->QueryObject(partner, info)
		&& (info.state & world::QUERY_OBJECT_STATE_ZOMBIE != 0) 
		&& (info.pos.squared_distance(self->_parent->pos) < 50.f*50.f);
}

//
//��������
void player_link::Heartbeat(gplayer_imp * pImp)
{
	if(!_mode) return ;
	switch(_mode)
	{
		case MODE_REQUEST:
			{
				CheckInviteMap(applied_map);
				if(applied_map.empty()){
					ClearAllState(pImp);
				}
			}
			break;

		case MODE_INVITE:
			{
				CheckInviteMap(invite_map);
				if(invite_map.empty()){
					ClearAllState(pImp);
				}
			}
			break;

		case MODE_PREPARE:
			{
				_timeout --;
				if(_timeout <=0)
				{
					ClearAllState(pImp);
				}
			}
			break;

		case MODE_LEADER:
			{
				CheckInviteMap(applied_map);
				CheckInviteMap(invite_map);

				//û�г�ʱ�����з�Χ��λ�õ��ж�
				abase::vector<int, abase::fast_alloc<> > todel;
				MEMBER_MAP::iterator iter = members.begin();
				for(; iter != members.end(); ++iter)
				{
					if (!IsValidPartner(pImp, iter->second.id)) 
						todel.push_back(iter->second.id.id);
				}

				abase::vector<int,abase::fast_alloc<> >::iterator it = todel.begin();
				for(; it != todel.end(); ++it)
				{
					MsgMemberLeaveRequest(pImp,XID(GM_TYPE_PLAYER, *it));
				}
			}
			break;

		case MODE_MEMBER:
			{
				CheckInviteMap(applied_map);
				CheckInviteMap(invite_map);
				//û�г�ʱ�����з�Χ��λ�õ��ж�
				if (!IsValidPartner(pImp, _leader))
				{
					SendMessage(pImp, GM_MSG_PLAYER_LINK_LEAVE_REQUEST, _leader);
					CliMemberLeave(pImp, _leader);
				}
			}
			break;
	}	
}

//////////////////////////////////////////////////////////////////////////////////////

bool player_link_couple::IsMale(gplayer_imp * pImp)
{
	if(pImp->IsPlayerFemale())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_GENDER);
		return false;
	}
	return true;
}

bool player_link_couple::IsFemale(gplayer_imp * pImp){
	if(!pImp->IsPlayerFemale())
	{
		pImp->_runner->error_message(S2C::ERR_INVALID_GENDER);
		return false;
	}
	return true;
}


void player_link_couple::CliMsgInvite(gplayer_imp * pImp, const XID & target){
	if(!pImp->IsPlayerFemale()) return;
	pImp->_runner->player_bind_invite(target);
}

void player_link_couple::CliMsgRequest(gplayer_imp * pImp, const XID & target){
	if(pImp->IsPlayerFemale()) return;
	pImp->_runner->player_bind_request(target);
}

void player_link_couple::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param){
	pImp->_runner->player_bind_invite_reply(target,param);
}

void player_link_couple::CliMsgRequestReply(gplayer_imp * pImp, const XID & target, int param){
	pImp->_runner->player_bind_request_reply(target,param);
}

void player_link_couple::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_bind_start(target);
	//�����״̬
}

void player_link_couple::CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	//��������ƶ�
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);
}

void player_link_couple::CliMemberLeave(gplayer_imp *pImp, const XID &target)
{
	pImp->_runner->player_bind_stop();
	ClearAllState(pImp);
}

void player_link_couple::CliLeaderLeave(gplayer_imp *pImp)
{
	pImp->_runner->player_bind_stop();
	ClearAllState(pImp);
}

bool player_link_couple::CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkRequest(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;
	if(!IsFemale(pImp)) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;
	
	return false;
}

bool player_link_couple::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;
	if(!IsMale(pImp)) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;

	return true;
}

int player_link_couple::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_BIND_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return GM_MSG_PLAYER_BIND_REQUEST;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_BIND_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return GM_MSG_PLAYER_BIND_REQ_REPLY;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return GM_MSG_PLAYER_BIND_PREPARE;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_BIND_LINK;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return GM_MSG_PLAYER_BIND_FOLLOW;
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_BIND_STOP;
			/*		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
					return GM_MSG_PLAYER_BIND_LEAVE_REQUEST;
					case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
					return GM_MSG_PLAYER_BIND_MEMBER_LEAVE;
					case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
					return GM_MSG_PLAYER_BIND_LEADER_LEAVE;		
			 */
	}
	return -1;
}

int player_link_couple::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_INVALID_BIND_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return S2C::ERR_CANNOT_BIND_HERE;
		case ERR_LINK_INVITE_FULL:
			return -1;
		case ERR_LINK_ALREADY_INVITE:
			return -1;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
void player_link_ride::CliMsgInvite(gplayer_imp * pImp, const XID & target)
{
	pImp->_runner->player_bind_ride_invite(target);
}



void player_link_ride::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	pImp->_runner->player_bind_ride_invite_reply(target,param);
}


//����member join�Ĺ㲥��Ϣ���ͻ���	
void player_link_ride::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_bind_ride_join(target, pImp->_parent->ID, _seq);
}

//����member follow
void player_link_ride::CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);
}

//����member leave�Ĺ㲥��Ϣ���ͻ���
void player_link_ride::CliMemberLeave(gplayer_imp *pImp, const XID & target)
{
	pImp->_runner->player_bind_ride_leave(_leader, pImp->_parent->ID, _seq);
	ClearAllState(pImp);
}

//����leader leave�Ĺ㲥��Ϣ���ͻ���
void player_link_ride::CliLeaderLeave(gplayer_imp *pImp)
{
	pImp->_runner->player_bind_ride_leave(_leader, _leader, _seq);	
	ClearAllState(pImp);
};


void player_link_ride::CliKickMember(gplayer_imp *pImp, const XID & leader, char pos)
{
	pImp->_runner->player_bind_ride_kick(leader, _seq);
	ClearAllState(pImp);
}

//Member������ʱ���Լ��Ƚ�� �ٸ�leader����Ϣ
//�����ܱ�֤������һ����
void player_link_ride::OnMemberDeath(gplayer_imp *pImp)
{
	//ת�Ƶ�����״̬
	pImp->_runner->player_bind_ride_leave(_leader, pImp->_parent->ID, _seq);
	SendMessage(pImp, GM_MSG_PLAYER_LINK_LEAVE_REQUEST, _leader);	
	ClearAllState(pImp);
}

bool player_link_ride::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	gplayer * pPlayer = pImp->GetParent();
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(!pPlayer->IsMountMode() || pImp->GetMountType() != 1) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;

	return true;
}

int player_link_ride::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_LINK_RIDE_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return -1;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_LINK_RIDE_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return -1;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return -1;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_LINK_RIDE_START;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return GM_MSG_PLAYER_LINK_RIDE_FOLLOW; 
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_LINK_RIDE_STOP;
		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
			return GM_MSG_PLAYER_LINK_RIDE_LEAVE_REQUEST;
		case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
			return GM_MSG_PLAYER_LINK_RIDE_MEMBER_LEAVE;
		case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
			return GM_MSG_PLAYER_LINK_RIDE_LEADER_LEAVE;
		case GM_MSG_PLAYER_LINK_KICK_MEMBER:
			return GM_MSG_PLAYER_LINK_RIDE_KICK_MEMBER;		
	}
	return -1;
}

int player_link_ride::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_INVALID_LINK_RIDE_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return S2C::ERR_CANNOT_LINK_RIDE_HERE;
		case ERR_LINK_INVITE_FULL:
			return S2C::ERR_LINK_RIDE_INVITE_FULL;
		case ERR_LINK_ALREADY_INVITE:
			return S2C::ERR_LINK_RIDE_ALREADY_INVITE;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}

void player_link_bath::CliMsgInvite(gplayer_imp * pImp, const XID & target){
	pImp->_runner->player_bath_invite(target);
}

void player_link_bath::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	pImp->_runner->player_bath_invite_reply(target,param);
}

void player_link_bath::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_bath_start(target);
	//�����״̬
}

void player_link_bath::CliLeaderLeave(gplayer_imp *pImp)
{
	pImp->_runner->player_bath_stop();
	ClearAllState(pImp);
}

void player_link_bath::CliMemberLeave(gplayer_imp *pImp, const XID &target)
{
	pImp->_runner->player_bath_stop();
	ClearAllState(pImp);
}


bool player_link_bath::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode() && pImp->GetParent()->mount_type == 1) return false;
	if(pImp->GetShape() != 0 && pImp->GetShape() != 8) return false;

	world::object_info info;
	if(!pImp->_plane->QueryObject(target,info,false))
	{
		return false;
	}

	if(pImp->GetParent()->base_info.gender == info.gender)
	{
		return false;
	}
	
	return true;
}

bool player_link_bath::EnterBindLeaderState(gplayer_imp * pImp, const XID & target, char type)
{
	if(!pImp->EnterBindMoveState(target, type)) return false;

	pImp->_commander->DenyCmd(controller::CMD_MOVE);
	pImp->_commander->DenyCmd(controller::CMD_PET);
	pImp->_bind_to_ground ++;
	pImp->OnBathStart(true);

	return true;
}

bool player_link_bath::EnterBindMemberState(gplayer_imp * pImp, const XID & target, char type, const char seq)
{
	if(!pImp->EnterBindFollowState(target, type, seq)) return false;

	pImp->OnBathStart(false);
	return true;
}

int player_link_bath::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_LINK_BATH_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return -1;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_LINK_BATH_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return -1;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return -1;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_LINK_BATH_START;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return -1; 
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_LINK_BATH_STOP;
		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
			return GM_MSG_PLAYER_LINK_BATH_LEAVE_REQUEST;
		case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
			return GM_MSG_PLAYER_LINK_BATH_MEMBER_LEAVE;
		case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
			return GM_MSG_PLAYER_LINK_BATH_LEADER_LEAVE;
		case GM_MSG_PLAYER_LINK_KICK_MEMBER:
			return -1;		
	}
	return -1;
}


int player_link_bath::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_BATH_INVALID_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return -1; 
		case ERR_LINK_INVITE_FULL:
			return S2C::ERR_BATH_INVITE_FULL;
		case ERR_LINK_ALREADY_INVITE:
			return S2C::ERR_BATH_ALREADY_INVITE;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}

void player_link_qilin::CliMsgInvite(gplayer_imp * pImp, const XID & target){
	pImp->_runner->player_qilin_invite(target);
}

void player_link_qilin::CliMsgInviteReply(gplayer_imp * pImp, const XID & target, int param)
{
	pImp->_runner->player_qilin_invite_reply(target,param);
}

void player_link_qilin::CliMemberJoin(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

	pImp->_runner->player_qilin_start(target);
	//�����״̬
}

void player_link_qilin::CliLeaderLeave(gplayer_imp *pImp)
{
	gplayer* pPlayer = pImp->GetParent();
	pPlayer->bind_disconnect = false;
	pImp->_runner->player_qilin_stop();
	ClearAllState(pImp);
}

void player_link_qilin::CliMemberLeave(gplayer_imp *pImp, const XID &target)
{
	gplayer* pPlayer = pImp->GetParent();
	pPlayer->bind_disconnect = false;
	pImp->_runner->player_qilin_stop();
	ClearAllState(pImp);
}


bool player_link_qilin::CheckPlayerLinkInvite(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkInvite(pImp, target)) return false;
	if(!pImp->IsInTeam() || !pImp->IsMember(target)) return false;

	world::object_info info;
	if(!pImp->_plane->QueryObject(target,info,false))
	{
		return false;
	}

	gplayer* pPlayer = pImp->GetParent();
	if(!pImp->IsRenMa() || !pPlayer->IsQilinState()) return false;
	
	_team_id = pImp->GetTeamID(); 
	return true;
	
}

bool player_link_qilin::CheckPlayerLinkRequest(gplayer_imp * pImp, const XID & target)
{
	if(!player_link::CheckPlayerLinkRequest(pImp, target)) return false;
	if(pImp->GetParent()->IsMountMode())  return false;
	if(!pImp->IsInTeam() || !pImp->IsMember(target)) return false;
	if(pImp->IsRenMa()) return false;
	
	_team_id = pImp->GetTeamID(); 
	return true;
}

void player_link_qilin::PlayerLinkDisconnect(gplayer_imp * pImp)
{
	gplayer* pPlayer = pImp->GetParent();
	if(pPlayer->bind_disconnect) return;
	pImp->_commander->AllowCmd(controller::CMD_MOVE);
	pImp->_commander->AllowCmd(controller::CMD_PET);

	pPlayer->bind_disconnect = true;
	int index = gmatrix::FindPlayer(_leader.id);
	if(index >= 0)
	{
		gplayer * pPlayer = gmatrix::GetPlayerByIndex(index);
		pPlayer->bind_disconnect = true;
	}

	pImp->_runner->player_qilin_disconnect();
}

void player_link_qilin::PlayerLinkReconnect(gplayer_imp * pImp)
{
	gplayer* pPlayer = pImp->GetParent();
	if(!pPlayer->bind_disconnect) return;
	if (!IsValidPartner(pImp, _leader)) return;

	world::object_info info;
	pImp->_plane->QueryObject(_leader, info);

	if((info.state & world::QUERY_OBJECT_STATE_ZOMBIE) != 0 || (info.pos.squared_distance(pImp->_parent->pos)) > 50.f*50.f) return; 

	pImp->_commander->DenyCmd(controller::CMD_MOVE);
	pImp->_commander->DenyCmd(controller::CMD_PET);

	pPlayer->bind_disconnect = false;
	int index = gmatrix::FindPlayer(_leader.id);
	if(index >= 0)
	{
		gplayer * pPlayer = gmatrix::GetPlayerByIndex(index);
		pPlayer->bind_disconnect = false;
	}

	A3DVECTOR tmp = info.pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);

//	pImp->_runner->notify_pos(info.pos);
	pImp->_runner->player_qilin_reconnect(info.pos);
}

void player_link_qilin::CliMemberFollow(gplayer_imp * pImp, const XID & target,const A3DVECTOR & pos)
{
	gplayer* pPlayer = pImp->GetParent();
	if(pPlayer->bind_disconnect) return;
	A3DVECTOR tmp = pos;
	tmp -= pImp->_parent->pos;
	pImp->StepMove(tmp);
}


int player_link_qilin::GetMapMessage(int message)
{
	switch(message)
	{
		case GM_MSG_PLAYER_LINK_INVITE:
			return GM_MSG_PLAYER_LINK_QILIN_INVITE;
		case GM_MSG_PLAYER_LINK_REQUEST:
			return -1;
		case GM_MSG_PLAYER_LINK_INV_REPLY:
			return GM_MSG_PLAYER_LINK_QILIN_INV_REPLY;
		case GM_MSG_PLAYER_LINK_REQ_REPLY:
			return -1;
		case GM_MSG_PLAYER_LINK_PREPARE:
			return -1;
		case GM_MSG_PLAYER_LINK_START:
			return GM_MSG_PLAYER_LINK_QILIN_START;
		case GM_MSG_PLAYER_LINK_FOLLOW:
			return GM_MSG_PLAYER_LINK_QILIN_FOLLOW; 
		case GM_MSG_PLAYER_LINK_STOP:
			return GM_MSG_PLAYER_LINK_QILIN_STOP;
		case GM_MSG_PLAYER_LINK_LEAVE_REQUEST:
			return GM_MSG_PLAYER_LINK_QILIN_LEAVE_REQUEST;
		case GM_MSG_PLAYER_LINK_MEMBER_LEAVE:
			return GM_MSG_PLAYER_LINK_QILIN_MEMBER_LEAVE;
		case GM_MSG_PLAYER_LINK_LEADER_LEAVE:
			return GM_MSG_PLAYER_LINK_QILIN_LEADER_LEAVE;
		case GM_MSG_PLAYER_LINK_KICK_MEMBER:
			return -1;		
	}
	return -1;
}

int player_link_qilin::GetMapErrorMessage(int message)
{
	switch(message)
	{
		case ERR_LINK_INVALID_REQUEST:
			return S2C::ERR_QILIN_INVALID_REQUEST;
		case ERR_LINK_CANNOT_HERE:
			return -1; 
		case ERR_LINK_INVITE_FULL:
			return S2C::ERR_QILIN_INVITE_FULL;
		case ERR_LINK_ALREADY_INVITE:
			return S2C::ERR_QILIN_ALREADY_INVITE;
		case ERR_LINK_ALREADY_EXIST:
			return -1;
		case ERR_LINK_REQUEST_FULL:
			return -1;
		case ERR_LINK_ALREADY_REQUEST:
			return -1;
		case ERR_LINK_INVITE_TIMEOUT:
			return -1;
		case ERR_LINK_FAR_DISTANCE:
			return -1;
	}
	return -1;
}


bool player_link_qilin::IsValidPartner(gplayer_imp * self, const XID & partner)
{
	if(!player_link::IsValidPartner(self, partner))  return false;
	if(!self->IsInTeam() || !self->IsMember(partner)) return false;
	return true;
}

