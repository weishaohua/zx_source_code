#ifndef __ONLINEGAME_GS_ITEM_ADDON_H__
#define __ONLINEGAME_GS_ITEM_ADDON_H__

#include <hashmap.h>
#include "../template/exptypes.h"
#include "../config.h"

class item_body;
class item;
class gactive_imp;
class addon_handler;
struct equip_data;

struct  addon_data
{
	int id;
	int arg[3];
};

struct addon_entry
{
	int id;
	addon_handler * handler;
	addon_entry():id(-1),handler(NULL)
	{}
};


//����Ķ�����������ʼ��������ʹ��ASmallObject
class addon_handler
{
public:
	virtual ~addon_handler(){}
	virtual int UpdateItem(const addon_data & , equip_data *) = 0;
	virtual int Use(const addon_data & , const item_body *, gactive_imp *) = 0;
	virtual int Activate(const addon_data & , const item_body *, gactive_imp *, const item*) = 0;
	virtual int Deactivate(const addon_data & , const item_body *, gactive_imp *, const item*) = 0;
	virtual int ScaleActivate(const addon_data & a, const item_body *e, gactive_imp *g, size_t l, const item* parent)
	{
		return Activate(a,e,g,parent);
	}

	virtual int ScaleDeactivate(const addon_data & a, const item_body *e, gactive_imp *g, size_t l, const item* parent)
	{
		return Deactivate(a,e,g,parent);
	}
	virtual bool TransformData(addon_data & data)
	{
		return false;
	}
	virtual bool IsTalismanAddonHandler() { return false; }
};

struct addon_data_spec
{	
	addon_handler 	* handler;
	addon_data	  data;
	addon_data_spec():handler(0){};
	addon_data_spec( addon_handler 	* handler, const addon_data &data):handler(handler),data(data)
	{}
};

class addon_manager
{
public:
	typedef abase::hashtab<addon_entry,int,abase::_hash_function>  ADDON_MAP;
private:
	static addon_manager __instance;
	addon_manager():_map(500){}
	ADDON_MAP _map;

	addon_entry * Find(int id)
	{
		ADDON_MAP::iterator it = _map.find(id);
		if(it == _map.end())
		{
			return NULL;
		}
		return it.value();
	}

	int __CheckAndUpdate(const addon_data & data,equip_data * item)
	{	
		int id = data.id;
		addon_entry * ent = Find(id);
		if(ent == NULL) return ADDON_MASK_INVALID;
		return ent->handler->UpdateItem(data,item);
	}
	int __Use(const addon_data & data,const item_body * e,gactive_imp * imp)
	{
		int id = data.id;
		ADDON_MAP::iterator it = _map.find(id);
		if(it == _map.end()) return ADDON_MASK_INVALID;
		it->handler->Use(data,e,imp);
		return 0;
	}

	int __Activate(const addon_data & data, const item_body *e, gactive_imp * imp, const item* parent)
	{
		int id = data.id;
		ADDON_MAP::iterator it = _map.find(id);
		if(it == _map.end()) return ADDON_MASK_INVALID;
		it->handler->Activate(data,e,imp,parent);
		return 0;
	}

	int __Deactivate(const addon_data & data, const item_body *e, gactive_imp * imp, const item* parent)
	{
		int id = data.id;
		ADDON_MAP::iterator it = _map.find(id);
		if(it == _map.end()) return ADDON_MASK_INVALID;
		it->handler->Deactivate(data,e,imp,parent);
		return 0;
	}
	
	void __Insert(const addon_entry & ent)
	{
		ADDON_MAP::iterator it = _map.find(ent.id);
		ASSERT(it == _map.end());
		if(!_map.put(ent.id, ent))
		{
			ASSERT(false);
			throw -101;
		}
	}

	bool __TransformData(addon_data & data)
	{
		ADDON_MAP::iterator it = _map.find(data.id);
		if(it == _map.end()) return false;
		return it->handler->TransformData(data);
	}
	
 	addon_handler * __QueryHandler(int id)
	{
		ADDON_MAP::iterator it = _map.find(id);
		if(it == _map.end()) return NULL;
		return it->handler;
		return 0;
	}


public:
	friend class addon_inserter;
	static addon_manager & GetInstance()
	{
		return __instance;
	}

//�������Զ�Ӧ������ģ��
	enum
	{
		ADDON_MASK_INVALID	= -1,
		ADDON_MASK_STATIC 	= 1,		//��̬����
		ADDON_MASK_ACTIVATE 	= 2,		//��ǿ(filter)���ԣ���װ����ʱ�����
		ADDON_MASK_USE		= 4,		//��Ʒ����ʹ�ã���ʹ�õ�ʱ����ã�ͬʱֻ����һ��
	};
/**
 *		�������������addon�������÷�ʽ������Ǿ�̬��ʽ�Ļ�,
 *		�˺����Ὣ���ݸ��µ���Ʒ�Ļ������Ժ���ǿ����֮��
 *		���ķ���ֵ��һ��mask�������ں�ʱ������
 *		���ӵ�gactive_imp �ǹ���ʱ֮��
 */
	static int CheckAndUpdate(const addon_data & data,equip_data * idata)
	{
		return GetInstance().__CheckAndUpdate(data,idata);
	}

/*
 *	�����������ʹ���ض���װ��,ֻ���������ʹ�õ�,���������Ļ�cooldown��μ�?
 *	���Ǽ�¼��equp_item���� ,�����ڴ���ʱҲ��Ҫ���⿼����һ����
 */
	static int Use(const addon_data & data,const item_body *e,gactive_imp *imp)
	{
		return GetInstance().__Use(data,e,imp);
	}

/*
 *	����������𼤻����͵�addon������Ʒ��װ��ʱ����������ᱻ���õ�
 */
	static int Activate(const addon_data & data, const item_body *it, gactive_imp * imp, const item* parent)
	{
		return GetInstance().__Activate(data,it,imp,parent);
	}

/*
 *	��������Ǽ�����ķ�����������Ʒ��ж��ʱ����������ᱻ���õ�
 */
	static int Deactivate(const addon_data & data, const item_body *it, gactive_imp *imp, const item* parent)
	{
		return GetInstance().__Deactivate(data,it,imp,parent);
	}

	static bool TransformData(addon_data & data)
	{
		return GetInstance().__TransformData(data);
	}

/*
 *	ȡ��Addon�Ĵ�����
 */
 	static addon_handler * QueryHandler(int id)
	{
		return GetInstance().__QueryHandler(id);
	}
	
};

class addon_data_man
{
	typedef abase::hash_map<int, addon_data_spec,abase::_hash_function>  MAP;
	MAP _map;
	static addon_data_man __instance;
public:
	static addon_data_man & Instance()
	{
		return __instance;
	}
	bool InsertAddon(int id, const addon_data & data)
	{
		addon_data pdata = data;
		if(!addon_manager::TransformData(pdata))
		{
			return false;
		}
		addon_handler * handler = addon_manager::QueryHandler(pdata.id);
		if(!handler) return false;
		_map[id] = addon_data_spec(handler, pdata);
		return true;
	}

	const addon_data_spec * GetAddon(int id) const
	{
		MAP::const_iterator it = _map.find(id);
		if(it == _map.end()) return NULL;
		return &(it->second);
	}

	inline void ActivateAddon(int id, const item_body *it, gactive_imp * imp, const item* parent)
	{
		const addon_data_spec * spec = GetAddon(id);
		if(!spec) return;
		spec->handler->Activate(spec->data, it, imp, parent);
	}
	
	inline void DeactivateAddon(int id, const item_body *it, gactive_imp *imp, const item* parent)
	{
		const addon_data_spec * spec = GetAddon(id);
		if(!spec) return;
		spec->handler->Deactivate(spec->data, it, imp, parent);
	}

	void ActivateAddonList(int *id , int count, const item_body *it, gactive_imp * imp, const item* parent)
	{
		for(int i = 0; i < count; i ++)
		{
			ActivateAddon(id[i],it, imp, parent);
		}
	}
	
	void DeactivateAddonList(int *id, int count, const item_body *it, gactive_imp *imp, const item* parent)
	{
		for(int i = 0; i < count; i ++)
		{
			DeactivateAddon(id[i],it, imp, parent);
		}
	}

};

class addon_inserter
{
public:
//	sample:
//	addon_inserter foo(0,100,(test_addon_handler*)NULL);
	template <typename T>
	addon_inserter(int id,T * )
	{
		addon_entry entry;
		entry.id = id;
		entry.handler = new T;
		addon_manager::GetInstance().__Insert(entry);
	}
	
	static int _counter;
	static void foo(const addon_inserter & test)
	{
		_counter ++;
	}
	template <typename T>
	static void foo2(int id,T * handler)
	{
		addon_entry entry;
		entry.id = id;
		entry.handler = handler;
		addon_manager::GetInstance().__Insert(entry);
	}
};

#define INSERT_ADDON(id,T) addon_inserter::foo(addon_inserter(id, (T*)NULL))
inline void INSERT_ADDON_INSTANCE(int id,addon_handler * obj) {addon_inserter::foo2(id, obj);}

bool InitAllAddon();
bool InsertSetAddon(int id ,int critical_value, int origin_addon);

class essence_addon : public addon_handler
{
public:
	virtual int UpdateItem(const addon_data & , equip_data *)
	{
		//do nothing
		return addon_manager::ADDON_MASK_STATIC;
	}
	
	virtual int Use(const addon_data & , const item_body *, gactive_imp *)
	{
		ASSERT(false);
		return 0;
	}
	
	virtual int Activate(const addon_data & , const item_body *, gactive_imp *, const item* parent)
	{
		ASSERT(false);
		return 0;
	}
	virtual int Deactivate(const addon_data & , const item_body *, gactive_imp *, const item* parent)
	{
		ASSERT(false);
		return 0;
	}
};
enum
{
	ITEM_ADDON_FASION_WEAPON_SKILL = 62,  // Youshuang add
	ITEM_ADDON_SKILL_STONE = 1024,
	ITEM_ADDON_SET_BEGIN = 10240,
	ITEM_ADDON_SET_END = 10240 + 10240,
};


#endif

