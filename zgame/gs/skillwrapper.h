#ifndef __SKILL_SKILLWRAPPER_H
#define __SKILL_SKILLWRAPPER_H

#include <map>
#include <set>

#include "common/types.h"
#include "common/base_wrapper.h"
#include "obj_interface.h"

namespace SKILL
{
	enum
	{
		SKILL_STATE_INIT = 0,
		SKILL_STATE_PRAY = 1,
		SKILL_STATE_PERFORM = 2,
	};
	
	struct Data
	{
		unsigned int	id;     // �������, ����ID
		char	forceattack;    // ����������Ƿ�ǿ�ƹ���

		bool	skippable;      // ������������ǰ״̬�Ƿ����ǰ����
		int	stateindex;     // ��ǰ״̬ID
		int	nextindex;      // �¸�״̬ID
		int	warmuptime;     // ����ʱ��
		short	level;          // ����
		int 	cooltime;       // ��ȴʱ��
		int 	item_id;	// ��ƷID
		short 	item_index;	// ��Ʒλ��
		bool    consumable;	// ��Ʒ�Ƿ�����	
		char 	spirit_index;// ������ԯ�ͷż��ܵ�����Index, �������ͷŵ�Ĭ��Ϊ-1
		A3DVECTOR pos;
		char 	skillstate;	
		std::vector<short> talismanRefineSkillList;
		Data(unsigned int i) : id(i),forceattack(0),skippable(false),stateindex(-1),nextindex(-1),warmuptime(-1), item_id(-1), item_index(-1), consumable(false)
		{ 
			level = 0;
			cooltime = 0;
			spirit_index = -1;
			skillstate = SKILL_STATE_INIT;
		}
	};
}

namespace GNET
{

enum{
	MASK_DEFAULT_SKILL = 0x01,
	MASK_ADDON_SKILL   = 0x02,
	MASK_FAMILY_SKILL  = 0x04,
	MASK_SECT_SKILL    = 0x08,
	MASK_NOLEARNTRIGGER_SKILL = 0x10,
	MASK_ADDON_SKILL_PERMANENT = 0x20,
	MASK_SYSTEM_DELIVERED_SKILL = 0x40,
};

enum
{
	SERIAL_SKILL_NONE = 0,
	SERIAL_SKILL_START,
	SERIAL_SKILL_SUCCEED,
	SERIAL_SKILL_TERMINAL,
};


enum { 
	FEEDBACK_HIT        = 0x01, //����������Ϣ
	FEEDBACK_DAMAGE     = 0x02, //�������˺���Ϣ
	FEEDBACK_KILL       = 0x04, //����ɱ����Ϣ
	FEEDBACK_MISS       = 0x08, //����δ������Ϣ
	FEEDBACK_CRIT    	= 0x10, //������������Ϣ
};

#define MAX_SKILL_ELEM_NUM 10

class Skill;

class SkillWrapper
{
public:
	typedef unsigned int	ID;

protected:
	struct PersistentData
	{
		char	baselevel;  // ��������
		char	reallevel;  // ʵ�ʼ���
		char	actilevel;  // �����
		char    mask;
		int     cooltime;
		PersistentData(char _base = 0):baselevel(_base), reallevel(0),actilevel(0),mask(0),cooltime(0){ }
	};

	struct WeaponAddonTalent
	{
		int value1;
		int value2;
		WeaponAddonTalent(int v1=0, int v2=0):value1(v1), value2(v2){}
	};

	struct SkillElems
	{
		int	curr_num; //����д����Ԫ�ص�����	
		unsigned short ids[MAX_SKILL_ELEM_NUM];
		SkillElems() : curr_num(0) { memset(ids, 0, sizeof(ids)); }
		SkillElems(const SkillElems& elems)
		{
			curr_num = elems.curr_num;
			memcpy(ids, elems.ids, sizeof(ids));
		}

		SkillElems& operator=(const SkillElems& elems)
		{
			curr_num = elems.curr_num;
			memcpy(ids, elems.ids, sizeof(ids));
			return *this;
		}
	};
	
	typedef std::map<ID,PersistentData>	StorageMap;
	typedef std::map<ID, bool>	SkillActiveMap;
	typedef std::map<ID,WeaponAddonTalent>	WeaponAddonMap;
	typedef std::map<ID, int>	ProficiencyMap;
	typedef std::map<ID, SkillElems>	SkillElemsMap;

	StorageMap	   map;
	WeaponAddonMap	   wamap;
	ProficiencyMap	prfmap;
	SkillElemsMap 	semap;
	SkillActiveMap trigger_active_map;

	short prayspeed;  
	short commonlevel;
	int   rejectrate;    	  // ���Ʊ��ӷ����ɹ�����
	
	int dec_skill_level; 	  // ״̬Ч�����ͼ��ܵȼ�
	int curr_cast_speed_rate; // ʩ��ʱ�����,�ٷ���

	int dark_skill_cd_adjust;
	int light_skill_cd_adjust;

	int record_map_id;
	A3DVECTOR record_pos;
	
public:
	SkillWrapper();

	// ������� 1.����������2.����ף����3.�������䣬4.��Ʒ���ܣ�5.������6.��������, 7.������8.˲��
	static char GetType(ID id); 	
	// ���ܹ�����Χ��0�� 1�� 2������ 3Ŀ���� 4Բ׶�� 5����, 6, ������Ŀ��
	static char RangeType(ID id);

	int Learn( ID id, object_interface player );	// �����µļ��𣬴��󷵻�-1
	int Forget(bool all, object_interface player);   // ϴ�� all: 1,ϴȥ���м���; 0,ϴȥ��ǰְҵ����
	int ForgetTalent(object_interface player);   // ϴȥ�����츳
	void ForgetCulSkills(object_interface player); //ϴȥ�����컯����
	void ForgetDeitySkills(object_interface player);	//ϴȥ���з�����

	void OnAddSkill(ID id, object_interface player);
	void OnRemoveSkill(ID id, object_interface player);

	// ������������,ĳ���ܼ���+n,idΪ0�����м��ܼ���+n
	int Upgrade(ID id, unsigned int level, object_interface player); 
	int Degrade(ID id, unsigned int level, object_interface player); 

	// ����ʯ,װ�����������ض�����
	int InsertSkill(ID id, unsigned int level, object_interface player); 
	int RemoveSkill(ID id, unsigned int level, object_interface player); 

	int InsertSkillPermament(ID id, unsigned int level, object_interface player);

	//��������, ��״̬Ч������ļ���
	int InsertTriggerSkill(ID id, unsigned int level, object_interface player);
	int ClearTriggerSkill(ID id, object_interface player); 

	//ϵͳ���ŵļ��ܣ��԰�ϲ������ʹ��
	int InsertSysDeliveredSkill(ID id, unsigned int level, object_interface player, bool sendClient);
	int ClearSysDeliveredSkill(ID id, object_interface player, bool sendClient);

	// ���������,���ޡ���;
	int ClearSpouseSkill(object_interface player);

	int Condition( ID id, object_interface player, const XID * target, int size ); // ���� error_code

	// ����ֵΪ�´ε���ʱ����(����),-1��ʾ����
	int StartSkill( SKILL::Data & skilldata, object_interface player, const XID * target, int size, int & next_interval);
	int Run( SKILL::Data & skilldata, object_interface player, const XID * target, int size, int & next_interval );
	int StartSkill( SKILL::Data & skilldata, object_interface player, const A3DVECTOR& target,int & next_interval);
	int Run( SKILL::Data & skilldata, object_interface player, const A3DVECTOR& target, int & next_interval );
	// ��ϣ��ܹ���ʱ����
	bool Interrupt( SKILL::Data & skilldata, object_interface player );
	// �������ȡ������ִ�еļ���
	bool Cancel( SKILL::Data & skilldata, object_interface player );
	// ��ǰ������ǰ״̬����������
	int Continue( SKILL::Data& skilldata, object_interface player, const XID* target,int size, int& next,int elapse);

	// ʹ��˲������, 0 �ɹ���-1 ʧ��
	int InstantSkill( SKILL::Data & skilldata, object_interface player, const XID * target, int size, const A3DVECTOR& chargeDestPos, const XID& chargeTarget);

	// ʹ����Ʒ���Ӽ���, 0 �ɹ�, -1 ʧ��
	int CastRune(SKILL::Data & skilldata, object_interface player, int level, int coolid);

	int CastExtraSkillToSelf( SKILL::Data& skilldata, object_interface player, int level, const attack_msg* original_attack ); // Youshuang add

	// ��ȡ�ͱ���PersistentData
	void LoadDatabase(object_interface player, archive & ar );
	void StoreDatabase( archive & ar );
	void StorePartial( archive & ar );
	void StorePartialAddonPermanent(archive& ar);
	
	int GetBaseLevel(ID id);
	int GetLevelSum(int cls);

	// ��������Ч������
	bool Attack(object_interface target, const XID&, const A3DVECTOR&,attack_msg& msg, bool invader, int damage, int is_crit);
	bool Attack(object_interface target, const XID&, const A3DVECTOR&,enchant_msg& msg, bool invader );

	// �������ܣ������ʼ��
	bool EventReset(object_interface player);
	// �������ܣ�����
	bool EventChange(object_interface player, int form);
	// �������ܣ��Ƿ��з������, Add by Houjun 2010-03-11
	bool EventCloneExist(object_interface player, bool isCloneExist);
	// �������ܣ���������������Լ�����ɺ���Ч
	bool EventAfterEnterWorld(object_interface player);
	//��Ҹı���ħ����Ӫ
	void CultivationChange(object_interface player, int culold, int culnew);

	// ����ֵΪ�´ε���ʱ����(����),-1��ʾ����
	int NpcStart(ID id, object_interface npc, const XID * target, int size, int& next_interval);
	void NpcEnd(ID id, object_interface npc, int level, const XID * target, int size );
	// ��ϣ��ܹ���ʱ����
	bool NpcInterrupt(ID id, object_interface npc, int level);
	float NpcCastRange(ID id, object_interface npc, int level);

	// �޸��������
	int IncPrayTime(int inc);
	int DecPrayTime(int dec);
	
	int GetCurrCastSpeedRate() { return prayspeed; } 

	// �޸�ĳ������ȴʱ��
	int IncCoolTime(ID id, int inc);
	int DecCoolTime(ID id, int dec);

	int GetDarkSkillCDAdjust() const;
	int GetLightSkillCDAdjust() const;
	int IncDarkSkillCoolTime(int inc);
	int DecDarkSkillCoolTime(int dec);
	int IncLightSkillCoolTime(int inc);
	int DecLightSkillCoolTime(int dec);
	
	void RemoveAura(object_interface player);
	void UndoPassive(ID id, int level, object_interface player);
	int GetTalentSum();
	int GetSpSum();
	void SetSkillTalent(Skill* skill);
	void SetSkillTalent(Skill* skill, const int* list);

	void SetSkillTalent(Skill* skill, object_interface player, bool adjustLevel = false);
	
	//Add by Houjun 2010-06-08, ��ȡ���鼼�ܣ�����ŵ�list��
	void SetSkillTalent(ID id, int list[8], object_interface player);

	//Add by Houjun 2010-03-15, ��Ӻ�ɾ����ϼ�����Ԫ��
	void AddSkillElement(ID id, int elemID);
	void SetSkillElement(ID id, int idx, int elemID);
	void DelSkillElement(ID id, int idx);	
	void SetSkillElemsNum(Skill* skill, const SkillElems& elem);
	void SetSkillElemsNum(Skill* skill, unsigned short elems[10]);
	//Add end.

	//Add by Houjun 2010-03-05, ����Ͷ�ȡSkillElems����
	void SaveSkillElems(archive& ar);
	void LoadSkillElems(archive& ar);
	void SaveSkillElemsClient(archive& ar);
	//Add end.

	//Add by Houjun 2010-06-29, �������鼼��������غ���	
	bool OnTalentChange(object_interface player, int talent_skill_id, int old_talent_skill_lvl, int new_talent_skill_level);
	//Add end.

	bool CheckConsistency();
	static int GetCooldownId(ID id);
	static int GetMpCost(ID id, int level);
	
	int GetSerialSkillType(ID id);
	void GetSucceedSkillColor(ID id, int& color, int& num);
	void SetSucceedSkillColor(Skill* skill, object_interface player);
	void SetSucceedSkillColor(Skill* skill, char colors[5]);

	void SetDecSkillLevel(int level);

	void RecordPos(object_interface player);
	void ClearRecordPos();
	int GetRecordMapId() const { return record_map_id; }
	const A3DVECTOR& GetRecordPos() const { return record_pos; }

	bool SetXPSkill(int id);
	bool ClearXPSkill(object_interface player);
	bool ClearXPSkillCoolTime(object_interface player);
	
public:
	void Swap(SkillWrapper&);
	int GetSkillLevel(ID id);
	short GetSkillData(ID id, short& level, int& cooltime, object_interface player);
	virtual ~SkillWrapper(){}

	void Store( archive & ar ){}
	void Load( archive & ar ){}
	void SetFamilySkill(int *pairs, int size,object_interface player);

	void SetSectSkill(int *pairs, int size,object_interface player);
	void GetSectSkill(int *pairs, int& size);
	int  Forget(object_interface player, int mask);   
	void WeaponAddon(ID id, int value1, int value2);   
	void SetRejectRate(int rate);
	int  GetRejectRate();

	void AddProficiency(object_interface player, ID skillid, int add);			//���Ӽ���������
	bool DecProficiency(object_interface player, ID skillid, int dec);			//���ټ��������ȣ����سɹ���ʧ��

	static unsigned int GetSkillLimit(ID id);			//��ü���ʹ������
 	static bool IsMonsterLimitSkill(ID id);
	static bool IsPetLimitSkill(ID id);
	static bool IsSummonLimitSkill(ID id);
	static bool IsPlayerLimitSkill(ID id);
	static bool IsSelfLimitSkill(ID id);
	static bool IsBattleLimitSkill(ID id);
	static bool IsCombatLimitSkill(ID id);
	static bool IsNonCombatLimitSkill(ID id);
	static bool IsNoSummonPetLimitSkill(ID id);
	static bool IsMountLimitSkill(ID id);
	static bool IsFlyLimitSkill(ID id);
	static bool IsNoSummonLimitSkill(ID id);
	static int 	LimitSkillSummonID(ID id);
	static bool IsBossLimitSkill(ID id);

	static bool IsWarmup(ID id);
	static bool IsCycle(ID id);
	static bool IsTalisman(ID id);

	static int GetMaxSkillLevel(ID id);

	//0, None; 1, Dark; 2, Light;
	static char GetDarkLightType(ID id);
	static bool IsCastInPreSkill(ID id);
	static int  GetPreCastSkillId(ID id);

	static bool IsXPSkill(ID id);

};

};

#endif

