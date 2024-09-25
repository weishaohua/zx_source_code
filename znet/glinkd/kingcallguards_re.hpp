
#ifndef __GNET_KINGCALLGUARDS_RE_HPP
#define __GNET_KINGCALLGUARDS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingCallGuards_Re : public GNET::Protocol
{
	#include "kingcallguards_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid ,this);	
	}
};

};

#endif
