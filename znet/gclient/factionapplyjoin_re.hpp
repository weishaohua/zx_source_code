
#ifndef __GNET_FACTIONAPPLYJOIN_RE_HPP
#define __GNET_FACTIONAPPLYJOIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionchoice.h"
#include "glinkclient.hpp"
namespace GNET
{

class FactionApplyJoin_Re : public GNET::Protocol
{
	#include "factionapplyjoin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		int roleid=GLinkClient::GetInstance()->roleid;
		switch (retcode)
		{
		case ERR_FC_APPLY_REJOIN:
			printf("�Ѿ���ĳ�����ɵĳ�Ա�������Ѿ��������ĳ���ɣ�����ʧ��\n");
			break;
		case ERR_FC_JOIN_SUCCESS:
			printf("�ɹ��������%d\n",factionid);
			break;
		case ERR_FC_JOIN_REFUSE:
			printf("���뱻�ܾ�\n");	
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
