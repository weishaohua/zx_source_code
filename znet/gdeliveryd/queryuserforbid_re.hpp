
#ifndef __GNET_QUERYUSERFORBID_RE_HPP
#define __GNET_QUERYUSERFORBID_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "forbid.hxx"
#include "gmshutup.hpp"
#include "mapforbid.h"
#include "maplinkserver.h"
#include "mapuser.h"
#include "gdeliveryserver.hpp"
namespace GNET
{

class QueryUserForbid_Re : public GNET::Protocol
{
	#include "queryuserforbid_re"
	enum { LISTTYPE_LOGINLIST = 0, LISTYPE_FORBIDREPLY = 1 };

	void SendForbidInfo(int userid,const GRoleForbid& forbid)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		UserInfo * pinfo = UserContainer::GetInstance().FindUser( userid );
		if ( NULL!=pinfo )
		{
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,AnnounceForbidInfo(userid,pinfo->localsid,forbid));
		}
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int time_left=0;
		bool blForbidTalk=false;
		bool blForbidLogin=false;
		
		for (size_t i=0;i<forbid.size();i++)
		{
			time_left=forbid[i].time;
			switch (forbid[i].type)
			{
				case Forbid::FBD_FORBID_TALK:
					blForbidTalk=true;
					if ( time_left>1 ) 
					{
						LinkServer::GetInstance().BroadcastProtocol(GMShutup(_ROLE_INVALID,_SID_INVALID,
								userid,time_left,forbid[i].reason));
						ForbidUserTalk::GetInstance().SetForbidUserTalk(userid, forbid[i]);
					}
					break;
				case Forbid::FBD_FORBID_LOGIN:
					blForbidLogin=true;
					if (time_left<=1)
					{
						forbid[i].time=0;
						ForbidLogin::GetInstance().RmvForbidLogin(userid);
						break;
					}
					forbid[i].time=time_left;
					ForbidLogin::GetInstance().SetForbidLogin(userid, forbid[i]);
					break;	
				case Forbid::FBD_FORBID_TRADE: //Ŀǰֻ֧�ֶԽ�ɫ�ķ����������ﲻ������ 
					break;
				case Forbid::FBD_FORBID_SELL: //Ŀǰֻ֧�ֶԽ�ɫ�ķ�������ﲻ������
					break;
			}
		}

		if( list_type != LISTTYPE_LOGINLIST ) return;

		// ��¼ʱͬ��au��delivery�ķ����Ϣ

		if (!blForbidLogin) ForbidLogin::GetInstance().RmvForbidLogin(userid);
	}
};

};

#endif
