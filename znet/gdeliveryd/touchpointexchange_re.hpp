
#ifndef __GNET_TOUCHPOINTEXCHANGE_RE_HPP
#define __GNET_TOUCHPOINTEXCHANGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TouchPointExchange_Re : public GNET::Protocol
{
	#include "touchpointexchange_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
