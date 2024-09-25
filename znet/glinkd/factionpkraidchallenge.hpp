
#ifndef __GNET_FACTIONPKRAIDCHALLENGE_HPP
#define __GNET_FACTIONPKRAIDCHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidChallenge : public GNET::Protocol
{
	#include "factionpkraidchallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif