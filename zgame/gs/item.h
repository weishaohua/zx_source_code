#ifndef __ONLINEGAME_GS_ITEM_H__
#define __ONLINEGAME_GS_ITEM_H__

#include <algorithm>
#include <vector.h>
#include "substance.h"
#include "property.h"
#include "matter.h"
#include "actobject.h"
#include <common/packetwrapper.h>
#include <db_if.h>
#include "itemdata.h"
#include <crc.h>

class item;
class item_body;
class item_list;
struct equip_essence_new;
class equip_item;
class gplayer_imp;

/** 
 * ��Ʒģ��
 * ��û�������Ʒ��������ʵ��
 */
 /*
struct item_template
{
	int type;		//����
	int pile_limit;		//�ѵ�����
	int equip_mask;		//װ����־	��ʾ����װ�����ĸ��ط�
	int guid;		//��Ӧitem�����GUID �������-1����ʾ����Ҫitem����
	int price;		//���ۣ������Ǳ仯�ģ������body����body����ֵΪ׼
};
*/
struct exchange_item_info
{
	exchange_item_info(unsigned char src,unsigned char dst):isrc(src),idst(dst) {}
	unsigned char isrc; //ԭ����λ��
	unsigned char idst; //��������յ�λ��
};

enum
{
	ITEM_INIT_TYPE_TASK,	//�����ʼ��
	ITEM_INIT_TYPE_SHOP,	//�̳ǳ�ʼ��
	ITEM_INIT_TYPE_NPC,	//NPC������
	ITEM_INIT_TYPE_LOTTERY,	//��Ʊ
	ITEM_INIT_TYPE_NPC_DROP,//NPC����
	ITEM_INIT_TYPE_COMBINE,	//�ϳ�
	ITEM_INIT_TYPE_PRODUCE,	//����
	ITEM_INIT_TYPE_CATCH, 	//������׽
};


inline item_data * DupeItem(const item_data & data)
{
	size_t len = sizeof(item_data) + data.content_length;
	item_data * pData=(item_data*)abase::fast_allocator::align_alloc(len);
	*pData = data;
	pData->item_content = ((char*)pData) + sizeof(item_data);
	if(data.content_length)
	{
		memcpy(pData->item_content,data.item_content,data.content_length);
	}
	return pData;
}

inline void FreeItem(item_data * pData)
{
	ASSERT(!pData->content_length || 
		pData->content_length && pData->item_content == ((char*)&(pData->item_content)) + sizeof(char*));
	
	size_t len = sizeof(item_data) + pData->content_length;
	abase::fast_allocator::align_free(pData,len);
}


/*
 *	����Ʒ����������Ʒ��Ŀ�ĺ���
 */
bool MakeItemEntry(item& entry,const item_data & data);
bool MakeItemEntry(item& entry,const GDB::itemdata &data); //�����ݿ������������Ŀ�ĺ���
bool MakeItemEntry(item& entry, const GDB::pocket_item &data);
void ConvertDBItemEntry(GDB::itemdata &data, const item& entry, int index); //����Ʒ����Ŀת�������ݿ����Ŀ ע������buf����item ����󶨵�
void ConvertDBItemEntry(GDB::pocket_item &data, const item& it, int index);
void ConvertItemToData(item_data & data,const item & it); // ע������buf����item ����󶨵�
void ConvertItemToDataForClient(item_data_client & data,const item & it); // ע������buf����item ����󶨵�

struct item_content 
{
	char * buf; 
	size_t size;
	item_content():buf(0),size(0){}
	void Release()
	{
		if(buf) abase::fastfree(buf,size);
		buf = 0;
		size = 0;
	}
	void Clear()
	{
		buf = 0;
		size = 0;
	}

	void BuildFrom(const item_content & rhs)
	{
		buf = rhs.buf;
		size = rhs.size;
		if(size)
		{
			buf = (char*)abase::fastalloc(size);
			memcpy(buf, rhs.buf, size);
		}
	}

	void BuildFrom(const void * __buf, size_t __len)
	{
		if(__len)
		{
			buf = (char*)abase::fastalloc(__len);
			size = __len;
			memcpy(buf, __buf, __len);
		}
		else
		{
			__buf = 0;
			__len = 0;
		}
	}

};
��
struct item
{
	int type;		//��Ʒ������
	size_t count;		//��Ʒ������
	size_t pile_limit;	//�ѵ�����
	int equip_mask;		//װ����־	��ʾ����װ�����ĸ��ط� 0x8000 ��λ�ñ�ʾ�Ƿ���Ƕ�� 
	int proc_type;		//��Ʒ�Ĵ���ʽ
	size_t price;		//����
	int expire_date;	//����ʱ�䣬���<=0���޵���ʱ��
	struct 
	{
		int guid1;
		int guid2;
	}guid;
	char is_active;		//�Ƿ񼤻�
	item_body * body;
	item_content content;

	inline	void GetItemData(const void **data, size_t & size) const;
	//inline void GetItemDataForClient(const void **data, size_t & size) const;
	inline void GetItemDataForClient(item_data_client& data) const;
	
	enum
	{
		EQUIP_MASK_WEAPON		= 0x00001,
		EQUIP_MASK_HEAD			= 0x00002,
		EQUIP_MASK_BODY			= 0x00004,
		EQUIP_MASK_FOOT			= 0x00008,
		EQUIP_MASK_NECK			= 0x00010,
		EQUIP_MASK_FINGER1		= 0x00020,
		EQUIP_MASK_FINGER2		= 0x00040,
		EQUIP_MASK_FASHION_EYE		= 0x00080,	
		EQUIP_MASK_MAGIC		= 0x00100,
		EQUIP_MASK_FASHION_LIP		= 0x00200,
		EQUIP_MASK_FASHION_HEAD		= 0x00400,
		EQUIP_MASK_FASHION_BODY		= 0x00800,
		EQUIP_MASK_FASHION_LEG		= 0x01000,
		EQUIP_MASK_FASHION_FOOT		= 0x02000,
		EQUIP_MASK_FASHION_BACK		= 0x04000,
		EQUIP_MASK_WING			= 0x08000,
		EQUIP_MASK_TALISMAN1		= 0x10000,
		EQUIP_MASK_TALISMAN2		= 0x20000,
		EQUIP_MASK_TALISMAN3		= 0x40000,
		EQUIP_MASK_BUGLE		= 0x80000,
		EQUIP_MASK_XITEM1		= 0x0100000,
		EQUIP_MASK_XITEM2		= 0x0200000,
		EQUIP_MASK_XITEM3		= 0x0400000,
		EQUIP_MASK_RUNE			= 0x0800000,
		EQUIP_MASK_XITEM4		= 0x1000000,
		EQUIP_MASK_XITEM5		= 0x2000000,
		EQUIP_MASK_FASHION_WEAPON	= 0x4000000,  // Youshuang add
		
		EQUIP_MASK_CAN_BIND		= 0xFFFFFF,

		EQUIP_INDEX_WEAPON		= 0,
		EQUIP_INDEX_HEAD		= 1,
		EQUIP_INDEX_BODY		= 2,
		EQUIP_INDEX_FOOT		= 3,
		EQUIP_INDEX_NECK		= 4,
		EQUIP_INDEX_FINGER1		= 5,
		EQUIP_INDEX_FINGER2		= 6,
		EQUIP_INDEX_FASHION_EYE		= 7,	
		EQUIP_INDEX_MAGIC		= 8,
		EQUIP_INDEX_FASHION_LIP		= 9,
		EQUIP_INDEX_FASHION_HEAD	= 10,
		EQUIP_INDEX_FASHION_BODY	= 11,
		EQUIP_INDEX_FASHION_LEG		= 12,
		EQUIP_INDEX_FASHION_FOOT	= 13,
		EQUIP_INDEX_FASHION_BACK	= 14,
		EQUIP_INDEX_WING		= 15,
		EQUIP_INDEX_TALISMAN1		= 16,
		EQUIP_INDEX_TALISMAN2		= 17,
		EQUIP_INDEX_TALISMAN3		= 18,
		EQUIP_INDEX_BUGLE		= 19,
		EQUIP_INDEX_XITEM1		= 20,		//����
		EQUIP_INDEX_XITEM2		= 21,		//���
		EQUIP_INDEX_XITEM3		= 22,		//���
		EQUIP_INDEX_RUNE		= 23,		//Ԫ��
		EQUIP_INDEX_XITEM4		= 24,
		EQUIP_INDEX_XITEM5		= 25,
		EQUIP_INDEX_FASHION_WEAPON	= 26,		// Youshuang add for fashion weapon
		EQUIP_INVENTORY_COUNT,

		EQUIP_VISUAL_START	= EQUIP_INDEX_WEAPON,
		//EQUIP_VISUAL_END	= EQUIP_INDEX_RUNE + 1, 
		EQUIP_VISUAL_END	= EQUIP_INDEX_FASHION_WEAPON + 1,  // Youshuang add

		EQUIP_MASK_ALL  	= 0xFFFFFF,
	};

	enum
	{
		ITEM_PROC_TYPE_NODROP 		= 0x0001,	//����ʱ������
		ITEM_PROC_TYPE_NOTHROW2		= 0x0002,	//�޷����ڵ���  down
		ITEM_PROC_TYPE_NOSELL		= 0x0004,	//�޷�����NPC   down
		ITEM_PROC_TYPE_CASHITEM 	= 0x0008,	//���������Ʒ
		ITEM_PROC_TYPE_NOTRADE2 	= 0x0010,	//��Ҽ䲻�ܽ���
		ITEM_PROC_TYPE_TASKITEM 	= 0x0020,	//��������Ʒ
		ITEM_PROC_TYPE_BIND2		= 0x0040,	//װ�����󶨵���Ʒ
		ITEM_PROC_TYPE_BIND		= 0x0080,	//���Ѿ��󶨵���Ʒ
		ITEM_PROC_TYPE_GUID		= 0x0100,	//Ӧ����GUID
		ITEM_PROC_TYPE_RECYCLE		= 0x0200,	//�Ƿ���ս��������Ʒ
		ITEM_PROC_TYPE_ZONETRADE 	= 0x0400,	//������Խ���
		ITEM_PROC_TYPE_ZONEUSE		= 0x0800,	//���ڿ������ʹ��
		ITEM_PROC_TYPE_ZONENOUSE 	= 0x1000,	//���������ʹ��
		ITEM_PROC_TYPE_NOTOTRASH	= 0X2000,	//���������ֿ�
		ITEM_PROC_TYPE_LOCK		= 0x80000000,	//��������Ʒ
	};

	enum
	{
		REINFORCE_SUCCESS,		//�����ɹ�
		REINFORCE_UNAVAILABLE,		//��������
		REINFORCE_FAILED_LEVEL_0,	//����ʧ�� ������ʧ ���𽵵�һ��
		REINFORCE_FAILED_LEVEL_1,	//����ʧ�� ������ʧ ������ʧ
		REINFORCE_FAILED_LEVEL_2,	//����ʧ�� ������ʧ ��Ʒ��ʧ
		REINFORCE_FAILED_LEVEL_00,	//����ʧ�� ������ʧ ��������

		REMOVE_REINFORCE_SUCCESS = 0,
		REMOVE_REINFORCE_OUT_OF_FUND,
		REMOVE_REINFORCE_FAILD,
	};


private:
	friend class item_list;

public:
	item():type(-1),count(0),pile_limit(0),equip_mask(0),proc_type(0),price(0),expire_date(0),is_active(false),body(0){}
			       
	~item()
	{
		ASSERT(body == NULL);
	}
	inline void Release();
	inline void Clear();

	void SetContent(const void * data, size_t size)
	{
		content.Release();
		if(size)
		{
			content.BuildFrom(data,size);
		}
	}

	void * GetContent(size_t & len)
	{
		len = content.size;
		return content.buf;
	}

	const void * GetContent(size_t & len) const
	{
		len = content.size;
		return content.buf;
	}
	

	bool Save(archive & ar);
	bool Load(archive & ar);

	void LocateBody();
	
	enum LOCATION
	{
		INVENTORY,
		BODY,
		TASK_INVENTORY,
		BACKPACK,
		TEMP_INV,
		PET_BEDGE_INVENTORY,
		PET_EQUIP_INVENTORY,
		POCKET_INVENTORY,
		FASHION_INVENTORY,
		MOUNT_WING_INVENTORY,
		GIFT_INVENTORY,
		FUWEN_INVENTORY,
	};

	bool IsEquipment()  { return equip_mask & EQUIP_MASK_ALL;}
	
	inline bool IsActive() { return is_active; }
	inline void SetActive(bool active) { is_active = active;}
	inline bool CanTrade() const { return !(proc_type & (ITEM_PROC_TYPE_NOTRADE2 | ITEM_PROC_TYPE_BIND | ITEM_PROC_TYPE_LOCK)); }
	inline bool CanThrow() const { return !(proc_type & (ITEM_PROC_TYPE_NOTRADE2 | ITEM_PROC_TYPE_BIND | ITEM_PROC_TYPE_LOCK | ITEM_PROC_TYPE_NOTHROW2)); }
	inline bool CanDrop() const { return !(proc_type & (ITEM_PROC_TYPE_NODROP)); }
	inline bool CanSell() const { return !(proc_type &(ITEM_PROC_TYPE_NOSELL|ITEM_PROC_TYPE_LOCK));}
	inline bool CanRecycle() const { return (proc_type & ITEM_PROC_TYPE_RECYCLE) && !(proc_type & (ITEM_PROC_TYPE_CASHITEM | ITEM_PROC_TYPE_GUID)); }
	inline bool CanPutToTrash() const { return !(proc_type & ITEM_PROC_TYPE_NOTOTRASH); }
	inline bool IsBindOnEquip() const { return proc_type & ITEM_PROC_TYPE_BIND2; }
	inline bool IsEraseOnDrop() const { return !CanTrade();}
	inline bool IsBind() const { return proc_type & ITEM_PROC_TYPE_BIND; }
	inline void Bind()
	{
		proc_type |= ITEM_PROC_TYPE_NOTHROW2| ITEM_PROC_TYPE_NOTRADE2 | ITEM_PROC_TYPE_BIND;
	}

	inline bool CanLock() const { return !(proc_type & ITEM_PROC_TYPE_LOCK) && pile_limit == 1; }
	inline bool IsLocked() const { return proc_type & ITEM_PROC_TYPE_LOCK; }
	inline void Lock() { proc_type |= ITEM_PROC_TYPE_LOCK; }
	inline void ClearLockFlag() { proc_type &= ~ITEM_PROC_TYPE_LOCK; }



	inline void BindOnEquip() { proc_type |= ITEM_PROC_TYPE_NOTHROW2| ITEM_PROC_TYPE_NOTRADE2 | ITEM_PROC_TYPE_BIND; proc_type &= ~ITEM_PROC_TYPE_BIND2; } 
	

	static inline void Bind(int & __proc_type) { __proc_type |= ITEM_PROC_TYPE_NOTHROW2| ITEM_PROC_TYPE_NOTRADE2 | ITEM_PROC_TYPE_BIND; }
	static inline void CustomBind(int & __proc_type) { __proc_type |= ITEM_PROC_TYPE_NOTHROW2| ITEM_PROC_TYPE_NOSELL | ITEM_PROC_TYPE_NOTRADE2; }
	static inline void CustomLock(int & __proc_type) { __proc_type |= ITEM_PROC_TYPE_LOCK;}
	// Youshuang add
	static inline void BindPileItem( int& proc_type )
	{
		if( proc_type & item::ITEM_PROC_TYPE_NOTRADE2 ){ proc_type |= item::ITEM_PROC_TYPE_BIND; }
	}
	// end

/* Ҫת����body�ĺ��� */
	inline bool CheckAttack(item_list & list) const;  
	inline bool AddColor( int color, int quality );  // Youshuang add
	inline void AfterAttack(item_list & list,bool * pUpdate) const;  
	inline bool CanActivate(item_list & list,gactive_imp * obj) const;
	inline bool CanUse(item::LOCATION l,gactive_imp* pImp);
	inline bool CheckZoneUse(item::LOCATION l, gactive_imp *pImp);	//����Ƿ��������Ʒ
	inline bool SitDownCanUse(item::LOCATION l,gactive_imp* pImp);
	inline bool IsBroadcastUseMsg();
	inline bool IsBroadcastArgUseMsg();
	inline bool CanUseWithTarget(item::LOCATION l,gactive_imp* pImp);
	inline bool CanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp);
	inline int  GetUseDuration();
	inline void Activate(size_t index,item_list & list, gactive_imp * obj);
	inline void Deactivate(size_t index,gactive_imp * obj);
	inline void ActivateProp(size_t index, gactive_imp * obj);
	inline void DeactivateProp(size_t index,gactive_imp * obj);
	inline void Init(item::LOCATION l,size_t index,gactive_imp * obj);
	inline void PutIn(item::LOCATION l,size_t index,gactive_imp * obj);
	inline void TakeOut(item::LOCATION l,size_t index, gactive_imp * obj);
	inline int Use(item::LOCATION l,size_t index, gactive_imp * obj);
	inline int Use(item::LOCATION l,size_t index, gactive_imp * obj, const char * arg, size_t arg_size);
	inline int UseWithTarget(item::LOCATION l,size_t index, gactive_imp * obj,const XID & target, char force_attack);
	inline unsigned int GetCRC() const;
	inline int64_t GetIdModify() const ; 	//����ID����װ��ʹ��
	inline void InitFromShop(gplayer_imp* pImp,int value);	//��һ�δ��̵����룬���еĳ�ʼ������
	inline int GetItemType() const;
	inline bool NeedUseLog() const;
	inline int GetProctypeState()
	{
		return Proctype2State(proc_type);
	}

	inline static int Proctype2State(const item_data & data)
	{
		return Proctype2State(data.proc_type);
	}
	inline static int Proctype2State(int proc_type)
	{
		int state = 0;
		if(proc_type & ITEM_PROC_TYPE_BIND) state = 0x001;
		if(proc_type & ITEM_PROC_TYPE_BIND2) state |= 0x002;
		if(proc_type & ITEM_PROC_TYPE_LOCK) state |= 0x004;
		return state;
	}
	inline bool LotteryCashing(gactive_imp *,int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period);
	inline bool NeedGUID()
	{
		return proc_type & ITEM_PROC_TYPE_GUID;
	}
	
	bool CanMoveToPocket() const;
	bool CanMove() const;
	inline int GetEquipType() const; 
	inline int GetClientSize() const;
	inline bool GetTransformInfo(unsigned char & level, unsigned char & exp_level, float & energy_drop_speed, int & exp_speed, int & duration); 
	inline int GetReinforceLevel() const;
	inline bool InitReinforceLevel(int level);
	inline int GetSpiritLevel() const;

	inline void ChangeOwnerName(item::LOCATION l,size_t index,gactive_imp * obj);
	inline void ChangeOwnerID(item::LOCATION l,size_t index,gactive_imp * obj);

};

class item_body : public substance
{
protected:
	int _tid;		//��Ʒ����
	friend class item;

public:
	DECLARE_SUBSTANCE(item_body);
	enum ITEM_TYPE
	{
		// ��������
		ITEM_TYPE_NULL = -1, 	//��ʾ��Ч��δ��ʼ������Ʒ����
		ITEM_TYPE_EQUIPMENT, 	//װ�����ͣ����ܰ������������ߵȡ�
		ITEM_TYPE_REINFORCE, 	//ǿ�����ͣ���������װ�����Ե���Ʒ��
		ITEM_TYPE_DUMMY,		//������Ʒ���������ڲ��Ի�ռλ��
		ITEM_TYPE_RECIPE,		//�䷽���ͣ���������������Ʒ���䷽��

		//������Ʒ
		ITEM_TYPE_POPPET,		//��Ʒ��������
		ITEM_TYPE_POPPET_ITEM,	//��Ʒ��������
		ITEM_TYPE_POPPET_EXP,	//������������
		ITEM_TYPE_TELEPORT,		//�������ͣ����ڽ���ɫ���͵�ָ��λ�á�
		ITEM_TYPE_POTION,		//ҩˮ���ͣ����ڻָ�����ֵ��ħ��ֵ��

		//��Ϸ������Ʒ
		ITEM_TYPE_TASKDICE,		//�������ӣ���������ĳЩ������е�����¼���
		ITEM_TYPE_DBL_EXP,		//˫��������Ʒ��������ʱ���ӻ�õľ���ֵ��
		ITEM_TYPE_PK_RESET,		//PK��Player Killing��������Ʒ���������������ɫ��PKֵ��
		ITEM_TYPE_SKILL,		//�����������Ʒ������ѧϰ���������ܡ�
		ITEM_TYPE_MOUNT,		//�������ͣ����ڽ�ɫ��˵����
		ITEM_TYPE_COUPLE_JUMP,	//������Ծ�������������¼�Ĵ��ͻ򻥶���
		ITEM_TYPE_LOTTERY,		//�齱��Ʒ�����ڲ���齱���

		//���ֺ�װ����Ʒ
		ITEM_TYPE_FIREWORKS,	//�̻����ͣ����ڷ��̻���
		ITEM_TYPE_RECORDER,		//¼�������������ڼ�¼��Ϸ�е�������������Ϣ��
		ITEM_TYPE_CD_ERASER,	//CD�������������������ĳЩ��Ʒ���ܵ���ȴʱ�䡣

		//���������Ʒ
		ITEM_TYPE_PET,					//�������ͣ������ٻ��������
		ITEM_TYPE_TALISMAN,				//��������ͣ������ṩĳЩ����������Ч����
		ITEM_TYPE_BUGLE,				//�Žǣ��������ڷ���ȫ��������ټ����ѡ�
		ITEM_TYPE_TOWN_SCROLL,			//������ᣬ���ڿ��ٷ��س���
		ITEM_TYPE_TOWN_SCROLL_PAPER,	//�������Ĳ��ϻ��䷽��
		ITEM_TYPE_TANK_CONTROL,			//̹�˿��������������ڿ���ĳЩ��е���ؾߡ�
		ITEM_TYPE_PET_BEDGE,			//������£���������������������Ի�ȼ���
		ITEM_TYPE_PET_EQUIP,			//����װ�������ڸ����ﴩ����װ����
		ITEM_TYPE_PET_FOOD,				//����ʳ�����ι�����
		ITEM_TYPE_PET_AUTO_FOOD,		//�Զ�ιʳ�������������Զ�ι�����
		ITEM_TYPE_PET_REFINE,			//����������Ʒ�����������������ԡ�
		ITEM_TYPE_PET_ASSIST_REFINE,	//���︨��������Ʒ���������ڸ�������������

		//��ͨ��������Ʒ
		ITEM_TYPE_AIRCRAFT,				//���������ͣ����ڽ�ɫ���е��ؾߡ�
		ITEM_TYPE_OFFLINE_AGENT,		//���ߴ��������������߹����ɫ��ĳЩ���ܡ�
		ITEM_TYPE_SPECIAL_NAME,			//����������Ʒ���������ڸ��Ľ�ɫ���������֡�
		ITEM_TYPE_GIFTBOX,				//��Ʒ�У��������ͻ�򿪻�ý�����
		ITEM_TYPE_VIPCARD,				//VIP���������ṩĳЩ��Ȩ������Ч����
		ITEM_TYPE_MAGIC,				//ħ����Ʒ����������ʩ��ĳЩħ��Ч����
		ITEM_TYPE_ASSIST_CARD,			//�����������������ṩĳЩ�������ܡ�
		ITEM_TYPE_TELESTATION,			//����վ�����ڿ��ٴ��͵�ĳЩ�ص㡣
		ITEM_TYPE_LOTTERY2,				//�����齱��Ʒ�����ڲ���߼��齱���
		ITEM_TYPE_LOTTERY3,				//�����齱��Ʒ�����ڲ�����߼��ĳ齱���
		ITEM_TYPE_ONLINEAWARD_GIFTBOX,	//���߽�����Ʒ�У�������������ʱ��������
		ITEM_TYPE_TREASURE_DIGCOUNT,	//�����ھ���������������ھ򱦲صĴ������ơ�
		ITEM_TYPE_RUNE,					//�������ͣ������ṩĳЩ����Ч����
		ITEM_TYPE_ASTROLOGY,			//ռ����Ʒ����������ռ����Ԥ�⡣
		ITEM_TYPE_BATTLEFLAG,			//ս�����ͣ���������ս���е�ĳЩ����Ч����
		ITEM_TYPE_ASTROLOGY_ENERGY,		//ռ����������������ռ�ǻ��
		ITEM_TYPE_BOTTLE,				//ƿ�����ͣ���������װĳЩҺ�����Ʒ��
		ITEM_TYPE_SALE_PROMOTION,		//������Ʒ���������ڴ��ۻ�������
        ITEM_TYPE_GIFTBOX_DELIVERY,		//��Ʒ�е��ͣ��������ڵ�����Ʒ�С�
		ITEM_TYPE_LOTTERY_TY,			//�ر�齱��Ʒ�����ڲ�������齱���
		ITEM_TYPE_GIFTBOX_SET,			//��Ʒ����װ�����ܰ��������Ʒ�С�
		ITEM_TYPE_PROPADD,				//����������Ʒ������������ɫ���ԡ�
		ITEM_TYPE_FUWEN,				//�������ͣ�����������Ƕ������װ�����ԡ�
		ITEM_TYPE_COLORANT,  			//Ⱦ�����ͣ����ڸı���Ʒ��װ������ɫ��
	};

	inline void SetTID(int tid) 
	{
		_tid = tid;
	}
	inline int GetTID() const
	{
		return _tid;
	}
public:
	item_body():_tid(0){}
	virtual ~item_body(){}
	
	bool CheckAttack(item_list & list) const { return OnCheckAttack(list);}
	void AfterAttack(item_list & list,bool *pUpdate) const { return OnAfterAttack(list,pUpdate);}
	bool CanActivate(item_list & list,gactive_imp * obj,const item * parent) const {return VerifyRequirement(list,obj,parent);}
	bool CanUse(item::LOCATION l,gactive_imp* pImp) const { return IsItemCanUse(l,pImp);}
	bool SitDownCanUse(item::LOCATION l,gactive_imp* pImp) const { return IsItemSitDownCanUse(l,pImp);}
	bool IsBroadcastUseMsg() const { return IsItemBroadcastUse();}
	bool IsBroadcastArgUseMsg() const { return IsItemBroadcastArgUse();}
	bool CanUseWithTarget(item::LOCATION l,gactive_imp* pImp) const { return IsItemCanUseWithTarget(l,pImp);}
	bool CanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) { return IsItemCanUseWithArg(l, buf_size,pImp);}

	void Activate(size_t index, item_list & list,gactive_imp * obj, item * parent) const
	{
		if(!parent->IsActive() && CanActivate(list,obj,parent) )
		{
			OnActivate(index,obj,parent);
			parent->SetActive(true);
		}
	}

	void Deactivate(size_t index,gactive_imp * obj, item * parent) const
	{
		if(parent->IsActive())
		{
			OnDeactivate(index,obj,parent);
			parent->SetActive(false);
		}
	}


	int Use(item::LOCATION l,size_t index, gactive_imp * obj,item * parent) const
	{
		return OnUse(l,index, obj,parent);
	}

	int Use(item::LOCATION l,size_t index, gactive_imp * obj,item * parent, const char * arg, size_t arg_size) const
	{
		return OnUse(l,index, obj,parent,arg, arg_size);
	}

	int UseWithTarget(item::LOCATION l, size_t index, gactive_imp * obj,const XID & target, char force_attack,item * parent) const
	{
		return OnUseWithTarget(l,index,obj,target,force_attack,parent);
	}

	int GetUseDuration() const
	{
		return OnGetUseDuration();
	}
	
	inline int GetEquipMask() const { return OnGetEquipMask();}
	inline int GetEquipType() const { return OnGetEquipType();}
	
protected:
	void Init(item::LOCATION l,size_t index, gactive_imp * obj,item * parent) const
	{
		OnInit(l,index,obj,parent);
	}

	void PutIn(item::LOCATION l,size_t index, gactive_imp * obj,item * parent) const
	{
		OnPutIn(l,index,obj,parent);
	}

	void TakeOut(item::LOCATION l,size_t index, gactive_imp * obj,item * parent) const
	{	
		OnTakeOut(l,index, obj,parent);
	}
	
	void ChangeOwnerName(item::LOCATION l,size_t index, gactive_imp * obj,item * parent) const
	{
		OnChangeOwnerName(l,index,obj,parent);
	}

	void ChangeOwnerID(item::LOCATION l,size_t index, gactive_imp * obj,item * parent) const
	{
		OnChangeOwnerID(l,index,obj,parent);
	}
public:
public:
	virtual ITEM_TYPE GetItemType() const = 0;
	virtual bool IsNeedCRC() const { return true;}
	virtual int64_t GetIdModify(const item * parent) const {return 0;}
	virtual void InitFromShop(gplayer_imp* pImp,item * parent, int value) const {}

	virtual bool DoBloodEnchant(size_t index, gactive_imp * obj, item * parent) const { return false;}
	virtual int InsertSpiritAddon(gactive_imp * obj, item * parent, int addon_id,size_t addon_slot) const { return -1;}
	virtual int RemoveSpiritAddon(gactive_imp * obj, item * parent, size_t addon_slot, int * removed_soul) const { return -1;}
	virtual int RestoreSpiritPower(gactive_imp * obj, item * parent) const { return -1;}
	virtual int SpiritDecompose(gactive_imp * obj, item * parent, int *spirit_unit1, int * spirit_unit2) const { return -1;}
	virtual int DecSpiritPower(gactive_imp * obj, item* parent, int count,int index) const { return -1;}
	virtual int GetSpiritPower(item* parent) const { return -1;}

	virtual bool AddColor( item* parent, int color, int quality ){ return false; }  // Youshuang add

	virtual int DecMagicDuration(gactive_imp * obj, item* parent, int count,int index) const { return -1;}

	virtual int ReinforceActivate(size_t,const equip_item *,const equip_essence_new &, gactive_imp*, float adjust_ratio, int adjust_point, const item* parent){return 0;}
	virtual int ReinforceDeactivate(size_t,const equip_item *,const equip_essence_new &, gactive_imp*, float adjust_ratio,int adjust_point, const item* parent){return 0;}
	virtual int DoReinforce(const equip_item * item, equip_essence_new & value, gactive_imp * pImp, const float aoffset[4], int & level_result,int & fee,int &level_before, bool newmode, int require_level, const float adjust2[20]) const
	{ 
		return item::REINFORCE_UNAVAILABLE;
	}
	virtual int ReinforceEquipment(item * parent, gactive_imp * pImp, int stone_id, const float aoffset[4], int & level_result, int & fee, int & level_before,bool newmode, int require_level, const float adjust2[20]) const
	{
		return item::REINFORCE_UNAVAILABLE;
	}
	virtual int RemoveReinforce(item * parent, gactive_imp * pImp , size_t & money ,bool rp) const
	{
		money = 0;
		return item::REMOVE_REINFORCE_FAILD;
	}
	virtual int GetReinforceLevel(const item * parent) const
	{
		return 0;
	}
	virtual bool InitReinforceLevel(item * parent, int level)
	{
		return false;
	}
	virtual int GetSpiritLevel() const
	{
		return 0;
	}
	virtual void ModifyRequirement(int & level , int &require_level, int64_t &cls, int64_t &cls_require, int64_t &cls_require1, int value){}
	virtual int GainExp(item::LOCATION, int exp, item * parent, gactive_imp * pImp, int index, bool& level_up)  const
	{
		return -1;
	}
	virtual bool LevelUp(item * parent, gactive_imp * pImp) const
	{
		return false;
	}

	virtual void GetRefineSkills(item* parent, int& level, std::vector<short>& skills) const
	{
		level = 0;
	}

	virtual void GetEmbedAddons(item* parent, std::vector<int>& addons) const
	{
	}

	virtual bool IsHolyLevelup(item* parent) const
	{
		return false;
	}

	virtual bool ItemReset(item * parent, gactive_imp * pImp) const
	{
		return false;
	}
	virtual bool RestoreDuration(item * parent, gactive_imp * pImp) const
	{
		return false;
	}
	virtual bool Refine(item *parent, gactive_imp *pImp, size_t index1, size_t index2)
	{
		return false;
	}

	virtual bool Charge(item *parent, gactive_imp *pImp, size_t index1, size_t index2, int day)
	{
		return false;
	}

	virtual bool Identify(item *parent, gactive_imp *imp, size_t index1, size_t index2)
	{
		return false;
	}
	
	virtual bool Combine(item *parent, gactive_imp *pImp, size_t index1, size_t index2)
	{
		return false;
	}

	
	virtual bool Reset(item *parent, gactive_imp *imp, size_t index1, size_t index2)
	{
		return false;
	}

	virtual int GetItemSkillLevel(int skill_id, gactive_imp *pImp) const {return -1;}
	virtual bool IsConsumableItemSkill() const {return false;}
	
	virtual bool UpgradeEquipment(item * parent, gactive_imp *pImp, int item_index, int item_type,  int output_id,
	   	bool is_special_rt, int special_level, int stone_index, int stone_id, int stone_num, int rt_index, int rt_id){return false;}

	//��ʯ��Ƕ��ؽӿ�
	virtual bool IdentifyGemSlots(size_t index, gactive_imp * obj, item * parent) const { return false; }
	virtual bool RebuildGemSlots(gactive_imp* obj, item* parent, bool locked[3]) const { return false; }
	virtual bool CustomizeGemSlots(gactive_imp * obj, item* parent, int gem_seal_id[3], int gem_seal_index[3]) const { return false; }
	virtual bool EmbededGems(gactive_imp * obj, item* parent, int gem_id[3], int gem_index[3]) const { return false; }
	virtual bool RemoveGems(gactive_imp * obj, item* parent, bool remove[3]) const { return false; }

	virtual void SetPotionCapacity(item* parent, int v) {}
	virtual int GetPotionCapacity(item* parent) const { return 0; }
	virtual int FillBottle(gactive_imp* obj, item* parent, size_t bottle_index, int potion_type, int potion_tid, int potion_value, size_t potion_canuse_zone, size_t potion_cannotuse_zone, size_t potion_cantrade_zone, int& potion_left) { return false; }

	virtual bool CanFillToBottle() const { return false; }
	virtual bool DeliveryGiftBox(gactive_imp* obj, item* parent) {return false; } 
	virtual void SetGiftBoxAccPoint(int p, item* parent) {}
	virtual int GetGiftBoxAccPoint(item* parent) { return 0; }
	virtual int GetPropAddMaterialId() { return -1; }

protected:
	virtual void OnInit(item::LOCATION ,size_t index , gactive_imp*,item * parent) const{}
	virtual void OnPutIn(item::LOCATION ,size_t index , gactive_imp*,item * parent) const{}
	virtual void OnTakeOut(item::LOCATION ,size_t index, gactive_imp*,item * parent) const{}
	virtual void OnChangeOwnerName(item::LOCATION ,size_t index , gactive_imp*,item * parent) const{}
	virtual void OnChangeOwnerID(item::LOCATION ,size_t index , gactive_imp*,item * parent) const{}
	virtual bool VerifyRequirement(item_list & list,gactive_imp*,const item * parent) const {return false;}
private:
	virtual int OnGetUseDuration() const { return -1;} //������������ʹ�ã��������Ŷ�
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return false;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l,gactive_imp* pImp) const { return false;}
	virtual bool IsItemCanUseWithTarget(item::LOCATION l,gactive_imp* pImp) const { return false;}
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return false;}
	virtual void OnActivate(size_t index,gactive_imp*, item * parent) const {}
	virtual void OnDeactivate(size_t index,gactive_imp*, item * parent) const {}
	virtual void OnActivateProp(size_t index, gactive_imp *obj, item *parent) const{}
	virtual void OnDeactivateProp(size_t index, gactive_imp *obj, item *parent) const{}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp*,item * parent) const{return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp*,item * parent,const char * arg, size_t arg_size) const{return -1;}
	virtual int OnUseWithTarget(item::LOCATION l,size_t index, gactive_imp * obj,const XID & target,char force_attack,item * parent) const{return -1;}
	virtual bool IsItemBroadcastUse() const {return false;}
	virtual bool IsItemBroadcastArgUse() const {return false;}
	virtual bool OnCheckAttack(item_list & list) const { return false;}
	virtual void OnAfterAttack(item_list & list,bool * pUpdate) const { ASSERT(false); return;}
	virtual int OnGetEquipMask() const { return 0;}
	virtual int OnGetEquipType() const { return -1;}
	virtual bool NeedUseLog() const { return true;}
	virtual void GetItemData(const void **data, size_t & size, const void * buf, size_t len) const
	{
		*data = buf;
		size = len;
	}
	/*virtual void GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const
	{
		*data = buf;
		size = len;
	}*/

	virtual void GetItemDataForClient(item_data_client & data, const void * buf, size_t len) const
	{
		data.item_content = (char*)buf;
		data.content_length = len;
	}

	virtual bool LotteryCashing(gactive_imp *, item * parent, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period)
	{
		return false;
	}
	virtual int GetClientSize(const void * buf, size_t len) const
	{
		return 0;
	}
	virtual bool GetTransformInfo(item * parent, unsigned char & level, unsigned char & exp_level, float & energy_drop_speed, int & exp_speed, int & duration){return false;}
	

};

inline void item::Release()
{
	content.Release();
	Clear();
}

inline void item::Clear()
{
	type = -1;
	count = 0;
	pile_limit = 0;
	equip_mask = 0;
	body = NULL;
	price = 0;
	expire_date = 0;
	content.Clear();
	is_active = false;
}

inline bool item::CheckAttack(item_list & list) const
{
	if(body) 
		return body->CheckAttack(list);
	else
		return true;
}

inline void item::AfterAttack(item_list & list,bool * pUpdate)  const
{
	if(body) body->AfterAttack(list,pUpdate);
}

inline bool item::CanActivate(item_list &list,gactive_imp * obj) const
{
	if(body)
		return body->CanActivate(list,obj,this);
	else
		return false;

}

// Youshuang add
inline bool item::AddColor( int color, int quality )
{
	if( body )
	{ 
		return body->AddColor( this, color, quality ); 
	}
	return false;
}
// end

inline bool item::CanUse(item::LOCATION l,gactive_imp* pImp)
{
	if(body)
		return body->CanUse(l,pImp);
	else 
		return false;
}

inline bool item::CheckZoneUse(item::LOCATION l, gactive_imp *pImp)
{
	if(pImp->IsZoneServer() && (proc_type & ITEM_PROC_TYPE_ZONENOUSE)) return false;	
	if(!pImp->IsZoneServer() && (proc_type & ITEM_PROC_TYPE_ZONEUSE)) return false;	
	return true;
}

inline bool item::SitDownCanUse(item::LOCATION l,gactive_imp* pImp)
{
	if(body)
		return body->SitDownCanUse(l,pImp);
	else 
		return false;
}

inline bool item::IsBroadcastUseMsg()
{
	if(body)
		return body->IsBroadcastUseMsg();
	else 
		return false;
}

inline bool item::IsBroadcastArgUseMsg()
{
	if(body)
		return body->IsBroadcastArgUseMsg();
	else 
		return false;
}

inline bool item::CanUseWithTarget(item::LOCATION l,gactive_imp* pImp)
{
	if(body)
		return body->CanUseWithTarget(l,pImp);
	else 
		return false;
}

inline bool item::CanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp)
{
	if(body)
		return body->CanUseWithArg(l,buf_size,pImp);
	else 
		return false;
}

inline int item::GetUseDuration()
{
	if(body)
		return body->GetUseDuration();
	else 
		return -1;
}

inline void item::Activate(size_t index,item_list & list,gactive_imp * obj)
{
	if(body) return body->Activate(index,list,obj,this);
}

inline void item::Deactivate(size_t index,gactive_imp * obj)
{
	if(body) return body->Deactivate(index,obj,this);
}

inline void item::ActivateProp(size_t index,gactive_imp * obj)
{
	if(body) return body->OnActivateProp(index,obj,this);
}

inline void item::DeactivateProp(size_t index,gactive_imp * obj)
{
	if(body) return body->OnDeactivateProp(index,obj,this);
}

inline void item::Init(item::LOCATION l,size_t index,gactive_imp * obj)
{
	if(body) body->Init(l,index,obj,this);
}

inline void item::PutIn(item::LOCATION l,size_t index,gactive_imp * obj)
{
	if(l != TEMP_INV)
	{
		if(expire_date > 0)
		{
			//������������Ʒ�����ʱ�򣬼���һ�����������ֵ
			obj->UpdateExpireItem(expire_date);
		}
	}

	//if(l != BACKPACK && l != TEMP_INV)
	if(l != TEMP_INV)
	{
		if(body) body->PutIn(l,index,obj,this);
	}
}

inline void item::TakeOut(item::LOCATION l,size_t index,gactive_imp * obj)
{	
	if(l != BACKPACK && l != TEMP_INV)
	{
		if(body) return body->TakeOut(l,index, obj,this);
	}
}

inline void item::ChangeOwnerName(item::LOCATION l,size_t index,gactive_imp * obj)
{
	if(body)body->ChangeOwnerName(l, index, obj, this);
}

inline void item::ChangeOwnerID(item::LOCATION l,size_t index,gactive_imp * obj)
{
	if(body)body->ChangeOwnerID(l, index, obj, this);
}


inline int item::Use(item::LOCATION l,size_t index ,gactive_imp * obj)
{
	if(body) 
		return body->Use(l,index, obj,this);
	else
		return 0;
}

inline int item::Use(item::LOCATION l,size_t index ,gactive_imp * obj, const char * arg, size_t arg_size)
{
	if(body) 
		return body->Use(l,index, obj,this, arg, arg_size);
	else
		return 0;
}

inline int item::UseWithTarget(item::LOCATION l,size_t index, gactive_imp * obj,const XID & target, char force_attack)
{
	if(body) 
		return body->UseWithTarget(l,index, obj,target,force_attack,this);
	else
		return 0;
}

inline unsigned int item::GetCRC() const
{
	if(body && !body->IsNeedCRC())  return 0;
	if(content.size == 0) return 0;
	return crc32((const char*) content.buf, content.size);
}

inline int64_t item::GetIdModify() const
{
	if(body) return body->GetIdModify(this);
	return 0;
}

inline void item::InitFromShop(gplayer_imp* pImp,int value)
{
	if(body) return body->InitFromShop(pImp,this, value);
	return;
}

inline void item::GetItemData(const void **data, size_t & size) const
{
	if(body)
	{
		body->GetItemData(data , size, content.buf, content.size);
	}
	else
	{
		*data = content.buf;
		size = content.size;
	}
}

/*inline void item::GetItemDataForClient(const void **data, size_t & size) const
{
	if(body)
	{
		body->GetItemDataForClient(data , size, content.buf, content.size);
	}
	else
	{
		*data = content.buf;
		size = content.size;
	}
}*/

inline void item::GetItemDataForClient(item_data_client& data) const
{
	if(body)
	{
		body->GetItemDataForClient(data, content.buf, content.size);
	}
	else
	{
		data.item_content = content.buf;
		data.content_length = content.size;
	}
}

inline int item::GetItemType() const
{
	if(body)
	{
		return body->GetItemType();
	}
	else
	{
		return item_body::ITEM_TYPE_NULL;
	}
}

inline bool item::NeedUseLog() const
{
	if(body)
	{
		return body->NeedUseLog();
	}
	else
	{
		return true;
	}
}


inline bool item::LotteryCashing(gactive_imp * obj, int *bonus_level, int * prize_money, int * prize_id, int * prize_count,int * prize_bind, int * prize_period)
{
	if(pile_limit != 1) return false;
	if(count != 1) return false;
	if(!body) return false;
	if(body->GetItemType() != item_body::ITEM_TYPE_LOTTERY && body->GetItemType() != item_body::ITEM_TYPE_LOTTERY_TY) return false;
	return body->LotteryCashing(obj, this, bonus_level, prize_money, prize_id, prize_count,prize_bind, prize_period);
}

inline int item::GetClientSize() const
{
	if(body)
	{
		return body->GetClientSize(content.buf, content.size);
	}
	return 0;
}
inline int item::GetEquipType() const
{
	if(body)
	{
		return body->GetEquipType();
	}
	else
	{
		return -1;
	}
}

inline bool item::GetTransformInfo(unsigned char & level, unsigned char & exp_level, float & energy_drop_speed, int & exp_speed, int & duration) 
{
	if(body)
	{
		return body->GetTransformInfo(this, level, exp_level, energy_drop_speed, exp_speed, duration);
	}
	return false;
}

inline	int item::GetReinforceLevel() const
{
	if(body)
	{
		body->GetReinforceLevel(this);
	}
	return 0;
}

inline	bool item::InitReinforceLevel(int level)
{
	if(body)
	{
		return body->InitReinforceLevel(this, level);
	}
	return false;
}

inline int item::GetSpiritLevel() const
{
	if(body)
	{
		body->GetSpiritLevel();
	}
	return 0;
}


/*
 *	�����ڵ��ϵ�item ,��matter����װ,����ʵ����Ӧ�Ĵ���
 */
class gmatter_item_imp : public gmatter_item_base_imp
{
	item_data * _data;
public:
	DECLARE_SUBSTANCE(gmatter_item_imp);
public:
	gmatter_item_imp():_data(NULL)
	{
	}
	virtual void Init(world * pPlane,gobject*parent)
	{
		ASSERT(_data && "Ҫ��SetData����\n");
		gmatter_item_base_imp::Init(pPlane,parent);
		gmatter * pMatter = (gmatter *) parent;
		pMatter->matter_type = _data->type;

	}

	~gmatter_item_imp();
	void SetData(const item_data & data);
	void AttachData(item_data * data);
	virtual int MessageHandler(const MSG & msg);
	virtual void OnPickup(const XID & who,int team_id,bool is_team);
	virtual void OnRelease();
};


class gmatter_item_controller : public  gmatter_controller
{
public:
	DECLARE_SUBSTANCE(gmatter_item_controller);
public:
};

void DropItemFromData(world * pPlane,const A3DVECTOR &pos,const item_data & data,const XID & owner, int owner_team,int seq,int drop_id = 0);

//���data�����Ƿ��������,���ú�����ڲ�������,���Բ�Ҫ�������ͷ�
void DropItemData(world * pPlane,const A3DVECTOR &pos, item_data * data,const XID & owner, int owner_team, int seq, int name_id = 0, char battle_faction = 0);

inline bool CanRecycle(item_data *pData)
{
	return (pData->proc_type & item::ITEM_PROC_TYPE_RECYCLE) && !(pData->proc_type & (item::ITEM_PROC_TYPE_CASHITEM | item::ITEM_PROC_TYPE_GUID));
}

#endif

