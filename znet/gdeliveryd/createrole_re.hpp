
#ifndef __GNET_CREATEROLE_RE_HPP
#define __GNET_CREATEROLE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "roleinfo"
namespace GNET
{

class CreateRole_Re : public GNET::Protocol
{
	#include "createrole_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
