
#ifndef __GNET_STARTFACTIONMULTIEXP_HPP
#define __GNET_STARTFACTIONMULTIEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StartFactionMultiExp : public GNET::Protocol
{
	#include "startfactionmultiexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//Ĭ�ϰ���һ������ �Ҵ˴�һ�����Գɹ�
		Log::formatlog("startfactionmultiexp", "fid %d multi %d end_time %d", fid, multi, end_time);
		FactionManager::Instance()->OnStartMultiExp(fid, multi, end_time);
	}
};

};

#endif
