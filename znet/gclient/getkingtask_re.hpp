
#ifndef __GNET_GETKINGTASK_RE_HPP
#define __GNET_GETKINGTASK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetKingTask_Re : public GNET::Protocol
{
	#include "getkingtask_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
