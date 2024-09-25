#include "item_petbedge.h"
#include "../clstab.h"
#include "../player_imp.h"

void pet_bedge_essence::UpdateContent(item* parent,bool force)
{
	if(!_dirty_flag && !force) return;
	packet_wrapper h1(2048);
	//����essence
	h1.push_back(&_enhanced_essence,sizeof(_enhanced_essence));
	//����param inner data
	size_t size = _inner_data.size() * sizeof(float);
	h1 << size;
	if(size > 0)
	{
		h1.push_back(&_inner_data[0],size);
	}
	//����������
	parent->SetContent(h1.data(), h1.size());
	//�������û���
	_content= parent->GetContent(_size);
}

namespace
{
void* GetLuaPointer(lua_State* L,int n,const char* name)
{
	lua_pushstring(L,name);
	lua_gettable(L,-2 - n);
	void *p = lua_touserdata(L,-1);
	lua_pop(L,1);
	return p;
}

void* GetAndTest(lua_State* L ,int n,const char* name)
{
	void *p = GetLuaPointer(L,n, name);
	if(!p)
	{
		return NULL;
	}
	return p;
}

bool CheckArgNum(lua_State* L,int n)
{
	int nArg = lua_gettop(L);    /* number of arguments */
	if(nArg == n + 1) return true;
	printf("lua ���ò�������\n");
	return false;
}
}

#define RESTORE_PET_BEDGE_DATA(L,n) \
	if(!CheckArgNum(L,n)) return 0;\
	item_pet_bedge* pItem = (item_pet_bedge*)GetAndTest(L,n,"__ITEM");\
	(void)(pItem);\
	pet_bedge_essence * pEss = (pet_bedge_essence*)GetAndTest(L,n,"__ESS");\
	if(!pEss) return 0;

int pet_bedge_essence::script_QueryInnerData(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushnumber(L,pEss->QueryInnerData(index));
	return 1;
}

int pet_bedge_essence::script_GetInnerDataCount(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetInnerDataCount());
	return 1;
}

int pet_bedge_essence::script_SetInnerData(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	float value = lua_tonumber(L,-1);
	pEss->SetInnerData(index,value);
	return 0;
}

int pet_bedge_essence::script_ClearInnerData(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	pEss->ClearInnerDara();
	return 0;
}

int pet_bedge_essence::script_QueryLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetLevel());
	return 1;
}

int pet_bedge_essence::script_SetLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetLevel(value);
	return 0;
}

int pet_bedge_essence::script_QueryExp(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetExp());
	return 1;
}

int pet_bedge_essence::script_SetExp(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetExp(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurHP(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurHP());
	return 1;
}

int pet_bedge_essence::script_SetCurHP(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurHP(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurVigor(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurVigor());
	return 1;
}

int pet_bedge_essence::script_SetCurVigor(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurVigor(value);
	return 0;
}

int pet_bedge_essence::script_QueryStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetStar());
	return 1;
}

int pet_bedge_essence::script_SetStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetStar(value);
	return 0;
}

int pet_bedge_essence::script_QueryRebornStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetRebornStar());
	return 1;
}

int pet_bedge_essence::script_SetRebornStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetRebornStar(value);
	return 0;
}

int pet_bedge_essence::script_QueryMainType(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetMainType());
	return 1;
}

int pet_bedge_essence::script_SetMainType(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetMainType(value);
	return 0;
}

int pet_bedge_essence::script_QueryElement(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetElement());
	return 1;
}

int pet_bedge_essence::script_SetElement(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetElement(value);
	return 0;
}

int pet_bedge_essence::script_QueryIdentify(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetIdentify());
	return 1;
}

int pet_bedge_essence::script_SetIdentify(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetIdentify(value);
	return 0;
}

int pet_bedge_essence::script_QueryRace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetRace());
	return 1;
}

int pet_bedge_essence::script_SetRace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetRace(value);
	return 0;
}

int pet_bedge_essence::script_QueryHoroscope(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetHoroscope());
	return 1;
}

int pet_bedge_essence::script_SetHoroscope(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetHoroscope(value);
	return 0;
}

int pet_bedge_essence::script_QueryCombatAttrInit(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetCombatAttrInit(index));
	return 1;
}

int pet_bedge_essence::script_SetCombatAttrInit(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetCombatAttrInit(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryCombatAttrPotential(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushnumber(L,pEss->GetCombatAttrPotential(index));
	return 1;
}

int pet_bedge_essence::script_SetCombatAttrPotential(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetCombatAttrPotential(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryCombatAttrCur(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetCombatAttrCur(index));
	return 1;
}

int pet_bedge_essence::script_SetCombatAttrCur(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetCombatAttrCur(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryRebornPropAdd(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushnumber(L,pEss->GetRebornPropAdd(index));
	return 1;
}

int pet_bedge_essence::script_SetRebornPropAdd(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	float value = lua_tonumber(L,-1);
	pEss->SetRebornPropAdd(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryExtraDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetExtraDamage());
	return 1;
}

int pet_bedge_essence::script_SetExtraDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetExtraDamage(value);
	return 0;
}

int pet_bedge_essence::script_QueryExtraDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetExtraDamageReduce());
	return 1;
}

int pet_bedge_essence::script_SetExtraDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetExtraDamageReduce(value);
	return 0;
}

int pet_bedge_essence::script_QueryCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetCritRate());
	return 1;
}

int pet_bedge_essence::script_SetCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetCritRate(value);
	return 0;
}

int pet_bedge_essence::script_QueryCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetCritDamage());
	return 1;
}

int pet_bedge_essence::script_SetCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetCritDamage(value);
	return 0;
}

int pet_bedge_essence::script_QueryAntiCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetAntiCritRate());
	return 1;
}

int pet_bedge_essence::script_SetAntiCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetAntiCritRate(value);
	return 0;
}

int pet_bedge_essence::script_QueryAntiCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetAntiCritDamage());
	return 1;
}

int pet_bedge_essence::script_SetAntiCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetAntiCritDamage(value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillAttackRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetSkillAttackRate());
	return 1;
}

int pet_bedge_essence::script_SetSkillAttackRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetSkillAttackRate(value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillArmorRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetSkillArmorRate());
	return 1;
}

int pet_bedge_essence::script_SetSkillArmorRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetSkillArmorRate(value);
	return 0;
}

int pet_bedge_essence::script_QueryDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetDamageReduce());
	return 1;
}

int pet_bedge_essence::script_SetDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetDamageReduce(value);
	return 0;
}

int pet_bedge_essence::script_QueryDamageReducePercent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetDamageReducePercent());
	return 1;
}

int pet_bedge_essence::script_SetDamageReducePercent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetDamageReducePercent(value);
	return 0;
}

int pet_bedge_essence::script_QueryMaxHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetMaxHungerPoint());
	return 1;
}

int pet_bedge_essence::script_SetMaxHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetMaxHungerPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurHungerPoint());
	return 1;
}

int pet_bedge_essence::script_SetCurHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurHungerPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryMaxHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetMaxHonorPoint());
	return 1;
}

int pet_bedge_essence::script_SetMaxHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetMaxHonorPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurHonorPoint());
	return 1;
}

int pet_bedge_essence::script_SetCurHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurHonorPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryAge(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetAge());
	return 1;
}

int pet_bedge_essence::script_SetAge(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetAge(value);
	return 0;
}

int pet_bedge_essence::script_QueryLife(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetLife());
	return 1;
}

int pet_bedge_essence::script_SetLife(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetLife(value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillID(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetSkillID(index));
	return 1;
}

int pet_bedge_essence::script_SetSkillID(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetSkillID(index,value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetSkillLevel(index));
	return 1;
}

int pet_bedge_essence::script_SetSkillLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetSkillLevel(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryTalent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetTalent(index));
	return 1;
}

int pet_bedge_essence::script_SetTalent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetTalent(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryAcuity(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetAcuity());
	return 1;
}

int pet_bedge_essence::script_SetAcuity(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetAcuity(value);
	return 0;
}

int pet_bedge_essence::script_QueryStamina(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetStamina());
	return 1;
}

int pet_bedge_essence::script_SetStamina(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetStamina(value);
	return 0;
}

int pet_bedge_essence::script_QuerySawy(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetSawy());
	return 1;
}

int pet_bedge_essence::script_SetSawy(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetSawy(value);
	return 0;
}

int pet_bedge_essence::script_QueryDowry(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetDowry());
	return 1;
}

int pet_bedge_essence::script_SetDowry(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetDowry(value);
	return 0;
}

int pet_bedge_essence::script_QueryWisdom(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetWisdom());
	return 1;
}

int pet_bedge_essence::script_SetWisdom(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetWisdom(value);
	return 0;
}

int pet_bedge_essence::script_QueryCNR(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCNR());
	return 1;
}

int pet_bedge_essence::script_SetCNR(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCNR(value);
	return 0;
}

int pet_bedge_essence::script_QueryFace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetFace());
	return 1;
}

int pet_bedge_essence::script_SetFace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetFace(value);
	return 0;
}

int pet_bedge_essence::script_QueryShape(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetShape());
	return 1;
}

int pet_bedge_essence::script_SetShape(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetShape(value);
	return 0;
}

int pet_bedge_essence::script_QueryRebornCnt(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetRebornCnt());
	return 1;
}

int pet_bedge_essence::script_SetRebornCnt(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetRebornCnt(value);
	return 0;
}

void item_pet_bedge::InitFromShop(gplayer_imp* pImp,item* parent,int origin) const
{
	size_t len;
	void* buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� InitFromShop NULL == L(_tid: %d)",_tid);
		__PRINTF("������Ϣ(����) ִ�нű����� InitFromShop NULL == L(_tid: %d)\n",_tid);
		return;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Init_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushinteger(L,origin);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,pImp->_basic.level);
	__PRINTF("level : %d\n",pImp->_basic.level);
	if(lua_pcall(L,5,1,0))
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� InitFromShop lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("������Ϣ(����) ִ�нű����� InitFromShop lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return;
	}
	if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� InitFromShop ����ֵ���ݴ���(_tid: %d)",_tid);
		__PRINTF("������Ϣ(����) ִ�нű����� InitFromShop ����ֵ���ݴ���(_tid: %d)\n",_tid);
		lua_pop(L,1);
		return;
	}
	int ret = lua_tointeger(L,-1);
	lua_pop(L,1);
	if(_tid != ret)
	{
		GLog::log(LOG_INFO,"������Ϣ(����): ����������ʧ��(_tid: %d,pet: %d)",_tid,ret);
		__PRINTF("������Ϣ(����): ����������ʧ��(_tid: %d,pet: %d)\n",_tid,ret);
		return;
	}
	if(ess.GetCurHP() <= 0) ess.SetCurHP(1);
	if(ess.GetCurHP() > ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP))
	{
		ess.SetCurHP(ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP));
	}
	ess.SetOrigin(origin);
	ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
	ess.UpdateContent(parent,true);
}

int item_pet_bedge::AdoptPet(gplayer_imp* imp,int inv_index)
{
	if(inv_index < 0 || (size_t)inv_index >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	/*
	if(ess.GetLevel() > imp->GetRebornCount() * 30 + imp->_basic.level)
	{
		//���𲻷���
		return S2C::ERR_PET_LEVEL_TO_HIGH;
	}
	*/
	if(imp->_parent->ID.id == ess.GetOwnerID())
	{
		//�Ѿ���������
		return S2C::ERR_PET_ALREADY_ADOPTED;
	}
	//�ű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� AdoptPet NULL == L(_tid: %d)",_tid);
		__PRINTF("������Ϣ(����) ִ�нű����� AdoptPet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Adopt_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushinteger(L,ess.GetOrigin());
	lua_pushlightuserdata(L,&ess);
	if(lua_pcall(L,4,1,0))
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� AdoptPet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("������Ϣ(����) ִ�нű����� AdoptPet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� AdoptPet ����ֵ���ݴ���(_tid: %d)",_tid);
		__PRINTF("������Ϣ(����) ִ�нű����� AdoptPet ����ֵ���ݴ���(_tid: %d)\n",_tid);
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	int ret = lua_tointeger(L,-1);
	lua_pop(L,1);
	if(_tid != ret)
	{
		GLog::log(LOG_INFO,"������Ϣ(����): ����������ʧ��(_tid: %d,pet: %d)",_tid,ret);
		__PRINTF("������Ϣ(����): ����������ʧ��(_tid: %d,pet: %d)\n",_tid,ret);
		return S2C::ERR_FATAL_ERR;
	}
	//��Ǯ
	imp->DecMoneyAmount((size_t)g_config.fee_adopt_pet);
	if (g_config.fee_adopt_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_adopt_pet,it.type);
	}
	//���ó�ʼ���ܶ�
	//������id
	ess.SetOwnerID(((gplayer*)(imp->_parent))->ID.id);
	ess.SetRank(0);
	ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
	const void* pname = imp->GetPlayerName(len);
	if(len > pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH) len = pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH;
	ess.SetOwnerName(pname,len);
	ess.UpdateContent(&it);
	//֪ͨ�ͻ����µ�����
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index);
	GLog::log(LOG_INFO,"������Ϣ(����): ��� %d ����˳�����������(pet_tid: %d)",imp->_parent->ID.id,it.type);
	__PRINTF("������Ϣ(����): ��� %d ����˳�����������(pet_tid: %d)\n",imp->_parent->ID.id,it.type);

	// ���³��������Ծ��
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

int item_pet_bedge::FreePet(gplayer_imp* imp,int inv_index)
{
	if(inv_index < 0 || (size_t)inv_index >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//����δ���������Բ���
		return S2C::ERR_PET_NOT_ADOPTED;
	}
	if(0 == imp->GetInventory().GetEmptySlotCount())
	{
		//������Ҫ���°�����û�пռ�
		return S2C::ERR_INVENTORY_IS_FULL;
	}
	//������Ҫ���ýű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� FreePet NULL == L(_tid: %d)",_tid);
		__PRINTF("������Ϣ(����) ִ�нű����� FreePet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Free_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	if(lua_pcall(L,3,5,0))
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� FreePet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("������Ϣ(����) ִ�нű����� FreePet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3) || !lua_isnumber(L,-4) || !lua_isnumber(L,-5))
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ִ�нű����� FreePet ����ֵ���ݴ���(_tid: %d)",_tid);
		__PRINTF("������Ϣ(����) ִ�нű����� FreePet ����ֵ���ݴ���(_tid: %d)\n",_tid);
		lua_pop(L,5);
		return S2C::ERR_FATAL_ERR;
	}
	//ȡ�÷���ֵ
	int item_count = lua_tointeger(L,-1);
	int item_tid = lua_tointeger(L,-2);
	int msg_id = lua_tointeger(L,-3);
	int channel_id = lua_tointeger(L,-4);
	int pet_tid = lua_tointeger(L,-5);
	lua_pop(L,5);
	if(pet_tid != _tid)
	{
		__PRINTF("������Ϣ(����) ʧ��(pet_tid: %d,channel_id: %d,msg_id: %d,item_tid: %d,item_count: %d)",
			pet_tid,channel_id,msg_id,item_tid,item_count);
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return 0;
	}
	if(0 == item_count || 0 == item_tid)
	{
		GLog::log(LOG_ERR,"������Ϣ(����) ʧ��(pet_tid: %d,channel_id: %d,msg_id: %d,item_tid: %d,item_count: %d)",
			pet_tid,channel_id,msg_id,item_tid,item_count);
		__PRINTF("������Ϣ(����) ʧ��(pet_tid: %d,channel_id: %d,msg_id: %d,item_tid: %d,item_count: %d)\n",
			pet_tid,channel_id,msg_id,item_tid,item_count);
		return S2C::ERR_FATAL_ERR;
	}
	//�ж�item_tid�ǲ��ǳ���������ҩ����
	DATA_TYPE dt2;
	const PET_REFINE_ESSENCE& ess2 = *(const PET_REFINE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_tid,ID_SPACE_ESSENCE,dt2);
	if(DT_PET_REFINE_ESSENCE != dt2 || NULL == &ess2)
	{
		GLog::log(LOG_ERR,"������Ϣ(����): ��� %d ����tid: %d,�����Ʒtid: %d ����,���ǳ���������ҩ����",imp->_parent->ID.id,pet_tid,item_tid);
		__PRINTF("������Ϣ(����): ��� %d ����tid: %d,�����Ʒtid: %d ����,���ǳ���������ҩ����\n",imp->_parent->ID.id,pet_tid,item_tid);
		return S2C::ERR_FATAL_ERR;
	}
	//��Ǯ
	imp->DecMoneyAmount((size_t)g_config.fee_free_pet);
	if (g_config.fee_free_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_free_pet,it.type);
	}
	//�Ӷ���
	imp->UseItemLog(it,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index,pet_tid,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index,1);
	//������
	const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_tid);
	if(!pItem || pItem->pile_limit <= 0) return S2C::ERR_FATAL_ERR;
	item_data * data = DupeItem(*pItem);
	if((size_t)item_count > data->pile_limit) item_count = data->pile_limit;
	data->count = item_count;
	if(imp->ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
	GLog::log(LOG_INFO,"������Ϣ(����): ��� %d ����˳����������(pet_tid: %d,item_tid: %d,item_count: %d)",
		imp->_parent->ID.id,it.type,item_tid,item_count);
	__PRINTF("������Ϣ(����): ��� %d ����˳����������(pet_tid: %d,item_tid: %d,item_count: %d)\n",
		imp->_parent->ID.id,it.type,item_tid,item_count);
	//�ɹ���ҲҪ���߿ͻ��˸���Ϣ
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);

	// ���³��������Ծ��
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

int item_pet_bedge::RefinePet(gplayer_imp* imp,int inv_index_pet,int inv_index_c,int inv_index_a)
{
	if(inv_index_pet < 0 || (size_t)inv_index_pet >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	if(inv_index_c < 0 || (size_t)inv_index_c >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index_pet];
	bool is_lock = it.IsLocked();
	//�����ж�pet_tid;
	DATA_TYPE dt2;
	const PET_BEDGE_ESSENCE& ess2 = *(const PET_BEDGE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt2);
	if(DT_PET_BEDGE_ESSENCE != dt2 || NULL == &ess2)
	{
		__PRINTF("������Ϣ(�ϳ�): ��� %d pet_tid: %d ���ʹ���,���ǳ���������\n",imp->_parent->ID.id,it.type);
		return S2C::ERR_FATAL_ERR;
	}
	int pet_tid_in = it.type;
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//����δ���������Բ���
		return S2C::ERR_PET_NOT_ADOPTED;
	}
	item& it_c = imp->GetInventory()[inv_index_c];
	//�����ж�cid
	DATA_TYPE dt3;
	const PET_REFINE_ESSENCE& ess3 = *(const PET_REFINE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it_c.type,ID_SPACE_ESSENCE,dt3);
	if(DT_PET_REFINE_ESSENCE != dt3 || NULL == &ess3)
	{
		__PRINTF("������Ϣ(�ϳ�): ��� %d cid: %d ���ʹ���,���ǳ���������ҩ����\n",imp->_parent->ID.id,it_c.type);
		return S2C::ERR_FATAL_ERR;
	}
	int cid = it_c.type;
	//�����aid�ж�aid
	int aid = 0;
	if(-1 != inv_index_a)
	{
		if(inv_index_a < 0 || (size_t)inv_index_a >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
		item& it_a = imp->GetInventory()[inv_index_a];
		DATA_TYPE dt4;
		const PET_ASSIST_REFINE_ESSENCE& ess4 = *(const PET_ASSIST_REFINE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it_a.type,ID_SPACE_ESSENCE,dt4);
		if(DT_PET_ASSIST_REFINE_ESSENCE != dt4 || NULL == &ess4)
		{
			__PRINTF("������Ϣ(�ϳ�): ��� %d aid: %d ���ʹ���,���ǳ����������������\n",imp->_parent->ID.id,it_a.type);
			return S2C::ERR_FATAL_ERR;
		}
		aid = it_a.type;
	}
	pet_bedge_essence output(NULL,0);
	output.PrepareData();
	output.SetOwnerID(ess.GetOwnerID());
	size_t name_len;
	const void* name_buf = ess.GetName(name_len);
	output.SetName(name_buf,name_len);
	name_buf = ess.GetOwnerName(name_len);
	output.SetOwnerName(name_buf,name_len);
	//������Ҫ���ýű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"������Ϣ(�ϳ�) ִ�нű����� RefinePet NULL == L(_tid: %d)",_tid);
		__PRINTF("������Ϣ(�ϳ�) ִ�нű����� RefinePet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Combine_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushinteger(L,cid);
	lua_pushinteger(L,aid);
	lua_pushlightuserdata(L,&ess);
	lua_pushlightuserdata(L,&output);
	lua_pushinteger(L,imp->GetCultivation());
	if(lua_pcall(L,6,3,0))
	{
		GLog::log(LOG_ERR,"������Ϣ(�ϳ�) ִ�нű����� RefinePet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("������Ϣ(�ϳ�) ִ�нű����� RefinePet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3))
	{
		GLog::log(LOG_ERR,"������Ϣ() ִ�нű����� RefinePet ����ֵ���ݴ���(_tid: %d)",_tid);
		__PRINTF("������Ϣ(�ϳ�) ִ�нű����� RefinePet ����ֵ���ݴ���(_tid: %d)\n",_tid);
		lua_pop(L,3);
		return S2C::ERR_FATAL_ERR;
	}
	
	object_interface oif(imp);
	imp->GetAchievementManager().OnRefinePet(oif, ess.GetStar());

	//ȡ�÷���ֵ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid_out = lua_tointeger(L,-3);
	lua_pop(L,3);
	//�����pet���ɽ��
	item outitem;
	if(pet_tid_out > 0)
	{
		const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(pet_tid_out);
		if(!pItem) return S2C::ERR_FATAL_ERR;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem,*pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem,*pItem);
		}
	        DATA_TYPE dt2;
	        const void* no_use = gmatrix::GetDataMan().get_data_ptr(pet_tid_out,ID_SPACE_ESSENCE,dt2);
		if(DT_PET_BEDGE_ESSENCE!= dt2 || NULL == no_use)
		{
			//���ɵ���Ʒ���ǳ������� ���⴦��
		}
		else
		{
			if(output.IsDirty())
			{
				if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
				//������Ʒ
				output.SetOrigin(ITEM_INIT_TYPE_COMBINE);
				if(ess.GetCurHP() <= 0) ess.SetCurHP(1);
				if(ess.GetCurHP() > ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP))
				{
					ess.SetCurHP(ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP));
				}
				output.UpdateContent(&outitem);
			}
			else
			{
				//�����petbedge�ͱ���,��Ϊû��д����Ʒ(�ű�δ�����κβ���)
				if(outitem.GetItemType() == item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
			}
		}
	}
	else
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return 0;
	}
	//��Ǯ
	imp->DecMoneyAmount((size_t)g_config.fee_refine_pet);
	if (g_config.fee_refine_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_refine_pet,it.type);
	}
	//�۶���
	imp->UseItemLog(it,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_pet,it.type,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index_pet,1);
	//����Ƿ���Ҫ��¼����ֵ
	imp->CheckSpecialConsumption(it_c.type, 1);
	imp->UseItemLog(it_c,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_c,cid,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index_c,1);
	if(-1 != inv_index_a)
	{
		item& it_a = imp->GetInventory()[inv_index_a];
		imp->UseItemLog(it_a,1);
		imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_a,aid,1,S2C::DROP_TYPE_USE);
		imp->GetInventory().DecAmount(inv_index_a,1);
	}
	//���������¶���
	if(-1 != outitem.type)
	{
		//�������ɵĳ����Ʒ���ԭ���ĵط�����ͻ�������������
//		int rst = imp->GetInventory().Push(outitem);
		imp->GetInventory().Put(inv_index_pet, outitem);
		if(is_lock)
		{
			imp->GetInventory()[inv_index_pet].Lock();
		}
		int state = item::Proctype2State(outitem.proc_type);
		imp->_runner->put_item(pet_tid_out,inv_index_pet,0,1,imp->GetInventory()[inv_index_pet].count,gplayer_imp::IL_INVENTORY,state);
	}
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	//֪ͨ�ͻ����µ�����
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index_pet);


	GLog::log(LOG_INFO,"������Ϣ(�ϳ�): ��� %d ����˳�����������(pet_tid_in: %d,cid: %d,aid: %d,pet_tid_out: %d,channel_id: %d,msg_id: %d",
		imp->_parent->ID.id,pet_tid_in,cid,aid,pet_tid_out,channel_id,msg_id);
	__PRINTF("������Ϣ(�ϳ�): ��� %d ����˳�����������(pet_tid_in: %d,cid: %d,aid: %d,pet_tid_out: %d,channel_id: %d,msg_id: %d\n",
		imp->_parent->ID.id,pet_tid_in,cid,aid,pet_tid_out,channel_id,msg_id);

	// ���³��������Ծ��
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

//ֱ��ʹ���ɶ����� (refinepet��ʹ���ɶ��һ�����)
int item_pet_bedge::RefinePet2(gplayer_imp* imp,int inv_index_pet,int type)
{
	if(inv_index_pet < 0 || (size_t)inv_index_pet >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	if(type < 0 || type >= 36) return S2C::ERR_FATAL_ERR;

	item& it = imp->GetInventory()[inv_index_pet];
	bool is_lock = it.IsLocked();
	//�����ж�pet_tid;
	DATA_TYPE dt2;
	const PET_BEDGE_ESSENCE& ess2 = *(const PET_BEDGE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt2);
	if(DT_PET_BEDGE_ESSENCE != dt2 || NULL == &ess2)
	{
		__PRINTF("������Ϣ(�ϳ�): ��� %d pet_tid: %d ���ʹ���,���ǳ���������\n",imp->_parent->ID.id,it.type);
		return S2C::ERR_FATAL_ERR;
	}
	int pet_tid_in = it.type;
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("������Ϣ: �û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//����δ���������Բ���
		return S2C::ERR_PET_NOT_ADOPTED;
	}

	int cid = 0;
	int aid = 0;	//ֱ��ʹ���ɶ��Ļ�aidΪ0
	int need_count = 0;
	if(!GetRefineInfoByType(type, cid, need_count)) return S2C::ERR_FATAL_ERR;
	if(imp->GetInventory().CountItemByID(18794) < need_count) return S2C::ERR_FATAL_ERR;

	pet_bedge_essence output(NULL,0);
	output.PrepareData();
	output.SetOwnerID(ess.GetOwnerID());
	size_t name_len;
	const void* name_buf = ess.GetName(name_len);
	output.SetName(name_buf,name_len);
	name_buf = ess.GetOwnerName(name_len);
	output.SetOwnerName(name_buf,name_len);
	//������Ҫ���ýű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"������Ϣ(�ϳ�) ִ�нű����� RefinePet NULL == L(_tid: %d)",_tid);
		__PRINTF("������Ϣ(�ϳ�) ִ�нű����� RefinePet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Combine_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushinteger(L,cid);
	lua_pushinteger(L,aid);
	lua_pushlightuserdata(L,&ess);
	lua_pushlightuserdata(L,&output);
	lua_pushinteger(L,imp->GetCultivation());
	if(lua_pcall(L,6,3,0))
	{
		GLog::log(LOG_ERR,"������Ϣ(�ϳ�) ִ�нű����� RefinePet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("������Ϣ(�ϳ�) ִ�нű����� RefinePet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3))
	{
		GLog::log(LOG_ERR,"������Ϣ() ִ�нű����� RefinePet ����ֵ���ݴ���(_tid: %d)",_tid);
		__PRINTF("������Ϣ(�ϳ�) ִ�нű����� RefinePet ����ֵ���ݴ���(_tid: %d)\n",_tid);
		if(type < 0 || type >= 36) return S2C::ERR_FATAL_ERR;

		lua_pop(L,3);
		return S2C::ERR_FATAL_ERR;
	}
	
	object_interface oif(imp);
	imp->GetAchievementManager().OnRefinePet(oif, ess.GetStar());

	//ȡ�÷���ֵ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid_out = lua_tointeger(L,-3);
	lua_pop(L,3);
	//�����pet���ɽ��
	item outitem;
	if(pet_tid_out > 0)
	{
		const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(pet_tid_out);
		if(!pItem) return S2C::ERR_FATAL_ERR;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem,*pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem,*pItem);
		}
	        DATA_TYPE dt2;
	        const void* no_use = gmatrix::GetDataMan().get_data_ptr(pet_tid_out,ID_SPACE_ESSENCE,dt2);
		if(DT_PET_BEDGE_ESSENCE!= dt2 || NULL == no_use)
		{
			//���ɵ���Ʒ���ǳ������� ���⴦��
		}
		else
		{
			if(output.IsDirty())
			{
				if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
				//������Ʒ
				output.SetOrigin(ITEM_INIT_TYPE_COMBINE);
				if(ess.GetCurHP() <= 0) ess.SetCurHP(1);
				if(ess.GetCurHP() > ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP))
				{
					ess.SetCurHP(ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP));
				}
				output.UpdateContent(&outitem);
			}
			else
			{
				//�����petbedge�ͱ���,��Ϊû��д����Ʒ(�ű�δ�����κβ���)
				if(outitem.GetItemType() == item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
			}
		}
	}
	else
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return 0;
	}
	//��Ǯ
	imp->DecMoneyAmount((size_t)g_config.fee_refine_pet);
	if (g_config.fee_refine_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_refine_pet,it.type);
	}
	//�۶���
	imp->UseItemLog(it,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_pet,it.type,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index_pet,1);

	imp->TakeOutItem(18794, need_count);

	//���������¶���
	if(-1 != outitem.type)
	{
		//�������ɵĳ����Ʒ���ԭ���ĵط�����ͻ�������������
//		int rst = imp->GetInventory().Push(outitem);
		imp->GetInventory().Put(inv_index_pet, outitem);
		if(is_lock)
		{
			imp->GetInventory()[inv_index_pet].Lock();
		}
		int state = item::Proctype2State(outitem.proc_type);
		imp->_runner->put_item(pet_tid_out,inv_index_pet,0,1,imp->GetInventory()[inv_index_pet].count,gplayer_imp::IL_INVENTORY,state);
	}
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	//֪ͨ�ͻ����µ�����
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index_pet);


	GLog::log(LOG_INFO,"������Ϣ(�ϳ�): ��� %d ����˳�����������(pet_tid_in: %d,cid: %d,aid: %d,pet_tid_out: %d,channel_id: %d,msg_id: %d, type: %d",
		imp->_parent->ID.id,pet_tid_in,cid,aid,pet_tid_out,channel_id,msg_id,type);

	// ���³��������Ծ��
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);

	return 0;
}

int item_pet_bedge::RenamePet(gplayer_imp* imp,int inv_index,const char* name_buf,int name_size)
{
	if(inv_index < 0 || (size_t)inv_index >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"�û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("�û� %d ��������Ʒ���ݴ��� item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//����δ���������Ը�����
		return S2C::ERR_PET_NOT_ADOPTED;
	}
	//��Ǯ
	imp->DecMoneyAmount((size_t)g_config.fee_rename_pet);
	if (g_config.fee_rename_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_rename_pet,it.type);
	}
	//������
	ess.SetName(name_buf,name_size);
	ess.UpdateContent(&it);
	//֪ͨ�ͻ����µ�����
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index);
	GLog::log(LOG_INFO,"������Ϣ(����): ��� %d ����˳����������(pet_tid: %d)",imp->_parent->ID.id,it.type);
	__PRINTF("������Ϣ(����): ��� %d ����˳����������(pet_tid: %d)\n",imp->_parent->ID.id,it.type);

	// ���³��������Ծ��
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

bool item_pet_bedge::RebornPet(gplayer_imp* imp, size_t inv_index_pet)
{
	item& it = imp->GetInventory()[inv_index_pet];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid()) return false;
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID()) return false;
	if(ess.GetRebornCnt() > 0) return false;
	if(ess.GetStar() < 12) return false;
	if(ess.GetLevel() < 150) return false;

	//������Ҫ���ýű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false; 
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Reborn_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	if(lua_pcall(L,3,3,0))
	{
		lua_pop(L,1);
		return false; 
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3)) 
	{
		lua_pop(L,3);
		return false; 
	}

	//ȡ�÷���ֵ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid = lua_tointeger(L,-3);

	lua_pop(L,3);
	if(pet_tid != _tid)
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return false;
	}
	ess.SetRebornCnt(1);
	ess.UpdateContent(&it);

	GLog::log(LOG_INFO,"������Ϣ(����) �): ��� %d ����˷�������, pet_id=%d", imp->_parent->ID.id,it.type);
	//�ɹ���ҲҪ���߿ͻ��˸���Ϣ
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, inv_index_pet);
	return true;
}

bool item_pet_bedge::ChangeShape(gplayer_imp* imp, size_t inv_index_pet, int shape_id)
{
	item& it = imp->GetPetBedgeInventory()[inv_index_pet];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid()) return false;
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID()) return false;
	if(ess.GetRebornCnt() <= 0) return false;

	//������Ҫ���ýű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false; 
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_ChangeShape_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,shape_id);
	if(lua_pcall(L,4,3,0))
	{
		lua_pop(L,1);
		return false; 
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3)) 
	{
		lua_pop(L,3);
		return false; 
	}

	//ȡ�÷���ֵ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid = lua_tointeger(L,-3);

	lua_pop(L,3);
	if(pet_tid != _tid)
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return false;
	}
	ess.UpdateContent(&it);

	GLog::log(LOG_INFO,"���%d�ı�������γɹ�: pet_id=%d, shape_id=%d", imp->_parent->ID.id, it.type, shape_id);
	//�ɹ���ҲҪ���߿ͻ��˸���Ϣ
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	imp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE, inv_index_pet);
	return true;
}

bool item_pet_bedge::RefineAttr(gplayer_imp* imp, size_t inv_index_pet, size_t attr_type, size_t assist_id, size_t assist_count) 
{
	item& it = imp->GetInventory()[inv_index_pet];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid()) return false;
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID()) return false;
	if(ess.GetRebornCnt() <= 0) return false;

	//������Ҫ���ýű�
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false; 
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_RefineAttr_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,ess.GetOrigin());
	lua_pushinteger(L,imp->_basic.level);
	lua_pushinteger(L,attr_type);
	lua_pushinteger(L,assist_id);
	lua_pushinteger(L,assist_count);
	if(lua_pcall(L,8,3,0))
	{
		lua_pop(L,1);
		return false; 
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3)) 
	{
		lua_pop(L,3);
		return false; 
	}

	//ȡ�÷���ֵ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid = lua_tointeger(L,-3);

	lua_pop(L,3);
	if(pet_tid != _tid)
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return false;
	}
	ess.UpdateContent(&it);

	GLog::log(LOG_INFO,"������Ϣ(����ϴ��) ���%d�������ϴ������ pid=%d, attr_type=%d, assist_id=%d, assist_num=%d", imp->_parent->ID.id,it.type, attr_type, assist_id, assist_count);
	//�ɹ���ҲҪ���߿ͻ��˸���Ϣ
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, inv_index_pet);
	return true;
}

void item_pet_bedge::GetItemDataForClient(item_data_client& data,const void* buf,size_t len) const
{
	if(len == 0)
	{
		data.item_content = (char*)buf;
		data.content_length = len;
	}
	else
	{
		pet_bedge_essence ess(buf,len);
		if(ess.IsValid())
		{
			ess.PrepareData();
			//û�м����ĳ�����������
			ess.MakeFakeData();
			pet_bedge_enhanced_essence * epEss = ess.QueryEssence();
			data.item_content = (char*)epEss;
			data.content_length = sizeof(pet_bedge_enhanced_essence);
		}
		else
		{
			data.item_content = NULL;
			data.content_length = 0;
		}
	}
}

bool item_pet_bedge::GetRefineInfoByType(int type, int & c_id, int & count)
{
	if(type < 0 || type >= 36) return false;

	static int refine_table[][2] = {{16030, 1}, {16031, 2}, {16032, 5}, {16050, 1}, {16051, 2}, {16052, 5}, {16070, 1}, {16071, 2}, {16072, 5},
		{16090, 1}, {16091, 2}, {16092, 5}, {16110, 1}, {16111, 2}, {16112, 5}, {16130, 1}, {16131, 1}, {16132, 2}, {16150, 1}, {16151, 1},
		{16152, 2}, {16170, 1}, {16171, 1}, {16172, 2}, {16190, 1}, {16191, 1}, {16192, 2}, {16250, 1}, {16251, 1}, {16252, 2},
		{16210, 1}, {16211, 1}, {16212, 2}, {16230, 1}, {16231, 1}, {16232, 2}};

	c_id = refine_table[type][0];
	count = refine_table[type][1];
	return true;
}

void item_pet_bedge::TryConvertData(item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != 0 && len > sizeof(int))
	{
		size_t magic = *(size_t*)buf;
		if(magic != pet_bedge_enhanced_essence::PET_MAGIC_NUMBER)
		{
			pet_bedge_essence ess;
			if(ess.ConvertData(buf, len))
			{
				ess.UpdateContent(parent,true);
			}
		}
	}
}


void item_pet_bedge::OnInit(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	TryConvertData(parent);

}


void item_pet_bedge::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	TryConvertData(parent);

	size_t len;
	void * buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(ess.IsValid())
	{
		ess.PrepareData();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(ess.GetOwnerID() == pImp->_parent->ID.id)
		{
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			size_t ess_name_len;
			if(memcmp(ess.GetOwnerName(ess_name_len), name, MAX_USERNAME_LENGTH) != 0)
			{
				OnChangeOwnerName(item::BODY, index, obj, parent);
			}
		}
	}
}

void item_pet_bedge::OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(ess.IsValid())
	{
		ess.PrepareData();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(ess.GetOwnerID() == pImp->_parent->ID.id)
		{
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			if(name_len > MAX_USERNAME_LENGTH) name_len = MAX_USERNAME_LENGTH;
			ess.SetOwnerName(name,name_len);
			ess.UpdateContent(parent,true);
			pImp->PlayerGetItemInfo(l,index);
		}
	}
}


void item_pet_bedge::OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(ess.IsValid())
	{
		ess.PrepareData();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		ess.SetOwnerID(pImp->_parent->ID.id);
		ess.UpdateContent(parent,true);
		pImp->PlayerGetItemInfo(l,index);
	}
}


