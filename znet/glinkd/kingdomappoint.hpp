
#ifndef __GNET_KINGDOMAPPOINT_HPP
#define __GNET_KINGDOMAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomAppoint : public GNET::Protocol
{
	#include "kingdomappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(KINGDOM_PRIVILEGE_POLICY))
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
