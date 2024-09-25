#ifndef __ONLINEGAME_GS_OBJ_PROPERTY_H__
#define __ONLINEGAME_GS_OBJ_PROPERTY_H__ 
#include "config.h"

//����Ļ�������(���������Ӱ������ԣ�
struct basic_prop
{
	short level;		//����
	short sec_level;	//���漶��
	int exp;		//����ֵ
	int hp;			//��ǰhp
	int mp;			//��ǰmp
	int status_point;	//ʣ������Ե�
};


template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, basic_prop &rhs)
{
	return wrapper.push_back(&rhs, sizeof(rhs));
}


template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, basic_prop &rhs)
{
	return wrapper.pop_back(&rhs, sizeof(rhs));
}

struct q_extend_prop
{
	int 	max_hp;
	int 	max_mp;
	int 	hp_gen[4];
	int 	mp_gen[4];

	float	walk_speed;
	float 	run_speed;

	int 	crit_rate;
	float 	crit_damage;

	int 	damage_low;
	int	damage_high;

	int 	attack;
	int 	defense;
	int	armor;

	int resistance[6];	//6������ħ������
	float attack_range;	//��������

};

template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const q_extend_prop &rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, q_extend_prop &rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}

//װ�������ı�����ǿ
struct q_item_prop
{
	int weapon_class;		//������id
	int damage_low;			//������͹�����
	int damage_high;		//������߹�����
	float attack_range;		//����������Χ
	int max_hp;
	int max_mp;
	int armor;
	int defense;
	int attack;
	int attack_cycle;		//����ѭ����ʱ��
	int attack_point;		//�������ʱ��
};

struct q_enhanced_param
{
	int 	max_hp;
	int 	max_mp;
	int 	defense;
	int 	attack;
	int	armor;
	int	damage;
	int 	resistance[6];	//6������ħ������
	int 	crit_rate;
	float 	crit_damage;
	float 	run_speed;
	int 	dmg_reduce;
	int 	spec_damage;
	float 	override_speed;		//�����ٶȣ�ֻ��½����Ч
};

struct q_scale_enhanced_param
{
	int 	max_hp;
	int 	max_mp;
	int 	hp_gen;
	int 	mp_gen;
	int 	defense;
	int 	attack;
	int	armor;
	int	damage;
	int	dmg_reduce;

	int 	resistance[6];	//6������ħ������
};


//��չ���ԣ���ʱ���յ����Ӱ�������
struct extend_prop
{
	/* �������� */
	int vitality;		//��
	int energy;		//��
	int strength;		//��
	int agility;		//��
	int max_hp;		//���hp
	int max_mp;		//���mp
	int hp_gen;		//hp�ָ��ٶ�
	int mp_gen;		//mp�ָ��ٶ� 

	/* �˶��ٶ�*/
	float walk_speed;	//�����ٶ� ��λ  m/s
	float run_speed;	//�����ٶ� ��λ  m/s
	float swim_speed;	//��Ӿ�ٶ� ��λ  m/s
	float flight_speed;	//�����ٶ� ��λ  m/s

	/* ��������*/
	int attack;		//������ attack rate
	int damage_low;		//���damage
	int damage_high;	//�������damage
	int attack_speed;	//����ʱ���� ��tickΪ��λ
	float attack_range;	//������Χ
	struct 			//����������ħ���˺�
	{
		int damage_low;
		int damage_high;
	} addon_damage[MAGIC_CLASS];		
	int damage_magic_low;		//ħ����͹�����
	int damage_magic_high;		//ħ����߹�����

	/* �������� */	
	int resistance[MAGIC_CLASS];	//ħ������
	int defense;		//������
	int armor;		//�����ʣ�װ�׵ȼ���
	int max_ap;		//���ŭ��ֵ

	void BuildFrom(const q_extend_prop &  rhs)
	{
		max_hp 		= rhs.max_hp;
		max_mp 		= rhs.max_hp;
		hp_gen		= rhs.hp_gen[0];
		mp_gen		= rhs.mp_gen[0];
		

		walk_speed 	= rhs.walk_speed;
		run_speed 	= rhs.run_speed;
		swim_speed 	= rhs.run_speed;
		flight_speed 	= rhs.run_speed;

		attack		= rhs.attack;
		damage_low 	= rhs.damage_low;
		damage_high 	= rhs.damage_high;
		attack_speed	= 10;
		attack_range	= 2.5f;
		damage_magic_low= 0;
		damage_magic_high =0;
		defense 	= rhs.defense;
		armor 		= rhs.armor;
	}
};

	template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const extend_prop &rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

	template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, extend_prop &rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}


//װ�������ı�����ǿ
struct item_prop
{
	short weapon_type;		//�����������melee,range,missile��
	short weapon_delay;		//������������ӳ�ʱ�� ��λ����һ��tick
	int attack_delay;		//�������ӳ�ʱ�䣬������ݲ���ֱ�Ӵ�����������
	int weapon_class;		//�����Ĺ�������
	int weapon_level;		//�����ļ���
	int damage_low;			//������͹�����
	int damage_high;		//������߹�����
	int damage_magic_low;		//ħ��������͹�����
	int damage_magic_high;		//ħ��������߹�����
	int attack_speed;		//���������ٶ�����
	float attack_range;		//����������Χ
	float short_range;		//��������С�������루����������뽫�ܵ������ͷ���
	/*
	   int attack;			//���������Ĺ���������
	 */
	//	int defense;			//װ�������ķ���
	//	int armor;			//װ����������
};

//���ֲ��������ķǱ�����ǿ������Ϊ��ֵ
struct enhanced_param
{
	/*��������*/
	int max_mp;			//���mana��ֵ
	int max_hp;			//���������ֵ
	int hp_gen;
	int mp_gen;
	int vit;
	int eng;
	int agi;
	int str;

	/*��������*/
	//int weapon_damage;		//����������ֵ	��������ӳ�
	int damage_low;
	int damage_high;		//��������ֵ
	int attack;			//������
	int attack_speed;
	float attack_range;
	float walk_speed;
	float run_speed;
	float flight_speed;		//����
	
	int addon_damage[MAGIC_CLASS];	//����������ħ���˺�
	/*ħ������*/
	int resistance[MAGIC_CLASS];	//ħ������
	int magic_dmg_low;			//ħ��������ֵ
	int magic_dmg_high;			//ħ��������ֵ
	//	int magic_weapon_dmg;		//ħ������������ֵ ��������ӳ�

	/*��������*/	
	int defense;			//������ֵ(װ��)
	int armor;			//�����ֵ(װ��)

	float override_speed;		//�����ٶȣ�ֻ��½����Ч
};

//���ֲ��������ķǱ�����ǿ(���ǰٷֱ�)������Ϊ��ֵ
struct scale_enhanced_param
{
	int max_mp;			//���mana��ֵ
	int max_hp;			//���������ֵ

	int hp_gen;			//hp�ָ��ٶ����ӱ���
	int mp_gen;			//mp�ָ��ٶ����ӱ���

	int walk_speed;			//��·
	int run_speed;			//�ܲ�
	int swim_speed;			//��Ӿ	
	int flight_speed;		//����

	int attack_speed;		//�����ٶ�

	int damage;			//��������ֵ
	int attack;			//������

	int base_damage;		//�ɱ������ݲ����İٷֱȼӳ�
	int base_magic;			//�ɱ������ݲ�����ħ�������ٷֱȼӳ�

	int magic_dmg;			//ħ��������ֵ
	int resistance[MAGIC_CLASS];	//ħ������

	int defense;			//������ֵ(װ��)
	int armor;			//�����ֵ(װ��)
};

class world_manager;
struct team_mutable_prop
{
	short level;
	char  combat_state;
	unsigned char sec_level;
	int hp;
	int mp;
	int max_hp;
	int max_mp;
	int world_tag;
	team_mutable_prop(){}
	template <typename ACTIVE_IMP>
		explicit team_mutable_prop(ACTIVE_IMP * pImp)
		{
			Init(pImp);
		}

/*	team_mutable_prop(const basic_prop & bp, const extend_prop &ep,char cs, int tag)
		:level(bp.level),combat_state(cs),sec_level(bp.sec_level),hp(bp.hp),mp(bp.mp),max_hp(ep.max_hp),max_mp(ep.max_mp)
		{}*/

	template <typename ACTIVE_IMP>
		void Init(ACTIVE_IMP * pImp)
		{
			const basic_prop & bp = pImp->_basic;
			const q_extend_prop &ep = pImp->_cur_prop;
			level = bp.level;
			combat_state = pImp->IsCombatState()?1:0;
			sec_level = bp.sec_level;
			hp = bp.hp;
			mp = bp.mp;
			max_hp = ep.max_hp;
			max_mp = ep.max_mp;
			world_tag = pImp->GetWorldTag();
		}

	bool operator == (const team_mutable_prop & rhs)
	{
		return memcmp(this,&rhs,sizeof(rhs)) == 0;
	}

	bool operator != (const team_mutable_prop & rhs)
	{
		return memcmp(this,&rhs,sizeof(rhs));
	}
};

template <typename WRAPPER>
WRAPPER & operator <<(WRAPPER & wrapper, const team_mutable_prop & rhs)
{
	wrapper.push_back(&rhs,sizeof(rhs));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator >>(WRAPPER & wrapper, team_mutable_prop &rhs)
{
	wrapper.pop_back(&rhs,sizeof(rhs));
	return wrapper;
}

/*
struct  team_member_data
{
	int id;
	team_mutable_prop data;
	team_member_data()
	{}

	team_member_data(int member,const team_mutable_prop &prop):id(member),data(prop)
	{}
};
*/
#endif
