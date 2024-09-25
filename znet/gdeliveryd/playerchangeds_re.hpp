
#ifndef __GNET_PLAYERCHANGEDS_RE_HPP
#define __GNET_PLAYERCHANGEDS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "fetchplayerdata.hrp"
#include "raidmanager.h"
#include "blockedrole.h"
#include "crossbattlemanager.hpp"
#include "centraldeliveryclient.hpp"
#include "battleleave.hpp"
namespace GNET
{
class PlayerChangeDS_Re : public GNET::Protocol
{
	#include "playerchangeds_re"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv PlayerChangeDS_Re retcode %d roleid %d gsid %d flag %d localsid %d", retcode, roleid, gsid, flag, localsid);
		GDeliveryServer* lsm = GDeliveryServer::GetInstance();
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if( NULL == pinfo || pinfo->localsid!=localsid || BlockedRole::GetInstance()->IsRoleBlocked(roleid))
			return;
		if( ERR_CHGS_SUCCESS == retcode )
		{
			//��PlayerLogin����һ��
			UserInfo * user = pinfo->user;
			RoleInfoCache::Instance().Remove(roleid);
			FetchPlayerData * rpc = (FetchPlayerData * )Rpc::Call(RPC_FETCHPLAYERDATA, FetchPlayerDataArg(roleid, user->userid, flag));
			GameDBClient::GetInstance()->SendProtocol(rpc);

			UserContainer::GetInstance().RoleLogout(user, true);
			RaidManager::GetInstance()->HideSeekApplyingRoleQuit(roleid);
			RaidManager::GetInstance()->OnLogout(roleid,pinfo->gameid);
			//ֻ������GS��������
			if (IsTransToCentralDS(flag))
				user->status = _STATUS_REMOTE_HALFLOGIN;
			else if (IsTransToNormalDS(flag))
				user->status = _STATUS_REMOTE_CACHERANDOM;
			else
				Log::log(LOG_ERR, "Recv PlayerChangeDS_Re invalid flag %d roleid %d userid %d", flag, roleid, user->userid);
			RemoteLoggingUsers::GetInstance().Push(user->userid, roleid, user->status);
			Octets random_key;
			Security *rand = Security::Create(RANDOM);
			rand->Update(random_key.resize(32));
			rand->Destroy();
			user->rand_key = random_key;
		}
		else if( ERR_CHGS_DBERROR == retcode )
		{
			lsm->Send(pinfo->linksid, DisconnectPlayer( roleid, -1, pinfo->localsid, -1 ));
			UserContainer::GetInstance().UserLogout(pinfo->user);
		}
		else
		{
			LOG_TRACE("PlayerChangeDS errno %d roleid %d", retcode, roleid);
/*
			pinfo->user->status = _STATUS_ONGAME;
			// ������߲��ɹ����ԭgsid
			gslineid = pinfo->gameid;
*/
		}
		if( ERR_CHGS_SUCCESS != retcode )
		{
			// �л�DSʧ�ܣ���ʱ���ܴ��ڰ�̯�����ף�ս����״̬����Ҫ�����¼�Ŀ��ս����Ϣ
			CrossBattleManager::GetInstance()->EraseFightingTag(roleid);
			CentralDeliveryClient::GetInstance()->SendProtocol(BattleLeave(roleid, 0, true));
		}
	}
};

};

#endif
