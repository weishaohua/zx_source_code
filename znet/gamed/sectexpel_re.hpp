
#ifndef __GNET_SECTEXPEL_RE_HPP
#define __GNET_SECTEXPEL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void sect_del_member(int master, int disciple);
namespace GNET
{

class SectExpel_Re : public GNET::Protocol
{
	#include "sectexpel_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		//ʦ������ͽ��
		//sect_id ʦ�� role_id  �� sect_id == role_id �������ʦ��
		//disciple ����˭
		if(retcode == 0)
		{
			sect_del_member(sectid, disciple);
		}
	}
};

};

#endif
