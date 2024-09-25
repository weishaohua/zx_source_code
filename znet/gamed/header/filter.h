#ifndef __ONLINEGAME_GS_FILTER_H__
#define __ONLINEGAME_GS_FILTER_H__

#include <ASSERT.h>
#include <common/base_wrapper.h>
#include "attack.h"
#include "property.h"
#include "substance.h"
#include "obj_interface.h"

class filter : public substance
{
protected:
	int _mask;	//处理那种内容
	int _filter_id;	//filter 的ID
	bool _active;
	bool _is_deleted;	//是否标记为删除
	object_interface _parent;

	bool IsDeleted() { return _is_deleted;}
public:
DECLARE_SUBSTANCE(filter);
	enum
	{	
		FILTER_MASK_TRANSLATE_SEND_MSG  = 0x0001,
		FILTER_MASK_TRANSLATE_RECV_MSG	= 0x0002,
		FILTER_MASK_HEARTBEAT		= 0x0004,
		FILTER_MASK_ADJUST_DAMAGE	= 0x0008,
		FILTER_MASK_DO_DAMAGE		= 0x0010,
		FILTER_MASK_ADJUST_EXP		= 0x0020,
		FILTER_MASK_ADJUST_MANA_COST	= 0x0040,
		FILTER_MASK_BEFORE_DEATH	= 0x0080,
		FILTER_MASK_TRANSLATE_ENCHANT 	= 0x0100,
		FILTER_MASK_TRANSLATE_SEND_ENCHANT= 0x0200,
		FILTER_MASK_ALL			= 0x03FF,


		FILTER_MASK_DEBUFF		= 0x001000,
		FILTER_MASK_BUFF		= 0x002000,
		FILTER_MASK_UNIQUE		= 0x004000,		//覆盖原来所有相同filter_id  filter
		FILTER_MASK_WEAK		= 0x008000,		//如果原来有相同的filter_id  则不进行加入
		FILTER_MASK_REMOVE_ON_DEATH	= 0x010000,		//死亡时会被自动删除
		FILTER_MASK_MERGE		= 0x020000,		//如果发现了一致的filter_id  会试图融合
		FILTER_MASK_SKILL1		= 0x040000,
		FILTER_MASK_SKILL2		= 0x080000,
		FILTER_MASK_SKILL3		= 0x100000,
		FILTER_MASK_SAVE_DB_DATA	= 0x200000,		//是否需要保存到数据库中
		FILTER_MASK_NOSAVE		= 0x400000,		//转移服务器时不保存
		FILTER_MASK_BATTLE_BUFF		= 0x800000,		//战场BUFF
	};
	int GetMask() { return _mask;}
	int GetFilterID() { return _filter_id;}
	virtual bool Save(archive & ar)
	{
		ar << _mask << _filter_id << _active << _is_deleted;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ar >> _mask >> _filter_id >> _active >> _is_deleted;
		return true;
	}
	void MergeMask(int mask) { _mask  |= mask; }
private:
	/*
	*	filter中将被调用的函数
	*	这些函数被调用后如果返回非0，则会在调用后被删除 释放
	*/
	virtual void TranslateSendAttack(const XID & target,attack_msg & msg){ASSERT(false);}	//在发出攻击消息之前处理一下消息内容
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg){ASSERT(false);}	//在接收到消息后首先处理一下
	virtual void TranslateEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}	//在接收到消息后首先处理一下
	virtual void TranslateSendEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}//在发送enchant的消息之前
	virtual void Heartbeat(int tick){ASSERT(false);}			//在心跳时作处理,tick表示本次间隔几秒
	virtual void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg){ASSERT(false);}		//在做伤害之前处理一下
	virtual void DoDamage(const damage_entry & dmg){ASSERT(false);}		//对最终的伤害产生的影响进行修正
	virtual void AdjustExp(int type, int & exp){ASSERT(false);}		//对经验值进行修正
	virtual void AdjustManaCost(int &mana){ASSERT(false);}			//在耗费mana前做修正
	virtual void BeforeDeath(){ASSERT(false);}				//在调用死亡的OnDeath前调用

	virtual void Merge(filter * f) { ASSERT(false);}
	virtual void OnAttach() = 0;
	virtual void OnRelease() {}
	void Release() 
	{
		OnRelease();
		delete this;
	}
	virtual void  OnModify(int ctrlname,void * ctrlval,size_t ctrllen) { }

protected:
	friend class filter_man;
	filter(object_interface parent,int mask):_mask(mask),_filter_id(0),
						 _active(false),_is_deleted(false),_parent(parent)
	{}
	filter(){}
	virtual ~filter(){};
	inline void  Modify(int ctrlname,void * ctrlval,size_t ctrllen) 
	{
		OnModify(ctrlname,ctrlval,ctrllen);
	}
	inline void ClearMask(int xmask)
	{
		_mask &= ~xmask;
	}
};

class timeout_filter : public filter
{

protected:
	int _timeout;
	timeout_filter(object_interface object,int timeout,int mask)
			:filter(object,mask),_timeout(timeout)
	{
		ASSERT(mask & FILTER_MASK_HEARTBEAT);
	}

	virtual bool Save(archive & ar)
	{
		filter::Save(ar);
		ar << _timeout;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		filter::Load(ar);
		ar >> _timeout;
		return true;
	}

	timeout_filter(){}

	static inline int GetTimeOut(timeout_filter * rhs)
	{
		return rhs->_timeout;
	}

protected:
	virtual void Heartbeat(int tick)
	{
		_timeout -= tick;
		if(_timeout <=0) _is_deleted = true;
	}
};


//for filter_man
enum
{
	FILTER_IDX_TRANSLATE_SEND_MSG 	,
	FILTER_IDX_TRANSLATE_RECV_MSG	,
	FILTER_IDX_HEARTBEAT		,
	FILTER_IDX_ADJUST_DAMAGE	,
	FILTER_IDX_DO_DAMAGE		,
	FILTER_IDX_ADJUST_EXP		,
	FILTER_IDX_ADJUST_MANA_COST	,
	FILTER_IDX_BEFORE_DEATH		,
	FILTER_IDX_TRANSLATE_ENCHANT	,
	FILTER_IDX_TRANSLATE_SEND_ENCHANT,

	FILTER_IDX_MAX	
};

#endif
