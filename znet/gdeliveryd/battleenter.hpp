
#ifndef __GNET_BATTLEENTER_HPP
#define __GNET_BATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "playerchangegs.hpp"
#include "battleenter_re.hpp"
#include "crossbattlemanager.hpp"

namespace GNET
{

class BattleEnter : public GNET::Protocol
{
	#include "battleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		bool b_gm = false;
		LOG_TRACE( "BattleEnter:roleid=%d gs_id=%d map_id=%d iscross=%d", roleid, gs_id, map_id, iscross);
		if(iscross && !iscentral) // ��ԭ��ͬ�����ս������Ҫ����Ϣת�������������
		{
			CrossBattleManager::GetInstance()->EnterBattle(NORMAL_BATTLE, roleid);
			return;
		}
		else if(!iscross) // ��ɫ�����ڷ�����ͬ�����ս������ԭս���߼�
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
			if ( NULL==pinfo )
			{
				return;
			}
			if (pinfo->gameid != gs_id)
			{
				return;
			}
			if (pinfo->IsGM())
				b_gm = true;
		}

		BattleEnter_Re res;
		res.roleid = roleid;
		res.localsid = localsid;
		res.endtime = 0;
		res.iscross = iscross;
		res.retcode = BattleManager::GetInstance()->OnBattleEnter(roleid, gs_id, map_id, res.endtime, res.battle_type, b_gm);
		LOG_TRACE( "BattleEnter:roleid=%d retcode=%d gs_id=%d map_id=%d iscross=%d endtime=%d enter battle endtime =%s\n", roleid, res.retcode, gs_id, map_id, iscross, res.endtime, ctime((time_t*)&res.endtime));
		if(res.retcode != ERR_SUCCESS)
		{
			CrossBattleManager::GetInstance()->EraseFightingTag(roleid);
		}
		else
		{
			CrossBattleManager::GetInstance()->ActiveFightingTag(roleid);
		}
		if(!iscross)
		{
			GDeliveryServer::GetInstance()->Send(sid, res);
			// �������ڷ�����ս���ģ����Ȼظ��Ƿ����ɹ���Ȼ������ǽ�����ս��������Ҫ֪ͨԭ��
			if(iscentral)
			{
				int zoneid = CrossBattleManager::GetInstance()->GetSrcZoneID(roleid);
				if(res.retcode == ERR_SUCCESS)
				{
					res.retcode = ERR_BATTLE_ENTERCROSSBATTLE;
				}
				
				CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, res);
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, NORMAL_BATTLE);
			}
		}
		else // �յ������ԭ��������ս������Ϣ
		{
			CrossBattleManager::GetInstance()->EnterBattle(NORMAL_BATTLE, res);
		}
	
		return;
	}
};

};

#endif
