#include "callid.hxx"

#ifdef WIN32
#include <winsock2.h>
#include "gnproto.h"
#include "gncompress.h"
#else
#include "protocol.h"
#include "binder.h"
#endif

namespace GNET
{

static GNET::Protocol::Type _state_GAuthServer[] = 
{
	PROTOCOL_KEYEXCHANGE,
	PROTOCOL_STATUSANNOUNCE,
	PROTOCOL_ACCOUNTINGREQUEST,
	PROTOCOL_ANNOUNCEZONEID,
	PROTOCOL_ANNOUNCEZONEID2,
	PROTOCOL_ANNOUNCEZONEID3,
	PROTOCOL_GAME2AU,
	PROTOCOL_QUERYUSERPRIVILEGE,
	PROTOCOL_QUERYUSERPRIVILEGE2,
	PROTOCOL_QUERYUSERFORBID,
	PROTOCOL_QUERYUSERFORBID2,
	PROTOCOL_GMKICKOUTUSER,
	PROTOCOL_GMFORBIDSELLPOINT,
	PROTOCOL_GMSHUTUP,
	PROTOCOL_TRANSBUYPOINT,
	PROTOCOL_ADDCASH,
	PROTOCOL_ADDCASH_RE,
	PROTOCOL_VERIFYMASTER,
	PROTOCOL_VERIFYMASTER_RE,
	RPC_GQUERYPASSWD,
	RPC_MATRIXPASSWD,
	RPC_MATRIXPASSWD2,
	RPC_MATRIXTOKEN,
	PROTOCOL_MATRIXFAILURE,
	RPC_USERLOGIN,
	RPC_USERLOGIN2,
	RPC_USERLOGOUT,
	RPC_GETADDCASHSN,
	RPC_CASHSERIAL,
	PROTOCOL_BILLINGREQUEST,
	PROTOCOL_BILLINGBALANCE,
	RPC_NETMARBLELOGIN,
	RPC_NETMARBLEDEDUCT,
	PROTOCOL_NETMARBLELOGOUT,
};

GNET::Protocol::Manager::Session::State state_GAuthServer(_state_GAuthServer,
						sizeof(_state_GAuthServer)/sizeof(GNET::Protocol::Type), 86400);


};
