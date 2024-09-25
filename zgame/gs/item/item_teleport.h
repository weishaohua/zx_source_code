#ifndef __ONLINEGAME_GS_TELEPORT_ITEM_H__
#define __ONLINEGAME_GS_TELEPORT_ITEM_H__

#include "../item.h"
#include "../config.h"
#include <common/types.h>

// 传送道具类，继承自item_body
class item_teleport : public item_body
{
	int _tag;			// 标签
	A3DVECTOR _pos;		// 传送目标位置
	int _reborn_cnt;	// 重生次数
	int _require_level;	// 使用所需等级
	int _use_time;		// 使用时间（以tick为单位）
public:
	// 构造函数：初始化传送道具的各项属性
	item_teleport(int tag,float x, float y ,float z, int reborn_cnt, int require_level, float use_time=0.f):_tag(tag),_pos(x,y,z), _reborn_cnt(reborn_cnt),
		_require_level(require_level),_use_time(SECOND_TO_TICK(use_time))
	{}

public:
	// 获取道具类型
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TELEPORT;}
    // 检查道具是否可以使用
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	// 获取道具使用持续时间
	virtual int OnGetUseDuration() const { return _use_time;}
	// 道具使用时的处理函数
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	// 检查是否广播道具使用
	virtual bool IsItemBroadcastUse() const {return true;}
};
#endif



