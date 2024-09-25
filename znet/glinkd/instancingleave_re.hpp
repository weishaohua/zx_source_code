
#ifndef __GNET_INSTANCINGLEAVE_RE_HPP
#define __GNET_INSTANCINGLEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingLeave_Re : public GNET::Protocol
{
	#include "instancingleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
