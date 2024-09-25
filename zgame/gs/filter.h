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
	int _timeout_t;
	bool _active;
	int _mask;	//������������
	int _filter_id;	//filter ��ID
	bool _is_deleted;	//�Ƿ���Ϊɾ��
	object_interface _parent;
	bool _attach;
	int _buff_id;

	bool IsDeleted() { return _is_deleted;}

public:
DECLARE_SUBSTANCE(filter);
	enum
	{	
		FILTER_MASK_TRANSLATE_SEND_MSG  	= 0x00000001,
		FILTER_MASK_TRANSLATE_RECV_MSG		= 0x00000002,
		FILTER_MASK_HEARTBEAT			= 0x00000004,
		FILTER_MASK_ADJUST_DAMAGE		= 0x00000008,
		FILTER_MASK_BEFORE_DAMAGE		= 0x00000010,	
		FILTER_MASK_ADJUST_EXP			= 0x00000020,
		FILTER_MASK_ADJUST_MANA_COST		= 0x00000040,
		FILTER_MASK_BEFORE_DEATH		= 0x00000080,
		FILTER_MASK_TRANSLATE_ENCHANT 		= 0x00000100,
		FILTER_MASK_TRANSLATE_SEND_ENCHANT	= 0x00000200,
		FILTER_MASK_BE_ATTACKED			= 0x00000400,
		FILTER_MASK_BE_REFLECTED		= 0x00000800,		//���ܷ���
		FILTER_MASK_ON_FILTER_ADD		= 0x00001000,           //��ұ�����ĳ��״̬
		FILTER_MASK_CRITENEMY			= 0x00002000,
		FILTER_MASK_DAMAGEENEMY			= 0x00004000,
		FILTER_MASK_ADJUST_SKILL		= 0x00008000,
		FILTER_MASK_AFTER_DEATH			= 0x00010000,
		FILTER_MASK_ON_MOVED			= 0x00020000,

		FILTER_MASK_REMOVE_ON_DEATH		= 0x00200000,		//����ʱ�ᱻ�Զ�ɾ��
		FILTER_MASK_MERGE			= 0x00400000,		//���������һ�µ�filter_id  ����ͼ�ں�
		FILTER_MASK_SKILL2			= 0x00800000,
		FILTER_MASK_SKILL3			= 0x01000000,
		FILTER_MASK_SAVE_DB_DATA		= 0x02000000,		//�Ƿ���Ҫ���浽���ݿ���
		FILTER_MASK_NOSAVE			= 0x04000000,		//ת�Ʒ�����ʱ������
		FILTER_MASK_BATTLE_BUFF			= 0x08000000,		//ս��BUFF
		FILTER_MASK_DEBUFF			= 0x10000000,
		FILTER_MASK_BUFF			= 0x20000000,
		FILTER_MASK_UNIQUE			= 0x40000000,		//����ԭ��������ͬfilter_id  filter
		FILTER_MASK_WEAK			= 0x80000000,		//���ԭ������ͬ��filter_id  �򲻽��м���
	};
	int GetMask() { return _mask;}
	int GetFilterID() { return _filter_id;}
	virtual bool Save(archive & ar)
	{
		ar << _mask << _filter_id << _active << _is_deleted << _attach;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ar >> _mask >> _filter_id >> _active >> _is_deleted << _attach;
		return true;
	}
	void MergeMask(int mask) { _mask  |= mask; }

	inline bool IsActived() { return _active; }   

	virtual int OnGetTimeOut() {return 0;} //��filter�Ƿ�timeout <=0��ֵ��ʾ������ʧ

	inline int GetTimeout()
	{
		return OnGetTimeOut();
	}

	void Release() 
	{
		Detach();
		delete this;
	}

	virtual void Attach()
	{
		if(!_active)
		{
			OnAttach();
			if(_buff_id >= 0)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), 1);
			}
			_active = true;
		}
	}

	virtual void Detach()
	{       
		if(_active)
		{
			_active = false;                //�� OnDetach ֮ǰ��ֵ ���˳�����Ҫ
			if(_buff_id >= 0)
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
			OnDetach();
		}
	}
	virtual ~filter(){};

	void SetAttach() { _attach = true; }
	virtual char GetOverlayCnt() { return 0;} 
	virtual char GetMaxOverlayCnt() {return 0;}
	virtual bool CanOverlay() { return false; }
	virtual bool Overlay() { return true; }

protected:
	/*
	*	filter�н������õĺ���
	*	��Щ���������ú�������ط�0������ڵ��ú�ɾ�� �ͷ�
	*/
	virtual void TranslateSendAttack(const XID & target,attack_msg & msg, char type, float radius, int max_cnt){ASSERT(false);}	//�ڷ���������Ϣ֮ǰ����һ����Ϣ����, 0, Attack, 1, RegionAttack1, 2, RegionAttack2, 3, RegionAttack3, 4, MultiAttack
	virtual void TranslateRecvAttack(const XID & attacker,attack_msg & msg){ASSERT(false);}	//�ڽ��յ���Ϣ�����ȴ���һ��
	virtual void TranslateSendEnchant(const XID & target,enchant_msg & msg){ASSERT(false);}//�ڷ���enchant����Ϣ֮ǰ
	virtual void TranslateRecvEnchant(const XID & attacker,enchant_msg & msg){ASSERT(false);}	//�ڽ��յ���Ϣ�����ȴ���һ��
	virtual void Heartbeat(int tick){ASSERT(false);}			//������ʱ������,tick��ʾ���μ������
	virtual void AdjustDamage(damage_entry & dmg,const XID & attacker, const attack_msg & msg){ASSERT(false);}		//�����˺�֮ǰ����һ��
	virtual void BeforeDamage(int & damage){ASSERT(false);}		//�����յ��˺�������Ӱ���������
	virtual void AdjustExp(int type, int & exp){ASSERT(false);}		//�Ծ���ֵ��������
	virtual void AdjustManaCost(int &mana){ASSERT(false);}			//�ںķ�manaǰ������
	virtual void BeforeDeath(bool is_duel){ASSERT(false);}				//�ڵ���������OnDeathǰ����
	virtual void AfterDeath(bool is_duel){ASSERT(false);}				//�ڵ���������OnDeath�����
	virtual void BeAttacked(const XID & target, int damage, bool crit){ASSERT(false);}
	virtual void BeReflected(const XID & target){ASSERT(false);}
	virtual void OnFilterAdd(int filter_id, const XID& caster) { ASSERT(false); }
	virtual void CritEnemy(const XID& target) { ASSERT(false); }
	virtual void DamageEnemy(const XID& target, int damage) { ASSERT(false); }
	virtual void AdjustSkill(int skill_id, short& level) { ASSERT(false); }
	virtual void OnMoved(){ASSERT(false);}				//�ڵ���������OnDeath�����
	virtual void Merge(filter * f) { ASSERT(false);}
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;

	virtual void  OnModify(int ctrlname,void * ctrlval,size_t ctrllen) { }

	friend class filter_man;
	filter(object_interface parent,int mask):_timeout_t(0),_active(false),_mask(mask),_filter_id(0),_is_deleted(false),_parent(parent),_attach(false), _buff_id(-1)
	{}
	filter(){}
	inline void  Modify(int ctrlname,void * ctrlval,size_t ctrllen) 
	{
		OnModify(ctrlname,ctrlval,ctrllen);
	}
	inline void ClearMask(int xmask)
	{
		_mask &= ~xmask;
	}

protected:
	virtual bool ResetTimeout(); //�����ʱֻ���ӳ���������ǰ���� ��ǰ������is_deleted
	int GetTimeLeft();
};

class timeout_filter : public filter
{
protected:
	int _time;
	int _timeout;
	bool _can_overlay;
	int  _max_overlay_cnt;
	int  _overlay_cnt;

	timeout_filter(object_interface object,int timeout,int mask)
		:filter(object,mask),_time(timeout),_timeout(timeout),_can_overlay(false),_max_overlay_cnt(1), _overlay_cnt(1)
	{
		ASSERT(mask & FILTER_MASK_HEARTBEAT);
	}

	virtual ~timeout_filter(){}

	virtual bool Save(archive & ar)
	{
		filter::Save(ar);
		ar << _timeout;
		ar << _can_overlay;
		ar << _max_overlay_cnt;
		ar << _overlay_cnt;
		ar << _attach;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		filter::Load(ar);
		ar >> _timeout;
		ar >> _timeout;
		ar >> _can_overlay;
		ar >> _max_overlay_cnt;
		ar >> _overlay_cnt;
		ar >> _attach;
		return true;
	}

	timeout_filter(){}

	virtual int OnGetTimeOut()
	{
		return _timeout;
	}

	virtual bool ResetTimeout();
	virtual void OnModify(int ctrlname,void * ctrlval,size_t ctrllen);

	virtual void Attach()
	{
		if(!_active)
		{
			OnAttach();
			_active = true;
			if(_buff_id >= 0 && _overlay_cnt > 0)
			{
				_parent.UpdateBuff(_buff_id, _filter_id, GetTimeout(), _overlay_cnt);
			}
		}
	}

	virtual void Detach()
	{       
		if(_active)
		{
			_active = false;                //�� OnDetach ֮ǰ��ֵ ���˳�����Ҫ
			OnDetach();
			if(_buff_id >= 0)
			{
				_parent.RemoveBuff(_buff_id, _filter_id);
			}
		}
	}

public:

	virtual bool CanOverlay()
	{
		return _can_overlay;
	}
	
	virtual char GetOverlayCnt()
	{
		return _overlay_cnt;
	}

	virtual char GetMaxOverlayCnt()
	{
		return _max_overlay_cnt;
	}
 
	virtual bool Overlay();

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
	FILTER_IDX_BEFORE_DAMAGE	,
	FILTER_IDX_ADJUST_EXP		,
	FILTER_IDX_ADJUST_MANA_COST	,
	FILTER_IDX_BEFORE_DEATH		,
	FILTER_IDX_TRANSLATE_ENCHANT	,
	FILTER_IDX_TRANSLATE_SEND_ENCHANT,
	FILTER_IDX_BE_ATTACKED		,
	FILTER_IDX_BE_REFLECTED		,
	FILTER_IDX_ON_FILTER_ADD	,
	FILTER_IDX_CRIT_ENEMY		,
	FILTER_IDX_DAMAGE_ENEMY		,
	FILTER_IDX_ADJUST_SKILL		,
	FILTER_IDX_AFTER_DEATH		,
	FILTER_IDX_ON_MOVED		,

	FILTER_IDX_MAX	
};

#endif

