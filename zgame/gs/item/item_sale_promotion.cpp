
#include "item_sale_promotion.h"
#include "../player_imp.h"

extern abase::timer      g_timer;

std::set<int> item_sale_promotion::_pre_task_ids;			// ǰ������id����
std::set<int> item_sale_promotion::_pre_achieve_ids;		// ǰ�óɾ�id����


int 
item_sale_promotion::OnUse(item::LOCATION l, size_t index, gactive_imp * imp, item * parent, const char* arg, size_t arg_size) const
{
	gplayer_imp* pImp = (gplayer_imp*)imp;
	if(l != item::INVENTORY) return -1;

	if (arg_size != sizeof(sale_promotion_item_use_args)) return -1;;
	sale_promotion_item_content* pContent = GetDataEssence(parent);
	if (pContent == NULL) return -1; // �汾???
	sale_promotion_item_use_args* pArgs = (sale_promotion_item_use_args*)arg;

	if (pArgs->index == -1) { // pArgs->index = -1��ʾ��ȡ���⽱��
		if (pContent->is_get_extra_award) return -1; // ����ȡ����
		if (pContent->extra_count < _extra_award_need_count) return -1; // ��������
		if (!pImp->InventoryHasSlot(1)) return -1; 

		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(_extra_award_id);
		if (pItem == NULL) return -1;

		int expire_date = 0;
		if (_extra_award_valid_time) {
			expire_date = g_timer.get_systime() + _extra_award_valid_time;
		} 
		pImp->DeliverItem(_extra_award_id, 	_extra_award_num, _extra_award_is_bind > 0, expire_date, ITEM_INIT_TYPE_TASK);
		pContent->is_get_extra_award = 1;
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, index);
		return 0;	
	} else {
		if (pArgs->index < 0 || pArgs->index >= 20) return -1;

		sale_promotion cur = _sale_promotions[pArgs->index];
		if (cur._valid_start == 0 && cur._valid_end == 0) return -1; // δ���ģ�Ӧ�ñ�����ʱ������
	
		int cur_time = g_timer.get_systime();
		if (cur_time < cur._valid_start || cur_time > cur._valid_end) return -1; // ʱ�䲻��������

		if (cur._cond_type != COND_TYPE_LOTTERY) { // �齱�޴���
			if (pContent->usedcount[pArgs->index] >= cur._get_count) return -1; // ʹ�ô�������
		}
		// ��齱����Ʒ����
		for (int i = 0; i < 2; ++i) {	
			if (cur._award_items[i].award_item_id == 0 || cur._award_items[i].award_item_count == 0) continue;

			element_data::item_tag_t tag = {element_data::IMT_CREATE, 0};
			const item_data * pItem = gmatrix::GetDataMan().generate_item(cur._award_items[i].award_item_id, &tag, sizeof(tag));
			if (pItem == NULL || pItem->pile_limit <= 0) return -1;
			if (cur._award_items[i].award_item_count > (int)pItem->pile_limit)	return -1; // ���ڶѵ����ޣ�����
			if (cur._award_items[i].award_item_valid_time && pItem->pile_limit > 1) return -1; // ʱ�����ԣ��ѵ�������Ϊ1
		}	

		// ֱ��ʹ�ú��
		parent->Bind();

		// �������ռ�
		int award_item_num = 0;
		if (cur._award_items[0].award_item_id > 0) {
			award_item_num += 1;	
		}	
		if (cur._award_items[1].award_item_id > 0) {
			award_item_num += 1;	
		}	
		if (award_item_num != 0) { // �齱��û�н������������ڳ齱ҳ������
			if (!pImp->InventoryHasSlot(award_item_num)) return -1; 
		}
		// ��鲢����ǰ������
		switch (cur._cond_type) {
		case COND_TYPE_DIRECT: 
			break;
		case COND_TYPE_PRE_TASK:
			if (!pImp->isSalePromotionTaskCompleted(cur._cond_arg1)) { // ǰ������δ���
				return -1;	
			}
			break;
		case COND_TYPE_PRE_ACHIEVEMENT:
			if (!pImp->IsAchievementFinish(cur._cond_arg1)) { // ǰ��ɾ�δ���
				return -1;	
			}
			break;
		case COND_TYPE_USE_INGOT: 
		{
			if (cur._award_items[0].award_item_id == 0) return -1; // RMB�࣬��Ҫ��һ����Ʒ���ڣ�����־����
			if (pImp->MallInfo().GetCash() < cur._cond_arg1) { // Ԫ������
				return -1;	
			}

			// ʵ��RMB�̳ǹ�����־
			// ������Ҽ�¼formatlog��ʱ�������⴦��, cash_need�ֶμ�Ϊ0(����ͳ������) 
			int expire_date = 0;
			if (cur._award_items[0].award_item_valid_time) {
				expire_date = g_timer.get_systime() + cur._award_items[0].award_item_valid_time;
			}

			int order_id = pImp->MallInfo().GetOrderID(); 
			pImp->PlayerUseCash(cur._award_items[0].award_item_id, cur._award_items[0].award_item_count, expire_date, cur._cond_arg1, order_id);
			break;
		}
		case COND_TYPE_USE_MONEY:
			if ((int)pImp->GetMoney() < cur._cond_arg1) { // Ǯ����
				return -1;		
			}

			pImp->SpendMoney(cur._cond_arg1);
			pImp->_runner->spend_money(cur._cond_arg1);
			break;
		case COND_TYPE_USE_ITEM:
			if (pImp->GetInventory().CountItemByID(cur._cond_arg1) < cur._cond_arg2) { // ��Ʒ����
				return -1;	
			}
			pImp->TakeOutItem(cur._cond_arg1, cur._cond_arg2);
			break;
		case COND_TYPE_USE_AREA_YHD:
			if (pImp->GetRegionReputation(cur._cond_arg1) < cur._cond_arg2) { // ������������
				return -1;	
			}
			pImp->ModifyRegionReputation(cur._cond_arg1, -cur._cond_arg2);
			break;
		case COND_TYPE_LOTTERY: // �齱�������ť������ͳ�����
			break;
		default: // δ֪����
			return -1;
		}

		// ���ϴ���
		pContent->usedcount[pArgs->index] += 1;

		int extra_count = 0;
		for (int i = 0; i < 20; i++) {
			if (pContent->usedcount[i] > 0)	
				extra_count++;
		}
		pContent->extra_count = extra_count;

		// ͨ����Ʒ���ݱ仯
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, index);

		// �ӽ�����Ʒ
		for (int i = 0; i < 2; ++i) {	
			if (cur._award_items[i].award_item_id == 0 || cur._award_items[i].award_item_count == 0) continue;

			const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(cur._award_items[i].award_item_id);
			if (pItem == NULL) continue;

			pImp->DeliverItem(cur._award_items[i].award_item_id, 
					cur._award_items[i].award_item_count, 
					cur._award_items[i].award_item_is_bind > 0, 
					cur._award_items[i].award_item_valid_time, ITEM_INIT_TYPE_TASK);
		}	
		// XXX ȫ����ȡ���Ƿ�Ҫ����??

		return 0;
	}
}

void 
item_sale_promotion::InitFromShop(gplayer_imp* pImp, item* parent, int value) const
{
	GetDataEssence(parent); // ��������
}

void
item_sale_promotion::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	GetDataEssence(parent); // ���������ж�����InitFromShop�������������¸�ʽ�����û�У�����������
}
