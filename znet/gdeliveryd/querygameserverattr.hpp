
#ifndef __GNET_QUERYGAMESERVERATTR_HPP
#define __GNET_QUERYGAMESERVERATTR_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "querygameserverattr_re.hpp"
#include "mapgameattr.h"
namespace GNET
{

class QueryGameServerAttr : public GNET::Protocol
{
	#include "querygameserverattr"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		QueryGameServerAttr_Re qgsa_re;
		GameAttrMap::Get( qgsa_re.attr );
		manager->Send( sid,qgsa_re );
	}
};

};

#endif
