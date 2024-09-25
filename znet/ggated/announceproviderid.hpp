
#ifndef __GNET_ANNOUNCEPROVIDERID_HPP
#define __GNET_ANNOUNCEPROVIDERID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "lineinfo"
#include "gproviderserver.hpp"

namespace GNET
{

class AnnounceProviderID : public GNET::Protocol
{
	#include "announceproviderid"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("announceproviderid id=%d sid=%d", id, sid);
		GProviderServer* psm=GProviderServer::GetInstance();
		if (psm->gameservermap.find(id)!=psm->gameservermap.end())
		{
			Log::log(LOG_ERR,"Identical game server id(%d) exist. Check gameserver's \".conf\" file.",
					id);
			manager->Close(sid);
			return;
		}
		psm->gameservermap[id] = sid;
		this->id=psm->GetProviderServerID();
		this->attr = 0;
		if (GTPlatformClient::GetInstance()->IsGTSessionOK())
			this->attr |= GT_SERVER_CONNECTED;
		//gs ���յ� gate �� announceproviderid �� (attr&GT_SERVER_CONNECTED) Ϊ��ʱ �� �յ� GTReconnect Э��ʱ ���� GTSyncTeams
		this->serialno = 0;
		this->edition.clear();
		psm->Send(sid,this);
	}
};

};

#endif
