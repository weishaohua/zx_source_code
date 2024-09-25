
#ifndef __GNET_TRADESTART_RE_HPP
#define __GNET_TRADESTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class TradeStart_Re : public GNET::Protocol
{
	#include "tradestart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		DEBUG_PRINT("glinkd:: receive tradestart_re from gdelivery.\n");
		if (!GLinkServer::ValidLocalsid(localsid,roleid)) 
			return;
		unsigned int tmplocalsid=localsid;
		this->localsid=_SID_INVALID;
		lsm->Send(tmplocalsid,this);	
	}
};

};

#endif
