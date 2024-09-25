#ifndef __ONLINE_GAME_GS_PET_MANAGER_H__
#define __ONLINE_GAME_GS_PET_MANAGER_H__

#include <common/types.h>
#include <amemory.h>
#include <common/base_wrapper.h>
#include "config.h"
#include "property.h"
#include "item.h"
#include "petdataman.h"
#include "template_loader.h"
#include "cooldown.h"

#define PET_DB_DATA_VERSION	0x00000001
#define PET_FALLOW_RANGE_LIMIT	80.0f

#pragma pack(1)
//ע��,����Ǵ����ݿ�Ľṹ,ֻ����������С
struct pet_data_t
{
	int version;
	size_t pet_bedge_inv_size;
	size_t pet_civilization;	//����������
	size_t pet_construction;	//���ｨ���
	int reserved1;
	int reserved2;
	int reserved3;
	int reserved4;
};
#pragma pack()

enum PET_CHECK_DB_DATA_ERR
{
	PCDDE_SUCCESS,
	PCDDE_SIZE_INVALID,
	PCDDE_VERSION_INVALID,
	PCDDE_VERSION_VALID_BUT_SIZE_INVALID,
	PCDDE_BEDGE_INV_SIZE_INVALID,
};

struct msg_pet_info_changed_t
{       
	//ֻ��Ҫ��¼Ӱ��npc�Ǿ����Ա仯������Ӱ��ֵ������ȷ��npc������ȶ�����
	int level;                              //����
	int honor_level;                        //���ܶȵȼ�
	int hunger_level;			//��ʳ�ȵȼ�
	int damage_reduce;			//�˺�����
	float damage_reduce_percent;		//�˺�����ٷֱ�
	q_extend_prop base_prop;                //��������
	q_enhanced_param en_point;		//��ǿ����
}; 

struct msg_pet_notify_hp_vp_t
{
	int cur_hp;
	int max_hp;
	int cur_vp;
	int max_vp;
	int aggro_state;
	int stay_state;
	bool combat_state;
};

struct msg_pet_skill_cooldown_t
{
	int cooldown_index;
	int cooldown_time;
};

struct msg_pet_auto_skill_set_t
{
	int skill_id;
	int set_flag;
};

enum
{
	SUMMON_PET_TIME = 60,
	RECALL_PET_TIME = 10,
	COMBINE_PET_TIME = 10,
	UNCOMBINE_PET_TIME = 10,
	FREE_PET_TIME = 60,
	ADOPT_PET_TIME = 60,
	REFINE_PET_TIME = 60,
	SUMMON_INIT_HUNGER_POINT = 20,
	PET_VISIBLE_STATE_COMBINE1 = 23,
	PET_VISIBLE_STATE_COMBINE2 = 24,
	PET_COMBINE1_COOLDOWN_TIME = 30,
	PET_COMBINE2_COOLDOWN_TIME = 30,
};

enum
{
	COLLECT_TRIGGER_TIME = 900,
	HUNGER_TRIGGER_TIME = 300,
	HONOR_TRIGGER_TIME = 300,
	AGE_TRIGGER_TIME = 300,
	TALK_TRIGGER_TIME = 300,
	MAX_LIFE = 25000,
};

struct pet_data
{
	enum
	{
		PET_STATE_NONE,		// δ����
		PET_STATE_SUMMON,	// ����
		PET_STATE_COMBINE,	// ����
	};
	enum
	{
		HONOR_LEVEL_0,
		HONOR_LEVEL_1,
		HONOR_LEVEL_2,
		HONOR_LEVEL_3,
		HONOR_LEVEL_4,
		HONOR_LEVEL_5,
		HONOR_LEVEL_6,
		HONOR_LEVEL_7,
		HONOR_LEVEL_8,
		HONOR_LEVEL_9,
		HONOR_LEVEL_10,
		HONOR_LEVEL_COUNT,
	};

	enum
	{
		HUNGER_LEVEL_0,
		HUNGER_LEVEL_1,
		HUNGER_LEVEL_2,
		HUNGER_LEVEL_3,
		HUNGER_LEVEL_COUNT,
		
	};
	//������pet��ʱ����Ҫ���̵�����
	//���Ƕ�����г������
	//��Ӧ�ٻ������������
	int active_state;		//����״̬
	XID pet_id;			//��ǰ����ID

	int summon_stamp;		//�����ٻ���������������ͬ���ٻ�
	int summon_timestamp;		//�����ٻ��ĵ�ʱ���
	int summon_timer;		//��ǰ�����ٻ���ʱ������ͳ�ƾ���ֵ
	int aggro_state;		//��ǰ����ĳ�޷�ʽ
	int stay_state;			//��ǰ����ĸ��淽ʽ
	int pet_counter;
	int cur_notify_counter;		//��ǰ���͸�����������ݵ���Ϣ��ʱ
	int cur_state;			//�������֪ͨ���Լ�����Ϣ����˱�־��λ

	int combine_type;		//��������
	int combine_counter;		//combineʱ��
	int collect_counter;		//�ɼ���ʱ��
	int hunger_counter;		//��ʳ�ȼ�ʱ��
	int honor_counter;		//���ܶȼ�ʱ��
	int age_counter;		//�����ʱ��
	int talk_counter;		//˵����ʱ��

	bool need_refresh;		//ˢ�±�־

	msg_pet_info_changed_t effect_prop;		//��ʱ�洢Ӧ����Ч������
	itemdataman::_pet_armor_essence equip_added;	//װ������
	int equip_mask;

};

class gplayer_imp;
class pet_manager
{
public:
	static int CheckDBData(const void* buf,size_t size);
	static int GetHonorLevel(int honor_point);
	static int GetHungerLevel(int cur_hunger_point,int max_hunger_point);
	static float GetHungerLevelDamageAdjust(int hunger_level);

	static size_t GetCivilizationNeed(int rank);
	static int GetMaxCount(int rank);
	static int GetLevelNeed(int rank);
	static float GetMaxHPAdd(int rank);
	static float GetAttackAdd(int rank);
	static int GetResistanceAdd(int rank);
	static int GetLifeSkillAdd(int rank);
	static float GetMakeSkillAdd(int rank);
	
	static void GenerateBaseProp(const pet_bedge_essence& ess,const pet_bedge_data_temp& pbt,q_extend_prop& base_prop);
	static void GenerateEnhancedProp(const pet_bedge_essence& ess,const pet_bedge_data_temp& pbt,q_enhanced_param& en_point);
	static void AddEquipProp(gplayer_imp* pImp,const pet_bedge_essence& ess,q_extend_prop& base_prop,int& equip_mask,itemdataman::_pet_armor_essence* pAdded = NULL);
	static void AddSkillProp(gplayer_imp* pImp,const pet_bedge_essence& ess,q_extend_prop& base_prop);
public:
	enum
	{
		NOTIFY_MASTER_TIME = 16,
	};

private:
	pet_data _pet_list[MAX_PET_BEDGE_LIST_SIZE]; //�����б�
	pet_data_t _data;
	int _cur_summon_pet_index;	//��ǰ����ĳ�������
	bool _change_flag;		//��Ϊ�����³���״̬�ı�ı�־
	cd_manager _cooldown;

public:
	pet_manager();
	~pet_manager();

	void Swap(pet_manager& rhs);

public:
	inline bool IsPetSummon(size_t pet_index)
	{
		if(pet_data::PET_STATE_SUMMON == _pet_list[pet_index].active_state) return true;
		return false;
	}

	inline bool IsPetCombine(size_t pet_index)
	{
		if(pet_data::PET_STATE_COMBINE == _pet_list[pet_index].active_state) return true;
		return false;
	}

private:
	inline bool IsPetActive(size_t pet_index)
	{
		if(pet_data::PET_STATE_NONE == _pet_list[pet_index].active_state) return false;
		return true;
	}

	int GetActivePetCount()
	{
		int count = 0;
		for(int i = 0;i < MAX_PET_BEDGE_LIST_SIZE;++i)
		{
			if(_pet_list[i].active_state)
			{
				++count;
			}
		}
		return count;
	}

	int GetSummonPetCount()
	{
		int count = 0;
		for(int i = 0;i < MAX_PET_BEDGE_LIST_SIZE;++i)
		{
			if(pet_data::PET_STATE_SUMMON == _pet_list[i].active_state)
			{
				++count;
			}
		}
		return count;
	}

	int GetCombinePetCount()
	{
		int count = 0;
		for(int i = 0;i < MAX_PET_BEDGE_LIST_SIZE;++i)
		{
			if(pet_data::PET_STATE_COMBINE == _pet_list[i].active_state)
			{
				++count;
			}
		}
		return count;
	}

	pet_data* GetPetData(size_t index)
	{
		if(index < MAX_PET_BEDGE_LIST_SIZE)
		{
			return &_pet_list[index];
		}
		else
		{
			return NULL;
		}
	}

	void DoSummonPet(gplayer_imp* pImp,size_t pet_index,const A3DVECTOR& pos);
	void DoRecallPet(gplayer_imp* pImp,size_t pet_index);
	void DoCombinePet(gplayer_imp* pImp,size_t pet_index,int type, bool need_cooldown);
	void DoRebornCombinePet(gplayer_imp* pImp,size_t pet_index,int type);
	void DoUncombinePet(gplayer_imp* pImp,size_t pet_index);

	bool RecvExp(gplayer_imp* pImp,size_t pet_index,int exp_added,bool medicine = false);

	void RefreshPetInfo(gplayer_imp* pImp,size_t pet_index);
	bool GetPetEffectProp(gplayer_imp* pImp,size_t pet_index,msg_pet_info_changed_t& prop,itemdataman::_pet_armor_essence& equip_added,int& equip_mask);

	void ModifyHungerPoint(gplayer_imp* pImp,size_t pet_index,pet_bedge_essence& ess,int value);
	void ModifyHonorPoint(gplayer_imp* pImp,size_t pet_index,pet_bedge_essence& ess,int value);

public:

	void SetDBData(gplayer_imp* pImp,const void* buf,size_t size);

	void* GetData()
	{
		return &_data;
	}

	size_t GetSize()
	{
		return sizeof(pet_data_t);
	}

	int GetCurSummonPetIndex() const
	{
		return _cur_summon_pet_index; 
	}


	inline void SetPetCivilization(size_t civilization)
	{
		_data.pet_civilization = civilization;
	}

	inline void AddPetCivilization(size_t civilization)
	{
		size_t temp = _data.pet_civilization + civilization;
		if(temp < _data.pet_civilization)
		{
			_data.pet_civilization = 0xFFFFFFFF;
		}
		else
		{
			_data.pet_civilization = temp;
		}
	}

	inline size_t GetPetCivilization() const
	{
		return _data.pet_civilization;
	}

	inline void SetPetConstuction(size_t construction)
	{
		_data.pet_construction = construction;
	}

	inline size_t GetPetConstruction() const
	{
		return _data.pet_construction;
	}

	void AddPetConstruction(gplayer_imp* pImp,int added);
	void SetPetBedgeInventorySize(gplayer_imp* pImp,size_t size);

	bool CheckPetBedgeExist(gplayer_imp* pImp,size_t pet_index);
	bool CheckPetEquipExist(gplayer_imp* pImp,size_t pet_equip_index);
	size_t GetBattleStatusPetCount(gplayer_imp* pImp);
	int TestCanEquipPetBedge(gplayer_imp* pImp,size_t inv_index,size_t pet_index);
	int TestCanMovePetBedge(gplayer_imp* pImp,size_t src_index,size_t dst_index) { return 0; }
	int TestCanEquipPetEquip(gplayer_imp* pImp,size_t inv_index,size_t pet_equip_index);
	void OnEquipPetBedge(gplayer_imp* pImp,size_t pet_index);
	void OnMovePetBedge(gplayer_imp* pImp,size_t dst_index,size_t src_index) {}
	void OnEquipPetEquip(gplayer_imp* pImp,size_t pet_equip_index);
	int SetPetStatus(gplayer_imp* pImp,size_t pet_index,int pet_tid,int main_status,int sub_status);
	int GetRankPetCount(gplayer_imp* pImp,int rank);
	int SetPetRank(gplayer_imp* pImp,size_t pet_index,int pet_tid,int rank);
	
	int TestCanSummonPet(gplayer_imp* pImp,size_t pet_index,A3DVECTOR& pos);
	int TestCanRecallPet(gplayer_imp* pImp,size_t pet_index);
	int TestCanCombinePet(gplayer_imp* pImp,size_t pet_index,int type, bool need_cooldown);
	int TestCanUncombinePet(gplayer_imp* pImp,size_t pet_index);

	int SummonPet(gplayer_imp* pImp,size_t pet_index);
	int RecallPet(gplayer_imp* pImp,size_t pet_index);
	int CombinePet(gplayer_imp* pImp,size_t pet_index,int type, bool need_cooldown);
	int UncombinePet(gplayer_imp* pImp,size_t pet_index,int type);

	void ClearPetBehavior(gplayer_imp* pImp,size_t pet_index);
	void ClearAllPetBehavior(gplayer_imp* pImp);
	void RecordCurSummonPet(gplayer_imp* pImp);
	void TrySummonLastSummonPet(gplayer_imp* pImp);

	void GetPetPropAddedInfo(gplayer_imp* pImp, size_t pet_index, S2C::CMD::player_pet_prop_added * data_prop_added);

public:
	bool PetRelocatePos(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,bool force_disappear = false);
	bool PetNotifyPetHPVPState(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,const msg_pet_notify_hp_vp_t& info);
	bool PetDeath(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,int kill_by_npc);
	bool PetSetAutoSkill(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,int skill_id,size_t set_flag);
	bool PetSetCoolDown(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,int cooldown_index,int cooldown_time); //msec
	bool IsRebornPet(gplayer_imp * pImp, size_t pet_index);

	void Heartbeat(gplayer_imp* pImp);
	void OnMasterDeath(gplayer_imp* pImp);
	void KillMob(gplayer_imp* pImp,int mob_level);
	void PlayerBeAttacked(gplayer_imp* pImp,const XID& attacker);
	void NotifyMasterInfo(gplayer_imp* pImp, leader_prop & data);
	int FeedPet(gplayer_imp* pImp,size_t pet_index,item* parent,int pet_level_min,int pet_level_max,
	                unsigned int pet_type_mask,int food_usage,unsigned int food_type,int food_value,int &cur_value);
	int AutoFeedOnePet(gplayer_imp* pImp,size_t pet_index,item *parent,float hp_gen,float mp_gen,float hunger_gen,int mask,int& cur_value);
	int AutoFeedPet(gplayer_imp* pImp,size_t pet_index,item *parent,float hp_gen,float mp_gen,float hunger_gen,int mask,int& cur_value);
	bool PlayerPetCtrl(gplayer_imp* pImp,size_t pet_index,int cur_target,int pet_cmd,const void* buf,size_t size);
	void NotifyStartAttack(gplayer_imp* pImp,const XID& target,char force_attack);
	void NotifyDuelStop(gplayer_imp *pImp);

	void OnSetInvisible(gplayer_imp *pImp);
	void OnClearInvisible(gplayer_imp *pImp);
	int RecoverPetFull(gplayer_imp* pImp, size_t pet_index);

public:
	int GetMakeAttr(gplayer_imp* pImp,int index);
	bool CheckProduceItem(gplayer_imp* pImp,const recipe_template& rt);

public:
	static bool FindGroundPos(world_manager* manager,A3DVECTOR& pos);

public:
	void DbgChangePetLevel(gplayer_imp* pImp,size_t pet_index,int level);
	void AskCollectRequuirement(item& it,pet_bedge_essence& ess,int collect_type,int& item_tid,int& item_count);
	void OnPlayerLogin(gplayer_imp* pImp);

	void SomePetBedgeExpired(gplayer_imp* pImp);
	void SomePetEquipExpired(gplayer_imp* pImp);

	bool PetBedgeRebuild(gplayer_imp* pImp,size_t pet_index);
	int GetActivePetIndex(gplayer_imp* pImp);
	int GetPetCombineType(gplayer_imp* pImp,size_t pet_index);

	void ClearPetSkillCoolDown(gplayer_imp * pImp);
	void GetSummonPetProp(gplayer_imp * pImp, int cs_index, int uid, int sid);
};

#endif

