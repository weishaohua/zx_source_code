
#ifndef __GNET_GETCUSTOMDATA_RE_HPP
#define __GNET_GETCUSTOMDATA_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class GetCustomData_Re : public GNET::Protocol
{
	#include "getcustomdata_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidLocalsid(localsid,roleid))
			return;
		GLinkServer::GetInstance()->Send(localsid,this);

	}
};

};

#endif
