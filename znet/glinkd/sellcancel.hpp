
#ifndef __GNET_SELLCANCEL_HPP
#define __GNET_SELLCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"

namespace GNET
{

class SellCancel : public GNET::Protocol
{
	#include "sellcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		if ( !GLinkServer::ValidRole(sid,roleid) ) return;
		localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
