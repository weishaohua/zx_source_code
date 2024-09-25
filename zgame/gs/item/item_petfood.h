#ifndef __ONLINEGAME_GS_PETFOOD_ITEM_H__
#define __ONLINEGAME_GS_PETFOOD_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../petman.h"
#include <crc.h>

class item_pet_food: public item_body
{
public:
	enum
	{
		MIN_USAGE_NUM = 200, //һ����������Ʒ����������
	};
	struct use_arg_t
	{
		int index;
	};
public:
	int _pet_level_min;
	int _pet_level_max;
	unsigned int _pet_type_mask; //��������
	int _food_usage; // 0 ���䱥ʳ�� 1 ����hp 2 ����mp 3 �������ܶȵ�ǰֵ 4 �������� 5 �������� 6 ���ӳ��ﾭ��
	unsigned int _food_type; // 0 ���ʹ�õ� ֻ��_food_usage Ϊ012ʱ������ 1 һ��������Ʒ
	int _food_value;

protected:
	//��Ǹ�����Ʒ����ʹ��
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return sizeof(int) == buf_size; }
	virtual bool IsItemBroadcastUse() const { return false; }
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* imp,item* parent) const;
	virtual int OnUse(item::LOCATION l,size_t index,gactive_imp* imp,item* parent,const char* arg,size_t arg_size) const;

public:
	item_pet_food(int pet_level_min,int pet_level_max,unsigned int pet_type_mask,int food_usage,unsigned int food_type,int food_value):
		_pet_level_min(pet_level_min),_pet_level_max(pet_level_max),_pet_type_mask(pet_type_mask),_food_usage(food_usage),_food_type(food_type),_food_value(food_value)
	{}
	
public:
	virtual ITEM_TYPE GetItemType() const
	{
		return ITEM_TYPE_PET_FOOD;
	}

	void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;
};

#endif

