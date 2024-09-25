#include "friendcallbackmanager.hpp"
#include "friendcallbackinfo.hpp"
#include "dbfriendcallbackinfo.hrp"
#include "friendcallbackinfo_re.hpp"
#include "friendcallbackmail.hpp"
#include "dbfriendcallbackmail.hrp"
#include "friendcallbackmail_re.hpp"
#include "dbupdatecallbackinfo.hrp"
#include "dbcheckaward.hrp"
#include "friendcallbackaward_re.hpp"
#include "friendcallbacksubscribe.hpp"
#include "dbfriendcallbacksubscribe.hrp"
#include "friendcallbacksubscribe_re.hpp"
#include "dbgetmaillist.hrp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "conv_charset.h"

namespace GNET
{
int FriendCallbackManager::_zoneoff = 0;
bool FriendCallbackManager::_isopen = false;

void FriendCallbackManager::QueryCallbackInfo(const FriendCallbackInfo *msg, unsigned int sid)
{
	if(!_isopen)
		return;

	int roleid = msg->roleid;
	int localsid = msg->localsid;
	PlayerInfo *playerinfo = NULL;
	if(!IsPlayerOnLine(roleid, playerinfo))
		return;

	if(!ExistCallbackLimit(roleid))
	{
		DBFriendCallbackInfo *rpc = (DBFriendCallbackInfo *)Rpc::Call(RPC_DBFRIENDCALLBACKINFO, DBFriendCallbackInfoArg(roleid));
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	else
	{
		ReplyCallbackInfo(ERR_CALLBACK_SUCCESS, roleid, localsid, sid);
	}
}

void FriendCallbackManager::ReplyCallbackInfo(unsigned char retcode, int roleid, unsigned int localsid, unsigned int sid)
{
	FriendCallbackInfo_Re re;
	re.retcode = retcode;
	re.roleid = roleid;
	re.localsid = localsid;
	if(ERR_CALLBACK_SUCCESS == retcode)
	{
		GetCallbackLimit(roleid, re.callback_limit);
	}
	
	//LOG_TRACE("GDelivery::ReplyCallbackInfo, retcode=%d, roleid=%d, mailnum=%d, timestamp=%d, cooldown_list.size=%d\n", retcode, roleid, re.callback_limit.mailnum, re.callback_limit.timestamp, re.callback_limit.cooldown_list.size());
	GDeliveryServer::GetInstance()->Send(sid, re);
}

void FriendCallbackManager::GetCallbackLimit(int roleid, GCallbackInfo &callback_info)
{
	Thread::Mutex::Scoped locker(_locker);
	if(_callback_limit.find(roleid) != _callback_limit.end())
	{
		const GCallbackLimit &limit = _callback_limit[roleid];
		callback_info.mailnum = limit.mailnum;
		callback_info.timestamp = limit.timestamp;
		callback_info.is_subscribe = limit.is_subscribe;

		callback_info.cooldown_list.clear();
		GCooldown cooldown;
		const map<int, int> &cooldown_map = limit.cooldown_map;
		map<int, int>::const_iterator it = cooldown_map.begin(), ie = cooldown_map.end();
		for(; it != ie; ++it)
		{
			cooldown.roleid = it->first;
			cooldown.send_time = it->second;
			callback_info.cooldown_list.push_back(cooldown);
		}
	}
	else
	{
		callback_info.mailnum = 0;
		callback_info.timestamp= 0;
		callback_info.is_subscribe = 1;
		callback_info.cooldown_list.clear();
	}
}

void FriendCallbackManager::LoadCallbackInfo(int roleid, const GCallbackInfo &callback_info)
{
	Thread::Mutex::Scoped locker(_locker);
	GCallbackLimit &limit = _callback_limit[roleid];
	limit.timestamp = callback_info.timestamp;
	limit.is_subscribe = callback_info.is_subscribe;
	// ������һ�η����ٻ��ʼ���ʱ�������ڲ���ͬһ���򽫸ý�ɫ������Է��͵��ٻ��ʼ�����Ϊ0
	limit.mailnum = IsNDaysBefore(limit.timestamp, RESETMAILNUM_TIME) ? 0 : callback_info.mailnum;

	limit.cooldown_map.clear();
	GCooldownVector::const_iterator it = callback_info.cooldown_list.begin(), ie = callback_info.cooldown_list.end();
	for(; it != ie; ++it)
	{
		//if(!IsNDaysBefore(it->send_time, COOLDOWN_TIME))
		if(IsCooldown(it->send_time, COOLDOWN_TIME * SECONDS_PERDAY))
		{
			limit.cooldown_map[it->roleid] = it->send_time;
		}
	}
}

void FriendCallbackManager::SendCallbackMail(const FriendCallbackMail *msg, unsigned int sid)
{
	if(!_isopen)
		return;

	int sender_userid = 0;
	unsigned char retcode = CanSendCallbackMail(msg, sender_userid);
	LOG_TRACE("GDelivery::SendCallbackMail, retcode=%d, roleid=%d, tempate_id=%d\n", retcode, msg->roleid, msg->template_id);
	if(ERR_CALLBACK_OFFLINE == retcode || ERR_CALLBACK_MSGCOOLDOWN == retcode || ERR_CALLBACK_UNVALIDTEMPID == retcode)
	{
		return;
	}
	else if(ERR_CALLBACK_SUCCESS != retcode)
	{
		ReplyCallbackMail(retcode, msg->roleid, msg->localsid, msg->receiver_roleid, sid);
	}
	else
	{
		// ����Delivery�иý�ɫ�ķ�������
		UpdateCallbackLimit(msg->roleid, msg->receiver_roleid);
		// �����ٻ��ʼ�
		DBFriendCallbackMailArg arg;
		MakeMail(arg.mail, msg->roleid, msg->receiver_roleid, msg->template_id);
		arg.sender_userid = sender_userid;
		DBFriendCallbackMail *rpc = (DBFriendCallbackMail *)Rpc::Call(RPC_DBFRIENDCALLBACKMAIL, arg);
		rpc->template_id = msg->template_id;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
}

unsigned char FriendCallbackManager::CanSendCallbackMail(const FriendCallbackMail *msg, int &userid)
{
	int sender = msg->roleid;
	int receiver = msg->receiver_roleid;
	PlayerInfo *sender_info = NULL, *receiver_info = NULL;

	// ��������߲���������Ը���Ϣ
	if(!IsPlayerOnLine(sender, sender_info))
		return ERR_CALLBACK_OFFLINE;
	userid = sender_info->userid;

	// ��������ߵĺ����б�δ��ʼ�����ܷ����ٻ��ʼ�����Ϊ�޷��жϽ������Ƿ�Ϊ����ѣ�
	if(sender_info->friend_ver < 0)
		return ERR_CALLBACK_FDLISTNOTINIT;

	// ����������ٻ��ʼ���������Ϣû�м������ܷ���
	if(!ExistCallbackLimit(sender))
		return ERR_CALLBACK_LIMITNOTINIT;

	int lastsend_time = _callback_limit[sender].timestamp;
	// ���������η����ٻ��ʼ���Ϣ��ʱ�������ܶ���3�룬������Ը���Ϣ
	if(IsCooldown(lastsend_time, MSG_INTERVAL))
		return ERR_CALLBACK_MSGCOOLDOWN;

	// �����ǰʱ����ϴη���ʱ������һ�죬����Ҫ���õ��췢���ٻ��ʼ���mailnumΪ0
	if(IsNDaysBefore(lastsend_time, RESETMAILNUM_TIME))
	{
		_callback_limit[sender].mailnum = 0;
	}

	// ��������ߵ��췢�͵��ٻ��ʼ����ﵽ5�����ܼ�������
	if(_callback_limit[sender].mailnum >= MAX_CALLBACKMAIL_PERDAY)
		return ERR_CALLBACK_MAXMAILNUM;

	// ������������߲��ܷ����ٻ��ʼ�
	if(IsPlayerOnLine(receiver, receiver_info))
		return ERR_CALLBACK_FRIENDONLINE;

	// ��������߲�����������ܷ����ٻ��ʼ�
	if(!IsFriend(sender_info, receiver))
		return ERR_CALLBACK_NOTHISFRIEND;

	// ��Ҹ�ͬһ���ѷ��������ٻ��ʼ���ʱ����Ӧ����10�죬����10��Ĳ��ܷ���
	if(IsReceiverInCooldown(sender, receiver))
		return ERR_CALLBACK_COOLDOWN;

	return ERR_CALLBACK_SUCCESS;
}

bool FriendCallbackManager::IsFriend(const PlayerInfo *playerinfo, int roleid)
{
	const GFriendInfoVector &friend_list = playerinfo->friends;
	GFriendInfoVector::const_iterator it = friend_list.begin(), ie = friend_list.end();
	for(; it != ie; ++it)
	{
		if(roleid == it->rid)
			return true;
	}
	return false;
}

bool FriendCallbackManager::IsReceiverInCooldown(int sender, int receiver)
{
	map<int, int> &cooldown_map = _callback_limit[sender].cooldown_map;
	if(cooldown_map.find(receiver) == cooldown_map.end())
		return false;

	const int lastsend_time = cooldown_map[receiver];
	//return IsNDaysBefore(lastsend_time, COOLDOWN_TIME) ? false : true;
	return IsCooldown(lastsend_time, COOLDOWN_TIME * SECONDS_PERDAY);
}

void FriendCallbackManager::ReplyCallbackMail(unsigned char retcode, int sender, unsigned int localsid, int receiver, unsigned int sid)
{
	FriendCallbackMail_Re re;
	re.retcode = retcode;
	re.roleid = sender;
	re.localsid = localsid;
	re.receiver_roleid = receiver;

	GDeliveryServer::GetInstance()->Send(sid, re);
}

void FriendCallbackManager::UpdateCallbackLimit(int sender, int receiver)
{
	int now = Timer::GetTime();
	Thread::Mutex::Scoped locker(_locker);

	GCallbackLimit &limit = _callback_limit[sender];
	// ��¼�ϴθ��ý�ɫ�����ٻ��ʼ���ʱ�䣬�Ա����ʱ���лع�
	_rollback[sender][receiver] = limit.cooldown_map[receiver];
	// ����delivery�б����������Ϣ����ʹ���������ݿ⽻���Ĺ�����Ҳ����ȷ�����ٻ��ʼ��ķ���
	limit.mailnum++;
	limit.timestamp = now;
	limit.cooldown_map[receiver] = now;

	LOG_TRACE("GDelivery::UpdateCallbackLimit, sender=%d, receiver=%d, mailnum=%d, send_time=%d, cooldown_list.size()=%d\n", 
		  sender, receiver, limit.mailnum, now, limit.cooldown_map.size());
}

void FriendCallbackManager::MakeMail(GMail &mail, int sender, int receiver, unsigned char template_id)
{
	PlayerInfo *sender_info = NULL;
	if(IsPlayerOnLine(sender, sender_info))
	{
		mail.header.sender_name = sender_info->name;
	}

	mail.header.id = 0;
	mail.header.sender = sender;
	mail.header.sndr_type = _MST_CALLBACK;
	mail.header.receiver = receiver;
	mail.header.send_time = Timer::GetTime();
	mail.header.attribute = (1 << _MA_UNREAD);
}

void FriendCallbackManager::Rollback(int sender, int receiver)
{
	LOG_TRACE("GDelivery::Rollback, sender=%d, receiver=%d\n", sender, receiver);

	Thread::Mutex::Scoped locker(_locker);
	// �ع���Ϣ����û�м�¼����Ա��λع�
	if(_rollback[sender].find(receiver) == _rollback[sender].end())
		return;

	_callback_limit[sender].mailnum--;
	if(0 == _rollback[sender][receiver])
	{
		_callback_limit[sender].cooldown_map.erase(receiver);
	}
	else
	{
		_callback_limit[sender].cooldown_map[receiver] = _rollback[sender][receiver];
	}
	_rollback[sender].erase(receiver);
}

void FriendCallbackManager::PlayerLogout(int roleid)
{
	if(!_isopen)
		return;

	LOG_TRACE("GDelivery::FriendCallback PlayerLogout, roleid=%d\n", roleid);
	// ����Ƿ���Ҫ����ǰDelivery�иý�ɫ�����ٻ��ʼ���������Ϣ�����ݿ�ͬ��
	if(NeedSync2DB(roleid))
	{
		GCallbackInfo callback;
		GetCallbackLimit(roleid, callback);
		DBUpdateCallbackInfo *rpc = (DBUpdateCallbackInfo *)Rpc::Call(RPC_DBUPDATECALLBACKINFO, DBUpdateCallbackInfoArg(roleid, callback));
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	// �����Ƿ���Ҫͬ������Ҫ��Delivery�ж�Ӧ��ҵ�������Ϣɾ��
	_callback_limit.erase(roleid);
}

bool FriendCallbackManager::NeedSync2DB(int roleid)
{
	Thread::Mutex::Scoped locker(_locker);

	// �����Ҳ��������б����������δʹ�ù����㴫�飬����ͬ��
	if(_callback_limit.find(roleid) == _callback_limit.end())
		return false;

	// �������������б��д��ڵ�ʱ��Ϊ0����������ݿ��в����ڸ���ҵ���Ϣ�������߹�����δ���͹��ٻ��ʼ�
	// ���ڴ������Ҳ����Ҫ����
	if(0 == _callback_limit[roleid].timestamp)
		return false;

	// ����Ƿ������ȴ�ڽ�������ң�������ڽ����������Ϣ��ɾ����ʹ���ܹ��������䷢���ٻ��ʼ�
	map<int, int> &cooldown_map = _callback_limit[roleid].cooldown_map;
	map<int, int>::iterator it = cooldown_map.begin();
	for(; it != cooldown_map.end(); )
	{
		//if(IsNDaysBefore(it->second, COOLDOWN_TIME))
		if(!IsCooldown(it->second, COOLDOWN_TIME * SECONDS_PERDAY))
		{
			cooldown_map.erase(it++);
		}
		else
		{
			++it;
		}
	}
	return true;
}

void FriendCallbackManager::PlayerLogin(int roleid)
{
	if(!_isopen)
		return;

	//LOG_TRACE("GDelivery::FriendCallbackManager::PlayerLogin, roleid=%d\n", roleid);
	DBCheckAward *rpc = (DBCheckAward *)Rpc::Call(RPC_DBCHECKAWARD, DBCheckAwardArg(roleid));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FriendCallbackManager::NotifyAwardInfo(int returnplayer_roleid, int award_type, const vector<int> &recaller_list)
{
	Octets name;
	int roleid = 0;
	FriendCallbackAward_Re re;
	PlayerInfo *playerinfo = NULL;

	for(unsigned int i = 0; i < (recaller_list.size() + 1); ++i)
	{
		if(0 == i)
		{
			roleid = returnplayer_roleid;
			if(!IsPlayerOnLine(roleid, playerinfo))
				return;
			re.award_type = award_type;
			re.returnplayer_name = name;
			name = playerinfo->name;
		}
		else
		{
			roleid = recaller_list[i - 1];
			if(!IsPlayerOnLine(roleid, playerinfo))
				continue;
			re.award_type = RECALL_AWARD;
			re.returnplayer_name = name;
			ReloadMaillist(roleid, playerinfo->localsid, playerinfo->linksid);
		}
		re.roleid = roleid;
		re.localsid = playerinfo->localsid;

		GDeliveryServer::GetInstance()->Send(playerinfo->linksid, re);
	}
}

void FriendCallbackManager::ReloadMaillist(int roleid, int localsid, int sid)
{
	DBGetMailList *rpc = (DBGetMailList *)Rpc::Call(RPC_DBGETMAILLIST, RoleId(roleid));
	rpc->save_linksid = sid;
	rpc->save_localsid = localsid;
	rpc->need_send2client = false;
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FriendCallbackManager::Subscribe(const FriendCallbackSubscribe *msg, unsigned int sid)
{
	if(!_isopen)
		return;

	DBFriendCallbackSubscribe *rpc = (DBFriendCallbackSubscribe *)Rpc::Call(RPC_DBFRIENDCALLBACKSUBSCRIBE, DBFriendCallbackSubscribeArg(msg->roleid, msg->is_subscribe));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void FriendCallbackManager::ReplySubscribeResult(unsigned char retcode, char is_subscribe, int roleid, unsigned int localsid, unsigned int sid)
{
	if(ERR_CALLBACK_SUCCESS == retcode)
	{
		_callback_limit[roleid].is_subscribe = is_subscribe;
	}
	FriendCallbackSubscribe_Re re;
	re.retcode = retcode;
	re.roleid = roleid;
	re.localsid = localsid;
	GDeliveryServer::GetInstance()->Send(sid, re);
}

};
