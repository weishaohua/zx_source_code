
#ifndef __GNET_REPORT2GM_HPP
#define __GNET_REPORT2GM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "report2gm_re.hpp"
#include "mapuser.h"

namespace GNET
{

class Report2GM : public GNET::Protocol
{
	#include "report2gm"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		PlayerInfo *userinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if( userinfo && userinfo->gameid != _GAMESERVER_ID_INVALID )
		{
			3[(unsigned char*)content.begin()] = (unsigned char)userinfo->gameid;
		}
		else
			return;
		this->zoneid=dsm->zoneid;
		if ( dsm->Send(dsm->iweb_sid, this) )
		{
			dsm->Send(sid,Report2GM_Re(ERR_SUCCESS,roleid,localsid));
		}
		else
			dsm->Send(sid,Report2GM_Re(ERR_GENERAL,roleid,localsid));
	}
};

};

#endif
