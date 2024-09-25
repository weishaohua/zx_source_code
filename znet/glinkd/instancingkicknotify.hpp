
#ifndef __GNET_INSTANCINGKICKNOTIFY_HPP
#define __GNET_INSTANCINGKICKNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingKickNotify : public GNET::Protocol
{
	#include "instancingkicknotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int lsid = localsid;
		localsid = 0;
		GLinkServer::GetInstance()->Send(lsid, this);
	}
};

};

#endif
