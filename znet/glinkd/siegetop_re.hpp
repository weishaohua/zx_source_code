
#ifndef __GNET_SIEGETOP_RE_HPP
#define __GNET_SIEGETOP_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "siegetoprecord"

namespace GNET
{

class SiegeTop_Re : public GNET::Protocol
{
	#include "siegetop_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
