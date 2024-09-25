
#ifndef __GNET_FACTIONUPGRADE_RE_HPP
#define __GNET_FACTIONUPGRADE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
namespace GNET
{

class FactionUpgrade_Re : public GNET::Protocol
{
	#include "factionupgrade_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( GLinkServer::IsRoleOnGame( localsid ) )
			GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
