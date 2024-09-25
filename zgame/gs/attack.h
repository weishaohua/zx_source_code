#ifndef __ONLINEGAME_GS_ATTACK_H__
#define __ONLINEGAME_GS_ATTACK_H__


#include "config.h"
#include <common/types.h>


//����������������������� �� ���ó��﹥�������ҵĹ���
struct attacker_info_t
{
	XID attacker;			//�����ߵ�ID ���ڳ�� ��ID����ϢID��һ��
	short level;			//�����߼���
	short eff_level;		//��Ч���� Ϊ�����׼����
	int team_id;			//��������id
	int team_seq;			//������������
	int cs_index;			//�����player����ʾ��player��cs_index
	int sid;			//�����player����ʾ��player��cs_index ��Ӧ��sid 
	int mafia_id;			//������ţ���0��Ϊ���Ź������˹�������Ӱ�����ڳ�Ա
	int family_id;			//����ID,���ڹ����ж�
	int master_id;			//ʦ��ID 
	int wallow_level;
	int invisible_rate;		//��������
	int zone_id;			//������id
};

//����ʹ������
enum
{
	SKILL_LIMIT_MONSTER 	= 0x00000001,	//�Թ�����Ч
	SKILL_LIMIT_PET			= 0x00000002,	//�Գ�����Ч
	SKILL_LIMIT_SUMMON		= 0x00000004,	//���ٻ���Ч
	SKILL_LIMIT_PLAYER		= 0x00000008,	//��������Ч
	SKILL_LIMIT_SELF		= 0x00000010,	//��������Ч
	SKILL_LIMIT_BATTLEFIELD	= 0x00000020,	//��ս���¿���

	//Add by Houjun 2010-02-08
	SKILL_LIMIT_COMBAT		= 0x00000040,	//ս��ʱ����ʹ��
	SKILL_LIMIT_NONCOMBAT	= 0x00000080, 	//ս���ⲻ��ʹ��
	SKILL_LIMIT_NOSUMMONPET = 0x00000100,	//���ٻ��޲���ʹ��
	//Add end.

	//Add by Houjun 2010-03-10
	SKILL_LIMIT_MOUNT 		= 0x00000200,	//���ʱ����ʹ��
	SKILL_LIMIT_FLY			= 0x00000400,	//����ʱ����ʹ��
	//Add end.
	
	SKILL_LIMIT_NOSUMMON	= 0x00000800,	//���ٻ��޲����ã������ٻ���ID����ָ��
	SKILL_LIMIT_BOSS		= 0x00001000,	//��BOSS��Ч
	SKILL_LIMIT_INVISIBLE_NPC	= 0x00002000,	//��������NPC��Ч
};
	
struct attack_msg
{
	attacker_info_t ainfo;		//�����ߵ���Ϣ
	
	int damage_low;			//��͹�����
	int damage_high;		//��󹥻���
	int damage_no_crit;		//�ޱ����˺�
	int spec_damage;		//���⸽�ӵĺ��ӹ�����damage
	float damage_factor;		//����ϵ�� ��������ϵ��˺�Ҫ�˵�ֵ
	int attack_rate;		//������������
	int skill_attack_rate;		//���ܹ���������
	int attacker_faction;		//��������Ӫ
	int target_faction;		//�����ߵĵ�����Ӫ(�Լ�����Ӫֻ�з��������Ӫ���ܱ���ǿ�ƹ����˺�)
	int crit_rate;			//���������ĸ���
	float crit_factor;		//�����������������Ӧ�ó˵�����
	char normal_attack;		//�Ƿ���ͨ���� ��ͨ�����Ż��������
	char force_attack;		//�Ƿ�ǿ�ƹ���
	char attack_state;		//0x01 �ػ�  0x02 �����Ż���
	char attacker_mode;		//�����ߵ�״̬(0x01 PK , 0x02 FREE, 0x04 �ѽ���PKģʽ)
	char is_invader;		//�Ƿ��ӹ���,������ǰ�ж���
	unsigned char attack_stamp;	//����ʱ��
	char cultivation;		//��ħ��
	char is_pet;
	char is_flying;			//�������Ƿ��ڷ���	
	char is_mirror;			//�Ƿ��Ƿ����ļ�����Ϣ
	char combo_color[5];
	int pet_attack_adjust;		//���﹥����ǿ

	int skill_id;			//�Ƿ��ܹ����� �����Ӧ�ļ��ܺ�
	int skill_modify[8];	
	unsigned int skill_limit;	//����ʹ������
	int feedback_mask;
	
	struct
	{
		int skill;
		int level;
	} attached_skill;

	int fashion_weapon_addon_id;	// Youshuang add
	
	int skill_var[16];
	unsigned short skill_element[10];
	int ign_dmg_reduce;		//�����˺�����İٷֱ�
	int dmg_change;			//����(����)�������˺��ٷֱ�
	int cult_attack[3];		//�컯���� �˷� ��ħ
	int deity_power;		//��ʥ��
	int fixed_damage;		//һ����Ч�Ĺ̶��˺�, �����κη����ͼ���

	short talismanSkills[8];//����Ƿ�������Ҫ����������������
	int resistance_proficiency[6];	//���Ծ�ͨ

	enum 
	{
		PVP_ENABLE	= 0x01,
		PVP_FREE   	= 0x02,		//����PK
		PVP_DURATION 	= 0x04,		//�Ѿ��ں����PK
		PVP_DUEL	= 0x08,		//�˴��Ǿ�������
		PVP_MAFIA_DUEL	= 0x10,		//�˴��ǰ��ɾ���
		PVP_SANCTUARY	= 0x20,		//��ȫ��
		PVP_HOSTILE_DUEL = 0x40,	// �˴ι����ǵж԰��ɹ���
	};
};

struct damage_entry
{
	float damage;
	float adjust;
	bool  ignore_attack;
	float damage_no_crit;
	damage_entry():damage(0),adjust(1.0f),ignore_attack(0),damage_no_crit(0) {}
	damage_entry(float damage, float adjust,float damage_no_crit = 0):damage(damage),adjust(adjust),ignore_attack(0),damage_no_crit(damage_no_crit)
	{
	}
};

struct old_attack_msg
{
	attacker_info_t ainfo;		//�����ߵ���Ϣ

	float attack_range;		//�˴ι����ķ�Χ������������Ϣ���棩
	float short_range;		//�˴ι�������С��Χ  �������������Χ�򹥻������� ħ����������
	int physic_damage;		//���������˺���
	int attack_rate;		//������������
	int magic_damage[MAGIC_CLASS];	//ħ���˺���
	int attacker_faction;		//��������Ӫ
	int target_faction;		//�����ߵĵ�����Ӫ(�Լ�����Ӫֻ�з��������Ӫ���ܱ���ǿ�ƹ����˺�)
	int crit_rate;
	char physic_attack;		//�Ƿ�������
	char force_attack;		//�Ƿ�ǿ�ƹ���
	char attacker_layer;		//�����ߴ���ʲôλ�� 0 ���� 1 ���� 2 ˮ�� 
	char attack_state;		//0x01 �ػ�  0x02 �����Ż���
	short speed;
	char attacker_mode;		//�����ߵ�״̬(0x01 PK , 0x02 FREE, 0x04 �ѽ���PKģʽ)
	char is_invader;		//�Ƿ��ӹ���,������ǰ�ж���
	unsigned char attack_stamp;	//����ʱ��
	int skill_id;			//�Ƿ��ܹ����� �����Ӧ�ļ��ܺ�
	struct
	{
		int skill;
		int level;
	} attached_skill;

	enum 
	{
		PVP_ENABLE	= 0x01,
		PVP_FREE   	= 0x02,		//����PK
		PVP_DURATION 	= 0x04,		//�Ѿ��ں����PK
		PVP_DUEL	= 0x08,		//�˴��Ǿ�������
	};
};

struct enchant_msg
{
	attacker_info_t ainfo;		//�����ߵ���Ϣ

	int attacker_faction;		//��������Ӫ
	int target_faction;		//�����ߵĵ�����Ӫ(�Լ�����Ӫֻ�з��������Ӫ���ܱ���ǿ�ƹ����˺�)
	float attack_range;
	int skill;
	int skill_reserved1;		//�����ڲ�ʹ��
	char force_attack;		//�Ƿ�ǿ�ƹ���
	short skill_level;
	char attacker_layer;
	char helpful;			//�Ƿ����ⷨ��
	char attacker_mode;		//�����ߵ�״̬(0 ��PK,1:PK)
	char is_invader;		//�Ƿ��ӹ���,������ǰ�ж���
	char attack_state;		//0x01 �ػ�  0x02 �����Ż���  0x80 ���ߴ˴ι���
	unsigned char attack_stamp;	//����ʱ��
	char is_flying;			//�������Ƿ��ڷ���	
	char is_mirror;			//�Ƿ��Ƿ����ļ�����Ϣ
	char combo_color[5];
	int skill_modify[8];	
	unsigned int skill_limit;	//����ʹ������
	int skill_var[16];
	int resistance_proficiency[6];	//���Ծ�ͨ
	unsigned short skill_element[10];
	short talismanSkills[8];//����Ƿ�������Ҫ����������������
};

struct old_damage_entry
{
	float physic_damage;
	float magic_damage[MAGIC_CLASS];
};

#endif

