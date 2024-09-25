
#ifndef __GNET_OPENBANQUETENTER_HPP
#define __GNET_OPENBANQUETENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "playerchangegs.hpp"
#include "battleenter_re.hpp"
#include "crossbattlemanager.hpp"
#include "openbanquetmanager.h"


namespace GNET
{

class OpenBanquetEnter : public GNET::Protocol
{
	#include "openbanquetenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetEnter roleid=%d, gs_id=%d, map_id=%d, iscross=%d, localsid=%d", roleid, gs_id, map_id, iscross, localsid);

		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		bool b_gm = false;
		if(iscross && !iscentral) // ��ԭ��ͬ�����ս������Ҫ����Ϣת�������������
		{
			CrossBattleManager::GetInstance()->EnterOpenBanquetBattle(OPENBANQUET_BATTLE, roleid);
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
			if (pinfo->IsGM())
				b_gm = true;
		}

		BattleEnter_Re res;
		res.roleid = roleid;
		res.localsid = localsid;
		res.endtime = 0;
		res.iscross = iscross;
		res.battle_type = OPENBANQUET_BATTLE;
		if(iscentral)
		{
			// ��� - ���
			res.retcode = OpenBanquetManager::GetInstance()->OnTryEnter(roleid, gs_id, map_id, res.endtime, res.battle_type, iscross, b_gm);	
		}
		LOG_TRACE( "openbanquetenter:roleid=%d retcode=%d gs_id=%d map_id=%d iscross=%d endtime=%d enter battle endtime =%s\n", roleid, res.retcode, gs_id, map_id, iscross, res.endtime, ctime((time_t*)&res.endtime));
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
				CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
			}
			
		}
		else // �յ������ԭ��������ս������Ϣ
		{
			CrossBattleManager::GetInstance()->EnterBattle(OPENBANQUET_BATTLE, res);
		}

		return;
	}
};

};

#endif
