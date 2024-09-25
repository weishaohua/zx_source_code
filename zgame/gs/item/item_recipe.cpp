#include "item_recipe.h"
#include "../player_imp.h"

/**
 * @brief ʹ���䷽��Ʒ�Ĵ�����
 * 
 * @param LOCATION ��Ʒλ�ã�δʹ�ã�
 * @param index ��Ʒ����
 * @param obj ʹ����Ʒ�Ķ���
 * @param parent ����Ʒ��δʹ�ã�
 * @return int ����1��ʾ�ɹ�������0��ʾʧ��
 */
int 
item_recipe::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{    
	// ȷ��obj��gplayer_imp���͵Ķ���
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	// �����ҵ������ȼ��Ƿ�����Ҫ�󣬲���������䷽
	if(pImp->GetProduceLevel() >= _require_produce_level && pImp->AddRecipe(_recipe_id))
	{
		// ��ҳɹ�ѧϰ�䷽��������غ���
		pImp->_runner->player_learn_recipe(_recipe_id);
		return 1;// ���سɹ�
	}
	
	//error message ����
	// ���ѧϰ�䷽ʧ�ܣ����ʹ�����Ϣ
	pImp->_runner->error_message(S2C::ERR_LEARN_RECIPE_FAILED);
	
	return 0;// ����ʧ��
}

