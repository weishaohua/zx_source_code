
#ifndef __GNET_GCIRCLECHAT_HPP
#define __GNET_GCIRCLECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "circlemanager.h"
#include "base64.h"


namespace GNET
{
class GCircleChat;
void CircleChat(GCircleChat & chat);
class GCircleChat : public GNET::Protocol
{
	#include "gcirclechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		CircleChat(*this);
	}
};

};

#endif
