
#ifndef __GNET_SIEGETOP_HPP
#define __GNET_SIEGETOP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SiegeTop : public GNET::Protocol
{
	#include "siegetop"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
