
#ifndef __GNET_PLAYERKICKOUT_RE_HPP
#define __GNET_PLAYERKICKOUT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "onlineannounce.hpp"
#include "gdeliveryserver.hpp"
#include "kickoutuser.hpp"
#include "mapforbid.h"
#include "mapuser.h"
#include "mapremaintime.h"
#include "kickoutremoteuser_re.hpp"
namespace GNET
{

class PlayerKickout_Re : public GNET::Protocol
{
	#include "playerkickout_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// �յ�GS���ص�������ӦЭ��,����ɹ�,������ǰ��¼����
		DEBUG_PRINT("playerkickout_re: receive playerkickout_re from gs, roleid=%d,result=%d,src_zoneid=%d\n",roleid,result,src_zoneid);
		int uid = UserContainer::Roleid2Userid(roleid);
		if(!uid)
			return;
		if (result==ERR_SUCCESS)
			ForbiddenUsers::GetInstance().Pop(uid);
		UserContainer::GetInstance().ContinueLogin(uid, result==ERR_SUCCESS);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{//֪ͨԭ�� �߿����ҳɹ� Ҳ�п�����GM�߿��������� ��ʱ��ʵ����Ҫ��ԭ����KickoutRemoteUser_Re 
			LOG_TRACE("Tell DS zoneid %d Kickout user %d success", src_zoneid, uid);
			CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, KickoutRemoteUser_Re(result, uid));
		}
	}
};

};

#endif
