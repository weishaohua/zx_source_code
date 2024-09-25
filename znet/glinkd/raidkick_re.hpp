
#ifndef __GNET_RAIDKICK_RE_HPP
#define __GNET_RAIDKICK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidKick_Re : public GNET::Protocol
{
	#include "raidkick_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
