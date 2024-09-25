
#ifndef __GNET_FACTIONCREATE_RE_HPP
#define __GNET_FACTIONCREATE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
namespace GNET
{

class FactionCreate_Re : public GNET::Protocol
{
	#include "factioncreate_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( GLinkServer::IsRoleOnGame( localsid ) )
			GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
