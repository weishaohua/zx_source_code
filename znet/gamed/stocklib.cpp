#include "types.h"
#include "obj_interface.h"
#include "stocklib.h"
#include "libcommon.h"
#include "stocktransaction.hpp"
#include "stockcommission.hpp"
#include "stockaccount.hpp"
#include "stockcancel.hpp"
#include "stockbill.hpp"
#include "gproviderclient.hpp"
#include "billingbalance.hpp"
#include "billingrequest2.hpp"

#define GDELIVERY_SERVER_ID  0

#define CASE_PROTO_HANDLE(_proto_name_)\
	case _proto_name_::PROTOCOL_TYPE:\
	{\
		_proto_name_ proto;\
		proto.unmarshal( os );\
		if ( proto.GetType()!=_proto_name_::PROTOCOL_TYPE || !proto.SizePolicy(os.size()) )\
			return 1;\
		return Handle_##_proto_name_( proto,player );\
	}

namespace GNET
{
	int Handle_StockCommission( StockCommission& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id || obj_if.TestSafeLock() ) 
			return 2;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0 : 1;
	}
	int Handle_StockCancel( StockCancel& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0 : 1;
	}
	int Handle_StockBill( StockBill& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0 : 1;
	}
	int Handle_StockAccount( StockAccount& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return 1;
		proto.localsid=obj_if.GetLinkSID();
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto ) ? 0 : 1;
	}

	bool SendStockTransaction(object_interface player, char withdraw, int cash, int money)
	{
		StockTransaction request(player.GetSelfID().id, withdraw, cash, money, player.GetLinkSID());
		if (!GetSyncData(request.syncdata,player))
			return false;
		if(player.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,request ) )
				return true;
			player.TradeUnLockPlayer();
		}
		return false;
	}

	int ForwardStockCmd(unsigned int type,const void* pParams,size_t param_len,object_interface player )
	{
		try {
			Marshal::OctetsStream os( Octets(pParams,param_len) );
			switch ( type )
			{
				CASE_PROTO_HANDLE(StockCommission)
				CASE_PROTO_HANDLE(StockBill)
				CASE_PROTO_HANDLE(StockCancel)
				CASE_PROTO_HANDLE(StockAccount)
				default:
					return 1;		
			}
		}
		catch ( Marshal::Exception )
		{
			return 1;
		}
	}
	
	bool SendBillingBalance(int roleid)
	{
		BillingBalance data;
		data.userid = roleid;
		data.request = 700;
		data.result  = 0;
		data.balance = 0;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, data);
	}
	bool SendBillingRequest(int roleid, int itemid, int itemnum, int timeout, int amount, int count)
	{
		BillingRequest2 data;
		data.userid  = roleid;
		data.request = 100;
		data.itemid  = itemid;
		data.itemnum = itemnum;
		data.amount  = amount;
		data.count   = count;
		data.timeout = timeout;
		data.result  = 0;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, data);
	}
	bool SendBillingConfirm(int roleid, int itemid, int itemnum, int timeout, int amount, int count, const char* txno, size_t len)
	{
		BillingRequest2 data;
		data.userid  = roleid;
		data.request = 110;
		data.itemid  = itemid;
		data.itemnum = itemnum;
		data.timeout = timeout;
		data.amount  = amount;
		data.count   = count;
		data.bxtxno.replace(txno, len);
		data.result  = 0;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, data);
	}
	bool SendBillingCancel (int roleid, int itemid, int itemnum, int timeout, int amount, int count,  const char* txno, size_t len)
	{
		BillingRequest2 data;
		data.userid  = roleid;
		data.request = 210;
		data.itemid  = itemid;
		data.itemnum = itemnum;
		data.amount  = amount;
		data.count   = count;
		data.timeout = timeout;
		data.bxtxno.replace(txno, len);
		data.result  = 0;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, data);
	}
	
}
