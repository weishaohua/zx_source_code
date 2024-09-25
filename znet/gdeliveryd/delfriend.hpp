
#ifndef __GNET_DELFRIEND_HPP
#define __GNET_DELFRIEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "delfriend_re.hpp"
#include "mapuser.h"
#include "hometownmanager.h"
#include "gtplatformagent.h"

namespace GNET
{

class DelFriend : public GNET::Protocol
{
	#include "delfriend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		char ret = ERR_FS_NOFOUND;
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
			if( NULL==pinfo )
				return;
			GFriendInfoVector* plist = &(pinfo->friends);
			if(pinfo->friend_ver>=0)
			{
				for(GFriendInfoVector::iterator itf=plist->begin(), ite=plist->end(); itf!=ite; ++itf)
				{
					if(itf->rid==friendid)
					{
						plist->erase(itf);
						HometownManager::GetInstance()->OnDelFriend(roleid, friendid);
						ret = 0;
						pinfo->friend_ver++;
						GTPlatformAgent::ModifyFriend(FRIEND_DEL,roleid,friendid,itf->gid);
						break;
					}
				}
			}
			else
				ret = ERR_FS_NOTINITIALIZED;
		}
		dsm->Send(sid, DelFriend_Re(ret, roleid, friendid, localsid));
	}
};

};

#endif
