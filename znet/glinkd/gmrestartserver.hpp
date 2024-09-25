
#ifndef __GNET_GMRESTARTSERVER_HPP
#define __GNET_GMRESTARTSERVER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
#include "gproviderserver.hpp"
#include "gdeliveryclient.hpp"
#include "privilege.hxx"
namespace GNET
{

class GMRestartServer : public GNET::Protocol
{
	#include "gmrestartserver"
	
	class DispatchData
	{
		const GMRestartServer* restart;
	public:
		DispatchData(const GMRestartServer* msg):restart(msg) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,restart);
		}	
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		GDeliveryClient* dsm=GDeliveryClient::GetInstance();
		//���Կͻ��˵�Э�飬����û��ǲ���GM������ǣ���ת����delivery
		if (manager==lsm)
		{
			//privilege check
			if (! lsm->PrivilegeCheck(sid, gmroleid,Privilege::PRV_SHUTDOWN_GAMESERVER) )
			{
				Log::log(LOG_ERR,"WARNING: user %d try to use GM_OP_RESTART privilege that he doesn't have.\n", gmroleid);
				return;
			}

			char content[256];
			sprintf(content,"ShutdownGameServer, Force all user offline.");
			Log::gmoperate(gmroleid,Privilege::PRV_SHUTDOWN_GAMESERVER,content);
			
			this->localsid=sid;
			dsm->SendProtocol(this);
		}
		else if (manager==dsm) //����Delivery����Ϣ����ת�������еĿͻ��ˣ���֪������������Ϣ
		{
			Thread::RWLock::RDScoped l(lsm->locker_map);
			this->gmroleid=-1;
			this->localsid=0;
			this->gsid=-1;
			if (lsm->roleinfomap.size())
				std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this));
		}
	}
};

};

#endif
