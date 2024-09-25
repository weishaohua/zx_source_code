
#ifndef __GNET_FACTIONPKRAIDCHALLENGECANCEL_HPP
#define __GNET_FACTIONPKRAIDCHALLENGECANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidChallengeCancel : public GNET::Protocol
{
	#include "factionpkraidchallengecancel"

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
