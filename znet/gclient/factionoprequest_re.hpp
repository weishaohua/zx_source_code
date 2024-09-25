
#ifndef __GNET_FACTIONOPREQUEST_RE_HPP
#define __GNET_FACTIONOPREQUEST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionchoice.h"
#include "glinkclient.hpp"
namespace GNET
{

class FactionOPRequest_Re : public GNET::Protocol
{
	#include "factionoprequest_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		int roleid=GLinkClient::GetInstance()->roleid;
		switch (retcode)
		{
			case ERR_FC_NETWORKERR:
				printf("������������ͨѶ�ж�\n");
				break;
			case ERR_FC_INVALID_OPERATION:
				printf("��Ч�Ĳ�������\n");
				break;
			case ERR_FC_OP_TIMEOUT:
				printf("������ʱ\n");
				break;
		}
		FactionChoice(roleid,manager,sid);

	}
};

};

#endif
