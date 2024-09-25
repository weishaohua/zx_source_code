
#ifndef __GNET_HOSTILEDELETE_RE_HPP
#define __GNET_HOSTILEDELETE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"

namespace GNET
{

class HostileDelete_Re : public GNET::Protocol
{
	#include "hostiledelete_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( GLinkServer::IsRoleOnGame( localsid ) )
			GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
