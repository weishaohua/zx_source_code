
#ifndef __GNET_BATTLEENTERFAIL_HPP
#define __GNET_BATTLEENTERFAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BattleEnterFail : public GNET::Protocol
{
	#include "battleenterfail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gs_id = GProviderServer::GetInstance()->GetGSID(sid);
		LOG_TRACE("battleenter fail, roleid=%d gsid=%d mapid=%d", roleid, gs_id, map_id);
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo)//��� pinfo == NULL �����ǽ�ɫ��ԭ��ֱ�Ӵ�����ս�� ��ʱ���� enterfail ��������
			BattleManager::GetInstance()->OnBattleEnterFail(roleid, gs_id, map_id);
	}
};

};

#endif
