
#ifndef __GNET_GFACBASEENTER_HPP
#define __GNET_GFACBASEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFacBaseEnter : public GNET::Protocol
{
	#include "gfacbaseenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
