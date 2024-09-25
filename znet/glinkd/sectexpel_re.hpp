
#ifndef __GNET_SECTEXPEL_RE_HPP
#define __GNET_SECTEXPEL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SectExpel_Re : public GNET::Protocol
{
	#include "sectexpel_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
