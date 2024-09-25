
#ifndef __GNET_FACBASEDATASEND_HPP
#define __GNET_FACBASEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseDataSend : public GNET::Protocol
{
	#include "facbasedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (manager == GLinkServer::GetInstance())
		{
			//�ͻ��˷�������Ϣ
			SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
			if (!sinfo || !sinfo->policy.Update(FAC_BASE_DATA_SEND) || sinfo->roleid != roleid/*��ͬ�� ValidRole ��֤*/)
				return;
			this->linkid = GProviderServer::GetProviderServerID();
			this->localsid = sid;
			GDeliveryClient::GetInstance()->SendProtocol(this);
		}
		else
		{ 	//����gsֱ�ӷ��ظ��ͻ��˵���Ϣ
			//������ deliveryd �㲥�Ļ�����Ϣ
			//������ deliveryd ת�������Ļ��س�Ա����Ϣ
			//GLinkServer::GetInstance()->Send(localsid, this);	
			GLinkServer* lsm=GLinkServer::GetInstance();
			//if (lsm->ValidRole(localsid, roleid))
			//facbasebroadcast ת������������ roleid ���� 0
			lsm->AccumulateSend(localsid, GamedataSend(data));
		}
	}
};

};

#endif
