#ifndef __ONLINEGAME_GS_EQUIPMENT_ITEM_H__
#define __ONLINEGAME_GS_EQUIPMENT_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include <crc.h>
#include "item_astrology.h"
enum
{
	MAX_REINFORCE_STONE_LEVEL = 20,
	MAX_PROPERTY_STONE_LEVEL  = 10,
	MAX_SKILL_STONE_LEVEL	  = 1,

	MAX_ENCHANT_SLOT	= 5,
	//Add by Houjun 2011-03-07, ���ʯ���
	MAX_GEM_SLOT = 3,
	MAX_ASTROLOGY_HOLE	= 1,

};

enum
{
	GEM_SHAPE_ALL		= 0,
	GEM_SHAPE_HUANGZHUO = 1,
	GEM_SHAPE_JIEDU		= 2,
	GEM_SHAPE_HUNDUN	= 3,
	GEM_SHAPE_DUOHUN	= 4,
	GEM_SHAPE_YUANYUN	= 5,
	GEM_SHAPE_WUJI		= 6,
	GEM_SHAPE_PEIYUAN	= 7,
	GEM_SHAPE_TIANXIN	= 8,
	GEM_SHAPE_SANWEI	= 9,
	GEM_SHAPE_BICHEN	= 10,
	GEM_SHAPE_XINGSHENG	= 11,
	GEM_SHAPE_SHENXING	= 12,
	GEM_SHAPE_MAX_NUM	= 13,
};

#pragma pack(1)

//����汾��װ���ṹ
struct equip_essence
{
	unsigned short rf_stone;
	short rf_stone_level;

	unsigned short pr_stone;
	short pr_stone_level;

	unsigned short sk_stone;
	unsigned short sk_stone_level; 
};

/*
struct equip_essence_fashion_colorant
{
	equip_essence old_essence;
	int color;
	int quality;
	int reserved1;
	int reserved2;
};
*/

//Ѫ���汾��װ���ṹ
struct equip_essence_spirit
{
	equip_essence old_essence;
	int master_id;			//����ID
	char master_name[20];		//��������
	int reserved1;
	int reserved2;
	int cur_spirit_power;		//��ǰ����
	int max_spirit_power;		//��������
	int soul_used_spirit;		//�����Ѿ�ռ�õĻ���
	int soul[MAX_ENCHANT_SLOT];	//��ľˮ����5������
	int soul_rune_word;		//����������Ե�CRC��ֻ�з�0����Ч
};

//Add by Houjun 2011-03-07, ��ʯ��Ƕ�������ӱ�ʯ��
//��ʯ�汾��װ���ṹ
struct equip_essence_gem
{
	equip_essence_spirit spirit_essence;
	char gem_slot_shape[MAX_GEM_SLOT]; //װ����ʯ����״, -1Ϊ�òۻ�û�б�����
	int gem_embeded[MAX_GEM_SLOT];	//�Ѿ���Ƕ�ı�ʯ��-1��ʾ�òۻ�û����Ƕ��ʯ
	int colorant_id;
	int quality;
	int reserved3;
};


struct astrology_slot 
{
	int astrology_id;
	int level;
	int score;
	int energy_consume;
	astrology_prop prop[6];
	bool is_lock;			//����������������Ϣ
	int reserver1;
	int reserver2;
};


//�����汾��װ���ṹ
struct equip_essence_astrology
{
	equip_essence_gem gem_essence;
	int cur_hole;		//��ǰ�Ŀ���
	int reserverd1;
	int reserverd2;
	astrology_slot hole[3];
};
//Add end.
#pragma pack()

// Youshuang add
enum
{
	EQUIP_ESSENCE_VERSION_OLD = 0,
	EQUIP_ESSENCE_VERSION_SPIRIT = 1,
	EQUIP_ESSENCE_VERSION_GEM = 2,
	EQUIP_ESSENCE_VERSION_ASTROLOGY = 3,
};

#pragma pack(1)
struct equip_essence_new
{
	int version;
	
	// old essence
	int rf_stone;
	short rf_stone_level;
	int pr_stone;
	short pr_stone_level;
	int sk_stone;
	unsigned int sk_stone_level; 

	// add spirit
	int master_id;				//����ID
	char master_name[20];			//��������
	int reserved1;
	int reserved2;
	int cur_spirit_power;			//��ǰ����
	int max_spirit_power;			//��������
	int soul_used_spirit;			//�����Ѿ�ռ�õĻ���
	int soul[MAX_ENCHANT_SLOT];		//��ľˮ����5������
	int soul_rune_word;			//����������Ե�CRC��ֻ�з�0����Ч

	// add gem
	char gem_slot_shape[MAX_GEM_SLOT]; 	//װ����ʯ����״, -1Ϊ�òۻ�û�б�����
	int gem_embeded[MAX_GEM_SLOT];		//�Ѿ���Ƕ�ı�ʯ��-1��ʾ�òۻ�û����Ƕ��ʯ
	int colorant_id;
	int reserved3;
	int reserved4;
	
	// add astrology
	int cur_hole;				//��ǰ�Ŀ���
	int reserved5;
	int reserved6;
	astrology_slot hole[3];

	// add reserved content
	int reserved[32];
};
#pragma pack()
// end

struct equip_install_fee
{
	int	fee_install_estone;		// ����ǿ��������
	int	fee_install_pstone;		// ����׷��������
	int	fee_install_sstone;		// ���Ӽ���������
	int	fee_uninstall_pstone;		// ���β�ж׷��ʯ������
	int	fee_uninstall_sstone;		// ���β�ж����ʯ������
};

struct equip_data
{
	int equip_type;			// װ����𣬿��ܵ��� 0-������1-���ߣ�2-��Ʒ��3-ʱװ��4-���5-��������
	int weapon_type;		//����������Ч �Ƿ�����
	int attack_cycle;
	int attack_point;
	int64_t class_required;
	int64_t class_required1;
	int level_required;
	int require_gender;		//Ů 1 �� ��0 
	int require_reborn_count;
	int64_t require_reborn_prof[3];	//1 2 3תʱ��ְҵҪ��
	int64_t require_reborn_prof1[3];	//1 2 3תʱ��ְҵҪ��
	
	int require_rep_idx1;
	int require_rep_val1;
	int require_rep_idx2;
	int require_rep_val2;
	int require_title;
	int require_cultivation;
	int require_territory;
	int require_kingdom_title;
	int hp;
	int mp;
	int dp;
	int attack;
	int damage_low;
	int damage_high;
	int deity_power;
	int armor;
	int defense;
	int equip_mask;
	float attack_range;
	int level;
	int equip_enchant_level;	//װ��Ѫ������
	bool can_be_enchanted;		//�Ƿ���Խ���Ѫ��
	equip_install_fee install_fee;
	bool can_gem_slot_identify; //װ���Ƿ���в�ۼ���
	bool can_open_astrology_slot;	//�Ƿ���Կ���������
	// Youshuang add
	int fashion_weapon_addon_id;
	int fashion_can_add_color;
	int fixed_init_color;
	int init_color_value;
	// end
};


class equip_item : public item_body
{
	equip_data _data;
	equip_item() {}
	abase::vector<addon_data_spec> _addon_list;
	abase::vector<addon_data_spec> _use_addon_list;
	int _estone_id;
	int _estone_value[MAX_REINFORCE_STONE_LEVEL];

	struct 
	{
		int level;
		struct		//����ṹӦ�ú�exptypes.h�����һ��
		{
			int             id;
			float   probability;
		} prop[5];
	}_reinforce_addon_data[2];

	struct
	{
		int level;
		int addon_id;
	}_hidden_addon_data[3];

	//Add by Houjun 2011-03-07, ��Ʒ�����״��������
	static float gem_shape_prob[GEM_SHAPE_MAX_NUM];

public:
	DECLARE_SUBSTANCE(equip_item);
	equip_item(const equip_data & data):_data(data) ,_estone_id(0)
	{
		ASSERT(data.require_gender & 0x03);
		ASSERT(data.level >=1 && data.level <= 10);
		memset(_estone_value, 0, sizeof(_estone_value));
		memset(_reinforce_addon_data,0, sizeof(_reinforce_addon_data));
	}
	
	inline void SetFashionWeaponAddonID( int addon_id )
	{
		_data.fashion_weapon_addon_id = addon_id;
	}
	inline int GetFashionWeaponAddonID() const
	{
		return _data.fashion_weapon_addon_id;
	}
	// Youshuang add, equipment version control
	inline bool IsFashionWeapon() const{ return GetEquipMask() & item::EQUIP_MASK_FASHION_WEAPON; }
	void FillOldEquipEssence( const equip_essence& old_ess, equip_essence_new& new_ess ) const;
	void FillSpiritEquipEssence( const equip_essence_spirit& spirit_ess, equip_essence_new& new_ess ) const;
	void FillGemEquipEssence( const equip_essence_gem& gem_ess, equip_essence_new& new_ess ) const;
	void FillAstrologyEquipEssence( const equip_essence_astrology& astrology_ess, equip_essence_new& new_ess ) const;
	void ResetEquipEssence( item* parent ) const;
	void Empty2OldEssence( item* parent ) const;
	equip_essence_new* GetOldDataEssence( item * parent, int version = 0 ) const;
	const equip_essence_new* GetOldDataEssence( const item * parent, int version = 0 ) const;
	equip_essence_new* GetSpiritDataEssence(item* parent) const;
	const equip_essence_new* GetSpiritDataEssence(const item* parent) const;
	equip_essence_new* GetGemDataEssence(item* parent) const;
	const equip_essence_new* GetGemDataEssence(const item* parent) const;
	equip_essence_new* GetAstrologyDataEssence(item* parent) const;
	const equip_essence_new* GetAstrologyDataEssence(const item* parent) const;
	bool IsEmptyEssence(const item * parent) const;
	bool IsOldEssence(const item * parent) const;
	bool IsSpiritEssence(const item * parent) const;
	bool IsGemEssence(const item * parent) const;
	bool IsAstrologyEssence(const item * parent) const;
	bool IsBloodEnchanted(const item* parent) const;
	bool IsIdentifyGem(const item * parent) const;
	int GetCurHole(item * parent);
	// end

	void InsertAddon(const addon_data & addon);
	inline const equip_data & GetEssence() const { return _data; }
	inline void SetReinforce(int stone_id, const int * value)
	{
		_estone_id = stone_id;
		for(size_t i = 0; i < MAX_REINFORCE_STONE_LEVEL; i ++)
		{
			_estone_value[i] = value[i];
		}
	}

	inline void SetReinforceAddon(int index, int level, const void * data)	//���data�ĸ�ʽ��exptypes.h��Ľṹ�й�
	{
		ASSERT(index == 0 || index == 1);
		_reinforce_addon_data[index].level = level;
		memcpy(_reinforce_addon_data[index].prop, data, sizeof(_reinforce_addon_data[index].prop));
	}

	inline void SetHiddenAddon(const void * data)
	{
		memcpy(_hidden_addon_data, data, sizeof(_hidden_addon_data));
	}
	
	inline int GetEStoneID() const 
	{
		return _estone_id;
	}
	
	inline int GetEStoneValue(size_t level) const
	{
		if(level > MAX_REINFORCE_STONE_LEVEL || level == 0) return 0;
		return _estone_value[level - 1];
	}

	void ActivateReinforce(equip_essence_new & value, int level_before) const;
	int CheckReinforceAddonValue(int new_addon, int index, int new_level, int old_level) const;
	void ActivateAddonByID(int id, gactive_imp * obj , bool active, item* parent) const ;
	void ActiveHiddenAddon(int reinforce_level, gactive_imp *obj, bool active, item* parent) const;

public:
	inline int GetLevel() const { return _data.level;}
	inline int GetEquipMask() const { return _data.equip_mask;}

public:
	virtual void OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const;
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const;
	virtual void OnActivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnDeactivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void ActiveAstrologyProp(item * parent, gactive_imp * obj) const;
	virtual void DeactiveAstrologyProp(item * parent, gactive_imp * obj) const;
	virtual void OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_EQUIPMENT;}
	virtual bool OnCheckAttack(item_list & list) const {return true;}
	virtual void OnAfterAttack(item_list & list,bool * pUpdate) const { *pUpdate = false;} 
	virtual int ReinforceEquipment(item * parent, gactive_imp * imp, int stone_id, const float aoffset[4], int & level_result, int & fee, int & level_before,bool newmode, int require_level, const float adjust2[20]) const;
	virtual int RemoveReinforce(item * parent, gactive_imp * pImp , size_t & money ,bool rp) const;
	virtual int64_t GetIdModify(const item * parent) const;
public:
//��Ѫ����
	virtual bool DoBloodEnchant(size_t index, gactive_imp * obj, item * parent) const;
//�ؽ��������ֵ
	bool RebuildEnchantEffect(equip_essence_new & ess, bool is_initial) const;
//ˢ�������������
	bool RefreshSoulRuneWord(equip_essence_new & ess) const;
//����Ч��
	void CheckRefineEffect(const equip_essence_new & ess, float & origin_effect, int & point_effect) const;
//���ǹ�ע
	virtual int InsertSpiritAddon(gactive_imp * obj, item * parent, int addon_id,size_t addon_slot) const;
//�����Ƴ�
	virtual int RemoveSpiritAddon(gactive_imp * obj, item * parent, size_t addon_slot, int * removed_soul) const;
//�����ָ�	
	virtual int RestoreSpiritPower(gactive_imp * obj, item * parent) const;
//װ����⣨����ֵ��errorcode ���Ϊ0, �ɲ������������Ԫ�������Ŀ�� ������������۳���Ǯ
	virtual int SpiritDecompose(gactive_imp * obj, item * parent, int *spirit_unit1, int * spirit_unit2) const;	

//�ռ�����Ч��
	int CollectSpiritEffect(const equip_essence_new& ess, int * pData, int dataCount) const;

	virtual int DecSpiritPower(gactive_imp * obj, item* parent, int count, int index) const;
	virtual int GetSpiritPower(item* parent) const;

	// Youshuang add for fashion colorant
	bool SetFashionContent( item* parent, int colorant_id ) const;
	virtual bool AddColor( item* parent, int colorant_id, int quality );
	virtual void InitFromShop(gplayer_imp* pImp,item * parent, int value) const;
	// end

	virtual int GetReinforceLevel(const item *parent) const;
	virtual bool InitReinforceLevel(item *parent, int level);
	virtual bool ResetHiddenAddon(item * parent) const;
	virtual int GetSpiritLevel(item * parent) const;
	virtual bool UpgradeEquipment(item * parent, gactive_imp *pImp, int item_index, int item_type,  int output_id, 
			bool is_special_rt, int special_level, int stone_index, int stone_id, int stone_num, int rt_index, int rt_id);

	//��ʯ��ۼ���
	virtual bool IdentifyGemSlots(size_t index, gactive_imp * obj, item * parent) const;
	//������ʯ���
	virtual bool RebuildGemSlots(gactive_imp* obj, item* parent, bool locked[3]) const;
	//���Ʊ�ʯ���
	virtual bool CustomizeGemSlots(gactive_imp * obj, item* parent, int gem_seal_id[3], int gem_seal_index[3]) const;
	//��ʯ��Ƕ
	virtual bool EmbededGems(gactive_imp * obj, item* parent, int gem_id[3], int gem_index[3]) const;
	//��ʯ���
	virtual bool RemoveGems(gactive_imp * obj, item* parent, bool remove[3]) const;	
	//�ռ���Ƕ��ʯ�����ĸ�������
	int CollectGemAddons(gactive_imp* obj, const equip_essence_new& ess, int * addons, int maxAddonCnt) const;
	//���ʯ��Ч
	void ActiveGemEffect(gactive_imp* obj, const equip_essence_new& ess, item* parent) const;
	//ȡ����ʯЧ��
	void DeactiveGemEffect(gactive_imp* obj, const equip_essence_new& ess, item* parent) const;

	
	bool OpenSlot(item * parent, gactive_imp *pImp, size_t item_index, size_t stone_index, int stone_id); 
	bool InstallAstrology(gactive_imp * obj, item * parent1, item * parent2, size_t item_index, int item_id, size_t stone_index, int stone_id, size_t slot_index); 
	bool UninstallAstrology(gactive_imp * obj, item * parent, size_t item_index, int item_id, size_t slot_index); 

	int GetAstrologyEnergyConsume(item * parent) const;
	void InitGemSlot(equip_essence_new & gemEss) const;
	void InitGemEssence(equip_essence_new & ess) const;
	
private:
	virtual int OnGetEquipMask() const 
	{
		return _data.equip_mask;
	}
	virtual int OnGetEquipType() const
	{
		return _data.equip_type;
	}
};
#endif

