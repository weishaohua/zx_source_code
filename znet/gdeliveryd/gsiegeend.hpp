
#ifndef __GNET_GSIEGEEND_HPP
#define __GNET_GSIEGEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GSiegeEnd : public GNET::Protocol
{
	#include "gsiegeend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT( "siege end, battle_id=%d, tag=%d, result=%d", battle_id, world_tag, result);
		SiegeManager::GetInstance()->OnSiegeEnd(battle_id, world_tag, result);
	}
};

};

#endif