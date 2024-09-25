
#ifndef __GNET_FACTIONLISTAPPLICANT_RE_HPP
#define __GNET_FACTIONLISTAPPLICANT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkclient.hpp"
namespace GNET
{

class FactionListApplicant_Re : public GNET::Protocol
{
	#include "factionlistapplicant_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		int roleid=GLinkClient::GetInstance()->roleid;
		switch (retcode)
		{
			case ERR_SUCCESS:
				printf("�����ˣ�\n");
				for (size_t i=0;i<rolelist.size();i++)
				{
					printf("\troleid=%d\n",rolelist[i]);
				}
				break;
			case ERR_FC_NO_PRIVILEGE:
				printf("���ǰ�����û�д�Ȩ��\n");
				break;	
			case ERR_FC_FACTION_NOTEXIST:
				printf("���ɲ�����\n");
				break;	
		}
		FactionChoice(roleid,manager,sid);
	}
};

};

#endif
