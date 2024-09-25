
#ifndef __GNET_FACTIONMASTERRESIGN_RE_HPP
#define __GNET_FACTIONMASTERRESIGN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionchoice.h"
#include "glinkclient.hpp"
namespace GNET
{

class FactionMasterResign_Re : public GNET::Protocol
{
	#include "factionmasterresign_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		printf("retcode=%d\n",retcode);
		int roleid=GLinkClient::GetInstance()->roleid;
		FactionChoice(roleid,manager,sid);
	}
};

};

#endif
