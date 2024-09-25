
#ifndef __GNET_PLAYERFAMILYINFO_RE_HPP
#define __GNET_PLAYERFAMILYINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "guserfamily"

namespace GNET
{

class PlayerFamilyInfo_Re : public GNET::Protocol
{
	#include "playerfamilyinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif