
#ifndef __GNET_SNSACCEPTAPPLY_RE_HPP
#define __GNET_SNSACCEPTAPPLY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SNSAcceptApply_Re : public GNET::Protocol
{
	#include "snsacceptapply_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		if (!GLinkServer::ValidRole(localsid, roleid))
			return;

		int lsid = localsid;
		localsid = 0;	
		GLinkServer::GetInstance()->Send(lsid, this);
	}
};

};

#endif