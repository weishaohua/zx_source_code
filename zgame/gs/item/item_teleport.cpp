#include "item_teleport.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

// 使用传送道具
int 
item_teleport::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *)obj;
	// 检查冷却时间
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_ITEM_TELEPORT))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	// 检查是否在禁止使用的世界中
	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	// 检查重生次数和等级要求	
	if((_reborn_cnt > 0 && pImp->GetRebornCount() < _reborn_cnt) || (_require_level > 0 && pImp->GetObjectLevel() < _require_level))
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	/*
	// 检查世界限制
	if(!pImp->GetWorldManager() | pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
		return -1;
	}
	*/

	// 设置冷却时间
	pImp->SetCoolDown(COOLDOWN_INDEX_ITEM_TELEPORT,5000);
	// 开始使用，执行长距离跳跃              
	if(pImp->LongJump(_pos,_tag))
	{
		return 1;
	}
	pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

