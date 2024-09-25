#include "item_recipe.h"
#include "../player_imp.h"

/**
 * @brief 使用配方物品的处理函数
 * 
 * @param LOCATION 物品位置（未使用）
 * @param index 物品索引
 * @param obj 使用物品的对象
 * @param parent 父物品（未使用）
 * @return int 返回1表示成功，返回0表示失败
 */
int 
item_recipe::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{    
	// 确保obj是gplayer_imp类型的对象
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	// 检查玩家的生产等级是否满足要求，并尝试添加配方
	if(pImp->GetProduceLevel() >= _require_produce_level && pImp->AddRecipe(_recipe_id))
	{
		// 玩家成功学习配方，调用相关函数
		pImp->_runner->player_learn_recipe(_recipe_id);
		return 1;// 返回成功
	}
	
	//error message 加入
	// 如果学习配方失败，发送错误信息
	pImp->_runner->error_message(S2C::ERR_LEARN_RECIPE_FAILED);
	
	return 0;// 返回失败
}

