
#ifndef __GNET_SNSPRESSMESSAGE_RE_HPP
#define __GNET_SNSPRESSMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSPressMessage_Re : public GNET::Protocol
{
	#include "snspressmessage_re"

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