
#ifndef __GNET_STOCKACCOUNT_HPP
#define __GNET_STOCKACCOUNT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "stockaccount_re.hpp"
#include "stockexchange.h"

namespace GNET
{

class StockAccount : public GNET::Protocol
{
	#include "stockaccount"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());

		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(pinfo)
		{
			StockAccount_Re res;
			res.localsid = pinfo->localsid;
			if(StockExchange::Instance()->GetAccount(pinfo->userid,res.cash,res.money,res.locked,res.prices))
				GDeliveryServer::GetInstance()->Send(pinfo->linksid,res);
			Log::log(LOG_MONEY,"money_hold:[roleid=%d,userid=%d]:moneyhold=%d:type=1:place=3:hint=0",roleid,pinfo->userid,res.money);
		}
	}
};

};

#endif
