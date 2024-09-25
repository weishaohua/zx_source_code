
#ifndef __GNET_TRADEEND_HPP
#define __GNET_TRADEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class TradeEnd : public GNET::Protocol
{
	#include "tradeend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidLocalsid(localsid,roleid))
			return;
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
