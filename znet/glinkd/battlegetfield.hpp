
#ifndef __GNET_BATTLEGETFIELD_HPP
#define __GNET_BATTLEGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleGetField : public GNET::Protocol
{
	#include "battlegetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
