
#ifndef __GNET_REPAIRROLELIST_HPP
#define __GNET_REPAIRROLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RepairRoleList : public GNET::Protocol
{
	#include "repairrolelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif