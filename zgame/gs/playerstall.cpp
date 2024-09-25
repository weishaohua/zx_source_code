#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>

#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include "userlogin.h"
#include "playertemplate.h"
#include <common/protocol_imp.h>
#include "playerstall.h"
#include "serviceprovider.h"
#include "task/taskman.h"

bool 
gplayer_imp::PlayerOpenPersonalMarket(int index, int item_id, size_t count, const char name[28], int * entry_list)
{
	if(GetWorldManager()->GetWorldLimit().nomarket) return false;
	if(_parent->IsZombie() || !_pstate.IsNormalState()) return false;
	if(!CanSitDown()) return false;
	if(_cur_session || _session_list.size()) return false;
	if(count > PLAYER_MARKET_SLOT_CAP || count == 0)return false;

	//��̯���ߣ����ڸı��̯������,�ͻ���ʹ��
	int market_item_id = -1;
	int market_item_index = -1;
	//�Ƿ�ʹ�õ��߰�̯
	if( (index >= 0 && index <  (int)_inventory.Size()) && -1 != item_id)
	{
		//�����Ʒ�����Ƿ��ǰ�̯����Ʒ
		int tmp_item_id = _inventory[index].type;
		if(tmp_item_id <= 0 || item_id != tmp_item_id) return false;
		DATA_TYPE dt2;
		const BOOTH_FIGURE_ITEM_ESSENCE &ess= *(const BOOTH_FIGURE_ITEM_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(tmp_item_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_BOOTH_FIGURE_ITEM_ESSENCE || &ess == NULL)
		{
			return false;
		}

		market_item_index = index;
		market_item_id    = item_id;

	}

	packet_wrapper h1(1024);
	using namespace S2C;
	CMD::Make<CMD::self_open_market>::From(h1, market_item_index, market_item_id, count);

	//��ͨ����
	//�����Ʒ�Ƿ�Ϻ���ȷ
	abase::vector<char> flag_list;
	flag_list.insert(flag_list.begin(),_inventory.Size(),0);
	
	int order_count = 0;
	C2S::CMD::open_personal_market::entry_t * ent = (C2S::CMD::open_personal_market::entry_t *)entry_list;
	for(size_t i = 0; i < count ; i ++)
	{
		if( ent[i].price == 0 || ent[i].price > MONEY_CAPACITY_BASE || 
				(ent[i].index != 0xFFFF && !_inventory.IsItemExist(ent[i].index,ent[i].type,ent[i].count)))
		{
			//֪ͨһ����Ʒ����ȷ
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return false;
		}
		if(ent[i].index == 0xFFFF)
		{
			item_data * pData=(item_data*)gmatrix::GetDataMan().get_item_for_sell(ent[i].type);
			if(pData == NULL || ent[i].count > pData->pile_limit)
			{
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}
			//���Ǽ�¼�������Ŀ��ͳ�ƿ��еĿռ�
			order_count ++;
		}
		else
		{
			if(!_inventory[ent[i].index].CanTrade())
			{
				//��ֹ���׵���Ʒ���ܽ���
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}

			
			if(flag_list[ent[i].index]++)
			{	
				//������һ����Ʒ��������
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}
		}
	}
	
	//�������ư�̯���ٶ�
	//����Ǻ����õ�
	int sys_time = g_timer.get_systime();
	if(sys_time == _stall_trade_timer) return false;
	_stall_trade_timer = sys_time;

	//��Ʒ��ȷ������̯�������̯״̬��������̯�Ķ���
	ASSERT(!_stall_obj);

	GLog::log(GLOG_INFO,"�û�%d��ʼ��̯",_parent->ID.id);

	_stall_obj = new player_stall(_inventory);
	_stall_obj->SetMarketName(name);
	for(size_t i = 0; i < count ; i ++)
	{
		CMD::Make<CMD::self_open_market>::AddGoods(h1,ent[i].type,ent[i].index,ent[i].count,ent[i].price);
		if(ent[i].index == 0xFFFF)
			_stall_obj->AddOrderGoods(ent[i].index,ent[i].type,ent[i].count,ent[i].price);
		else
		{
			_stall_obj->AddTradeGoods(ent[i].index,ent[i].type,ent[i].count,ent[i].price);
			GLog::log(GLOG_INFO,"�û�%d��Ӱ�̯����id=%d,index=%d,count=%d,price=%d",_parent->ID.id, ent[i].type, ent[i].index, ent[i].count, ent[i].price);
		}
	}


	_pstate.StartMarket();
	_stall_trade_id ++;
	if((_stall_trade_id & 0xFF) == 0) _stall_trade_id ++;
	//�޸��Լ���״̬
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->market_id = _stall_trade_id & 0xFF;
	pPlayer->object_state |= gactive_object::STATE_MARKET; 
	pPlayer->market_item_id = market_item_id;

	//�������Լ�����Ϣ
	send_ls_msg(pPlayer,h1);
		
	//�����㲥��Ϣ
	_runner->begin_personal_market(market_item_id, _stall_trade_id,_stall_obj->GetName(),_stall_obj->GetNameLen());
	return true;
}

bool 
gplayer_imp::PlayerTestPersonalMarket(int index, int item_id)
{
	if(GetWorldManager()->GetWorldLimit().nomarket) return false;
	if(_parent->IsZombie() || !_pstate.IsNormalState()) return false;
	if(_cur_session || _session_list.size()) return false;
	if(!CanSitDown()) return false;

	//�Ƿ�ʹ�õ��߰�̯
	if( (index >= 0 && index <  (int)_inventory.Size()) && -1 != item_id)
	{
		//�����Ʒ�����Ƿ��ǰ�̯����Ʒ
		int rt_id = _inventory[index].type;
		if(rt_id <= 0 || item_id != rt_id) return false;
		DATA_TYPE dt2;
		const BOOTH_FIGURE_ITEM_ESSENCE &ess= *(const BOOTH_FIGURE_ITEM_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(rt_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_BOOTH_FIGURE_ITEM_ESSENCE || &ess == NULL)
		{
			return false;
		}

		_runner->personal_market_available(index, item_id);
	}
	else
	{
		_runner->personal_market_available(-1, -1);
	}

	return true;
}

bool 
gplayer_imp::CancelPersonalMarket()
{
	//����֪ͨ���еĿͻ���ȡ��������
	//���ڲ������ˣ���Ϊ����Ҫά���ͻ���Ϣ��


	GLog::log(GLOG_INFO,"�û�%dֹͣ��̯",_parent->ID.id);

	//����г����� 
	delete _stall_obj;
	_stall_obj = NULL;
	
	//Ȼ���޸�״̬����
	_pstate.Normalize();
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->object_state &= ~(gactive_object::STATE_MARKET); 
	pPlayer->market_item_id = -1;

	//�㲥
	_runner->cancel_personal_market();

	//��̯���������¼��һ�����񣬷�ֹ���񱻿�
	//���ﲻ��ֱ�ӵ���OnTaskCheckState, ��Ϊ����յ��ͻ��˵�TASK_NOTIFY�����
	//��OnTaskCheckAwardDirect�����������ֱ������Ƿ�״̬�Ļ�, ��ȡ����̯״̬� ���ʱ�����ֱ�ӵ���OnTaskCheckState
	//�ᵼ���ڴ����
	MSG msg;
	BuildMessage(msg,GM_MSG_TASK_CHECK_STATE,_parent->ID,_parent->ID,A3DVECTOR(0,0,0),0);
	gmatrix::SendMessage(msg);
	return true;
}

int 
gplayer_imp::DoPlayerMarketTrade(const XID & trader,const XID & buyer,gplayer * pTrader, gplayer *pBuyer, const void *order, size_t length)
{
	//��һ������������Ƿ��Ӧ 
	if(!pTrader->IsActived() || pTrader->ID != trader || pTrader->imp == NULL || pTrader->IsZombie())
	{
		return -1;
	}
	
	if(!pBuyer->IsActived()  || pBuyer->ID != buyer || pBuyer->imp == NULL || pBuyer->IsZombie())
	{
		return 1;
	}

	//�ڶ���,��齻�������Ƿ�Ϸ�
	player_stall::trade_request & req = *(player_stall::trade_request *)order;
	if(length < sizeof(req) || length != sizeof(req) + req.count*sizeof(player_stall::trade_request::entry_t))
	{
		return -2;
	}
	
	size_t need_money = 0;
	//�쿴��Ʒ�Ƿ���� Ǯ���Ƿ�����
	if(!((gplayer_imp*)(pTrader->imp))->CheckMarketTradeRequest(req,need_money))
	{
		return -3;
	}

	//�쿴�������Ƿ����㹻��ʣ��ռ�,��Ǯ��Ŀ�Ƿ��㹻
	if(!((gplayer_imp*)(pBuyer->imp))->CheckMarketTradeRequire(req,need_money))
	{
		return -4;
	}

	//��������ͨ��,��ʼ��������
	((gplayer_imp*)(pTrader->imp))->DoPlayerMarketTrade(req,(gplayer_imp*)(pBuyer->imp),need_money);

	// ���»�Ծ�� -- ��̯��������Ʒ
	((gplayer_imp*)(pTrader->imp))->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_STALL_AND_SELL);

	return 0;

}

int 
gplayer_imp::DoPlayerMarketPurchase(const XID & trader,const XID & buyer,gplayer * pTrader, gplayer *pBuyer, const void *order, size_t length)
{
	//��һ������������Ƿ��Ӧ 
	if(!pTrader->IsActived() || pTrader->ID != trader || pTrader->imp == NULL || pTrader->IsZombie())
	{
		return -1;
	}
	
	if(!pBuyer->IsActived()  || pBuyer->ID != buyer || pBuyer->imp == NULL || pBuyer->IsZombie())
	{
		return 1;
	}

	//�ڶ���,��齻�������Ƿ�Ϸ�
	player_stall::trade_request & req = *(player_stall::trade_request *)order;
	if(length < sizeof(req) || length != sizeof(req) + req.count*sizeof(player_stall::trade_request::entry_t))
	{
		return -2;
	}
	
	size_t total_price = 0;
	//�����չ������Ƿ���ȷ�������չ��ļ۸��Ƿ���� �����Ƿ����㹻�Ŀռ䱣����Ʒ
	if(!((gplayer_imp*)(pTrader->imp))->CheckMarketPurchaseRequest(req,total_price))
	{
		return -3;
	}

	//�쿴�����������Ƿ��ж�Ӧ����Ʒ����
	if(!((gplayer_imp*)(pBuyer->imp))->CheckMarketPurchaseRequire(req,total_price))
	{
		return -4;
	}

	//��������ͨ��,��ʼ��������
	((gplayer_imp*)(pTrader->imp))->DoPlayerMarketPurchase(req,(gplayer_imp*)(pBuyer->imp),total_price);

	return 0;

}

int 
gplayer_imp::MarketHandler(const MSG & msg)
{
	switch(msg.message)
	{       

		case GM_MSG_QUERY_MARKET_NAME:
			_runner->send_market_name(msg.source,*(int*)(msg.content), msg.param,_stall_obj->GetName(),_stall_obj->GetNameLen());
			break;

		case GM_MSG_SERVICE_HELLO:
		{
			if(msg.pos.squared_distance(_parent->pos) < 36.f)	//��������
			{
				SendTo<0>(GM_MSG_SERVICE_GREETING,msg.source,0);
			}
		}
		return 0;

		case GM_MSG_SERVICE_REQUEST:
		if(msg.pos.squared_distance(_parent->pos) > 36.f)	//6������
		{
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		//�Է����������(Ҫ�����)
		if(msg.param == service_ns::SERVICE_ID_PLAYER_MARKET
				|| msg.param == service_ns::SERVICE_ID_PLAYER_MARKET2)
		{
			//����ID��ȷ,Ȼ���������������
			int index1 = 0;
			gplayer *pParent = GetParent();
			XID self = pParent->ID;
			gplayer *pPlayer1 = gmatrix::FindPlayer(msg.source.id,index1);
			if(!pPlayer1  || pPlayer1 == pParent || pPlayer1->tag != GetWorldTag())
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
				return 0;
			}

			//������ͼ����һ����� 
			if(pPlayer1 < pParent)
			{
				if(mutex_spinset(&pPlayer1->spinlock) != 0)
				{
					//��ͼ��ʧ��
					//���½��м�������
					mutex_spinunlock(&pParent->spinlock);
					mutex_spinlock(&pPlayer1->spinlock);
					mutex_spinlock(&pParent->spinlock);
				}
			}
			else
			{
				//����ֱ������
				mutex_spinlock(&pPlayer1->spinlock);
			}

			//ֱ�ӵ���ִ�н��׵ĺ���
			if(msg.param == service_ns::SERVICE_ID_PLAYER_MARKET)
			{
				//��ҹ����̵���Ʒ
				if(DoPlayerMarketTrade(self,msg.source, pParent,pPlayer1,msg.content,msg.content_length)<0)
				{
					//���ʹ������� ��ʹ�ñ����SendTo����Ϊ�и��������Ѿ����ͷ�
					MSG msg2;
					BuildMessage(msg2,GM_MSG_ERROR_MESSAGE,msg.source,self,A3DVECTOR(0,0,0),S2C::ERR_SERVICE_UNAVILABLE);
					gmatrix::SendMessage(msg2);
				}
			}
			else
			{
				//������̵�������Ʒ
				if(DoPlayerMarketPurchase(self,msg.source, pParent,pPlayer1,msg.content,msg.content_length)<0)
				{
					//���ʹ������� ��ʹ�ñ����SendTo����Ϊ�и��������Ѿ����ͷ�
					MSG msg2;
					BuildMessage(msg2,GM_MSG_ERROR_MESSAGE,msg.source,self,A3DVECTOR(0,0,0),S2C::ERR_SERVICE_UNAVILABLE);
					gmatrix::SendMessage(msg2);
				}
			}

			//�����˿� 
			mutex_spinunlock(&pPlayer1->spinlock);
		}
		else
		{
			//�������
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		return 0;

		case GM_MSG_SERVICE_QUIERY_CONTENT:
		if(msg.pos.squared_distance(_parent->pos) > 36.f)	//6������
		{
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		if(msg.content_length == sizeof(int) * 2)
		{
			int cs_index = *(int*)msg.content;
			int sid = *((int*)msg.content + 1);
			//���͵�ǰ��װ�����ݸ����
			ASSERT(_stall_obj);
			if(!_stall_obj) return 0;
			packet_wrapper  h1(2048);
			using namespace S2C;
			size_t count = _stall_obj->_goods_list.size();
			CMD::Make<CMD::player_market_info>::From(h1,_parent,_stall_trade_id,count);
			for(size_t i =0; i < count ;i ++)
			{
				const player_stall::entry_t & ent = _stall_obj->_goods_list[i];
				if(ent.count)
				{
					if(ent.index == 0xFFFF)
					{
						//������Ʒ
						CMD::Make<INFO::market_goods>::BuyItem(h1,ent.type,ent.count,ent.price);
						continue;
					}
					
					size_t index = ent.index;
					item_data_client data;
					if(_inventory.GetItemDataForClient(index,data) > 0 && ent.type == data.type 
							&& data.expire_date == ent.expire_date
							&& _inventory[index].GetCRC() == ent.crc
							)
					{
						CMD::Make<INFO::market_goods>::SellItem(h1,ent.type,ent.count,ent.price,data, item::Proctype2State(data.proc_type));
						continue;
					}
				}
				CMD::Make<INFO::market_goods>::From(h1);	//����һ������Ʒ
			}
			send_ls_msg(cs_index, msg.source.id, sid,h1.data(),h1.size());

		}
		else
		{
			ASSERT(false);
		}
		return 0;
	}
	return 0;
}

bool 
gplayer_imp::CheckMarketTradeRequest(player_stall::trade_request & req, size_t &need_money)
{
	//����Ƿ��ܹ����н���
	if(!_pstate.IsMarketState()) return false;
	if(!_stall_obj) { ASSERT(false); return false;}
	if(_stall_trade_id != req.trade_id) return false;
	size_t m = 0;
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_stall_obj->_goods_list.size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		size_t index = req.list[i].index;
		if(index >= _stall_obj->_goods_list.size()) return false;
		const player_stall::entry_t & ent = _stall_obj->_goods_list[index];
		size_t item_count = req.list[i].count;
		if(item_count == 0) return false;
		if(ent.index == 0xFFFF) return false; //���ǹ�����Ŀ
		if(ent.type != req.list[i].type) return false;
 		if(ent.count < item_count || ent.count < list[index] + item_count)
		{
			//�������Ŀ����
			return false;
		}
		list[index] += item_count;

		if(!_inventory.IsItemExist(ent.index,ent.type,ent.count)) return false;
		//�ж��Ƿ�������
		if(!_inventory[ent.index].CanTrade()) return false;
		if(ent.crc != _inventory[ent.index].GetCRC()) return false;
		if(ent.expire_date != _inventory[ent.index].expire_date) return false;

		//���м۸����
		size_t p = ent.price * item_count;
		if(p/item_count != ent.price) return false;
		size_t tmp =  m + p;
		if(tmp < m) return false;
		m = tmp;
	}
	
	size_t m_total =  m + GetMoney();
	if(m_total < GetMoney() || m_total > _money_capacity)
	{
		//Ӧ��ȡ������
		CancelPersonalMarket();
		return false;
	}
	//�����ж�����ͨ����
	need_money = m;
	return true;
}

bool 
gplayer_imp::CheckMarketPurchaseRequest(player_stall::trade_request & req, size_t &total_price)
{
	//����Ƿ��ܹ����н���
	if(!_pstate.IsMarketState()) return false;
	if(!_stall_obj) { ASSERT(false); return false;}
	if(_stall_trade_id != req.trade_id) return false;
	size_t m = 0;
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_stall_obj->_goods_list.size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		size_t index = req.list[i].index;
		if(index >= _stall_obj->_goods_list.size()) return false;
		size_t item_count = req.list[i].count;
		const player_stall::entry_t & ent = _stall_obj->_goods_list[index];
		if(item_count == 0) return false;     //��Ʒ��Ŀ����
		if(ent.index != 0xFFFF) return false; //�ⲻ�ǹ�����Ŀ
		if(ent.type != req.list[i].type ) return false; //��Ŀ�������Ͳ�ƥ��
		if(ent.count < item_count || ent.count < item_count + list[index])
		{
			//��������Ʒ��Ŀ����
			return false;
		}
		list[index] += item_count;

		//���м۸����
		size_t p = ent.price * item_count;
		if(p/item_count != ent.price) return false;		//���۳���ϵͳ����
		size_t tmp =  m + p;
		if(tmp < m) return false;				//�ܼ۸񳬽�
		m = tmp;
	}
	if(GetMoney() < m) 
	{
		//ûǮ�ˣ�Ӧ��ȡ������
		return false;
	}

	//�ж��Ƿ����㹻�Ŀռ䱣����Ʒ
	if(_inventory.GetEmptySlotCount() < req.count) return false;

	//�����ж�����ͨ����
	total_price = m;
	return true;
}

bool 
gplayer_imp::CheckMarketPurchaseRequire(player_stall::trade_request & req, size_t total_price)
{
	//����Ƿ��ܹ����н���
	if(!_pstate.CanTrade()) return false;
	if(OI_TestSafeLock()) return false;

	size_t m_total =  total_price + GetMoney();
	if(m_total < GetMoney() || m_total > _money_capacity)
	{
		//��Ǯ���࣬�޷�����
		return false;
	}
	
	//�ж���Ʒ�Ƿ����
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_inventory.Size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		int type = req.list[i].type;
		size_t count = req.list[i].count;
		size_t inv_index = req.list[i].inv_index;
		if(!_inventory.IsItemExist(inv_index,type,count)) return false;
		//�ж���Ʒ�Ƿ�������
		if(!_inventory[inv_index].CanTrade()) return false;
		if(list[inv_index]) return false;
		list[inv_index] = 1;	//�������ظ��۳���Ʒ
	}

	//ͨ���������ж������Թ�����
	return true;
}

bool 
gplayer_imp::CheckMarketTradeRequire(player_stall::trade_request & req, size_t need_money)
{
	//����Ƿ��ܹ����н���
	if(!_pstate.CanTrade()) return false;
	if(OI_TestSafeLock()) return false;
	if(GetMoney() < need_money) return false;	//�ʽ���
	
	//�ж��Ƿ����㹻�Ŀ�λ
	if(_inventory.GetEmptySlotCount() < req.count) return false;
	return true;
}

//��������̵�������Ʒ
void 
gplayer_imp::DoPlayerMarketTrade(player_stall::trade_request & req, gplayer_imp * pImp, size_t need_money)
{
	item_list & inv = pImp->_inventory;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,pImp->GetParent()->ID.id,need_money,req.count);

	int gold1 = GetMoney();
	int gold2 = pImp->GetMoney();
	//���׿�ʼ
	//���Ƚ�����Ʒ�Ľ���
	for(size_t i = 0; i < req.count;i ++)
	{
		size_t item_index = req.list[i].index;
		ASSERT(item_index < _stall_obj->_goods_list.size());
		player_stall::entry_t & ent = _stall_obj->_goods_list[item_index];
		size_t item_count = req.list[i].count;
		ASSERT(item_count);
		ASSERT(ent.type == req.list[i].type && ent.count >= item_count);
		ASSERT(_inventory.IsItemExist(ent.index,ent.type,ent.count));
		ASSERT(ent.crc == _inventory[ent.index].GetCRC()); 

		item it;
		if(_inventory[ent.index].count == item_count)
		{
			_inventory.Exchange(ent.index,it);
		}
		else
		{
			//Ҫ����Ʒ�ֿ�
			it = _inventory[ent.index];
			it.content.BuildFrom(_inventory[ent.index].content);
			it.count = item_count;
			_inventory.DecAmount(ent.index,item_count);
		}
		//��������������
		ent.count -= item_count;
		_runner->trade_away_item(pImp->GetParent()->ID.id ,ent.index, ent.type, item_count);
		
		int expire_date = it.expire_date;
		int rst = inv.Push(it);
		if(rst < 0) 
		{
			ASSERT(false);
			//�����ܵ�, ����ֻ������,�㵹ù�� contine 
			it.Release();
			continue;
		}
		GLog::log(GLOG_INFO,"�û�%d�����û�%d %d��%d��index=%d������%d",_parent->ID.id, pImp->_parent->ID.id, item_count, ent.type, ent.index, ent.price);

		gold1 += ent.price*item_count;
		gold2 -=  ent.price*item_count;
		GLog::action("baitan, huid=%d:hrid=%d:cuid=%d:crid=%d:mode=2:moneytype=1:price=%d:h_lv=%d:c_lv=%d:h_gold=%d:c_gold=%d:hitemid=%d,%d",
							_db_magic_number, _parent->ID.id, pImp->_db_magic_number, pImp->_parent->ID.id,ent.price * item_count,
							GetObjectLevel(),pImp->GetObjectLevel(),gold1,gold2, ent.type, item_count);

		//��֯Ҫ�����ͻ��˵�����
		int state = item::Proctype2State(it.proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,ent.type,expire_date,item_count,rst,state);

	}

	//�޸Ľ���˫���Ľ�Ǯ����
	//����Ӧ���Ѿ���֤��Ǯ����Ȼ��ȷ
	ASSERT(need_money + _player_money <= _money_capacity && need_money + _player_money >= _player_money);
	pImp->SpendMoney(need_money);
	GainMoney(need_money);
	_runner->get_player_money(GetMoney(),_money_capacity);
	send_ls_msg(pImp->GetParent(),h1);

	//���ͳɹ�����Ϣ
	_runner->market_trade_success(pImp->GetParent()->ID.id);
	pImp->_runner->market_trade_success(GetParent()->ID.id);

	//����˫�����׵Ĵ���ʱ��
	pImp->ReduceSaveTimer(100);
	ReduceSaveTimer(100);
}

//����̵깺��
void 
gplayer_imp::DoPlayerMarketPurchase(player_stall::trade_request & req, gplayer_imp * pImp, size_t total_price)
{
	item_list & inv = pImp->_inventory;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,pImp->GetParent()->ID.id,total_price,req.count,false);

	int gold1 = GetMoney();
	int gold2 = pImp->GetMoney();
	//���׿�ʼ
	//���Ƚ�����Ʒ�Ľ���
	for(size_t i = 0; i < req.count;i ++)
	{

		size_t item_index = req.list[i].inv_index;
		size_t item_count = req.list[i].count;
		size_t stall_index = req.list[i].index;
		ASSERT(item_count);
		ASSERT(inv.IsItemExist(item_index,req.list[i].type,item_count));
		player_stall::entry_t & ent = _stall_obj->_goods_list[stall_index];
		ASSERT(ent.type == req.list[i].type && ent.count >= item_count);

		item it;
		if(inv[item_index].count == item_count)
		{
			inv.Remove(item_index,it);
		}
		else
		{
			//Ҫ����Ʒ�ֿ�
			it = inv[item_index];
			it.content.BuildFrom(inv[item_index].content);
			it.count = item_count;
			inv.DecAmount(item_index,item_count);

		}

		//��������������
		ent.count -= item_count;
		pImp->_runner->player_drop_item(IL_INVENTORY,item_index,ent.type,item_count,S2C::DROP_TYPE_TRADEAWAY);
		
		int expire_date = it.expire_date;
		int rst = _inventory.Push(it);
		if(rst < 0) 
		{
			ASSERT(false);
			//�����ܵ�, ����ֻ������,�㵹ù�� contine 
			it.Release();
			continue;
		}
		GLog::log(GLOG_INFO,"�û�%d�չ��û�%d %d��%d������%d",_parent->ID.id, pImp->_parent->ID.id, item_count, ent.type, ent.price);

		gold1 -= ent.price*item_count;
		gold2 +=  ent.price*item_count;
		GLog::action("baitan, huid=%d:hrid=%d:cuid=%d:crid=%d:mode=1:moneytype=1:price=%d:h_lv=%d:c_lv=%d:h_gold=%d:c_gold=%d:hitemid=%d,%d",
							_db_magic_number, _parent->ID.id, pImp->_db_magic_number, pImp->_parent->ID.id,ent.price * item_count,
							GetObjectLevel(),pImp->GetObjectLevel(),gold1,gold2, ent.type, item_count);


		//��֯Ҫ�����ͻ��˵�����
		int state = item::Proctype2State(it.proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,ent.type,expire_date,item_count,rst,stall_index & 0xFF,state);
	}

	//�޸Ľ���˫���Ľ�Ǯ����
	ASSERT(total_price + pImp->_player_money <= pImp->_money_capacity && total_price + pImp->_player_money >= pImp->_player_money);
	SpendMoney(total_price);
	pImp->GainMoney(total_price);
	send_ls_msg(GetParent(),h1);
	pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);

	//���ͳɹ�����Ϣ
	_runner->market_trade_success(pImp->GetParent()->ID.id);
	pImp->_runner->market_trade_success(GetParent()->ID.id);

	//����˫�����׵Ĵ���ʱ��
	pImp->ReduceSaveTimer(100);
	ReduceSaveTimer(100);
}

