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

	//摆摊道具，用于改变摆摊的形象,客户端使用
	int market_item_id = -1;
	int market_item_index = -1;
	//是否使用道具摆摊
	if( (index >= 0 && index <  (int)_inventory.Size()) && -1 != item_id)
	{
		//检查物品类型是否是摆摊用物品
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

	//都通过了
	//检查物品是否合乎正确
	abase::vector<char> flag_list;
	flag_list.insert(flag_list.begin(),_inventory.Size(),0);
	
	int order_count = 0;
	C2S::CMD::open_personal_market::entry_t * ent = (C2S::CMD::open_personal_market::entry_t *)entry_list;
	for(size_t i = 0; i < count ; i ++)
	{
		if( ent[i].price == 0 || ent[i].price > MONEY_CAPACITY_BASE || 
				(ent[i].index != 0xFFFF && !_inventory.IsItemExist(ent[i].index,ent[i].type,ent[i].count)))
		{
			//通知一下物品不正确
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
			//考虑记录购买的数目，统计空闲的空间
			order_count ++;
		}
		else
		{
			if(!_inventory[ent[i].index].CanTrade())
			{
				//禁止交易的物品不能交易
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}

			
			if(flag_list[ent[i].index]++)
			{	
				//不允许一件物品卖出两次
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}
		}
	}
	
	//考虑限制摆摊的速度
	//这个是很有用的
	int sys_time = g_timer.get_systime();
	if(sys_time == _stall_trade_timer) return false;
	_stall_trade_timer = sys_time;

	//物品正确开启摆摊，进入摆摊状态，建立摆摊的对象
	ASSERT(!_stall_obj);

	GLog::log(GLOG_INFO,"用户%d开始摆摊",_parent->ID.id);

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
			GLog::log(GLOG_INFO,"用户%d添加摆摊货物id=%d,index=%d,count=%d,price=%d",_parent->ID.id, ent[i].type, ent[i].index, ent[i].count, ent[i].price);
		}
	}


	_pstate.StartMarket();
	_stall_trade_id ++;
	if((_stall_trade_id & 0xFF) == 0) _stall_trade_id ++;
	//修改自己的状态
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->market_id = _stall_trade_id & 0xFF;
	pPlayer->object_state |= gactive_object::STATE_MARKET; 
	pPlayer->market_item_id = market_item_id;

	//发出给自己的消息
	send_ls_msg(pPlayer,h1);
		
	//发出广播消息
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

	//是否使用道具摆摊
	if( (index >= 0 && index <  (int)_inventory.Size()) && -1 != item_id)
	{
		//检查物品类型是否是摆摊用物品
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
	//首先通知所有的客户，取消服务了
	//现在不进行了，因为不需要维护客户信息了


	GLog::log(GLOG_INFO,"用户%d停止摆摊",_parent->ID.id);

	//清除市场对象 
	delete _stall_obj;
	_stall_obj = NULL;
	
	//然后修改状态回来
	_pstate.Normalize();
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->object_state &= ~(gactive_object::STATE_MARKET); 
	pPlayer->market_item_id = -1;

	//广播
	_runner->cancel_personal_market();

	//摆摊结束后重新检查一下任务，防止任务被卡
	//这里不能直接调用OnTaskCheckState, 因为如果收到客户端的TASK_NOTIFY命令后
	//在OnTaskCheckAwardDirect里面如果可以直接清除非法状态的话, 会取消摆摊状态� 这个时候如果直接调用OnTaskCheckState
	//会导致内存错误
	MSG msg;
	BuildMessage(msg,GM_MSG_TASK_CHECK_STATE,_parent->ID,_parent->ID,A3DVECTOR(0,0,0),0);
	gmatrix::SendMessage(msg);
	return true;
}

int 
gplayer_imp::DoPlayerMarketTrade(const XID & trader,const XID & buyer,gplayer * pTrader, gplayer *pBuyer, const void *order, size_t length)
{
	//第一步：检查人物是否对应 
	if(!pTrader->IsActived() || pTrader->ID != trader || pTrader->imp == NULL || pTrader->IsZombie())
	{
		return -1;
	}
	
	if(!pBuyer->IsActived()  || pBuyer->ID != buyer || pBuyer->imp == NULL || pBuyer->IsZombie())
	{
		return 1;
	}

	//第二步,检查交易内容是否合法
	player_stall::trade_request & req = *(player_stall::trade_request *)order;
	if(length < sizeof(req) || length != sizeof(req) + req.count*sizeof(player_stall::trade_request::entry_t))
	{
		return -2;
	}
	
	size_t need_money = 0;
	//察看物品是否存在 钱数是否会溢出
	if(!((gplayer_imp*)(pTrader->imp))->CheckMarketTradeRequest(req,need_money))
	{
		return -3;
	}

	//察看包裹栏是否有足够的剩余空间,金钱数目是否足够
	if(!((gplayer_imp*)(pBuyer->imp))->CheckMarketTradeRequire(req,need_money))
	{
		return -4;
	}

	//所有条件通过,开始加入数据
	((gplayer_imp*)(pTrader->imp))->DoPlayerMarketTrade(req,(gplayer_imp*)(pBuyer->imp),need_money);

	// 更新活跃度 -- 摆摊并卖出物品
	((gplayer_imp*)(pTrader->imp))->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_STALL_AND_SELL);

	return 0;

}

int 
gplayer_imp::DoPlayerMarketPurchase(const XID & trader,const XID & buyer,gplayer * pTrader, gplayer *pBuyer, const void *order, size_t length)
{
	//第一步：检查人物是否对应 
	if(!pTrader->IsActived() || pTrader->ID != trader || pTrader->imp == NULL || pTrader->IsZombie())
	{
		return -1;
	}
	
	if(!pBuyer->IsActived()  || pBuyer->ID != buyer || pBuyer->imp == NULL || pBuyer->IsZombie())
	{
		return 1;
	}

	//第二步,检查交易内容是否合法
	player_stall::trade_request & req = *(player_stall::trade_request *)order;
	if(length < sizeof(req) || length != sizeof(req) + req.count*sizeof(player_stall::trade_request::entry_t))
	{
		return -2;
	}
	
	size_t total_price = 0;
	//交验收购请求是否正确，计算收购的价格是否过大 计算是否有足够的空间保存物品
	if(!((gplayer_imp*)(pTrader->imp))->CheckMarketPurchaseRequest(req,total_price))
	{
		return -3;
	}

	//察看购买者身上是否有对应的物品存在
	if(!((gplayer_imp*)(pBuyer->imp))->CheckMarketPurchaseRequire(req,total_price))
	{
		return -4;
	}

	//所有条件通过,开始加入数据
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
			if(msg.pos.squared_distance(_parent->pos) < 36.f)	//六米限制
			{
				SendTo<0>(GM_MSG_SERVICE_GREETING,msg.source,0);
			}
		}
		return 0;

		case GM_MSG_SERVICE_REQUEST:
		if(msg.pos.squared_distance(_parent->pos) > 36.f)	//6米限制
		{
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		//对服务的请求到来(要求服务)
		if(msg.param == service_ns::SERVICE_ID_PLAYER_MARKET
				|| msg.param == service_ns::SERVICE_ID_PLAYER_MARKET2)
		{
			//服务ID正确,然后处理到来的玩家请求
			int index1 = 0;
			gplayer *pParent = GetParent();
			XID self = pParent->ID;
			gplayer *pPlayer1 = gmatrix::FindPlayer(msg.source.id,index1);
			if(!pPlayer1  || pPlayer1 == pParent || pPlayer1->tag != GetWorldTag())
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
				return 0;
			}

			//首先试图锁定一下玩家 
			if(pPlayer1 < pParent)
			{
				if(mutex_spinset(&pPlayer1->spinlock) != 0)
				{
					//试图锁失败
					//重新进行加锁操作
					mutex_spinunlock(&pParent->spinlock);
					mutex_spinlock(&pPlayer1->spinlock);
					mutex_spinlock(&pParent->spinlock);
				}
			}
			else
			{
				//可以直接锁定
				mutex_spinlock(&pPlayer1->spinlock);
			}

			//直接调用执行交易的函数
			if(msg.param == service_ns::SERVICE_ID_PLAYER_MARKET)
			{
				//玩家购买商店物品
				if(DoPlayerMarketTrade(self,msg.source, pParent,pPlayer1,msg.content,msg.content_length)<0)
				{
					//发送错误数据 不使用本身的SendTo是因为有概率自身已经被释放
					MSG msg2;
					BuildMessage(msg2,GM_MSG_ERROR_MESSAGE,msg.source,self,A3DVECTOR(0,0,0),S2C::ERR_SERVICE_UNAVILABLE);
					gmatrix::SendMessage(msg2);
				}
			}
			else
			{
				//玩家向商店卖出物品
				if(DoPlayerMarketPurchase(self,msg.source, pParent,pPlayer1,msg.content,msg.content_length)<0)
				{
					//发送错误数据 不使用本身的SendTo是因为有概率自身已经被释放
					MSG msg2;
					BuildMessage(msg2,GM_MSG_ERROR_MESSAGE,msg.source,self,A3DVECTOR(0,0,0),S2C::ERR_SERVICE_UNAVILABLE);
					gmatrix::SendMessage(msg2);
				}
			}

			//解锁顾客 
			mutex_spinunlock(&pPlayer1->spinlock);
		}
		else
		{
			//报告错误
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		return 0;

		case GM_MSG_SERVICE_QUIERY_CONTENT:
		if(msg.pos.squared_distance(_parent->pos) > 36.f)	//6米限制
		{
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		if(msg.content_length == sizeof(int) * 2)
		{
			int cs_index = *(int*)msg.content;
			int sid = *((int*)msg.content + 1);
			//发送当前的装备数据给玩家
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
						//买入物品
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
				CMD::Make<INFO::market_goods>::From(h1);	//放置一个空物品
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
	//检查是否能够进行交易
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
		if(ent.index == 0xFFFF) return false; //这是购买条目
		if(ent.type != req.list[i].type) return false;
 		if(ent.count < item_count || ent.count < list[index] + item_count)
		{
			//买入的数目过多
			return false;
		}
		list[index] += item_count;

		if(!_inventory.IsItemExist(ent.index,ent.type,ent.count)) return false;
		//判断是否不允许交易
		if(!_inventory[ent.index].CanTrade()) return false;
		if(ent.crc != _inventory[ent.index].GetCRC()) return false;
		if(ent.expire_date != _inventory[ent.index].expire_date) return false;

		//进行价格计算
		size_t p = ent.price * item_count;
		if(p/item_count != ent.price) return false;
		size_t tmp =  m + p;
		if(tmp < m) return false;
		m = tmp;
	}
	
	size_t m_total =  m + GetMoney();
	if(m_total < GetMoney() || m_total > _money_capacity)
	{
		//应当取消交易
		CancelPersonalMarket();
		return false;
	}
	//所有判断条件通过了
	need_money = m;
	return true;
}

bool 
gplayer_imp::CheckMarketPurchaseRequest(player_stall::trade_request & req, size_t &total_price)
{
	//检查是否能够进行交易
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
		if(item_count == 0) return false;     //物品数目错误
		if(ent.index != 0xFFFF) return false; //这不是购买条目
		if(ent.type != req.list[i].type ) return false; //数目或者类型不匹配
		if(ent.count < item_count || ent.count < item_count + list[index])
		{
			//超过了物品数目上限
			return false;
		}
		list[index] += item_count;

		//进行价格计算
		size_t p = ent.price * item_count;
		if(p/item_count != ent.price) return false;		//单价超出系统上限
		size_t tmp =  m + p;
		if(tmp < m) return false;				//总价格超界
		m = tmp;
	}
	if(GetMoney() < m) 
	{
		//没钱了，应该取消交易
		return false;
	}

	//判断是否有足够的空间保存物品
	if(_inventory.GetEmptySlotCount() < req.count) return false;

	//所有判断条件通过了
	total_price = m;
	return true;
}

bool 
gplayer_imp::CheckMarketPurchaseRequire(player_stall::trade_request & req, size_t total_price)
{
	//检查是否能够进行交易
	if(!_pstate.CanTrade()) return false;
	if(OI_TestSafeLock()) return false;

	size_t m_total =  total_price + GetMoney();
	if(m_total < GetMoney() || m_total > _money_capacity)
	{
		//金钱过多，无法放下
		return false;
	}
	
	//判断物品是否存在
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_inventory.Size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		int type = req.list[i].type;
		size_t count = req.list[i].count;
		size_t inv_index = req.list[i].inv_index;
		if(!_inventory.IsItemExist(inv_index,type,count)) return false;
		//判断物品是否不允许交易
		if(!_inventory[inv_index].CanTrade()) return false;
		if(list[inv_index]) return false;
		list[inv_index] = 1;	//不允许重复售出物品
	}

	//通过了所有判定，可以购买了
	return true;
}

bool 
gplayer_imp::CheckMarketTradeRequire(player_stall::trade_request & req, size_t need_money)
{
	//检查是否能够进行交易
	if(!_pstate.CanTrade()) return false;
	if(OI_TestSafeLock()) return false;
	if(GetMoney() < need_money) return false;	//资金不足
	
	//判断是否有足够的空位
	if(_inventory.GetEmptySlotCount() < req.count) return false;
	return true;
}

//这是玩家商店卖出物品
void 
gplayer_imp::DoPlayerMarketTrade(player_stall::trade_request & req, gplayer_imp * pImp, size_t need_money)
{
	item_list & inv = pImp->_inventory;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,pImp->GetParent()->ID.id,need_money,req.count);

	int gold1 = GetMoney();
	int gold2 = pImp->GetMoney();
	//交易开始
	//首先进行物品的交换
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
			//要将物品分开
			it = _inventory[ent.index];
			it.content.BuildFrom(_inventory[ent.index].content);
			it.count = item_count;
			_inventory.DecAmount(ent.index,item_count);
		}
		//修正卖出的数据
		ent.count -= item_count;
		_runner->trade_away_item(pImp->GetParent()->ID.id ,ent.index, ent.type, item_count);
		
		int expire_date = it.expire_date;
		int rst = inv.Push(it);
		if(rst < 0) 
		{
			ASSERT(false);
			//不可能的, 但是只好跳过,算倒霉了 contine 
			it.Release();
			continue;
		}
		GLog::log(GLOG_INFO,"用户%d卖给用户%d %d个%d，index=%d，单价%d",_parent->ID.id, pImp->_parent->ID.id, item_count, ent.type, ent.index, ent.price);

		gold1 += ent.price*item_count;
		gold2 -=  ent.price*item_count;
		GLog::action("baitan, huid=%d:hrid=%d:cuid=%d:crid=%d:mode=2:moneytype=1:price=%d:h_lv=%d:c_lv=%d:h_gold=%d:c_gold=%d:hitemid=%d,%d",
							_db_magic_number, _parent->ID.id, pImp->_db_magic_number, pImp->_parent->ID.id,ent.price * item_count,
							GetObjectLevel(),pImp->GetObjectLevel(),gold1,gold2, ent.type, item_count);

		//组织要发给客户端的数据
		int state = item::Proctype2State(it.proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,ent.type,expire_date,item_count,rst,state);

	}

	//修改交易双方的金钱数量
	//这里应该已经保证了钱数必然正确
	ASSERT(need_money + _player_money <= _money_capacity && need_money + _player_money >= _player_money);
	pImp->SpendMoney(need_money);
	GainMoney(need_money);
	_runner->get_player_money(GetMoney(),_money_capacity);
	send_ls_msg(pImp->GetParent(),h1);

	//发送成功的消息
	_runner->market_trade_success(pImp->GetParent()->ID.id);
	pImp->_runner->market_trade_success(GetParent()->ID.id);

	//减少双方交易的存盘时间
	pImp->ReduceSaveTimer(100);
	ReduceSaveTimer(100);
}

//玩家商店购买
void 
gplayer_imp::DoPlayerMarketPurchase(player_stall::trade_request & req, gplayer_imp * pImp, size_t total_price)
{
	item_list & inv = pImp->_inventory;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,pImp->GetParent()->ID.id,total_price,req.count,false);

	int gold1 = GetMoney();
	int gold2 = pImp->GetMoney();
	//交易开始
	//首先进行物品的交换
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
			//要将物品分开
			it = inv[item_index];
			it.content.BuildFrom(inv[item_index].content);
			it.count = item_count;
			inv.DecAmount(item_index,item_count);

		}

		//修正卖出的数据
		ent.count -= item_count;
		pImp->_runner->player_drop_item(IL_INVENTORY,item_index,ent.type,item_count,S2C::DROP_TYPE_TRADEAWAY);
		
		int expire_date = it.expire_date;
		int rst = _inventory.Push(it);
		if(rst < 0) 
		{
			ASSERT(false);
			//不可能的, 但是只好跳过,算倒霉了 contine 
			it.Release();
			continue;
		}
		GLog::log(GLOG_INFO,"用户%d收购用户%d %d个%d，单价%d",_parent->ID.id, pImp->_parent->ID.id, item_count, ent.type, ent.price);

		gold1 -= ent.price*item_count;
		gold2 +=  ent.price*item_count;
		GLog::action("baitan, huid=%d:hrid=%d:cuid=%d:crid=%d:mode=1:moneytype=1:price=%d:h_lv=%d:c_lv=%d:h_gold=%d:c_gold=%d:hitemid=%d,%d",
							_db_magic_number, _parent->ID.id, pImp->_db_magic_number, pImp->_parent->ID.id,ent.price * item_count,
							GetObjectLevel(),pImp->GetObjectLevel(),gold1,gold2, ent.type, item_count);


		//组织要发给客户端的数据
		int state = item::Proctype2State(it.proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,ent.type,expire_date,item_count,rst,stall_index & 0xFF,state);
	}

	//修改交易双方的金钱数量
	ASSERT(total_price + pImp->_player_money <= pImp->_money_capacity && total_price + pImp->_player_money >= pImp->_player_money);
	SpendMoney(total_price);
	pImp->GainMoney(total_price);
	send_ls_msg(GetParent(),h1);
	pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);

	//发送成功的消息
	_runner->market_trade_success(pImp->GetParent()->ID.id);
	pImp->_runner->market_trade_success(GetParent()->ID.id);

	//减少双方交易的存盘时间
	pImp->ReduceSaveTimer(100);
	ReduceSaveTimer(100);
}

