
#ifndef __GNET_PLAYEROFFLINE_RE_HPP
#define __GNET_PLAYEROFFLINE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"

#include "mapforbid.h"
#include "mapuser.h"
namespace GNET
{

class PlayerOffline_Re : public GNET::Protocol
{
	#include "playeroffline_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gdelivery::playeroffline_re roleid=%d,result=%d\n",roleid,result);
		int uid = UserContainer::Roleid2Userid(roleid);
		if(!uid)
			return;
		if (result==ERR_SUCCESS)
			ForbiddenUsers::GetInstance().Pop(uid);
		UserContainer::GetInstance().ContinueLogin(uid, result==ERR_SUCCESS);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{//���enterworld֮ǰ��PlayerKickout GS����playeroffline_re
		//GS�����޷�������ȷ��src_zoneid ���Դ˴��ӻ����ȡRoleInfoCache
			GRoleInfo *roleinfo = RoleInfoCache::Instance().Get(roleid);
			if (roleinfo && roleinfo->src_zoneid != 0)
			{
				LOG_TRACE("Tell DS zoneid %d Kickout user %d success offline_re", roleinfo->src_zoneid, uid);
				CentralDeliveryServer::GetInstance()->DispatchProtocol(roleinfo->src_zoneid, KickoutRemoteUser_Re(result, uid));
			}
		}
	}
};

};

#endif
