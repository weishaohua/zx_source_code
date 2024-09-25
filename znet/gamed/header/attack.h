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
	int wallow_level;
};

struct attack_msg
{
	attacker_info_t ainfo;		//�����ߵ���Ϣ
	
	int damage_low;			//��͹�����
	int damage_high;		//��󹥻���
	int spec_damage;		//���⸽�ӵĺ��ӹ�����damage
	float damage_factor;		//����ϵ�� ��������ϵ��˺�Ҫ�˵�ֵ
	int attack_rate;		//������������
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

	int skill_id;			//�Ƿ��ܹ����� �����Ӧ�ļ��ܺ�
	char skill_modify[4];	
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
		PVP_MAFIA_DUEL	= 0x10,		//�˴��ǰ��ɾ���
		PVP_SANCTUARY	= 0x20,		//��ȫ��
		PVP_HOSTILE_DUEL = 0x40,	// �˴ι����ǵж԰��ɹ���
	};
};

struct damage_entry
{
	float damage;
	float adjust;
	damage_entry():damage(0),adjust(1.0f){}
	damage_entry(float damage, float adjust):damage(damage),adjust(adjust)
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
	char skill_level;
	char attacker_layer;
	char helpful;			//�Ƿ����ⷨ��
	char attacker_mode;		//�����ߵ�״̬(0 ��PK,1:PK)
	char is_invader;		//�Ƿ��ӹ���,������ǰ�ж���
	char attack_state;		//0x01 �ػ�  0x02 �����Ż���  0x80 ���ߴ˴ι���
	unsigned char attack_stamp;	//����ʱ��
	char skill_modify[4];	
};

struct old_damage_entry
{
	float physic_damage;
	float magic_damage[MAGIC_CLASS];
};

#endif

