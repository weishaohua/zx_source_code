
#ifndef __GNET_STOCKCOMMISSION_RE_HPP
#define __GNET_STOCKCOMMISSION_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class StockCommission_Re : public GNET::Protocol
{
	#include "stockcommission_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
