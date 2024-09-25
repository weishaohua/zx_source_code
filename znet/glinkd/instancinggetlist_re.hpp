
#ifndef __GNET_INSTANCINGGETLIST_RE_HPP
#define __GNET_INSTANCINGGETLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ginstancingfieldinfo"

namespace GNET
{

class InstancingGetList_Re : public GNET::Protocol
{
	#include "instancinggetlist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
