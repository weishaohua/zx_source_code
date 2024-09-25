
#ifndef __GNET_TRADEREMOVEGOODS_HPP
#define __GNET_TRADEREMOVEGOODS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "groleinventory"

#include "gdeliveryserver.hpp"
#include "trade.h"
#include "traderemovegoods_re.hpp"
#include "mapuser.h"
namespace GNET
{

class TradeRemoveGoods : public GNET::Protocol
{
	#include "traderemovegoods"
	void SendResult(GNET::Transaction* t,int retcode)
	{
		//send result to both traders
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		Trader::id_t* id;
		if((id=t->GetAlice())!=NULL)
			dsm->Send(id->linksid,TradeRemoveGoods_Re(retcode,tid,roleid,id->roleid,id->localsid,goods,money));
		if((id=t->GetBob())!=NULL)
			dsm->Send(id->linksid,TradeRemoveGoods_Re(retcode,tid,roleid,id->roleid,id->localsid,goods,money));

	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		GNET::Transaction* t=GNET::Transaction::GetTransaction(tid);
		if (t == NULL) return;
		
		int retcode;
		Log::formatlog("trade_debug","removegoods: roleid=%d,goods is (id=%d,pos=%d,count=%d),money=%d,tid=%d\n",
				roleid,goods.id,goods.pos,goods.count,money,tid);
		if ((retcode=t->RemoveExchgObject(roleid,goods,money))==ERR_SUCCESS)
			SendResult(t,retcode);
		else
			dsm->Send(sid,TradeRemoveGoods_Re(retcode,tid,roleid,roleid,localsid,goods,money));
	}
};

};

#endif
