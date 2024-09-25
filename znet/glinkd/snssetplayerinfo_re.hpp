
#ifndef __GNET_SNSSETPLAYERINFO_RE_HPP
#define __GNET_SNSSETPLAYERINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"

namespace GNET
{

class SNSSetPlayerInfo_Re : public GNET::Protocol
{
	#include "snssetplayerinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(localsid, roleid))
			return;

		int lsid = localsid;
		localsid = 0;	
		GLinkServer::GetInstance()->Send(lsid, this);
	}
};

};

#endif
