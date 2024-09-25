
#ifndef __GNET_GETUICONFIG_HPP
#define __GNET_GETUICONFIG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "getuiconfig_re.hpp"

#include "getrolebase.hrp"
#include "gamedbclient.hpp"
#include "gtplatformagent.h"
#include "crossbattlemanager.hpp"
#include "mapuser.h"
#include "raidmanager.h"
namespace GNET
{

class GetUIConfig : public GNET::Protocol
{
	#include "getuiconfig"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
		if(info)
		{
			LOG_TRACE( "GetUIConfig: roleid=%d,size=%d",roleid,info->config_data.size());
			GetUIConfig_Re	re(ERR_SUCCESS,roleid,localsid,Octets());
			//����4000˵��client�������̫�󣬴�ʱ�����Ѵ����ؿ�ֵ��client�����������ֵ			
			if(info->config_data.size()<=4000)
				re.ui_config = info->config_data;
			manager->Send(sid,re);
			GTPlatformAgent::OnPlayerLogin(roleid,*info);
	                PlayerInfo * player = UserContainer::GetInstance().FindRole(roleid);
        	        if(player && player->factionid)
				if(!RaidManager::GetInstance()->IsRaidLine(player->gameid))		
					RaidManager::GetInstance()->NotifyRolePkFaction(player->factionid,roleid);
		
			// �յ�����GetUIConfig�����ͻ����Ѿ���ʼ������
			// ���������ս��������ս���Ƿ����ظ���ͬ����Ϣ���Ա�ͻ�����ʾս����ѯ��ť
			
			CrossBattleManager *crossbattle = CrossBattleManager::GetInstance();
			// ��ˮϯս����Ҫ�������״̬����ʾ��������
			const CrossBattleInfo* pcbif = crossbattle->GetCrossBattleInfo(roleid, OPENBANQUET_BATTLE);
			if(pcbif != NULL && GDeliveryServer::GetInstance()->IsCentralDS())
			{
				OpenBanquetManager::GetInstance()->OnQueue(roleid, pcbif->_gsid, pcbif->_mapid);
			}

			const FightingTag *tag = crossbattle->GetFightingTag(roleid);
			if(tag != NULL && GDeliveryServer::GetInstance()->IsCentralDS())
			{
				if(tag->_isactive == true)
				{
					Log::log(LOG_DEBUG, "roleid=%d enter crossbattle", roleid);
					manager->Send(sid, BattleEnter_Re(ERR_SUCCESS, roleid, tag->_endtime, tag->_subbattle_type, 0, false));
				}

				OpenBanquetManager::GetInstance()->OnRealEnter(roleid, tag->_gsid, tag->_tag);
				// �յ��������Ϣ������Ѿ���ս���У������Ҫɾ������Խ����ս����Ϣ
				crossbattle->EraseFightingTag(roleid);
			}
			else
			{
				LOG_TRACE( "GetUIConfig: PlayerLogin roleid=%d",roleid);
				crossbattle->PlayerLogin(roleid);
			}

			
		}
		else
			Log::log(LOG_ERR, "GetUIConfig, role cache miss, roleid=%d", roleid);
	}
};

};

#endif
