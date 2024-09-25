
#ifndef __GNET_LOADEXCHANGE_HPP
#define __GNET_LOADEXCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "lineplayerlimit"
#include "centraldeliveryclient.hpp"
#include "mapuser.h"
#include "gproviderserver.hpp"
#include "gdeliveryserver.hpp"

namespace GNET
{

class LoadExchange : public GNET::Protocol
{
	#include "loadexchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("loadexchange zoneid %d version %d edition.size %d srv_limit %d srv_count %d line_num %d", 
				zoneid, version, edition.size(), server_limit, server_count, line_status.size());
		if (GDeliveryServer::GetInstance()->IsCentralDS())
			CentralDeliveryServer::GetInstance()->SetLoad(zoneid, server_limit, server_count, line_status);
		else
		{
			GDeliveryServer *dsm = GDeliveryServer::GetInstance();
			//LOG_TRACE("LoadExchange isdebug=%d", dsm->IsDebug());
			if (dsm->IsDebug() || (edition == dsm->GetEdition() && version == dsm->GetVersion()))
			{
				CentralDeliveryClient::GetInstance()->SetLoad(server_limit, server_count, line_status);
				this->zoneid = dsm->zoneid;
				this->version = dsm->GetVersion();
				this->edition = dsm->GetEdition();
				//��ͨ�������� server_limit �ʵ��ſ� ��ֹ����ԭ��ʱ���ַ����������������
				this->server_limit = UserContainer::GetInstance().GetPlayerLimit()+2000;
				this->server_count = UserContainer::GetInstance().Size();
				this->line_status = GProviderServer::GetInstance()->GetLimits();
				manager->Send(sid, this);
			}
			else
			{
				Log::log(LOG_ERR, "LoadEXchange CentralDeliveryServer version-edition.size %d-%d does not match local %d-%d, disconnect connection",
						version, edition.size(), dsm->GetVersion(), dsm->GetEdition().size());
				manager->Close(sid);
			}
		}
	}
};

};

#endif
