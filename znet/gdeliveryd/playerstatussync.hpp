
#ifndef __GNET_PLAYERSTATUSSYNC_HPP
#define __GNET_PLAYERSTATUSSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gproviderserver.hpp"
#include "playeroffline.hpp"
#include "mapforbid.h"
#include "mapuser.h"
namespace GNET
{

class PlayerStatusSync : public GNET::Protocol
{
	#include "playerstatussync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//�û�û�д������˻��߶���״̬��������ӦЭ��
		int userid = UserContainer::Roleid2Userid(roleid);
		if(!userid)
		{
			Log::log(LOG_ERR,"PlayerStatusSync. roleid=%d not found,linksid=%d,localsid=%d,gameid=%d",
					roleid,sid,localsid,gs_id);
			GProviderServer::GetInstance()->DispatchProtocol(gs_id,this);
			return;
		}
		if (ForbiddenUsers::GetInstance().IsExist(userid))
		{
			//�û��������˻����״̬��������Э�顣�ȴ����ˡ�����Э�鴦����û�    
			DEBUG_PRINT("gdelivery::PlayerStatusSync: roleid=%d not found,localsid=%d,gsid=%d\n",roleid,localsid,gs_id);
			return;
		}

		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (!pinfo||pinfo->user->linkid!=provider_link_id||pinfo->localsid!=localsid)
		{ 
			//����Ӧgameserver����statusSync
			GProviderServer::GetInstance()->DispatchProtocol(gs_id,this);
			DEBUG_PRINT("gdelivery::PlayerStatusSync: Send to gameserver. roleid=%d,localsid=%d,gsid=%d\n",roleid,localsid,gs_id);
		}
		else 
		{
			Log::log(LOG_ERR,"PlayerStatusSync from link,roleid=%d,userid=%d,linksid=%d,localsid=%d,gameid=%d",
					roleid,userid,sid,localsid,gs_id);
			UserContainer::GetInstance().UserLogout(pinfo->user);
		}
	}
};

};

#endif
