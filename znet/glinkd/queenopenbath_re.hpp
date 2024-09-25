
#ifndef __GNET_QUEENOPENBATH_RE_HPP
#define __GNET_QUEENOPENBATH_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueenOpenBath_Re : public GNET::Protocol
{
	#include "queenopenbath_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid ,this);	
	}
};

};

#endif
