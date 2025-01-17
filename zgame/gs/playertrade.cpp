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
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>
#include "userlogin.h"
#include "trade.h"
#include <factionlib.h>
#include "consign.h"
#include "task/taskman.h"

#define CONSIGN_ROLE_REQUIRED_LEVEL (90)

bool 
gplayer_imp::CanTrade(const XID & target)
{
	//现在一律禁止交易
//	return false;
	
	//非正常状态不能交易
	if(!_pstate.CanTrade()) return false;
	ASSERT(_trade_obj == NULL);

	//交易不能开始的条件有
	//当前有任何类型的session 正在执行中
	if(_cur_session || _session_list.size()) return false;

	if(OI_TestSafeLock()) return false;

	if(GetWorldManager()->GetWorldLimit().notrade) return false;
/*
	//现在战斗状态允许交易 
	//看看会不会出什么问题..
	//战斗状态也不能交易 
	if(IsCombatState()) return false;
	*/
	
	//死亡状态不能交易 在外面判断
	//还有条件是 距离不能超过范围，在外面判断

	//返回true
	return true;

}

void
gplayer_imp::StartTrade(int trade_id, const XID & target)
{
	//战斗状态现在允许交易
	ASSERT(_pstate.CanTrade() && !_cur_session);
	ASSERT(_trade_obj == NULL);

	//加入交易对象， 进入等待交易状态
	_trade_obj = new player_trade(trade_id);
	_pstate.WaitTrade();
	_trade_obj->SetTimeOut(30);	//设置三十秒存盘超时错误

	class WaitInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		int _trade_id;
		unsigned int _counter;
		unsigned int _counter2; 
		int _cur_order_id;
		world *_plane;
	public:
		WaitInfoWriteBack(int trade_id, gplayer_imp * imp,world * pPlane):_trade_id(trade_id),_plane(pPlane)
		{
			gplayer * pPlayer = imp->GetParent();
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
			_counter = imp->GetTrashBoxCounter();
			_counter2 = imp->_eq_change_counter;
			_cur_order_id = imp->_mallinfo.GetOrderID();
		}

		virtual void OnTimeOut()
		{
			CallWriteBack(false);
			delete this;
		}

		virtual void OnFailed()
		{
			CallWriteBack(false);
			delete this;
		}
		
		virtual void OnPutRole(int retcode)
		{
			ASSERT(retcode == 0);
			CallWriteBack(retcode == 0);
			delete this;
		}

		void CallWriteBack(bool success)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; //can't find
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;	// not match
			}
			ASSERT(pPlayer->imp);
			gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
			if(success)
			{
				if(_counter == pImp->GetTrashBoxCounter() )
				{
					pImp->TryClearTBChangeCounter();
				}

				if(_counter2 == pImp->_eq_change_counter )
				{
					pImp->_eq_change_counter = 0;
				}
				//设置最近一次存盘的百宝阁流水号
				pImp->_mallinfo.SaveDone(_cur_order_id);

				pImp->_db_save_error = 0;
			}
			pImp->WaitingTradeWriteBack(_trade_id,success);
		}


	};

	//进行存盘操作
	user_save_data((gplayer*)_parent,new WaitInfoWriteBack(trade_id,this,_plane),0);
	return ;
}

void
gplayer_imp::TradeComplete(int trade_id, int reason,bool need_read)
{
	if(!_pstate.CanCompleteTrade())
	{
		//未在合适的状态中
		return;
	}

	class WaitInfoReadBack: public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		int _trade_id;
		world * _plane;
	public:
		WaitInfoReadBack(int trade_id, gplayer * pPlayer,world *pPlane):_trade_id(trade_id),_plane(pPlane)
		{
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
		}

		virtual void OnTimeOut()
		{
			CallReadBack(NULL);
			delete this;
		}

		virtual void OnFailed()
		{
			CallReadBack(NULL);
			delete this;
		}
		
		virtual void OnGetMoneyInventory(size_t money, const GDB::itemlist & list, int timestamp)
		{
			CallReadBack(&list, money);
			delete this;
		}

		void CallReadBack(const GDB::itemlist * pInv,int money = 0)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; 
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;
			}
			ASSERT(pPlayer->imp);
			((gplayer_imp*)pPlayer->imp)->WaitingTradeReadBack(_trade_id,pInv,money);
		}


	};

	if(_trade_obj->GetTradeID() != trade_id)
	{
		//交易ID不符合则断线之
		GLog::log(GLOG_ERR,"用户%d交易ID%d完成后ID不符合(应为%d)",_parent->ID.id, trade_id,_trade_obj->GetTradeID());
		FromTradeToNormal(-1);
		return;
	}

	if(need_read)
	{	
		//进入读盘等待状态
		_pstate.WaitTradeRead();
		//发起读盘请求并设置超时
		_trade_obj->SetTimeOut(45);

		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack(trade_id,(gplayer*)_parent,_plane));

	}
	else
	{
		//不需要重新读盘，直接返回normal状态
		FromTradeToNormal();
	}
	return;
}

bool
gplayer_imp::StartFactionTrade(int trade_id,int get_mask, int put_mask ,bool no_response)
{
	if(!_pstate.CanTrade() || _cur_session) return false;
	ASSERT(_trade_obj == NULL);

	//加入交易对象， 进入等待交易状态
	_trade_obj = new faction_trade(trade_id, put_mask);
	_pstate.WaitFactionTrade();
	_trade_obj->SetTimeOut(45);	//设置三十秒超时错误

	if(!no_response)
	{
		//发送个人数据
		GNET::syncdata_t data(GetMoney(),0);
//BW		GNET::SendFactionLockResponse(0,trade_id,_parent->ID.id,data);
	}
	((gplayer_dispatcher*)_runner)->mafia_trade_start();
	return true;
}

//现在只用于元宝充值即时到账，因为没有动包裹所以不判断_cur_session
bool    
gplayer_imp::StartTradeCash(int trade_id, int get_mask, int put_mask)
{       
	if(!_pstate.CanTrade()) return false;
	ASSERT(_trade_obj == NULL);

	//加入交易对象，进入等待交易状态
	_trade_obj = new faction_trade(trade_id, put_mask);
	_pstate.WaitFactionTrade();
	_trade_obj->SetTimeOut(45); //设置四十五秒超时错误

	return true;
}

void 
gplayer_imp::WaitingTradeWriteBack(int trade_id,bool bSuccess)
{
	if(!_pstate.IsWaitTradeState()) return;
	if(_trade_obj->GetTradeID() != trade_id) return;

	gplayer * pPlayer = (gplayer*)_parent;
	if(bSuccess)
	{
		//清除一下仓库改变记录
		TryClearTBChangeCounter();

		//写入数据成功
		//发送同意交易的数据
		GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),pPlayer->ID.id,pPlayer->cs_sid,true);

		//进入交易状态
		_pstate.Trade();
	}
	else
	{
		//写入数据失败
		//发送不同意交易的数据
		GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),pPlayer->ID.id,pPlayer->cs_sid,false);

		//回到非交易状态
		FromTradeToNormal();
	}
}

void 
gplayer_imp::WaitingTradeReadBack(int trade_id,const GDB::itemlist * pInv,int money)
{
	if(!_pstate.IsWaitTradeReadState()) return;
	if(_trade_obj->GetTradeID() != trade_id) return;
	if(pInv)
	{
		GLog::log(GLOG_INFO,"用户%d交易成功，交易ID%d金钱改变为%d",_parent->ID.id, trade_id,money - GetMoney());
		//读取数据成功
		//重新整理自己的物品栏
		_inventory.Clear();
		_inventory.InitFromDBData(*pInv);
		if(((size_t)money) > _money_capacity) 
		{
			//
			GLog::log(GLOG_ERR,"用户%d交易金钱超过最大容量",_parent->ID.id);
			money = _money_capacity;
		}
		_player_money = money;

		//回到通常状态
		FromTradeToNormal();
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d交易ID%d完成后取数据失败",_parent->ID.id, trade_id);
		//读取数据失败 直接下线是个好主意
		//并且不进行写盘操作
		FromTradeToNormal(-1);
	}
}

void 
gplayer_imp::PutItemAfterTrade(item_list & backpack)
{
	XID self = _parent->ID;
	for(size_t i = 0; i < backpack.Size();i ++)
	{
		if(backpack[i].type == -1) break;
		item it;
		backpack.Remove(i,it);
		ASSERT(it.type != -1);
		size_t ocount = it.count;
		int expire_date = it.expire_date;
		int type = it.type;
		int rst = _inventory.Push(it);
		if(rst >= 0)
		{
			//获得了东西
			int state = item::Proctype2State(it.proc_type);
			((gplayer_dispatcher*)_runner)->receive_item_after_trade(type,expire_date,ocount-it.count,_inventory[rst].count,rst,state);
		}

		if(it.type != -1)
		{	
			//无法放下了，进行丢弃的操作
			item_data data;
			item_list::ItemToData(it,data);
			DropItemFromData(_plane,_parent->pos,data,self,0,0); 
			it.Release();
		}
	}
}

void 
gplayer_imp::DropAllAfterTrade(item_list & backpack,size_t money)
{
	XID self = _parent->ID;
	const A3DVECTOR & pos = _parent->pos;
	for(size_t i = 0; i < backpack.Size();i ++)
	{
		if(backpack[i].type == -1) break;
		item it;
		backpack.Remove(i,it);
		ASSERT(it.type != -1);
		item_data data;
		item_list::ItemToData(it,data);
		DropItemFromData(_plane,pos,data,self,0,0); 
		it.Release();
	}
	if(money) DropMoneyItem(_plane,pos,money,self,0,0);
}

void 
gplayer_imp::FromTradeToNormal(int type)
{
	if(type < 0)
	{
		//将物品扔在地上
		DropAllAfterTrade(_trade_obj->GetBackpack(),_trade_obj->GetMoney());

		delete _trade_obj;
		_trade_obj = NULL;
		_pstate.Normalize();

		//然后进行下线操作，不进行存盘
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}
	//告诉玩家当前的包裹栏内容
	PlayerGetInventoryDetail(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//首先将积累的物品和钱交给玩家
	size_t inc_money = _trade_obj->GetMoney();
	if(inc_money)
	{
		size_t tmp = GetMoney();
		GainMoneyWithDrop(inc_money);
		((gplayer_dispatcher*)_runner)->receive_money_after_trade(GetMoney() - tmp);
	}
	PutItemAfterTrade(_trade_obj->GetBackpack());

	//然后删除对象，回到交易状态
	delete _trade_obj;
	_trade_obj = NULL;

	//然后回到通用状态 并且通知player现在处于非交易状态
	//死亡状态已经独立出来了
	_pstate.Normalize();
	_write_timer = 512;

	if(_parent->b_disconnect)
	{
		//玩家已经断线，执行断线逻辑  并且存盘
		//进入断线逻辑
		_pstate.Disconnect();
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
	else
	{
		//交易结束后重新检查一下任务，防止任务被卡
		//这里不能直接调用OnTaskCheckState, 因为如果收到客户端的TASK_NOTIFY命令后
		//在OnTaskCheckAwardDirect里面如果可以直接清除非法状态的话, 会取消摆摊状态� 这个时候如果直接调用OnTaskCheckState
		//会导致内存错误
		MSG msg;
		BuildMessage(msg,GM_MSG_TASK_CHECK_STATE,_parent->ID,_parent->ID,A3DVECTOR(0,0,0),0);
		gmatrix::SendMessage(msg);
	}
}

void 
gplayer_imp::FromFactionTradeToNormal(int type)
{
	if(type)
	{
		//将物品扔在地上
		DropAllAfterTrade(_trade_obj->GetBackpack(),_trade_obj->GetMoney());

		delete _trade_obj;
		_trade_obj = NULL;
		_pstate.Normalize();

		//然后进行下线操作，不进行存盘
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}

	ASSERT(_pstate.IsWaitFactionTradeState() || _pstate.IsWaitFactionTradeReadState());
	//告诉玩家当前的包裹栏内容
	//判断是否告诉玩家刷新装备 尚未做
	//PlayerGetInventory(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//首先将积累的物品和钱交给玩家
	size_t inc_money = _trade_obj->GetMoney();
	if(inc_money)
	{
		size_t tmp = GetMoney();
		GainMoneyWithDrop(inc_money);
		((gplayer_dispatcher*)_runner)->receive_money_after_trade(GetMoney() - tmp);
	}
	PutItemAfterTrade(_trade_obj->GetBackpack());

	//帮派交易现在先不重新发送物品数据
	//PlayerGetInventory(0);

	//然后删除对象
	delete _trade_obj;
	_trade_obj = NULL;

	//然后回到通用状态 并且通知player现在处于非交易状态
	//死亡状态已经独立出来了
	_pstate.Normalize();

	((gplayer_dispatcher*)_runner)->mafia_trade_end();
	if(_parent->b_disconnect)
	{
		//玩家已经断线，执行断线逻辑  并且存盘
		Logout(GMSV::PLAYER_LOGOUT_FULL);
	}
}

void 
gplayer_imp::FactionTradeTimeout()
{
	ASSERT(_pstate.IsWaitFactionTradeState());
	class WaitInfoReadBack: public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		world * _plane;
	public:
		WaitInfoReadBack(gplayer * pPlayer,world *pPlane):_plane(pPlane)
		{
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
		}

		virtual void OnTimeOut()
		{
			//CallReadBack(NULL);
			//这里不能在调用rpc了，会死锁
			//所以使用trade_obj的超时
			delete this;
		}

		virtual void OnFailed()
		{
			CallReadBack(NULL);
			delete this;
		}
		
		virtual void OnGetMoneyInventory(size_t money, const GDB::itemlist & list,int timestamp)
		{
			CallReadBack(&list, money, timestamp);
			delete this;
		}

		void CallReadBack(const GDB::itemlist * pInv,int money = 0, int timestamp = 0)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; 
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;	
			}
			ASSERT(pPlayer->imp);
			gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
			pImp->WaitingFactionTradeReadBack(pInv, money, timestamp);
		}

	};

	//进入读盘等待状态
	_pstate.WaitFactionTradeRead();
	//发起读盘请求并设置超时
	_trade_obj->SetTimeOut(45);

	faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
	if(pTrade)
	{
		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack((gplayer*)_parent,_plane));
	}
	else
	{
		ASSERT(false);
		//应该让玩家断线，并且不存盘.....
		FromFactionTradeToNormal(-1);
	}
	return;
}

void 
gplayer_imp::WaitingFactionTradeReadBack(const GDB::itemlist * pInv,int money, int timestamp)
{
	if(!_pstate.IsWaitFactionTradeReadState()) return ;
	if(pInv)
	{
		if(((int)(timestamp -_db_timestamp)) >= 0)
		{
			//修正钱数
			if(((size_t)money) > _money_capacity) 
			{
				//
				GLog::log(GLOG_ERR,"用户%d外部交易超时后，金钱超过最大容量",_parent->ID.id);
				money = _money_capacity;
			}
			_player_money = money;
			GLog::log(GLOG_INFO,"用户%d外部交易超时后，金钱改变 �%d" ,_parent->ID.id,money - GetMoney());

			//读取数据成功
			//重新整理自己的物品栏
			_inventory.Clear();
			_inventory.InitFromDBData(*pInv);
			PlayerGetInventoryDetail(0);
			_db_timestamp = timestamp;
		}
		else
		{
			GLog::log(GLOG_ERR,"用户%d外部交易结束，但时戳较小%d(%d)",_parent->ID.id,timestamp,_db_timestamp);
			//读出来的时间戳过小，不使用 而是用自己的数据
		}

		//回到通常状态
		FromFactionTradeToNormal();
	}
	else
	{
		//读取数据失败 
		GLog::log(GLOG_ERR,"用户%d外部交易完成后取数据失败",_parent->ID.id);
		
		//保存一次非包裹数据
		faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
		if(pTrade)
		{
			user_save_data((gplayer*)_parent,NULL,2,pTrade->_put_mask);
		}
		else
		{
			ASSERT(false);
		}
		
		//并且不进行写盘操作
		FromFactionTradeToNormal(-1);
	}
}

void 
gplayer_imp::FactionTradeComplete(int trade_id,const GNET::syncdata_t & data)
{
	if(!_pstate.IsWaitFactionTradeState())
	{
		//未在合适的状态中
		return;
	}
	GLog::log(GLOG_INFO,"用户%d帮派操作完成，金钱改变为%d",_parent->ID.id, data.money - GetMoney());

	//更新数据
	ASSERT(_trade_obj != NULL);
	ASSERT(data.money >=0 && data.money <= _money_capacity);
	
	if(trade_id != _trade_obj->GetTradeID()) return ;

	//发送成功数据
	//通知客户端完成此操作，可以进行操作
	_player_money = data.money;
	
	SetRefreshState();
	//GSMV_SEND FACTIONTRADECOMPELETE
	//最好进行存盘操作
	
	FromFactionTradeToNormal();
	return;
}

void 
gplayer_imp::SyncTradeComplete(int trade_id,size_t money, const GDB::itemlist & item_change,bool writetrashbox, bool money_change, bool cash_change)
{
	if(!_pstate.IsWaitFactionTradeState())
	{
		//未在合适的状态中
		return;
	}
	//更新数据
	ASSERT(_trade_obj != NULL);
	if(money_change)
	{
		ASSERT(money >=0 && money <= _money_capacity);
	}
	else
	{
		money = GetMoney();
	}
	
	GLog::log(GLOG_INFO,"用户%d外部操作完成，金钱改变为%d",_parent->ID.id, money - GetMoney());

	if(trade_id != _trade_obj->GetTradeID()) return ;

	//发送成功数据
	//通知客户端完成此操作，可以进行操作
	_player_money = money;

	//进行包裹栏的修改
	for(size_t i = 0; i < item_change.count; i ++)
	{
		GDB::itemdata * pData = item_change.list + i;
		if(pData->count)
		{
			item & it = _inventory[pData->index];
			//更新操作
			if(pData->id != (unsigned int)it.type) 
			{
				_inventory.Remove(pData->index);
				item it2;
				if(MakeItemEntry(it2,*pData))
				{
					bool bRst = _inventory.Put(pData->index,it2);
					ASSERT(bRst);
				}
				else
				{
					GLog::log(LOG_ERR, "SyncTradeComplete时创建物品失败%d",it2.type);
					it2.Clear();
				}
			}
			else
			{
				if((unsigned int)pData->count > it.count)
				{
					_inventory.IncAmount(pData->index, pData->count - it.count);
				}
				else
				{
					_inventory.DecAmount(pData->index, it.count - pData->count);
				}
			}
		}
		else
		{
			//删除操作
			_inventory.Remove(pData->index);
		}
	}
	
	if(writetrashbox)
	{
		TryClearTBChangeCounter();
	}
	if(item_change.count)
	{
		PlayerGetInventoryDetail(0);
	}

	if(cash_change)
	{
		_mallinfo.SaveDone(_mallinfo.GetOrderID());
	}
	FromFactionTradeToNormal();
	return;
}

int gplayer_imp::CanConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time)
{
	if(!_pstate.CanTrade()) return -1;
	ASSERT(_consign_obj == NULL);
	if(_cur_session || _session_list.size()) return -2;
	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return -3;
	}

	//售价范围检测，不能低于10，不能高于10w, 单位RMB元
	if(price < 10 || price > 100000) return -12;
	if(sold_time < 1 || sold_time > 7) return -13;
	
	if(type == 1)
	{
		int cost = money + margin;
		if(money < 1000000 || money > 2000000000 || margin < 0  || margin > 2000000000 || cost < 0 || cost > 2000000000 || (int)_player_money < cost) 
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return -4;
		}
	}
	else if(type == 2)
	{
		if(item_id <= 0 || item_cnt <= 0 || item_idx < 0 || item_type <= 0 || (size_t)item_idx >= _inventory.Size()) 
		{
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return -5;
		}
		item& item = _inventory[item_idx];
		if(item.type == -1 || item.type != item_id) 
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return -7;
		}
		if(item_cnt > (int)item.count || item_cnt > (int)item.pile_limit) 
		{
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return -8;
		}
		if(!item.CanTrade() || item.IsBind() || item.expire_date > 0) 
		{
			
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return -9;
		}
		if(margin < 0 || margin > 2000000000 || (int)_player_money < margin) 
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return -10;
		}
	}
	else
	{
		return -11;
	}

	return 0;
}

void gplayer_imp::StartConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, char* sellto_name, char sellto_name_len)
{
	ASSERT(_pstate.CanTrade() && !_cur_session);
	ASSERT(_consign_obj == NULL);

	//加入寄售对象，进入交易等待状态
	_consign_obj = new player_consign;
	_pstate.WaitTrade();
	_consign_obj->SetTimeOut(30); //设置30秒存盘超时错误

	class WaitInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		unsigned int _counter;
		unsigned int _counter2; 
		int _cur_order_id;
		world *_plane;

		char _type;
		int _margin;
		int _item_id;
		int _item_idx;
		short _item_cnt;
		int _item_type;
		int _money;
		int _price;
		int _sold_time;
		char _sellto_name_len;
		char _sellto_name[28];	
		
	public:
		WaitInfoWriteBack(gplayer_imp * imp,world * pPlane, char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, const char* sellto_name, char sellto_name_len)
			:_plane(pPlane) 																																										  
		{
			gplayer * pPlayer = imp->GetParent();
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
			_counter = imp->GetTrashBoxCounter();
			_counter2 = imp->_eq_change_counter;
			_cur_order_id = imp->_mallinfo.GetOrderID();

			_type = type;
			_margin = margin;
			_item_id = item_id;
			_item_idx = item_idx;
			_item_cnt = item_cnt;
			_item_type = item_type;
			_money = money;
			_price = price;
			_sold_time = sold_time;

			_sellto_name_len = sellto_name_len > 28 ? 28 : sellto_name_len;
			memcpy(_sellto_name, sellto_name, _sellto_name_len);
		}
		
		virtual void OnTimeOut()
		{
			CallWriteBack(false);
			delete this;
		}

		virtual void OnFailed()
		{
			CallWriteBack(false);
			delete this;
		}
		
		virtual void OnPutRole(int retcode)
		{
			ASSERT(retcode == 0);
			CallWriteBack(retcode == 0);
			delete this;
		}

		void CallWriteBack(bool success)
		{
			//sleep(35);
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; //can't find
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;	// not match
			}
			ASSERT(pPlayer->imp);
			gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
			if(success)
			{
				if(_counter == pImp->GetTrashBoxCounter() )
				{
					pImp->TryClearTBChangeCounter();
				}

				if(_counter2 == pImp->_eq_change_counter )
				{
					pImp->_eq_change_counter = 0;
				}
				//设置最近一次存盘的百宝阁流水号
				pImp->_mallinfo.SaveDone(_cur_order_id);

				pImp->_db_save_error = 0;
			}
			pImp->WaitingConsignWriteBack(success, _type, _margin, _item_id, _item_cnt, _item_idx, _item_type, _money, _price, _sold_time, _sellto_name, _sellto_name_len);
		}
	};

	//进行存盘操作
	WaitInfoWriteBack* wb = new WaitInfoWriteBack(this,_plane, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time, sellto_name, sellto_name_len);
	user_save_data((gplayer*)_parent, wb, 0);
	//user_save_data((gplayer*)_parent,new WaitInfoWriteBack(this,_plane, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time, sellto_name, sellto_name_len),0);
	return ;
}

void gplayer_imp::CompleteConsign(int reason, char need_read)
{
	if(!_pstate.CanCompleteTrade()) return;

	class WaitInfoReadBack: public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		world * _plane;
	public:
		WaitInfoReadBack(gplayer * pPlayer,world *pPlane):_plane(pPlane)
		{
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
		}

		virtual void OnTimeOut()
		{
			CallReadBack(NULL);
			delete this;
		}

		virtual void OnFailed()
		{
			CallReadBack(NULL);
			delete this;
		}
		
		virtual void OnGetMoneyInventory(size_t money, const GDB::itemlist & list, int timestamp)
		{
			CallReadBack(&list, money);
			delete this;
		}

		void CallReadBack(const GDB::itemlist * pInv,int money = 0)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; 
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;
			}
			ASSERT(pPlayer->imp);
			((gplayer_imp*)pPlayer->imp)->WaitingConsignReadBack(pInv,money);
		}
	};

	if(need_read)
	{	
		//进入读盘等待状态
		_pstate.WaitTradeRead();
		//发起读盘请求并设置超时
		_consign_obj->SetTimeOut(45);
		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack((gplayer*)_parent,_plane));
	}
	else
	{
		//不需要重新读盘，直接返回normal状态
		FromConsignToNormal();
	}
	return;
}

void gplayer_imp::WaitingConsignWriteBack(bool bSuccess, char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, char* sellto_name, char sellto_name_len)
{
	if(!_pstate.IsWaitTradeState()) return;
	
	gplayer* pPlayer = (gplayer*)_parent;
	if(bSuccess)
	{
		//清除一下仓库改变记录
		TryClearTBChangeCounter();

		//写入数据成功
		//发送同意交易的数据
		GMSV::SendConsignRequest(pPlayer->ID.id, type, item_id, item_idx, item_cnt, item_type, money, sold_time, price, margin, sellto_name, sellto_name_len);
		GLog::log(GLOG_INFO,"玩家寄售开始 用户%d,  类型%d, 保证金%d, 物品id:%d, 数量%d, 包裹中位置%d, 物品寄售类型%d, 寄售金钱%d, 寄售价格%d(RMB元), 寄售时间%d(天)", pPlayer->ID.id, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time);

		//进入交易状态
		_pstate.Trade();
	}
	else
	{
		//回到非交易状态
		FromConsignToNormal();
	}
}

void gplayer_imp::WaitingConsignReadBack(const GDB::itemlist * pInv,int money)
{
	if(!_pstate.IsWaitTradeReadState()) return;
	if(pInv)
	{
		GLog::log(GLOG_INFO,"用户%d寄售成功，寄售金钱改变为%d",_parent->ID.id, money - GetMoney());
		//读取数据成功
		//重新整理自己的物品栏
		_inventory.Clear();
		_inventory.InitFromDBData(*pInv);
		if(((size_t)money) > _money_capacity)
		{
			GLog::log(GLOG_ERR,"用户%d寄售计数金钱超过最大容量",_parent->ID.id);
			money = _money_capacity;
		}
		_player_money = money;

		//回到通常状态
		FromConsignToNormal();
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d寄售完成后取数据失败",_parent->ID.id);
		//读取数据失败 直接下线是个好主意
		//并且不进行写盘操作
		FromConsignToNormal(-1);
	}
}

void gplayer_imp::FromConsignToNormal(int type)
{
	if(type < 0)
	{
		delete _consign_obj;
		_consign_obj = NULL;
		_pstate.Normalize();

		//然后进行下线操作，不进行存盘
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}

	//告诉玩家当前的包裹栏内容
	PlayerGetInventoryDetail(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//然后删除对象，回到交易状态
	delete _consign_obj;
	_consign_obj = NULL;

	//然后回到通用状态 并且通知player现在处于非交易状态
	//死亡状态已经独立出来了
	_pstate.Normalize();
	_write_timer = 512;

	if(_parent->b_disconnect)
	{
		//玩家已经断线，执行断线逻辑  并且存盘
		//进入断线逻辑
		_pstate.Disconnect();
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
}

//角色寄售
int gplayer_imp::CanConsignRole(char type, int item_idx, int sold_time, int price, int margin)
{
	if(!_pstate.CanTrade()) return -1;
	if(_cur_session || _session_list.size()) return -2;
	if(4 != type) return -3; //本函数只能用于寄售角色

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_CONSIGN_IN_COMBAT);
		return -1;
	}
	
	//售价范围检测，不能低于60，不能高于100w, 单位RMB元
	if(price < 60 || price > 1000000) return -4;
	if(sold_time < 1 || sold_time > 7) return -5;
	if(margin < 0 || margin > 2000000000) return -6;

	//检查等级
	if(GetRebornCount() <= 0 || GetObjectLevel() < CONSIGN_ROLE_REQUIRED_LEVEL)
	{
		_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -14;
	}

	//检查玩家是否处于红名状态
	if(GetPKValue() > 0)
	{
		_runner->error_message(S2C::ERR_CONSIGN_ROLE_REDNAME);
		return -15;
	}

	//检查玩家是否有召唤兽，怀光的分身会加血量
	if(GetSummonMan().HasSummonNPC())
	{
		_runner->error_message(S2C::ERR_HAS_SUMMON_CAN_NOT_CONSIGNROLE);
		return -19;
	}

	size_t inv_size = _inventory.Size();
	if(item_idx < 0 || (size_t)item_idx >= inv_size) return -16;

	if(_inventory[item_idx].type != g_config.consign_role_item_id) return -17;
	
	PlayerTaskInterface task_if(this);
	if(!task_if.CheckSpecialTasksForConsign()) return -18;

	return 0;
}

void gplayer_imp::StartConsignRole(char type, int item_id, int item_idx, int item_cnt, int item_type, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len)
{
	ASSERT(_pstate.CanTrade() && !_cur_session);

	gplayer* pPlayer = (gplayer*)_parent;
	GMSV::SendConsignStartRole(pPlayer->ID.id, type, item_id, item_idx, item_cnt, item_type, sold_time, price, margin, sellto_name, sellto_name_len);
	SetConsignRoleFlag(true);

	GLog::log(GLOG_INFO,"玩家寄售角色开始 用户%d, 类型%d, 保证金%d, 扣除物品id:%d, 数量%d, 包裹中位置%d, 寄售价格%d(RMB元), 寄售时间%d(天)", pPlayer->ID.id, type, margin, item_id, item_cnt, item_idx, price, sold_time);
}

bool gplayer_imp::CheckToSendConsignRoleInfo(int result)
{
	if(!IsConsignRole())
	{
		return false;
	}

	if(CONSIGN_ROLE_SUCCESS == result)
	{
		//清除所有buff
		_filters.ClearSpecFilter(0xFFFFFFFF);
		OI_UpdateAllProp();

		GMSV::role_basic_attribute basic_attr;
		GMSV::role_deity_info deity_info;
		raw_wrapper title_rp;
		raw_wrapper skill_rp;
		std::vector<GMSV::role_pet_prop_added> pet_prop_added_list;

		GetGsRoleInfoForConsign(basic_attr, deity_info, title_rp, skill_rp, pet_prop_added_list);
		GMSV::role_title_info title_info = {title_rp.size(), title_rp.data()};
		GMSV::role_skill_info skill_info = {skill_rp.size(), skill_rp.data()};
		GMSV::SendPlayerConsignOfflineRe(GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid, result, basic_attr, deity_info, title_info, skill_info, pet_prop_added_list);
	}
	else
	{
		GMSV::SendPlayerConsignFailedOfflineRe(GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid, result);
	}

	//去掉身上的标志
	SetConsignRoleFlag(false);
	//通知客户端玩家下线
	_runner->error_message(S2C::ERR_CONSIGN_ROLE_LOGOUT);
	GLog::log(GLOG_INFO, "玩家角色寄售offline_re发送，result=%d", result);

	return true;
}

