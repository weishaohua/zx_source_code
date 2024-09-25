#ifndef __ONLINEGAME_GS_ITEMLIST_H__
#define __ONLINEGAME_GS_ITEMLIST_H__

#include "item.h"
#include <amemobj.h>
#include <amemory.h>
#include <octets.h>
#include <crc.h>
#include "playertemplate.h"
#include <db_if.h>

class item_list : public abase::ASmallObject
{
	item::LOCATION 		_location;
	abase::vector<item> 	_list;
	gactive_imp		*_owner;
	size_t			_empty_slot_count;
	inline int __try_pile(int& __bind, int type,size_t & count,size_t pile_limit,item*&pEmpty);
	inline void __find_empty(item*&pEmpty);
public:
	inline item::LOCATION GetLocation() {return _location;}

	item_list(item::LOCATION location, size_t size)
		:_location(location),_list(size,item()),_owner(NULL),_empty_slot_count(size)
	{} 

	~item_list()
	{
		Clear();
	}

	void Swap(item_list & rhs)
	{
		abase::swap(_location, rhs._location);
		_list.swap(rhs._list);
		abase::swap(_empty_slot_count,rhs._empty_slot_count);
	}

	//��ҳ������Ʒ
	bool ArrangeItem(int pageind,std::vector<exchange_item_info>& vec_exchange_info);

	//�����������ݱ���Ҫ�ͷ�
	bool MakeDBData(GDB::itemlist &list)
	{
#ifdef _DEBUG
		size_t count = 0;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1) count ++;
		}
		ASSERT(count == _empty_slot_count);
#endif
		ASSERT(!list.count && !list.list);
		ASSERT(list.list == NULL || list.count == 0);
		size_t item_count = _list.size() - _empty_slot_count;
		if(item_count == 0) return true;
		list.list = (GDB::itemdata*)abase::fast_allocator::alloc(item_count*sizeof(GDB::itemdata));
		list.count = item_count;
		size_t index = 0;
		GDB::itemdata * pData = list.list;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1) continue;
			ConvertDBItemEntry(pData[index], _list[i], i );
			index ++;
		}
		return true;
	}

	int GetDBData(GDB::itemdata *pData, size_t size)
	{
#ifdef _DEBUG
		size_t count = 0;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1) count ++;
		}
		ASSERT(count == _empty_slot_count);
#endif
		if(size < _list.size())
		{
			ASSERT(false);
			return -1;
		}
		size_t item_count = _list.size() - _empty_slot_count;
		if(item_count == 0) return 0;
		size_t index = 0;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1) continue;
			ConvertDBItemEntry(pData[index], _list[i], i);
			index ++;
		}
		return index;
	}


	static void ReleaseDBData(GDB::itemlist & list)
	{
		//�ͷ�Make����������
		if(!list.list) return;
		size_t size = list.count * sizeof(GDB::itemdata);
		abase::fast_allocator::free(list.list,size);
		list.count = 0;
		list.list = 0;
	}

	bool InitFromDBData(const GDB::itemlist & list)
	{
		ASSERT(_empty_slot_count == _list.size());
		size_t count = list.count;
		/*if(count > _list.size()) 
		{
			ASSERT(false);
			return false;
		}*/
		const GDB::itemdata * pData = list.list;
		for(size_t i = 0; i < count ; i ++)
		{
			size_t index= pData[i].index;
			if(index >= _list.size()) continue;	//���Զ������Ʒ
			if(!MakeItemEntry(_list[index],pData[i]))
			{
				//���ִ������Ʒ ֱ�����
				_list[index].Clear();
				continue;
			}
			_list[index].Init(_location,index,_owner);
			_list[index].PutIn(_location,index,_owner);
			_empty_slot_count --;
		}
		return true;
	}

	bool MakeDBData(GDB::pocket_itemlist &list);
	void ReleaseDBData(GDB::pocket_itemlist & list);
	int GetDBData(GDB::pocket_item *pData, size_t size);
	bool InitFromDBData(const GDB::pocket_itemlist &list);

	void SetSize(size_t size)
	{
		if(size <= _list.size()) return;
		abase::vector<item> 	tmplist(size,item());
		size_t offset = size - _list.size();
		for(size_t i = 0; i < _list.size(); i ++)
		{
			item tmp = _list[i];
			_list[i] = tmplist[i];
			tmplist[i] = tmp;
			tmp.Clear();
		}
		_list.swap(tmplist);

		_empty_slot_count += offset;
	}

	int GetItemData(size_t index, item_data & data)
	{
		if(index >= _list.size()) return -1;
		if(_list[index].type == -1) return 0;
		
		ItemToData(_list[index],data);
		return index + 1;
	}

	int GetItemDataForClient(size_t index, item_data_client & data)
	{
		if(index >= _list.size()) return -1;
		if(_list[index].type == -1) return 0;
		
		ConvertItemToDataForClient(data, _list[index]);
		return index + 1;
	}

	void SimpleSave(abase::octets & data)
	{
		data.reserve(_list.size() * (sizeof(int)*2));
		for(size_t i = 0; i < _list.size(); i ++)
		{
			int type = _list[i].type;
			data.push_back(&type,sizeof(type));
			if(type < 0) continue;
			int expire_date = _list[i].expire_date;
			data.push_back(&expire_date, sizeof(expire_date));
			int state = (int)(_list[i].GetProctypeState());
			data.push_back(&state, sizeof(state));
			int count = _list[i].count;
			if(_list[i].IsActive())
			{
				count |= 0x80000000;
			}
			data.push_back(&count,sizeof(count));
		}
	}
	
	bool DetailSave(archive & ar)
	{
		size_t count = _list.size() - _empty_slot_count;
		ar << count;
		size_t i;
		for(i = 0; i < _list.size(); i ++)
		{
			item & it = _list[i];
			if(it.type == -1)
			{
				continue;
			}
			count --;
			ar << i;
			ar << it.type;
			ar << it.expire_date;
			ar << (int)(it.GetProctypeState());
			ar << it.count;
			/*size_t len;
			const void * buf;
			it.GetItemDataForClient(&buf, len);*/
			item_data_client data;
			it.GetItemDataForClient(data);
			if(data.use_wrapper)
			{
				ar << (short)data.ar.size();
				if(data.ar.size())
				{
					ar.push_back(data.ar.data(), data.ar.size());
				}
			}
			else
			{
				ar << (short)data.content_length;
				if(data.content_length)
				{
					ar.push_back(data.item_content,data.content_length);
				}
			}
		}
		ASSERT(count == 0);
		return true;
	}

	bool DetailSavePartial(archive & ar,const int spec_list[] , size_t ocount)
	{
		ASSERT(ocount >0 && ocount <= _list.size());
		ar << ocount;
		int i;
		size_t oindex = 0;
		for(i = 0; i < (int)_list.size(); i ++)
		{
			if(i != spec_list[oindex])
			{
				continue;
			}
			item & it = _list[i];
			if(it.type == -1)
			{
				ar << -1;
			}
			else
			{
				ar << i;
				ar << it.type;
				ar << it.expire_date;
				ar << (int)(it.GetProctypeState());
				ar << it.count;
				/*size_t len;
				const void * buf;
				it.GetItemDataForClient(&buf, len);
				ar << (short)len;
				if(len)
				{
					ar.push_back(buf,len);
				}*/
				
				item_data_client data;
				it.GetItemDataForClient(data);
				if(data.use_wrapper)
				{
					ar << (short)data.ar.size();
					if(data.ar.size())
					{
						ar.push_back(data.ar.data(), data.ar.size());
					}
				}
				else
				{
					ar << (short)data.content_length;
					if(data.content_length)
					{
						ar.push_back(data.item_content,data.content_length);
					}
				}
			}
			oindex ++;
			if(oindex == ocount) break;
			ASSERT(spec_list[oindex] > spec_list[oindex-1]);
		}
		ASSERT(oindex == ocount);
		return true;
	}

	bool Save(archive & ar)
	{
		ar << _list.size();
		size_t i;
		for(i = 0; i < _list.size(); i ++)
		{
			_list[i].Save(ar);
		}
		return true;
	}

	bool Load(archive & ar)
	{
		size_t count;
		ar >> count;
		ASSERT(count >= _list.size());
		if(count > _list.size())
		{
			SetSize(count);
		}
		ASSERT(_empty_slot_count == count);
		Clear();
		size_t i;
		for(i = 0; i < count; i ++)
		{
			_list[i].Load(ar);
			if(_list[i].type != -1) _empty_slot_count --; 
		}
		return true;
	}

	static inline void ItemToData(const item & it, item_data & data)
	{
		ASSERT(it.type != -1);
		ConvertItemToData(data, it);
	}
/*
	static inline void ItemToData(item & it, GDB::itemdata & data)
	{
		ASSERT(it.type != -1);
		data.id = it.type;
		data.index = -1;        //�����޷���ã�����һ������ֵ 
		data.count = it.count;
		data.max_count = it.pile_limit;
		data.guid1 = it.guid.guid1;
		data.guid2 = it.guid.guid2;
		data.proctype = it.proc_type;
		data.expire_date = it.expire_date;
		data.data = NULL;
		data.size = 0;

		it.GetItemData(&(data.data), data.size);
	}*/

/**
 *	ȡ��װ���ı������ݣ�����һ��abase::octets�ֻ�����е�װ��
 *
 */
 
	void GetEquipmentData(unsigned int & emask, abase::octets & os)
	{
		ASSERT(_location == item::BODY);
		unsigned int mask = 0;
		for(size_t i = item::EQUIP_VISUAL_START; i < item::EQUIP_VISUAL_END; i ++)
		{
			int64_t type = _list[i].type;
			if(type == -1) continue;
			type |= _list[i].GetIdModify();
			mask |= 1 << (i - item::EQUIP_VISUAL_START);
			os.push_back(&type,sizeof(type));
		}
		emask = mask;
		return; 
	}

/**
 *	ȡ��װ���ı������ݣ�����һ��abase::octets��
 *	��ԭ���е�������һ�������һ���仯��������Χ���˽���������
 */
	void GetEquipmentData(int oldmask,ushort & newmask,ushort & addmask, ushort & delmask,abase::octets & os,abase::octets & osn)
	{
		ASSERT(_location == item::BODY);
		unsigned short mask = 0;
		unsigned short mask_add = 0;
		unsigned short mask_del = 0;
		ASSERT(os.empty());
		ASSERT(osn.empty());
		for(int i = item::EQUIP_VISUAL_START; i < item::EQUIP_VISUAL_END; i ++)
		{
			unsigned short m = 1 << (i - item::EQUIP_VISUAL_START);
			int type = _list[i].type;
			if(type == -1) 
			{
				mask_del |= m;
				continue;
			}
			if((oldmask & m) == 0) 
			{
				mask_add |= m;
				osn.push_back(&type,sizeof(type));
			}
			mask |= m;
			os.push_back(&type,sizeof(type));
		}

		newmask = mask;
		addmask = mask_add;
		delmask = mask_del;
		return;
	}

	void Clear()
	{
		for(size_t i = 0;i < _list.size(); i ++)
		{
			if(_list[i].type != -1)
			{
				_list[i].TakeOut(_location, i, _owner);
				_list[i].Release();
			}
		}
		_empty_slot_count = _list.size();
	}
	
	size_t Size() const { return _list.size();}
	bool IsSlotEmpty(int index) { return _list[index].type == -1;}
	void SetOwner(gactive_imp * owner) {_owner = owner;}
	size_t GetEmptySlotCount() { return _empty_slot_count;}

	/*
	 *	С��ʹ���������������Ҫ�����ַ�ʽɾ����Ʒ
	 *	��Ӱ����Ʒ�ļ�������
	 */
	const item & operator[](size_t index) const  { return _list[index];}

	/*
	 *	С��ʹ���������������Ҫ�����ַ�ʽɾ����Ʒ
	 *	��Ӱ����Ʒ�ļ�������
	 */
	item & operator[](size_t index) { return _list[index];}

	bool IsItemActive(size_t index) { return _list[index].IsActive();}
//	item & operator[](size_t index) { return _list[index];}

	/*
	 *	Ѱ�ҵ�һ�����ϱ�׼����Ʒ
	 *	����startָ����λ�ÿ�ʼѰ��
	 *	���ҪѰ�ҿ�λ��������typeʹ��-1����
	 */
	int  Find(int start,int type)
	{
		for(size_t i = start; i < _list.size(); i ++)
		{
			if(_list[i].type == type) return i;
		}
		return -1;
	}
	
	int FindByType(int start , int item_type)
	{
		for(size_t i = start; i < _list.size(); i ++)
		{
			if(_list[i].type == -1) continue;
			if(_list[i].body == NULL) continue;
			if(_list[i].body->GetItemType() == item_type) return i;
		}
		return -1;
	}
	
	/**
	 *	Ѱ���ܷŶ����ĵ�һ����λ�������˶ѵ�����
	 */
	int FindEmpty(int type)
	{
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1 ) return i;
			if(_list[i].type == type && _list[i].count < _list[i].pile_limit) return i;
		}
		return -1;
	}

	int CountItemByType(int item_type)
	{
		int count = 0;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == -1) continue;
			if(_list[i].body == NULL) continue;
			if(_list[i].body->GetItemType() == item_type) count ++;
		}
		return count;
	}

	int CountItemByID(int item_id)
	{
		int count = 0;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == item_id) count += _list[i].count;
		}
		return count;

	}
	

	/*
	 *	���ض�����Ʒ�Ƿ��п��е�λ��	
	 */
	bool HasSlot(int type)
	{
		if(_empty_slot_count) return true;
		for(size_t i = 0; i < _list.size(); i ++)
		{
			if(_list[i].type == type && _list[i].count < _list[i].pile_limit) return true;
		}
		return false;
	}

	/*
	 *	���ض�����Ʒ�Ƿ��п��е�λ��	 ����ָ����λ��������
	 */
	bool HasSlot(int type,int count)
	{
		if(_empty_slot_count) return true;
		for(size_t i = 0; i < _list.size() && count > 0; i ++)
		{
			if(_list[i].type == type)
			{
				count -= _list[i].pile_limit - _list[i].count;
			}
		}
		return count <= 0;
	}

	/**
	*	�Ƿ�ǰ�Ѿ�û�пյ�λ��
	*/
	
	bool IsFull()
	{
		return !_empty_slot_count;
	}

	/**
	*	�쿴�ض�λ���Ƿ���ָ������Ʒ
	*/
	bool IsItemExist(size_t index,int type, size_t count)
	{
		if(index >= Size()) return false;
		const item & it = _list[index];
		if(it.type == -1 || it.type != type) return false;
		if(it.count < count) return false;
		return true;
	}


	 
	/**
	 *	����һ����Ʒ�����Զ�Ѱ�ҿ��еĿ�λ���߿��Ե��ӵ�λ��
	 *	�����������ô�᷵�ش���(����-1) itҲ���ᱻ�ÿ�
	 */
	int Push(item & it);

	/**
	 *	�����������һ����Ʒ,�����Ʒ��Ӵ�������������ɳ���
	 *	���ʧ�ܣ���ʾ��Ʒ���Ѿ�����
	 *	��������£����ط����λ������
	 *	�ڴ˺�������Զ�������Ʒ����������ڣ���PutIn����
	 */
	int Push(item_data & data);

	/**
	 *	�����������һ����Ʒ,�����Ʒ��Ӵ�������������ɳ���
	 */
	int Push(const item_data & data,int &count, int expire_date );

	/*
	 *	�ڿ�λ�������Ʒ
	 */
	int PushInEmpty(int start, const item_data & data , int count);

	/*
	 *	�ڿ�λ�������Ʒ
	 */
	int PushInEmpty(int start, item & it);

	/**
	*	��ָ��λ�÷���һ����Ʒ,
	*	�����λ���Ѿ�����һ������Ʒ���ǿ��Ե��ӵģ���ô����е���
	*	�����ӵ���Ʒ���ᱻ�зֳ����ɲ��֡�
	*	������ܵ��ӣ���᷵��ʧ��(С��0��ֵ��
	*	����ɹ��Ļ�������Ķ���ᱻClear()
	* 	�������ﲻ�ж���������ȷ���
	*/
	bool Put(int index, item & it)
	{
		item & old = _list[index]; 
		if(old.type == -1)
		{
			if(it.type == -1) return true;
			//ԭ��û����Ʒ��ֱ�ӷ���
			old = it;
			it.Clear();
			old.PutIn(_location,index,_owner);
			_empty_slot_count  --;
			return true;
		}
		else
		{
			if(old.type == it.type && old.pile_limit >= old.count + it.count && old.proc_type == it.proc_type)
			{ 
				//ԭ������Ʒ�����ҿ��Է���
				//������ȥ�������ͷŴ������Ʒ
				ASSERT(it.pile_limit == old.pile_limit);
				old.count += it.count;
				it.Release();
				return true;
			}
		}
		return false;
	}

	/*
	 *	��ָ��һ��λ�÷���һ����Ʒ�����ҽ���λ��ԭ������Ʒ����
	 */
	void Exchange(int index, item & it)
	{
		if(it.type != -1) _empty_slot_count --;
		item & src = _list[index];
		if(src.type != -1) src.TakeOut(_location,index,_owner);
		item tmp = it; it = src; src = tmp;
		tmp.Clear();
		if(src.type != -1) src.PutIn(_location,index,_owner);
		if(it.type != -1)
		{
			_empty_slot_count ++;
		}
		return ;
	}


	/*
	*	������Ʒ���������λ��
	*/
	bool ExchangeItem(size_t ind1, size_t ind2 )
	{
		if(ind1 >= _list.size() || ind2 >= _list.size()) return false;
		if(ind1 == ind2) return true;
		item tmp = _list[ind1];
		_list[ind1] = _list[ind2];
		_list[ind2] = tmp;
		tmp.Clear();
		return true;
	}

	/**
	*
	*/
	bool MoveItem(size_t src, size_t dest, size_t *pCount)
	{
		if(src >= _list.size() || dest >= _list.size()) return false;
		size_t count = *pCount;
		if(src == dest) return true;
		if(!count) return false;
		item & __src = _list[src];
		item & __dest= _list[dest];
		if(__src.type == -1 || __src.count < count) 
		{
			*pCount = 0;
			return true;
		}
		if(__dest.type == -1)
		{
			__dest = __src;
			__src.count -= count;
			if(__src.count)
			{
				__dest.content.BuildFrom(__src.content);
			}
			__dest.count = count;

			if(!__src.count)
			{
				__src.Clear();
			}
			else
			{
				__dest.PutIn(_location, dest, _owner);
				_empty_slot_count --;
			}
			return true;
		}
		if(__dest.type != __src.type || __dest.count + count > __dest.pile_limit || __dest.proc_type != __src.proc_type) 
		{
			*pCount = 0;
			return true;
		}

		__dest.count += count;
		__src.count -= count;
		if(!__src.count)
		{
			_empty_slot_count ++;
			__src.TakeOut(_location, src, _owner);
			__src.Release();
		}
		return true;
	}
	

	/*
	 *	ɾ����ȡ��һ����Ʒ
	 */
	bool Remove(size_t index,item & it)
	{
		if(_list[index].type == -1) 
		{
			return true;
		}
		it = _list[index];
		_empty_slot_count ++;
		_list[index].Clear();
		it.TakeOut(_location,index,_owner);
		return true;
	}

	void Remove(size_t index)
	{
		if(index >= _list.size()) 
		{
			ASSERT(false);
			return;
		}
		if(_list[index].type == -1)
		{
			return ;
		}
		_list[index].TakeOut(_location,index,_owner);
		_list[index].Release();
		_empty_slot_count ++;
		return ;
	}

	int DecAmount(size_t index,size_t count)	//���ػ�ʣ����٣�0��ʾ����û����
	{
		if(_list[index].type == -1) 
		{
			ASSERT(false);
			return 0;
		}

		item & it = _list[index];
		if(it.count <= count)
		{
			it.TakeOut(_location,index,_owner);
			it.Release();
			_empty_slot_count ++;
		}
		else
		{
			it.count -= count;
		}
		return it.count;
	}

	int IncAmount(size_t index, size_t count) //�����ж�������������
	{
		if(_list[index].type == -1) 
		{
			ASSERT(false);
			return -1;
		}

		item & it = _list[index];
		if(it.count >= it.pile_limit) return 0;
		size_t delta = it.pile_limit - it.count;
		if(delta > count) delta = count;
		it.count += delta;
		return (int)delta;
	}

	int UseItem(int index,gactive_imp * obj,int &count)
	{	
		if(_list[index].type == -1) 
		{
			return -1;
		}

		item & it = _list[index];
		int type = it.type;
		if(!it.CanUse(GetLocation(),obj))
		{
			return -2;
		}
		int rst = it.Use(GetLocation(),index, obj);
		if(rst < 0) return -3;
		ASSERT(it.type == type && it.count > 0);
		count = rst;
		if(rst > 0)
		{
			if(it.count <= (size_t)rst)
			{
				it.TakeOut(_location,index,_owner);
				it.Release();
				_empty_slot_count ++;
			}
			else
			{
				it.count -= rst;
			}
		}
		return type;
	}

	int UseItemWithArg(int index,gactive_imp * obj,int &count, const char * arg, size_t arg_size)
	{	
		if(_list[index].type == -1) 
		{
			return -1;
		}

		item & it = _list[index];
		int type = it.type;
		if(!it.CanUseWithArg(GetLocation(), arg_size,obj))
		{
			return -2;
		}
		int rst = it.Use(GetLocation(),index, obj, arg, arg_size);
		if(rst < 0) return -3;
		ASSERT(it.type == type && it.count > 0);
		count = rst;
		if(rst > 0)
		{
			if(it.count <= (size_t)rst)
			{
				it.TakeOut(_location,index,_owner);
				it.Release();
				_empty_slot_count ++;
			}
			else
			{
				it.count -= rst;
			}
		}
		return type;
	}

	int UseItemWithTarget(int index,gactive_imp * obj, const XID & target,char force_attack, int & count)
	{	
		if(_list[index].type == -1) 
		{
			return -1;
		}

		item & it = _list[index];
		int type = it.type;
		if(!it.CanUseWithTarget(GetLocation(),obj))
		{
			return -2;
		}
		int rst = it.UseWithTarget(GetLocation(),index ,obj,target,force_attack);
		if(rst < 0) return -3;
		count = 0;
		if(rst > 0)
		{
			if(it.count <= (size_t)rst)
			{
				count = it.count;
				it.TakeOut(_location,index,_owner);
				it.Release();
				_empty_slot_count ++;
			}
			else
			{
				count = rst;
				it.count -= rst;
			}
		}
		return type;
	}

	template<typename FUNC>
	void ForEachExpireItems(FUNC & func)
	{
		for(size_t i = 0; i < _list.size(); i ++)
		{
			item & it = _list[i];
			if(it.type == -1 || it.expire_date == 0) continue;
			func(this, i, it);
		}
	}

};

inline int MoveBetweenItemList(item_list & src, item_list & dest, size_t src_idx, size_t dest_idx,size_t count)
{
	if(src_idx >= src.Size() || dest_idx >= dest.Size())
	{
		return -1;
	}
	
	if(!count) return -1;

	if(src[src_idx].type == -1 ||
			(dest[dest_idx].type != -1 && (src[src_idx].type != dest[dest_idx].type || src[src_idx].proc_type != dest[dest_idx].proc_type)))
	{
		return -1;
	}

	if(!src[src_idx].CanMove() || !dest[dest_idx].CanMove()) return -1;

	item &it = src[src_idx];
	if(count > it.count) count = it.count;
	//�ж��Ƿ��ý���ʵ��
	if(dest[dest_idx].type == -1)
	{
		if(count == it.count)
		{
			item tmp;
			src.Exchange(src_idx,tmp);	
			dest.Exchange(dest_idx,tmp);
			src.Exchange(src_idx,tmp);	
		}
		else
		{
			item tmp = it;
			tmp.count = count;
			tmp.content.BuildFrom(it.content);
			bool bRst = dest.Put(dest_idx,tmp);
			ASSERT(bRst);
			src.DecAmount(src_idx,count);
		}
		return count;
	}

	//�������ƶ�
	int delta = dest.IncAmount(dest_idx,count);
	if(delta < 0)
	{
		//��Ϊ�Ѿ��жϹ���
		ASSERT(false);
		return -1;
	}
	src.DecAmount(src_idx,delta);
	return delta;
}


// ��������Ʒ������ͨ������ �����Ƕѵ�
inline int MoveAllItemBetweenItemList(item_list &src, item_list &dest)
{
	if(src.Size() - src.GetEmptySlotCount() > dest.GetEmptySlotCount()){
		return -1;
	}

	int j = 0;
	for(size_t i=0; i< src.Size(); i++){
		if(src[i].type == -1) continue;
		j = dest.PushInEmpty(j, src[i]);
		j++;
	}
	src.Clear();
	return 0;
}

inline void MoveMaxItemBetweenItemList(item_list &src, item_list &dest)
{
	int j = 0;
	for(size_t i=0; i< src.Size(); i++)
	{
		if(src[i].type == -1) continue;
		if( dest.GetEmptySlotCount() == 0 ){ return; }
		item it;
		src.Remove( i, it );
		j = dest.PushInEmpty(j, it);
		j++;
	}
}

int GetStoneColorLevel(int id, int & color);
#endif
