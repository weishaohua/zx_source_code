
#ifndef __GNET_INSTANCINGAPPOINTNOTIFY_HPP
#define __GNET_INSTANCINGAPPOINTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAppointNotify : public GNET::Protocol
{
	#include "instancingappointnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int lsid = localsid;
		localsid = 0;
		GLinkServer::GetInstance()->Send(lsid, this);
	}
};

};

#endif