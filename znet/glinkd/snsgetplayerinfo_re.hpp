
#ifndef __GNET_SNSGETPLAYERINFO_RE_HPP
#define __GNET_SNSGETPLAYERINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "snsplayerinfocache"

namespace GNET
{

class SNSGetPlayerInfo_Re : public GNET::Protocol
{
	#include "snsgetplayerinfo_re"

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
