#ifndef __ONLINEGAME_GS_OBJECT_INTERFACE_H__
#define __ONLINEGAME_GS_OBJECT_INTERFACE_H__

#include "property.h"
#include "attack.h"
#include <vector>

class gactive_imp;
class filter;
class world;
struct pet_data;
class pet_bedge_essence;
namespace GNET { class SkillWrapper; }
namespace GDB { struct itemdata; struct shoplog;struct pocket_item;}
class object_interface
{
	gactive_imp * _imp;
	bool _battleground_flag;
public: 
	enum
	{
		AT_STATE_DT_ATTACK_CRIT	= 0x01,		//��ʥ����
		AT_STATE_ATTACK_RUNE2  	= 0x02,		//magic attack rune
		AT_STATE_RESIST 	= 0x04,		//�ֿ�
		AT_STATE_SECT_SKILL	= 0x08,		//�T�ɼ���Ч��
		AT_STATE_ATTACK_CRIT  	= 0x10,		//crit
		AT_STATE_ATTACK_RETORT	= 0x20,		//���𹥻�
		AT_STATE_EVADE		= 0x40,		//��Ч����
		AT_STATE_IMMUNE		= 0x80,		//���ߴ˴ι���

	};

	enum COD_FACTION
	{
		TEAM_CIRCLE_OF_DOOM = 1, //���ģʽ��
		MAFIA_CIRCLE_OF_DOOM,    //����ģʽ��
		FAMILY_CIRCLE_OF_DOOM,   //����ģʽ��
	};

	object_interface():_imp(0), _battleground_flag(false)
	{}

	object_interface(gactive_imp * imp, bool flag = false):_imp(imp), _battleground_flag(flag)
	{}

	void Attach(gactive_imp * imp)
	{
		_imp = imp;
	}
	gactive_imp * GetImpl() { return _imp;}
	bool GetBattlegroundFlag() { return _battleground_flag; }
public:
	void BeHurt(const XID & who,const attacker_info_t & info, int damage,char attacker_mode = 0);
	void BeHurt(const XID & who,const attacker_info_t & info, int damage,bool invader, char attacker_mode = 0);
	void Heal(const XID & healer,size_t life);
	void Heal(size_t life, bool is_potion = false);
	void InjectMana(int mana, bool is_potion = false);
	bool DrainMana(int mana);
	void InjectDeity(int deity, bool is_potion = false);
	bool DecDeity(int deity); 
	void DecHP(int hp);
	void DecHP(const XID & who, int hp);
	bool SessionOnAttacked(int session_id);

	void SetATDefenseState(char state);

	//�������
	bool IsPlayerClass();
	int GetObjectType();
public:
	//filter ���
	void AddFilter(filter*);
	void RemoveFilter(int filter_id);
	void ClearSpecFilter(int mask,int count = 0);
	bool IsFilterExist(int filter_id);
	bool IsMultiFilterExist(int id);
	void RemoveMultiFilter(int id); 
	void OnFilterAdd(int filter_id, const XID& caster);
	int FilterCnt(int buff_type);
	void AdjustSkill(int skill_id, short& level);
	bool ModifyFilter(int filterid, int ctrlname, void * ctrlval, size_t ctrllen);	
	
public:
	class MsgAdjust
	{
	public:
		virtual ~MsgAdjust(){}
		virtual void AdjustAttack(attack_msg & attack) {}
	};

	//����
	bool UseProjectile(size_t count);
	bool CanAttack(const XID & target);	//�Ƿ���Խ���������
	void SetRetortState();			//�����´ι���Ϊ���𹥻���һ����Ч
	void SetNextAttackState(char state);
	bool GetRetortState();                  //�����´ι����Ƿ�Ϊ�����Ա�������TranslateSendAttack
	void Attack(const XID & target, attack_msg & attack,int use_projectile);
	void FillAttackMsg(const XID & target, attack_msg & attack,int use_projectile);

	void SendRegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,int count, std::vector<exclude_target>& target_exclude);
	
	void RegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,int count, std::vector<exclude_target>& target_exclude, bool transAttack = true);		//��
	void RegionAttack2(const A3DVECTOR& pos, float radius,attack_msg & attack,int up,float range,int count, std::vector<exclude_target>& target_exclude);									//��
	void RegionAttack3(const A3DVECTOR& pos, float cos_half_angle,attack_msg & attack,int up,float range, int count, std::vector<exclude_target>& target_exclude);						//׵

	void RegionAttackFaceLine(float radius,attack_msg & attack,int up,float range,int count, std::vector<exclude_target>& target_exclude);		//��
	void RegionAttackFaceSector(float cos_half_angle,attack_msg & attack,int up,float range,int count, std::vector<exclude_target>& target_exclude);	//׶
	void RegionAttackCross(float radius, float range, float angle_to_north, attack_msg& attack, int max_count, std::vector<exclude_target>& target_exclude);

	void MultiAttack(const XID * target,size_t size, attack_msg & attack,int use_projectile);
	void MultiAttack(const XID * target,size_t size, attack_msg & attack,MsgAdjust & adj,int use_projectile);

	unsigned char GetAttackStamp();
	void IncAttackStamp();
	
	//����
	void SendEnchantMsg(const XID& target, enchant_msg & msg);
	void SendRegionEnchant1Msg(const A3DVECTOR& pos, float radius,enchant_msg & enchant, size_t max_count, std::vector<exclude_target>& target_exclude);
	void Enchant(const XID & target, enchant_msg & msg);
	void EnchantZombie(const XID & target, enchant_msg & msg);

	void RegionEnchant1(const A3DVECTOR& pos, float radius,enchant_msg & msg,size_t mc, std::vector<exclude_target>& target_exclude);			//��
	void RegionEnchant2(const A3DVECTOR& pos, float radius,enchant_msg & msg,size_t mc, std::vector<exclude_target>& target_exclude);			//��
	void RegionEnchant3(const A3DVECTOR& pos, float cos_half_angle,enchant_msg & msg,size_t mc, std::vector<exclude_target>& target_exclude);	//׵
	void MultiEnchant(const XID * target,size_t size, enchant_msg & msg);
	void TeamEnchant(enchant_msg & msg,bool exclude_self, bool norangelimit);
	void TeamEnchantZombie(enchant_msg & msg,bool exclude_self, bool norangelimit, int max_count);

	void RegionEnchantFaceLine(float radius,enchant_msg & enchant,int max_count, std::vector<exclude_target>& target_exclude);				//��
	void RegionEnchantFaceSector(float cos_half_angle, enchant_msg & enchant, int max_count, std::vector<exclude_target>& target_exclude);	//׶
	void RegionEnchantCross(float radius, float angle_to_north, enchant_msg& enchant, int max_count, std::vector<exclude_target>& target_exclude);

	void EnterCombatState();

	void KnockBack(const XID & attacker, const A3DVECTOR &source,float distance);
	void Flee(const XID & attacker, float distance);

	//����
	bool Resurrect(float exp_reduce, float hp_recover); //0.0 ~ 1.0
	bool Resurrect(float exp_reduce);
	
	void SendHealMsg(const XID & target, int hp);		//��ĳ�������Ѫ
	void SendAddManaMsg(const XID & target, int mp);

	void Reclaim();					//��������ʧ

	//��ѯ 
	bool IsFemale();
	bool IsSpouse(const XID & target);
	bool IsMarried();
	bool IsDead();
	bool IsMember(const XID & member);
	bool IsInTeam();
	bool IsTeamLeader();
	bool GetLeader(XID & leader);
	int  GetClass();
	int  GetFaction();
	int  GetEnemyFaction();
	bool IsMafiaMember();
	int  GetMafiaID();
	bool IsFamilyMember();
	int GetFamilyID();
	const A3DVECTOR & GetPos();
	const int & GetTag();
	const XID & GetSelfID();
	const XID & GetCurTargetID();
	float GetBodySize();
	int QueryObject(const XID & who, A3DVECTOR & pos, float & body_size); 	//0 ������ 1: ���� 2: ����
	bool CheckGMPrivilege();
	bool IsPVPEnable();	//�Ƿ�����PK����
	bool IsMount();
	bool IsFlying();
	bool IsGathering();
	bool IsTransformState();

	//���ͻ��˲���
	void SendClientMsgSkillCasting(int target_cnt, const XID* targets, int skill, unsigned short time,unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index);
	void SendClientMsgSkillInterrupted(char reason, char spirit_index);
	void SendClientMsgSkillContinue(int skill_id, char spirit_index);
	void SendClientMsgSkillPerform(char spirit_index);
	void SendClientNotifyRoot(unsigned char type);	//�㲥��Ϊ
	void SendClientSelfNotifyRoot(unsigned char type);	//ֻ���͸�����
	void SendClientDispelRoot(unsigned char type);	//ֻ���͸�����
	void SendClientCurSpeed();		//�����ٶȸ��ͻ���
	void SendClientEnchantResult(const XID & caster, int skill, char level, bool invader,char at_state, char stamp,int value);
	void SendClientInstantSkill(int target_cnt, const XID* targets, int skill, unsigned char level, const A3DVECTOR& pos, char spirit_index);
	void SendClientSkillAbility(int id, int ability);
	void SendClientCastPosSkill(const A3DVECTOR & pos, int skill,unsigned short time, unsigned char level);
	void SendClientRushMode(unsigned char is_active);
	void SendClientAttackData();
	void SendClientDefenseData();
	void SendClientDuelStart(const XID & target);
	void SendClientDuelStop(const XID & target);

	void SendClientSkillAddon(int skill, int level);
	void SendClientSkillCommonAddon(int common);
	void SendClientExtraSkill(int skill, int level);
	void SendClientLearnSkill(int id, int level);
	void SendClientSkillProficiency(int id, int proficiency);
	
	void SendClientAchieveData(size_t size, const void * data);
	void SendClientAchieveFinish(unsigned short achieve_id, int achieve_point, int finish_time);
	void SendClientPremissData(unsigned short achieve_id, char premiss_id,  size_t size, const void * data);
	void SendClientPremissFinish(unsigned short achieve_id, char premiss_id);
	void SendAchievementMessage(unsigned short achieve_id, int broad_type, int param, int finish_time);

	void SendClientTriggerSkillTime(short skillid, short time);
	void SendClientAddonSkillPermanent(int id, int level);


	void UpdateFamilySkill(int skill ,int ability);
	void TalismanGainExp(int exp, bool is_aircraft = false);
	int SpendTalismanStamina(float cost);
	void ReceiveTaskExp(int exp);
	void MagicGainExp(int exp);
	

public:
	//����״̬����
	void IncVisibleState(unsigned short state);
	void DecVisibleState(unsigned short state);
	void ClearVisibleState(unsigned short state);

	void IncDirVisibleState(unsigned short state, short dir);
	void DecDirVisibleState(unsigned short state, short dir);
	void ClearDirVisibleState(unsigned short state, short dir);

	void UpdateBuff(short buff_id, short buff_level, int end_time, int overlayCnt = 1);
	void RemoveBuff(short buff_id, short buff_level);

	//���ñ����־	
	void ChangeShape(int shape);
	int GetShape();

	void LockEquipment(bool is_lock);
	void BindToGound(bool is_bind);

	//����״̬����
	bool IsSilentSeal();
	bool IsDietSeal();
	bool IsMeleeSeal();
	bool IsRootSeal();

	void SetSilentSeal(bool isSeal);
	void SetSelfSilentSeal(bool isSeal);
	void SetDietSeal(bool isSeal);
	void SetMeleeSeal(bool isSeal);
	void SetRootSeal(bool isSeal);

	void ForbidAttack();
	void AllowAttack();

	bool IsAggressive();
	void SetAggressive(bool isActive = true);

	void DuelStart(const XID & target);
	void DuelStop();
	
	//���Լ��ĳ���б������һ�����
	void AddAggro(const XID & attacker , int rage);
	void AddAggroToEnemy(const XID & helper, int rage);
	
	void BeTaunted(const XID & who,int aggro);

	void AddPlayerEffect(short effect);
	void RemovePlayerEffect(short effect);


	//ȡ�������Ĳ�������
	int GetImmuneMask();
	void SetImmuneMask(int mask);		//Ϊ1��λ��Ҫ�������ߵ�����
	void ClearImmuneMask(int mask);		//Ϊ1��λ��Ҫ�������������

	void ActiveMountState(int mount_id, int mount_lvl, bool no_notify, char mount_type = 0);
	void DeactiveMountState();

	bool IsEquipWing();

	void Die();
	void Disappear();

public:
	//��ǿ����
	void EnhanceScaleHPGen(int hpgen);
	void ImpairScaleHPGen(int hpgen);

	void EnhanceScaleMPGen(int mpgen);
	void ImpairScaleMPGen(int mpgen);
	
	void EnhanceScaleDPGen(int dpgen);
	void ImpairScaleDPGen(int dpgen);

	void EnhanceMaxHP(int hp);
	void ImpairMaxHP(int hp);

	void EnhanceMaxMP(int mp);
	void ImpairMaxMP(int mp);
	
	void EnhanceMaxDP(int mp);	
	void ImpairMaxDP(int mp);

	void EnhanceScaleMaxHP(int hp);	
	void ImpairScaleMaxHP(int hp);	

	void EnhanceScaleMaxMP(int mp);	
	void ImpairScaleMaxMP(int mp);	
	
	void EnhanceScaleMaxDP(int mp);	
	void ImpairScaleMaxDP(int mp);	

	void EnhanceDefense(int def);
	void ImpairDefense(int def);
	void EnhanceScaleDefense(int def);
	void ImpairScaleDefense(int def);

	void EnhanceArmor(int ac);
	void ImpairArmor(int ac);
	void EnhanceScaleArmor(int ac);
	void ImpairScaleArmor(int ac);

	void EnhanceResistance(size_t cls, int res);	 //cls = [0,5]
	void ImpairResistance(size_t cls, int res);

	void EnhanceScaleResistance(size_t cls, int res);	 //cls = [0,5] res ǧ����
	void ImpairScaleResistance(size_t cls, int res);

	void EnhanceResistanceTenaciy(size_t cls, int res);	 //cls = [0,5]
	void ImpairResistanceTenaciy(size_t cls, int res);

	void EnhanceScaleResistanceTenaciy(size_t cls, int res);	 //cls = [0,5] res ǧ����
	void ImpairScaleResistanceTenaciy(size_t cls, int res);

	void EnhanceResistanceProficiency(size_t cls, int res);	 //cls = [0,5]
	void ImpairResistanceProficiency(size_t cls, int res);

	void EnhanceScaleResistanceProficiency(size_t cls, int res);	 //cls = [0,5] res ǧ����
	void ImpairScaleResistanceProficiency(size_t cls, int res);

	void EnhanceScaleDamage(int dmg);
	void ImpairScaleDamage(int dmg);

	void EnhanceDamage(int dmg);
	void ImpairDamage(int dmg);

	void EnhanceDmgReduce(int dmg_reduce);
	void ImpairDmgReduce(int dmg_reduce);

	void EnhanceScaleDmgReduce(int dmg_reduce);
	void ImpairScaleDmgReduce(int dmg_reduce);
	
	void EnhanceScaleIgnDmgReduce(int ign_dmg_reduce);
	void ImpairScaleIgnDmgReduce(int ign_dmg_reduce);
	
	void EnhanceScaleDmgChange(int dmg_change);
	void ImpairScaleDmgChange(int dmg_change);

	void EnhanceSkillAttack(int skill_attack);
	void ImpairSkillAttack(int skill_attack);

	void EnhanceSkillArmor(int skill_armor);
	void ImpairSkillArmor(int skill_armor);

	void EnhanceAntiCritRate(int crit_rate);
	void ImpairAntiCritRate(int crit_rate);
	
	void EnhanceAntiCritDamage(float crit_damage);
	void ImpairAntiCritDamage(float crit_damage);

	void EnhanceDeityPower(int power);
	void ImpairDeityPower(int power);

	void EnhanceScaleDeityPower(int power);

	void EnhanceCultDefense(int index, int cult_defense);
	void ImpairCultDefense(int index, int cult_defense);

	void EnhanceCultAttack(int index, int cult_attack);
	void ImpairCultAttack(int index, int cult_attack);

	void EnhancePetDamage(int adjust);
	void EnhancePetAttackDefense(int adjust);

	void EnhanceOverrideSpeed(float speedup);
	void ImpairOverrideSpeed(float speedup);

	void EnhanceMountSpeed(float inc);
	void ImpairMountSpeed(float dec);

	void EnhanceSpeed(float speedup);
	void ImpairSpeed(float speedup);

	void EnhanceScaleSpeed(int inc);
	void ImpairScaleSpeed(int inc);

	void EnhanceScaleAttack(int attack);
	void ImpairScaleAttack(int attack);

	void EnhanceAttack(int attack);
	void ImpairAttack(int attack);

	void EnhanceCrit(int val);
	void ImpairCrit(int val);

	void EnhanceCritDmg(float val);
	void ImpairCritDmg(float val);

	void SetOverSpeed(float sp);

	//������
	void EnhanceAntiTransform(int val);
	void ImpairAntiTransform(int val);
	void EnhanceScaleAntiTransform(int val);
	void ImpairScaleAntiTransform(int val);

	//��ʳ����	
	void EnhanceAntiDiet(int val);
	void ImpairAntiDiet(int val);


	//���Ե����¼���
	void UpdateDefenseData();
	void UpdateAttackData();
	void UpdateMagicData();	//���￹��
	void UpdateSpeedData();
	void UpdateHPMPGen();
	void UpdateHPMP();
	void UpdateDPGen();
	void UpdateDP();
	void UpdateAllProp();

	void SetInvincibleFilter(bool is_vin, int timeout,bool immune);
	void AdjustDropRate(float rate);

//��ȴ
	bool TestCoolDown(unsigned short id);
	void SetCoolDown(unsigned short id, int ms);
	void ClrCoolDown(unsigned short id);

	void ReturnToTown();
	bool CanReturnToTown();

	void ReturnWaypoint(int point);
	bool CheckWaypoint(int point_index, int & point_domain);

	void JumpToSpouse();
	void SkillMove(const A3DVECTOR & pos);
	void BreakCurAction();

	int CalcPhysicDamage(int raw_damage, int attacker_level);
	int CalcMagicDamage(int dmg_class, int raw_damage, int attacker_level);

	float CalcLevelDamagePunish(int atk_level , int def_level);

	bool ModifySkillPoint(int offset);
	bool ModifyTalentPoint(int offset);
	void EnterSanctuary();
	void LeaveSanctuary();
	int GetCultivation();

public:
	//��Ʒ
	int CreateItem(int item_id,int count,int period);//������Ʒ
	int TakeOutItem(int item_id);			//ȥ��һ����Ʒ
	int TakeOutItem(int item_id, int count);	//ȥ�������Ʒ
	int TakeOutItem(int inv_index, int item_id, int count); //ȥ��ָ��λ��ĳ����Ʒ
	bool CheckItem(int item_id,size_t count);	//����Ƿ����ĳ����Ʒ
	bool CheckItem(int inv_index, int item_id, size_t count); //����ƶ�λ���Ƿ����ĳ����Ʒ
	size_t GetMoney();
	void DecMoney(size_t money);
	void AddMoney(size_t inc);
	size_t GetInventorySize();
	size_t QueryItemPrice(int inv_index,int item_id);
	size_t QueryItemPrice(int item_id);
	size_t GetMallOrdersCount();
	int GetMallOrders(GDB::shoplog * list, size_t size);
	int GetRegionReputation(int index);
	void ModifyRegionReputation(int index, int rep);

	int GetLinkIndex();
	int GetLinkSID();
	int GetInventoryDetail(GDB::itemdata * list, size_t size);
	int GetTrashBoxDetail(GDB::itemdata * list, size_t size);
	int GetMafiaTrashBoxDetail(GDB::itemdata * list, size_t size);
	int GetEquipmentDetail(GDB::itemdata * list, size_t size);
	bool GetMallInfo(int & cash_used,int & cash, int &cash_delta,  int &order_id);
	
	bool IsCashModified();
	size_t GetEquipmentSize();
	size_t GetTrashBoxCapacity();
	size_t GetMafiaTrashBoxCapacity();
	size_t GetTrashBoxMoney();
	bool IsTrashBoxModified();
	bool IsEquipmentModified();
	int TradeLockPlayer(int get_mask,int put_mask);
	int TradeUnLockPlayer();
	int GetDBTimeStamp();
	int InceaseDBTimeStamp();

	int GetCityOwner(int city_id);
	bool TestSafeLock();
	int RebornCount();
	void Teleport(int tag, const A3DVECTOR & pos);
	unsigned int GetDBMagicNumber();

	size_t GetPetBedgeInventorySize();
	int GetPetBedgeInventoryDetail(GDB::itemdata * list, size_t size);
	size_t GetPetEquipInventorySize();
	int GetPetEquipInventoryDetail(GDB::itemdata * list, size_t size);

	size_t GetPocketInventorySize();
	int GetPocketInventoryDetail(GDB::pocket_item* list, size_t size);
	
	size_t GetFashionInventorySize();
	int GetFashionInventoryDetail(GDB::itemdata* list, size_t size);

	size_t GetMountWingInventorySize();
	int GetMountWingInventoryDetail(GDB::itemdata* list, size_t size);

	size_t GetGiftInventorySize();
	int GetGiftInventoryDetail(GDB::itemdata* list, size_t size);

	size_t GetFuwenInventorySize();
	int GetFuwenInventoryDetail(GDB::itemdata* list, size_t size);
public:
	//��һ��С��
	struct minor_param
	{
		int mob_id;		//ģ��ID�Ƕ���
		int vis_id;		//�ɼ�id�����Ϊ0��ֵ��Ч
		int remain_time;	//0 ��ʾ���� �����ʾ����������
		int policy_classid;	//����������д�� ����ҪĬ����д0
		int policy_aggro;	//����������д�� ����ҪĬ����д0
		float exp_factor;	//����ֵ����
		float drop_rate;	//����������
		float money_scale;	//�����Ǯ����
		XID spec_leader_id;	//ָ����leader��˭ 
		bool parent_is_leader;	//�����߾���leader ��ʱ spec_leader_id ��Ч
		bool use_parent_faction;//ʹ�õ����ߵ���Ӫ��Ϣ������ʹ��Ĭ������
		bool die_with_leader;	//leader ��������ʧ���Լ�Ҳ��ʧ
		unsigned char mob_name_size;	//��0��������
		char mob_name[18];
		const void * script_data;
		size_t script_size;
	};

	struct mine_param
	{
		int mine_id;		//ģ��ID�Ƕ���
		int remain_time;	//0 ��ʾ���� �����ʾ����������
	};

	void GenOwnerIDByType(char type, int & owner_id1, int & owner_id2);

	void CreateMinors(const A3DVECTOR & pos ,const minor_param & p);//��ָ��λ�ô���С��
	void CreateMinors(const minor_param & p,float radius = 6.0f);	//�ڸ��������λ�ô���С��
	void CreateMine(const A3DVECTOR & pos , const mine_param & p);	//���ƶ�λ�ô�������
	void CreateMines(int mid, int count, int lifetime, float range);//���ƶ�λ�ô�������

	//Add by Houjun 2011-09-22 �����û���ߵĿ���͹��ﴴ���ӿڣ�����������
	void CreateMines(int mid, int tag, A3DVECTOR & pos, int lifetime, int count, char owner_type, float radius = 6.0f);//����������ָ����ͼ��λ��ˢ���й����Ŀ���
	void CreateMonster(int id, int tag, A3DVECTOR & pos, int lifetime, int count, char owner_type, float radius = 6.0f);//����������ָ����ͼ��λ��ˢ���й����Ĺ���
	//end

	void CreateNPC(const  object_interface::minor_param & param,float radius = 6.0f); //�ڸ������λ�ô���NPC
	void CreateNPC(const A3DVECTOR & pos, const  object_interface::minor_param & param); //��ָ��λ�ô���NPC
	void CreateProtectedNPC(int id, int lifetime, int task_id);

	bool CreatePet(const A3DVECTOR& pos,const pet_data* pData,const pet_bedge_essence* pEss,int tid,size_t pet_index,XID& who);

	static void CreateMob(world * pPlane,const A3DVECTOR & pos ,const minor_param & p);//����һ������ ��С��

	bool SummonMonster(int id, int count, int lifetime, char type, int skill_level, bool is_clone = false, bool exchange = false);  
	void UnSummonMonster(char type,  bool is_clone);

	void CreateMinors(float dist, float angle, const minor_param& p);//���������˳ʱ��תangle�Ƕȣ�dist�����λ�ô���С��
	bool CreateSkillObject(int id, int count, int lifetime, int skill_level, float dist, float angle, int move_state);
	bool CreateSkillObject(int id, int count, int lifetime, int skill_level, A3DVECTOR& pos, int move_state);

	long GetGlobalValue(long lKey);
	void PutGlobalValue(long lKey, long lValue);
	void ModifyGlobalValue(long lKey, long lValue);
	void AssignGlobalValue(long srcKey, long destKey);

	void PlayerCatchPet(const XID& catcher);
	void PlayerDiscover(int discover_type);

	//������
	bool CanTransform(int template_id);
	void TransformMonster(int template_id, int lifetime);

	//����
	bool StartTransform(int template_id, int level, int exp_level, int timeout, char type);
	void StopTransform(int template_id, char type);
	int  GetTransformID();
	int GetTransformLevel();
	int GetTransformExpLevel();

	//����ģ��ID
	int GetTemplateID();
	void DropItem(unsigned int item_id, unsigned int item_num, unsigned int expire_date);

	//������
	void GetComboColor(int & c1, int & c2, int & c3, int & c4, int & c5);
	void ClearComboSkill();

	//Ӱ��	
	bool CanSetInvisible();
	bool IsInvisible();
	void SetInvisible(int invisible_rate);
	void ClearInvisible(int invisible_rate);
	void EnhanceInvisible(int value);
	void ImpairInvisible(int value);
	void EnhanceAntiInvisible(int value);
	void ImpairAntiInvisible(int value);
	void SetDimState(bool is_dim);

	//����
	const XID  GetCloneID();
	bool IsCloneExist();
	void ExchangePos(const XID & who);
	void ExchangeStatus(const XID & who);

	//˲��
	A3DVECTOR GetPosBetween(const A3DVECTOR& start, const A3DVECTOR& end, float dist_from_start);
	bool CheckTargetPosReachable(const A3DVECTOR& targetPos);
	bool CollisionDetect(const A3DVECTOR& targetPos, A3DVECTOR& nearPos, float sampledist = 2.0f);
	bool CanCharge(const XID& target_id, const A3DVECTOR& destPos, char chargeType, float distance);
	void Charge(const XID& target_id, const A3DVECTOR& destPos, char chargeType);
	void ChargeToTarget(const XID& target_id, const A3DVECTOR& destPos);

	//�������ͼ��ܾ�
	void SetCloneMirror(bool is_mirror, int prop);
	void SetSkillMirror(bool is_mirror, int prop);
	void SetSkillReflect(bool is_reflect, int prop);

	//��������
	void SetAbsoluteZone(int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state);

	//��ҩЧ����ǿ�ͼ���
	void SetHealEffect(int value);

	void SetSkillTalent(int id, int skill_talent[8]);

	void SetBreakCasting();

	//���������״̬
	bool IsIgniteState() ;
	bool IsFrozenState() ;
	bool IsColdInjureState();
	bool IsFuryState() ;

	void SetIgniteState(bool on); 
	void SetFrozenState(bool on);
	void SetColdInjureState(bool on);
	void SetFuryState(bool on);
	void SetDarkState(bool on);

	//��ȡ�ض���Χ���������
	int GetRegionPlayerCount(float radius, std::vector<exclude_target>& target_exclude);

	//���ķ�����Ϊ
	bool DecDeityExp(int exp);	

	//ת�ƹ���
	void TransferAttack(XID & target, char force_attack,  int dmg_plus, int radius, int total); 

	unsigned char GetDirection();
	void SetTurnBuffState(bool on);
	void SetTurnDebuffState(bool on);
	void SetBloodThirstyState(bool on);
    bool IsBloodThirstyState();
	void ScalePlayer(char on, int scale_ratio);

	//������ؽӿ�
	void AddLimitCycleArea(int owner, const A3DVECTOR& center, float radius);
	void AddPermitCycleArea(int owner, const A3DVECTOR& center, float radius);
	void RemoveLimitCycleArea(int owner);
	void RemovePermitCycleArea(int owner);
	void ClearLimitCycleArea();
	void ClearPermitCycleArea();
	void GetPlayerInJail(std::vector<XID>& list, const A3DVECTOR& target, float radius, float height);
	void SendRemovePermitCycleArea(const XID& target);

	bool IsLimitCycleAreaExist(int owner);
	bool IsPermitCycleAreaExist(int owner);

	//������ؽӿ�
	bool PlayerFollowTarget(const XID& target, float speed, float stop_dist);
	bool StopPlayerFollowTarget();

	void PlayerPulling(bool on);
	void PlayerBePulled(const XID& pulling_id, bool on);
	void PlayerBeSpiritDraged(const XID& id_drag_me, bool on);
	void NotifyCancelPull(const XID& target);

	//��ײս��ר�ü���
	void pz_maxspeed();		//����
	void pz_halfspeed();		//����
	void pz_ashill(bool on);	//������ɽ
	void pz_nomove(bool on);	//����
	void pz_chaos(bool on);		//����
	void pz_void(bool on);		//����	

	//��ԯ�ⰵֵ
	void SetDarkLight(int);
	int GetDarkLight();
	void IncDarkLight(int);
	void DecDarkLight(int);

	void SetDarkLightForm(char);
	void ClearDarkLightForm();
	char GetDarkLightForm();
	bool IsXuanYuan();

	//����
	void PreFly();
	bool CheckCanFly();
	void ActiveFlyState(char type);
	void DeactiveFlyState(char type);

	//��ԯ����
	void SetDarkLightSpirit(int idx, char type);
	char GetDarkLightSpirit(int idx);
	void AddDarkLightSpirit(char type);

	void SummonCastSkill(int summonId, const XID& target, int skillid, int skilllevel);
	void SendMirrorImageCnt(int cnt);

	bool QueryTalismanEffects(int& level, std::vector<short>& skills);

	void SetTalismanEmbedSkillCooltime(int skill_id, int cooltime);
	void SendTalismanSkillEffects(const XID& target, int level, float range, char force, int skill_var[16], const std::vector<short>& skills);

	void SetExtraEquipEffectState(bool on, char weapon_effect_level);

	bool IsMaster();

	int GetXPSkill();

public:
	//�����
	/**
	 * @brief CircleOfDoomPrepare 
	 *
	 * @param radius: �뾶
	 * @param faction: ��ӡ����ɡ�����
	 * @param max_member_num: ��Ա������������������Լ���
	 * @param skill_id: ��ʹ�õ��󷨼���id
	 */
	void CircleOfDoomPrepare( float radius, int faction, int max_member_num, int skill_id);

	/**
	 * @brief CircleOfDoomStartup: �󷨽��ʱ���ã���Ҫ�����������������˵�״̬��
	 */
	void CircleOfDoomStartup();

	/**
	 * @brief CircleOfDoomStop: �󷨽�����ʱ����ã����������������������
	 */
	void CircleOfDoomStop();

	/**
	 * @brief GetPlayerInCircleOfDoom 
	 *
	 * @param playerlist:�󷨳�Ա��list���������Լ��� 
	 *
	 * @return: ����
	 */
	int  GetPlayerInCircleOfDoom( std::vector<XID> &playerlist );

	/**
	 * @brief IsInCircleOfDoom :�ж�ʩ�����Ƿ��Ѿ��ڱ���������ǲ������ͷż���
	 *
	 * @return :true��ʾ�Ѿ������false��ʾû�����
	 */
	bool IsInCircleOfDoom();

	int GetCircleMemberCnt();

	void EnhanceProp(int prop_id, int value);
	void ImpairProp(int prop_id, int value);

	
public:
	//ȡ�û�������
	const basic_prop & 		GetBasicProp();
	const q_extend_prop & 		GetExtendProp();
	const q_enhanced_param & 	GetEnhancedParam();
	const q_scale_enhanced_param & 	GetScaleEnhanecdParam();
	const q_item_prop &		GetCurWeapon();
	GNET::SkillWrapper &		GetSkillWrapper();
	const int GetCurWeaponType(); 
};
#endif

