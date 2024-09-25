
#ifndef __GNET_IMKEEPALIVE_HPP
#define __GNET_IMKEEPALIVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformclient.hpp"

namespace GNET
{

class IMKeepAlive : public GNET::Protocol
{
	#include "imkeepalive"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("IMKeepAlive code=%d",code);
//		GTPlatformClient::GetInstance()->SendProtocol(this);
		//SNS �� GT ���ᷢ��Э��
		manager->Send(sid, this);
	}
};

};

#endif
