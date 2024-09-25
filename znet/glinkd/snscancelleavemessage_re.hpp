
#ifndef __GNET_SNSCANCELLEAVEMESSAGE_RE_HPP
#define __GNET_SNSCANCELLEAVEMESSAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSCancelLeaveMessage_Re : public GNET::Protocol
{
	#include "snscancelleavemessage_re"

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
