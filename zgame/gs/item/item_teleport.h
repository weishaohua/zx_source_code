#ifndef __ONLINEGAME_GS_TELEPORT_ITEM_H__
#define __ONLINEGAME_GS_TELEPORT_ITEM_H__

#include "../item.h"
#include "../config.h"
#include <common/types.h>

// ���͵����࣬�̳���item_body
class item_teleport : public item_body
{
	int _tag;			// ��ǩ
	A3DVECTOR _pos;		// ����Ŀ��λ��
	int _reborn_cnt;	// ��������
	int _require_level;	// ʹ������ȼ�
	int _use_time;		// ʹ��ʱ�䣨��tickΪ��λ��
public:
	// ���캯������ʼ�����͵��ߵĸ�������
	item_teleport(int tag,float x, float y ,float z, int reborn_cnt, int require_level, float use_time=0.f):_tag(tag),_pos(x,y,z), _reborn_cnt(reborn_cnt),
		_require_level(require_level),_use_time(SECOND_TO_TICK(use_time))
	{}

public:
	// ��ȡ��������
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TELEPORT;}
    // �������Ƿ����ʹ��
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	// ��ȡ����ʹ�ó���ʱ��
	virtual int OnGetUseDuration() const { return _use_time;}
	// ����ʹ��ʱ�Ĵ�����
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	// ����Ƿ�㲥����ʹ��
	virtual bool IsItemBroadcastUse() const {return true;}
};
#endif



