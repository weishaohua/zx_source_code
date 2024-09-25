
#ifndef __GNET_TEAMRAIDAPPLY_RE_HPP
#define __GNET_TEAMRAIDAPPLY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidApply_Re : public GNET::Protocol
{
	#include "teamraidapply_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
