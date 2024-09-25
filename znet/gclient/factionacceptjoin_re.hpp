
#ifndef __GNET_FACTIONACCEPTJOIN_RE_HPP
#define __GNET_FACTIONACCEPTJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionchoice.h"
#include "glinkclient.hpp"
namespace GNET
{

class FactionAcceptJoin_Re : public GNET::Protocol
{
	#include "factionacceptjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		int roleid=GLinkClient::GetInstance()->roleid;
		switch (retcode)
		{
			case ERR_SUCCESS:
				printf("�ɹ��ύ��������\n");
				break;
			case ERR_FC_ACCEPT_REACCEPT:
				printf("����׼���뱾�������Ѿ��������\n"); 
				break;
			case ERR_FC_FACTION_NOTEXIST:
				printf("���ɲ�����or���û�������������\n");  
				break;
			case ERR_FC_NO_PRIVILEGE:
				printf("û����׼Ȩ��\n");	
				break;
			case ERR_FC_DBFAILURE:
				printf("���ݿ��дʧ��\n");	
				break;
		}
		FactionChoice(roleid,manager,sid);
	}
};

};

#endif
