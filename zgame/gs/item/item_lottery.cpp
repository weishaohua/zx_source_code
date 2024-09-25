#include "item_lottery.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"
#include "../item_manager.h"

int
item_lottery::CheckScript(int id, int n)
{
	if(n <= 0 || n > MAX_DICE_COUNT) return 1;
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_LOTTERY);
	lua_State * L = keeper.GetState();
	if(L == NULL) return 1;

	item_lottery l(n);
	l.SetTID(id);
	char tickets[MAX_DICE_COUNT];
	memset(tickets,0,sizeof(tickets));
	for(int i = 0; i < n;i ++)
	{
		if(!l.GenerateTicket(L, tickets, i)) return -1;
	}

	int m[6];
	if(!l.GetLotteryBonus(L, tickets, m)) return -2;
	return 0;
}

int
item_lottery::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_LOTTERY))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}

	bool bModify = false;
	bool bError = false;
	size_t len;
	void * buf = parent->GetContent(len);
	if((int)len != _number_limit)
	{
		//���Ȳ�����Ҫ��������֮ ���ֿ�����ֻ��1/_number_limit ����һ���õ�ʱ��
		char  content[MAX_DICE_COUNT];
		memset(content,0,sizeof(content));
		if(len == 0)
		{
			//�����ݣ����������Ӧ������
		}
		else if((int)len < _number_limit)
		{
			//�������� ֻ�����ǲ߻��޸������� �����Ĳ�����0
			memcpy(content, buf, len);
		}
		else 
		{
			//���࣬ ֻ�����ǲ߻��޸������� ����Ĳ��ֽض�
			memcpy(content, buf, _number_limit);
		}
		//�����������ݲ�����ȡ��
		parent->SetContent(content, _number_limit);
		buf = parent->GetContent(len);
		bModify = true;
	}
	ASSERT( (int)len == _number_limit);
	
	//�ҵ����һ��lottery
	int last_index = 0;
	char * tickets = (char*)buf;
	for(; last_index < _number_limit && tickets[last_index]; last_index ++) 
	{}
	
	while(last_index < _number_limit)
	{
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_LOTTERY);
		lua_State * L = keeper.GetState();
		ASSERT(L);	//֮ǰӦ��������
		//����Ҫ�ٴ�����
		if(GenerateTicket(L,tickets, last_index))
		{
			//���ɳɹ�
			last_index ++;
			bModify = true;
		}
		else
		{
			bError = true;
			break;
		}
	}
	int cooltime = LOTTERY_COOLDOWN_TIME;
	if(!bError) 
		cooltime = 0;
	else
		cooltime += 15000;
	if(cooltime) pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,cooltime);
	if(bModify)
	{
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
	}

	// ���»�Ծ��[����¯-��Ʊ]
	if (item_manager::IsCashItem((size_t)_tid)) {
		pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_LIANDANLU);
	}
	return 0;
}

bool 
item_lottery::LotteryCashing(gactive_imp* obj, item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period)
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_LOTTERY))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return false;
	}

	size_t len;
	void * buf = parent->GetContent(len);
	if((int)len != _number_limit)
	{
		return false;
	}
	
	pImp->SetCoolDown(COOLDOWN_INDEX_LOTTERY,LOTTERY_COOLDOWN_TIME);

	//�ҵ����һ��lottery
	int last_index = 0;
	char * tickets = (char*)buf;
	for(; last_index < _number_limit && tickets[last_index]; last_index ++) 
	{}
	
	if(last_index !=  _number_limit)
	{
		return false;
	}
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_LOTTERY);
	lua_State * L = keeper.GetState();
	ASSERT(L);	//֮ǰӦ��������

	//���ýű�����
	int result[6];
	if(!GetLotteryBonus(L,tickets,result))
	{
		return false;
	}


	//���ò���
	*bonus_level	= result[0];
	*prize_money	= result[1];
	*prize_id	= result[2];
	*prize_count	= result[3];
	*prize_bind 	= result[4];
	*prize_period	= result[5];

	return true;
}

bool 
item_lottery::GenerateTicket(lua_State * L, char * tickets, int index) const 
{
	lua_getfield(L, LUA_GLOBALSINDEX, "Entry_Lottery_Generate");
	lua_pushinteger(L, _tid);
	lua_pushinteger(L, _number_limit);
	lua_pushinteger(L, index);
	for(int i = 0; i < index; i ++)
	{
		lua_pushinteger(L, tickets[i]);
	}
	if(lua_pcall(L, index + 3, 1,0)) 
	{
		printf("error GenerateTicket %s\n", lua_tostring(L, -1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1)) return false;
	int rst = lua_tointeger(L,-1);
	lua_pop(L,1);
	tickets[index] = rst & 0xFF;
	return true;
}

bool 
item_lottery::GetLotteryBonus(lua_State * L,char * tickets,int n[6]) const 
{
	lua_getfield(L, LUA_GLOBALSINDEX, "Entry_Lottery_Cashing");
	lua_pushinteger(L, _tid);
	for(int i = 0; i < _number_limit; i ++)
	{
		lua_pushinteger(L, tickets[i]);
	}
	if(lua_pcall(L, _number_limit + 1, 6,0)) 
	{
		printf("error GetLotteryBonus %s\n", lua_tostring(L, -1));
		lua_pop(L,1);
		return false;
	}
	for(int i = -6; i <= -1; i ++)
	{
		if(!lua_isnumber(L, i)) return false;
		n[i+6] = lua_tointeger(L, i);
	}
	lua_pop(L,6);

	//���д�����
	if(!CheckBonusLevel(n[0])) return false;
	if(!CheckBonusMoney(n[1])) return false;
	if(!CheckBonusItem(n[2],n[3])) return false;
	if(!CheckBonusBindPeriod(n[4],n[5])) return false;
	return true;
}

