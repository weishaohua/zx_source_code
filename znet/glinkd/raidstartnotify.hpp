
#ifndef __GNET_RAIDSTARTNOTIFY_HPP
#define __GNET_RAIDSTARTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidStartNotify : public GNET::Protocol
{
	#include "raidstartnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
