
#ifndef __GNET_CRSSVRTEAMSROLENOTIFY_RE_HPP
#define __GNET_CRSSVRTEAMSROLENOTIFY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsRoleNotify_Re : public GNET::Protocol
{
	#include "crssvrteamsrolenotify_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
