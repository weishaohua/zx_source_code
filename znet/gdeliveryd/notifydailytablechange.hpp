
#ifndef __GNET_NOTIFYDAILYTABLECHANGE_HPP
#define __GNET_NOTIFYDAILYTABLECHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class NotifyDailyTableChange : public GNET::Protocol
{
	#include "notifydailytablechange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
