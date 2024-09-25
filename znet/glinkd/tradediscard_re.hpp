
#ifndef __GNET_TRADEDISCARD_RE_HPP
#define __GNET_TRADEDISCARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class TradeDiscard_Re : public GNET::Protocol
{
	#include "tradediscard_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidLocalsid(localsid,roleid))
			return;
		GLinkServer::GetInstance()->Send(localsid,this);

	}
};

};

#endif
