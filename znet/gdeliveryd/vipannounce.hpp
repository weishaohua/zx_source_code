#ifndef __GNET_VIPANNOUNCE_HPP
#define __GNET_VIPANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
#include "vipinfocache.hpp"
#include "gdeliveryserver.hpp"

namespace GNET
{

class VIPAnnounce : public GNET::Protocol
{
	#include "vipannounce"

	bool IsLegalMsg()
	{
		return ((viplevel >= 1) && (viplevel <= 6) && (starttime <= endtime)) ? true : false;
	}

	// VIPAnnounce��������������������յ�
	// 1����ҵ�¼�ɹ�����AU���ͣ�
	// 2�����VIP�ȼ�����ʱ����AU���ͣ���ʱ��һ�����ߣ������򲻴���
	// 3������VIP��ҵ���ʱ����delivery������AU����
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		VIPInfoCache *vipcache = VIPInfoCache::GetInstance();
		LOG_TRACE("VIPAnnounce:userid=%d,currenttime=%d,starttime=%d,endtime=%d,viplevel=%d:vipnum=%d:recenttotalcash=%d", 
			   userid, currenttime, starttime, endtime, viplevel, vipcache->GetOnlineVIPNum(), recenttotalcash);
		
		if(!IsLegalMsg())
		      return;

		UserInfo *puser = UserContainer::GetInstance().FindUser(userid);
		if(puser == NULL)
		      return;

		vipcache->UpdateVIPInfo(userid, currenttime, starttime, endtime, viplevel, status, totalcash, recenttotalcash, infolack);
		
		// ����ڱ��������Ѿ�����Ϸ����ֱ��ת���������������ڱ����ĵȵ�EnterWorldʱ��ת��
		if(puser->status == _STATUS_ONGAME)
		{
			vipcache->SendVIPInfoToGS(userid, puser->roleid, puser->gameid);
		}
		// �����ɫ�Ѿ��ڿ������Ҫת�������delivery��������ڿ�������ڻظ�RemoteLoginQuery_Reʱת��
		else if(puser->status == _STATUS_REMOTE_LOGIN && !GDeliveryServer::GetInstance()->IsCentralDS())
		{
			vipcache->SendVIPInfoToCentralDS(userid);
		}
	}
};

};

#endif
