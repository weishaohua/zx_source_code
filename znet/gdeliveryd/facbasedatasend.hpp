
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
		//Э������ client -> link -> gdeliveryd -> base_gs -> link -> client
		//���� base_gs -> gdeliveryd -> link -> client
		if (manager == GDeliveryServer::GetInstance())
		{//�ͻ��˷���������
			LOG_TRACE("c2sfacbasedatasend roleid %d linkid %d localsid %d data.size %d", roleid, linkid, localsid, data.size());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL == pinfo || pinfo->factionid == 0)
				return;
			int gs = 0;
			int ret = FacBaseManager::GetInstance()->GetBaseIndex(pinfo->factionid, gs, this->base_tag);
			if (ret != ERR_SUCCESS)
			{
				Log::log(LOG_ERR, "c2sfacbasedatasend roleid %d data.size %d, err %d", roleid, data.size(), ret);
				return;
			}
			GProviderServer::GetInstance()->DispatchProtocol(gs, this);	
		}
		else
		{//����gs���ظ��ͻ��˵�����
			LOG_TRACE("s2cfacbasedatasend roleid %d linkid %d localsid %d data.size %d", roleid, linkid, localsid, data.size());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
			if (NULL == pinfo)
				return;
			this->localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, this);
		}
	}
};

};

#endif
