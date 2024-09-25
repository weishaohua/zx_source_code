
#ifndef __GNET_PLAYERCHANGEDS_HPP
#define __GNET_PLAYERCHANGEDS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_change_ds(int roleid, char flag);

namespace GNET
{

class PlayerChangeDS : public GNET::Protocol
{
	#include "playerchangeds"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		player_change_ds(roleid, flag);
	}
};

};

#endif
