
#ifndef __GNET_PLAYERBASEINFO_RE_HPP
#define __GNET_PLAYERBASEINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class PlayerBaseInfo_Re : public GNET::Protocol
{
	#include "playerbaseinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
