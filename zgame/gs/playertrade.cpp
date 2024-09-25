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
	//����һ�ɽ�ֹ����
//	return false;
	
	//������״̬���ܽ���
	if(!_pstate.CanTrade()) return false;
	ASSERT(_trade_obj == NULL);

	//���ײ��ܿ�ʼ��������
	//��ǰ���κ����͵�session ����ִ����
	if(_cur_session || _session_list.size()) return false;

	if(OI_TestSafeLock()) return false;

	if(GetWorldManager()->GetWorldLimit().notrade) return false;
/*
	//����ս��״̬������ 
	//�����᲻���ʲô����..
	//ս��״̬Ҳ���ܽ��� 
	if(IsCombatState()) return false;
	*/
	
	//����״̬���ܽ��� �������ж�
	//���������� ���벻�ܳ�����Χ���������ж�

	//����true
	return true;

}

void
gplayer_imp::StartTrade(int trade_id, const XID & target)
{
	//ս��״̬����������
	ASSERT(_pstate.CanTrade() && !_cur_session);
	ASSERT(_trade_obj == NULL);

	//���뽻�׶��� ����ȴ�����״̬
	_trade_obj = new player_trade(trade_id);
	_pstate.WaitTrade();
	_trade_obj->SetTimeOut(30);	//������ʮ����̳�ʱ����

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
				//�������һ�δ��̵İٱ�����ˮ��
				pImp->_mallinfo.SaveDone(_cur_order_id);

				pImp->_db_save_error = 0;
			}
			pImp->WaitingTradeWriteBack(_trade_id,success);
		}


	};

	//���д��̲���
	user_save_data((gplayer*)_parent,new WaitInfoWriteBack(trade_id,this,_plane),0);
	return ;
}

void
gplayer_imp::TradeComplete(int trade_id, int reason,bool need_read)
{
	if(!_pstate.CanCompleteTrade())
	{
		//δ�ں��ʵ�״̬��
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
		//����ID�����������֮
		GLog::log(GLOG_ERR,"�û�%d����ID%d��ɺ�ID������(ӦΪ%d)",_parent->ID.id, trade_id,_trade_obj->GetTradeID());
		FromTradeToNormal(-1);
		return;
	}

	if(need_read)
	{	
		//������̵ȴ�״̬
		_pstate.WaitTradeRead();
		//��������������ó�ʱ
		_trade_obj->SetTimeOut(45);

		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack(trade_id,(gplayer*)_parent,_plane));

	}
	else
	{
		//����Ҫ���¶��̣�ֱ�ӷ���normal״̬
		FromTradeToNormal();
	}
	return;
}

bool
gplayer_imp::StartFactionTrade(int trade_id,int get_mask, int put_mask ,bool no_response)
{
	if(!_pstate.CanTrade() || _cur_session) return false;
	ASSERT(_trade_obj == NULL);

	//���뽻�׶��� ����ȴ�����״̬
	_trade_obj = new faction_trade(trade_id, put_mask);
	_pstate.WaitFactionTrade();
	_trade_obj->SetTimeOut(45);	//������ʮ�볬ʱ����

	if(!no_response)
	{
		//���͸�������
		GNET::syncdata_t data(GetMoney(),0);
//BW		GNET::SendFactionLockResponse(0,trade_id,_parent->ID.id,data);
	}
	((gplayer_dispatcher*)_runner)->mafia_trade_start();
	return true;
}

//����ֻ����Ԫ����ֵ��ʱ���ˣ���Ϊû�ж��������Բ��ж�_cur_session
bool    
gplayer_imp::StartTradeCash(int trade_id, int get_mask, int put_mask)
{       
	if(!_pstate.CanTrade()) return false;
	ASSERT(_trade_obj == NULL);

	//���뽻�׶��󣬽���ȴ�����״̬
	_trade_obj = new faction_trade(trade_id, put_mask);
	_pstate.WaitFactionTrade();
	_trade_obj->SetTimeOut(45); //������ʮ���볬ʱ����

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
		//���һ�²ֿ�ı��¼
		TryClearTBChangeCounter();

		//д�����ݳɹ�
		//����ͬ�⽻�׵�����
		GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),pPlayer->ID.id,pPlayer->cs_sid,true);

		//���뽻��״̬
		_pstate.Trade();
	}
	else
	{
		//д������ʧ��
		//���Ͳ�ͬ�⽻�׵�����
		GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),pPlayer->ID.id,pPlayer->cs_sid,false);

		//�ص��ǽ���״̬
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
		GLog::log(GLOG_INFO,"�û�%d���׳ɹ�������ID%d��Ǯ�ı�Ϊ%d",_parent->ID.id, trade_id,money - GetMoney());
		//��ȡ���ݳɹ�
		//���������Լ�����Ʒ��
		_inventory.Clear();
		_inventory.InitFromDBData(*pInv);
		if(((size_t)money) > _money_capacity) 
		{
			//
			GLog::log(GLOG_ERR,"�û�%d���׽�Ǯ�����������",_parent->ID.id);
			money = _money_capacity;
		}
		_player_money = money;

		//�ص�ͨ��״̬
		FromTradeToNormal();
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����ID%d��ɺ�ȡ����ʧ��",_parent->ID.id, trade_id);
		//��ȡ����ʧ�� ֱ�������Ǹ�������
		//���Ҳ�����д�̲���
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
			//����˶���
			int state = item::Proctype2State(it.proc_type);
			((gplayer_dispatcher*)_runner)->receive_item_after_trade(type,expire_date,ocount-it.count,_inventory[rst].count,rst,state);
		}

		if(it.type != -1)
		{	
			//�޷������ˣ����ж����Ĳ���
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
		//����Ʒ���ڵ���
		DropAllAfterTrade(_trade_obj->GetBackpack(),_trade_obj->GetMoney());

		delete _trade_obj;
		_trade_obj = NULL;
		_pstate.Normalize();

		//Ȼ��������߲����������д���
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}
	//������ҵ�ǰ�İ���������
	PlayerGetInventoryDetail(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//���Ƚ����۵���Ʒ��Ǯ�������
	size_t inc_money = _trade_obj->GetMoney();
	if(inc_money)
	{
		size_t tmp = GetMoney();
		GainMoneyWithDrop(inc_money);
		((gplayer_dispatcher*)_runner)->receive_money_after_trade(GetMoney() - tmp);
	}
	PutItemAfterTrade(_trade_obj->GetBackpack());

	//Ȼ��ɾ�����󣬻ص�����״̬
	delete _trade_obj;
	_trade_obj = NULL;

	//Ȼ��ص�ͨ��״̬ ����֪ͨplayer���ڴ��ڷǽ���״̬
	//����״̬�Ѿ�����������
	_pstate.Normalize();
	_write_timer = 512;

	if(_parent->b_disconnect)
	{
		//����Ѿ����ߣ�ִ�ж����߼�  ���Ҵ���
		//��������߼�
		_pstate.Disconnect();
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
	else
	{
		//���׽��������¼��һ�����񣬷�ֹ���񱻿�
		//���ﲻ��ֱ�ӵ���OnTaskCheckState, ��Ϊ����յ��ͻ��˵�TASK_NOTIFY�����
		//��OnTaskCheckAwardDirect�����������ֱ������Ƿ�״̬�Ļ�, ��ȡ����̯״̬� ���ʱ�����ֱ�ӵ���OnTaskCheckState
		//�ᵼ���ڴ����
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
		//����Ʒ���ڵ���
		DropAllAfterTrade(_trade_obj->GetBackpack(),_trade_obj->GetMoney());

		delete _trade_obj;
		_trade_obj = NULL;
		_pstate.Normalize();

		//Ȼ��������߲����������д���
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}

	ASSERT(_pstate.IsWaitFactionTradeState() || _pstate.IsWaitFactionTradeReadState());
	//������ҵ�ǰ�İ���������
	//�ж��Ƿ�������ˢ��װ�� ��δ��
	//PlayerGetInventory(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//���Ƚ����۵���Ʒ��Ǯ�������
	size_t inc_money = _trade_obj->GetMoney();
	if(inc_money)
	{
		size_t tmp = GetMoney();
		GainMoneyWithDrop(inc_money);
		((gplayer_dispatcher*)_runner)->receive_money_after_trade(GetMoney() - tmp);
	}
	PutItemAfterTrade(_trade_obj->GetBackpack());

	//���ɽ��������Ȳ����·�����Ʒ����
	//PlayerGetInventory(0);

	//Ȼ��ɾ������
	delete _trade_obj;
	_trade_obj = NULL;

	//Ȼ��ص�ͨ��״̬ ����֪ͨplayer���ڴ��ڷǽ���״̬
	//����״̬�Ѿ�����������
	_pstate.Normalize();

	((gplayer_dispatcher*)_runner)->mafia_trade_end();
	if(_parent->b_disconnect)
	{
		//����Ѿ����ߣ�ִ�ж����߼�  ���Ҵ���
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
			//���ﲻ���ڵ���rpc�ˣ�������
			//����ʹ��trade_obj�ĳ�ʱ
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

	//������̵ȴ�״̬
	_pstate.WaitFactionTradeRead();
	//��������������ó�ʱ
	_trade_obj->SetTimeOut(45);

	faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
	if(pTrade)
	{
		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack((gplayer*)_parent,_plane));
	}
	else
	{
		ASSERT(false);
		//Ӧ������Ҷ��ߣ����Ҳ�����.....
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
			//����Ǯ��
			if(((size_t)money) > _money_capacity) 
			{
				//
				GLog::log(GLOG_ERR,"�û�%d�ⲿ���׳�ʱ�󣬽�Ǯ�����������",_parent->ID.id);
				money = _money_capacity;
			}
			_player_money = money;
			GLog::log(GLOG_INFO,"�û�%d�ⲿ���׳�ʱ�󣬽�Ǯ�ı� �%d" ,_parent->ID.id,money - GetMoney());

			//��ȡ���ݳɹ�
			//���������Լ�����Ʒ��
			_inventory.Clear();
			_inventory.InitFromDBData(*pInv);
			PlayerGetInventoryDetail(0);
			_db_timestamp = timestamp;
		}
		else
		{
			GLog::log(GLOG_ERR,"�û�%d�ⲿ���׽�������ʱ����С%d(%d)",_parent->ID.id,timestamp,_db_timestamp);
			//��������ʱ�����С����ʹ�� �������Լ�������
		}

		//�ص�ͨ��״̬
		FromFactionTradeToNormal();
	}
	else
	{
		//��ȡ����ʧ�� 
		GLog::log(GLOG_ERR,"�û�%d�ⲿ������ɺ�ȡ����ʧ��",_parent->ID.id);
		
		//����һ�ηǰ�������
		faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
		if(pTrade)
		{
			user_save_data((gplayer*)_parent,NULL,2,pTrade->_put_mask);
		}
		else
		{
			ASSERT(false);
		}
		
		//���Ҳ�����д�̲���
		FromFactionTradeToNormal(-1);
	}
}

void 
gplayer_imp::FactionTradeComplete(int trade_id,const GNET::syncdata_t & data)
{
	if(!_pstate.IsWaitFactionTradeState())
	{
		//δ�ں��ʵ�״̬��
		return;
	}
	GLog::log(GLOG_INFO,"�û�%d���ɲ�����ɣ���Ǯ�ı�Ϊ%d",_parent->ID.id, data.money - GetMoney());

	//��������
	ASSERT(_trade_obj != NULL);
	ASSERT(data.money >=0 && data.money <= _money_capacity);
	
	if(trade_id != _trade_obj->GetTradeID()) return ;

	//���ͳɹ�����
	//֪ͨ�ͻ�����ɴ˲��������Խ��в���
	_player_money = data.money;
	
	SetRefreshState();
	//GSMV_SEND FACTIONTRADECOMPELETE
	//��ý��д��̲���
	
	FromFactionTradeToNormal();
	return;
}

void 
gplayer_imp::SyncTradeComplete(int trade_id,size_t money, const GDB::itemlist & item_change,bool writetrashbox, bool money_change, bool cash_change)
{
	if(!_pstate.IsWaitFactionTradeState())
	{
		//δ�ں��ʵ�״̬��
		return;
	}
	//��������
	ASSERT(_trade_obj != NULL);
	if(money_change)
	{
		ASSERT(money >=0 && money <= _money_capacity);
	}
	else
	{
		money = GetMoney();
	}
	
	GLog::log(GLOG_INFO,"�û�%d�ⲿ������ɣ���Ǯ�ı�Ϊ%d",_parent->ID.id, money - GetMoney());

	if(trade_id != _trade_obj->GetTradeID()) return ;

	//���ͳɹ�����
	//֪ͨ�ͻ�����ɴ˲��������Խ��в���
	_player_money = money;

	//���а��������޸�
	for(size_t i = 0; i < item_change.count; i ++)
	{
		GDB::itemdata * pData = item_change.list + i;
		if(pData->count)
		{
			item & it = _inventory[pData->index];
			//���²���
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
					GLog::log(LOG_ERR, "SyncTradeCompleteʱ������Ʒʧ��%d",it2.type);
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
			//ɾ������
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

	//�ۼ۷�Χ��⣬���ܵ���10�����ܸ���10w, ��λRMBԪ
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

	//������۶��󣬽��뽻�׵ȴ�״̬
	_consign_obj = new player_consign;
	_pstate.WaitTrade();
	_consign_obj->SetTimeOut(30); //����30����̳�ʱ����

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
				//�������һ�δ��̵İٱ�����ˮ��
				pImp->_mallinfo.SaveDone(_cur_order_id);

				pImp->_db_save_error = 0;
			}
			pImp->WaitingConsignWriteBack(success, _type, _margin, _item_id, _item_cnt, _item_idx, _item_type, _money, _price, _sold_time, _sellto_name, _sellto_name_len);
		}
	};

	//���д��̲���
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
		//������̵ȴ�״̬
		_pstate.WaitTradeRead();
		//��������������ó�ʱ
		_consign_obj->SetTimeOut(45);
		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack((gplayer*)_parent,_plane));
	}
	else
	{
		//����Ҫ���¶��̣�ֱ�ӷ���normal״̬
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
		//���һ�²ֿ�ı��¼
		TryClearTBChangeCounter();

		//д�����ݳɹ�
		//����ͬ�⽻�׵�����
		GMSV::SendConsignRequest(pPlayer->ID.id, type, item_id, item_idx, item_cnt, item_type, money, sold_time, price, margin, sellto_name, sellto_name_len);
		GLog::log(GLOG_INFO,"��Ҽ��ۿ�ʼ �û�%d,  ����%d, ��֤��%d, ��Ʒid:%d, ����%d, ������λ��%d, ��Ʒ��������%d, ���۽�Ǯ%d, ���ۼ۸�%d(RMBԪ), ����ʱ��%d(��)", pPlayer->ID.id, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time);

		//���뽻��״̬
		_pstate.Trade();
	}
	else
	{
		//�ص��ǽ���״̬
		FromConsignToNormal();
	}
}

void gplayer_imp::WaitingConsignReadBack(const GDB::itemlist * pInv,int money)
{
	if(!_pstate.IsWaitTradeReadState()) return;
	if(pInv)
	{
		GLog::log(GLOG_INFO,"�û�%d���۳ɹ������۽�Ǯ�ı�Ϊ%d",_parent->ID.id, money - GetMoney());
		//��ȡ���ݳɹ�
		//���������Լ�����Ʒ��
		_inventory.Clear();
		_inventory.InitFromDBData(*pInv);
		if(((size_t)money) > _money_capacity)
		{
			GLog::log(GLOG_ERR,"�û�%d���ۼ�����Ǯ�����������",_parent->ID.id);
			money = _money_capacity;
		}
		_player_money = money;

		//�ص�ͨ��״̬
		FromConsignToNormal();
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d������ɺ�ȡ����ʧ��",_parent->ID.id);
		//��ȡ����ʧ�� ֱ�������Ǹ�������
		//���Ҳ�����д�̲���
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

		//Ȼ��������߲����������д���
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}

	//������ҵ�ǰ�İ���������
	PlayerGetInventoryDetail(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//Ȼ��ɾ�����󣬻ص�����״̬
	delete _consign_obj;
	_consign_obj = NULL;

	//Ȼ��ص�ͨ��״̬ ����֪ͨplayer���ڴ��ڷǽ���״̬
	//����״̬�Ѿ�����������
	_pstate.Normalize();
	_write_timer = 512;

	if(_parent->b_disconnect)
	{
		//����Ѿ����ߣ�ִ�ж����߼�  ���Ҵ���
		//��������߼�
		_pstate.Disconnect();
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
}

//��ɫ����
int gplayer_imp::CanConsignRole(char type, int item_idx, int sold_time, int price, int margin)
{
	if(!_pstate.CanTrade()) return -1;
	if(_cur_session || _session_list.size()) return -2;
	if(4 != type) return -3; //������ֻ�����ڼ��۽�ɫ

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_CONSIGN_IN_COMBAT);
		return -1;
	}
	
	//�ۼ۷�Χ��⣬���ܵ���60�����ܸ���100w, ��λRMBԪ
	if(price < 60 || price > 1000000) return -4;
	if(sold_time < 1 || sold_time > 7) return -5;
	if(margin < 0 || margin > 2000000000) return -6;

	//���ȼ�
	if(GetRebornCount() <= 0 || GetObjectLevel() < CONSIGN_ROLE_REQUIRED_LEVEL)
	{
		_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -14;
	}

	//�������Ƿ��ں���״̬
	if(GetPKValue() > 0)
	{
		_runner->error_message(S2C::ERR_CONSIGN_ROLE_REDNAME);
		return -15;
	}

	//�������Ƿ����ٻ��ޣ�����ķ�����Ѫ��
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

	GLog::log(GLOG_INFO,"��Ҽ��۽�ɫ��ʼ �û�%d, ����%d, ��֤��%d, �۳���Ʒid:%d, ����%d, ������λ��%d, ���ۼ۸�%d(RMBԪ), ����ʱ��%d(��)", pPlayer->ID.id, type, margin, item_id, item_cnt, item_idx, price, sold_time);
}

bool gplayer_imp::CheckToSendConsignRoleInfo(int result)
{
	if(!IsConsignRole())
	{
		return false;
	}

	if(CONSIGN_ROLE_SUCCESS == result)
	{
		//�������buff
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

	//ȥ�����ϵı�־
	SetConsignRoleFlag(false);
	//֪ͨ�ͻ����������
	_runner->error_message(S2C::ERR_CONSIGN_ROLE_LOGOUT);
	GLog::log(GLOG_INFO, "��ҽ�ɫ����offline_re���ͣ�result=%d", result);

	return true;
}

