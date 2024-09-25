
#ifndef __GNET_BATTLELEAVE_HPP
#define __GNET_BATTLELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battleleave_re.hpp"
#include "battlemanager.h"
#include "crossbattlemanager.hpp"
#include "centraldeliveryclient.hpp"

namespace GNET
{

class BattleLeave : public GNET::Protocol
{
	#include "battleleave"

	void ReplyBattleLeave(int roleid, int localsid, int sid)
	{
		int gs_id ;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if ( NULL==pinfo)
				return;
			gs_id = pinfo->gameid;
		}
		int retcode = BattleManager::GetInstance()->LeaveTeam(roleid, gs_id);
		GDeliveryServer::GetInstance()->Send(sid, BattleLeave_Re(retcode, roleid, localsid, false));
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleLeave: roleid=%d, iscross=%d", roleid, iscross);
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		CrossBattleManager *crossbattle = CrossBattleManager::GetInstance();
		bool iscentral = dsm->IsCentralDS();

		if(!iscentral && !iscross) // ԭս���߼�
		{
			ReplyBattleLeave(roleid, localsid, sid);
		}
		else if(!iscentral && iscross) // ��ԭ���˳����ս��
		{
			CentralDeliveryClient::GetInstance()->SendProtocol(this);
		}
		else if(iscentral) // �ڿ���˳����ս��
		{
			crossbattle->LeaveBattle(NORMAL_BATTLE, roleid, sid);
		}
	}
};

};

#endif
