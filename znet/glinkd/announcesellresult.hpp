
#ifndef __GNET_ANNOUNCESELLRESULT_HPP
#define __GNET_ANNOUNCESELLRESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
namespace GNET
{

class AnnounceSellResult : public GNET::Protocol
{
	#include "announcesellresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		unsigned int tmp=localsid;
		localsid=0;
		GLinkServer::GetInstance()->Send( tmp,this );
	}
};

};

#endif
