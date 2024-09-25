#include "item_teleport.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

// ʹ�ô��͵���
int 
item_teleport::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *)obj;
	// �����ȴʱ��
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_ITEM_TELEPORT))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	// ����Ƿ��ڽ�ֹʹ�õ�������
	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	// ������������͵ȼ�Ҫ��	
	if((_reborn_cnt > 0 && pImp->GetRebornCount() < _reborn_cnt) || (_require_level > 0 && pImp->GetObjectLevel() < _require_level))
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	/*
	// �����������
	if(!pImp->GetWorldManager() | pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
		return -1;
	}
	*/

	// ������ȴʱ��
	pImp->SetCoolDown(COOLDOWN_INDEX_ITEM_TELEPORT,5000);
	// ��ʼʹ�ã�ִ�г�������Ծ              
	if(pImp->LongJump(_pos,_tag))
	{
		return 1;
	}
	pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

