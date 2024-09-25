//��ұ���ʵ��
#ifndef __ONLINEGAME_GS_PLAYER_TRANSFORM_H
#define __ONLINEGAME_GS_PLAYER_TRANSFORM_H

#include <vector.h>
#include <amemory.h>
#include <common/base_wrapper.h>
#include "property.h"

struct transform_skill
{
	int skill_id;
	int skill_level;
};

class gplayer_imp;
class player_transform
{
protected:
	int _template_id;			//�������õ�ģ�� 
	unsigned char _transform_type;		//��������: ��Ʒ�� ���ܣ� ����, BuffArea
	unsigned char _transform_level;		//����ȼ�
	unsigned char _transform_explevel;	//���������ȵȼ�
	unsigned char _move_mode;		//�ƶ���ʽ: 0: ���Կ����ƶ� 1: �����Կ����ƶ�  2:����ƶ�	
	unsigned char _buff_type;		// 0 - ����buff 1 - ����buff
		
	bool _can_skill;			//�Ƿ��ܹ�ʹ�ü���
	bool _can_bot;				//�Ƿ��ܹ����˺�һ 
	bool _can_attack;			//�Ƿ��ܹ�����
	bool _can_fly;				//�Ƿ��ܹ�����
	bool _can_use_potion;			//�Ƿ��ܹ���ҩ
	bool _can_change_faction;		//�Ƿ���Ըı���Ӫ
	
	unsigned int _old_faction;		//����ǰ����Ӫ
	unsigned int _old_enemy_faction;	//����ǰ�ж���Ӫ

	float _attack_recover_rate;						//�յ�������ԭ�ĸ���
	abase::vector<transform_skill, abase::fast_alloc<> > _transform_skill;	//�������б�

	enum MOVE_MODE
	{
		MOVE_FREE,
		MOVE_DISABLE,
		MOVE_RANDOM, 
	};
	
public:
	enum TRANSFORM_TYPE
	{
		TRANSFORM_ITEM,
		TRANSFORM_SKILL,
		TRANSFORM_TASK,	
		TRANSFORM_BUFFAREA,
	};


	player_transform(gplayer_imp *imp, unsigned char type, int id, unsigned char level, unsigned exp_level)
	{
		_template_id = id;
		_transform_type = type;
		_transform_level = level;
		_transform_explevel = exp_level;
		_can_change_faction = false;
		Init(imp);
	}

	virtual ~player_transform(){}

	int GetTransformID() const {return _template_id;}
	void SetTransformID(int id){_template_id = id;}

	unsigned char GetTransformType() const {return _transform_type;}
	void SetTransformType(unsigned char type){_transform_type = type;}
	
	unsigned char GetTransformLevel() const {return _transform_level;}
	void SetTransformLevel(unsigned char level){_transform_level = level;}

	unsigned char GetTransformExpLevel() const {return _transform_explevel;}
	void SetTransformExpLevel(unsigned char explevel){_transform_explevel = explevel;}

	void AddTransformSkill(int skill_id, int skill_level);
	int GetSkillLevel(int skill_id);
	void GetSkillData(archive & ar);

	bool CanUseSkill(){return _can_skill;}
	bool CanBot(){return _can_bot;}
	bool CanAttack(){return _can_attack;}
	bool CanFly(){return _can_fly;}
	bool CanUsePotion(){return _can_use_potion;}
	bool CanMount(){return false;}
	bool CanChangeTransform(int template_id, unsigned char new_type)
	{
		if(new_type == TRANSFORM_TASK) return true;
		if(new_type == TRANSFORM_SKILL && _transform_type == TRANSFORM_ITEM) return true;
		return false;
	}
	bool CanStopTransform(int template_id, unsigned char type)
	{
		if(type == TRANSFORM_ITEM && type != _transform_type) return false;
		if(template_id != _template_id && template_id != 0) return false;
		return true;
	}

	bool CanModifyTransform(int template_id, unsigned char type, unsigned char level, unsigned char explevel)
	{
		return _template_id == template_id && _transform_type == TRANSFORM_BUFFAREA && _transform_type == type && _transform_level == level && _transform_explevel == explevel;
	}

public:
	void StartTransform(gplayer_imp *pImp);
	void StopTransform(gplayer_imp *pImp);	
	void ModifyTransform(gplayer_imp* pImp);

	virtual void OnActiveTransformState(gplayer_imp *pImp);
	virtual void OnDeactiveTransformState(gplayer_imp *pImp); 
protected:
	virtual void AddTransformFilter(gplayer_imp *pImp) = 0;
	virtual void RemoveTransformFilter(gplayer_imp *pImp) = 0;
	virtual void ModifyTransformFilter(gplayer_imp* pImp) {}
	virtual bool OnActiveProp(gplayer_imp *pImp){return false;}
	virtual bool OnDeActiveProp(gplayer_imp *pImp){return false;}

	virtual int GetTimeOut(){return 0;}

public:
	virtual void AddHP(int hp){} 
	virtual void AddMP(int mp){} 
	virtual void AddDefense(int defense){}
     	virtual void AddAttack(int attack){}
	virtual void AddArmor(int armor){}
	virtual void AddDamage(int damage){}	
	virtual void AddResistance(const int resistance[6]){}	
	virtual void AddCrit(int crit_rate, float crit_damage){}	
	virtual void AddSpeed(float speed){}	
	virtual void AddAntiCrit(int anti_crit, float anti_crit_damage){}	
	virtual void AddSkillRate(int skill_attack_rate, int skill_armor_rate){}	
	virtual void AddAttackRange(float range){}

	virtual void AddScaleHP(int hp){}
	virtual void AddScaleMP(int mp){}
	virtual void AddScaleDefense(int defense){}
	virtual void AddScaleAttack(int attack){}
	virtual void AddScaleArmor(int armor){}
	virtual void AddScaleDamage(int damage){}
	virtual void AddScaleSpeed(int speed){}
	virtual void AddScaleResistance(const int resistance[6]){}

protected:
	bool OnActiveSkill(gplayer_imp *pImp);
	void OnDeActiveSkill(gplayer_imp *pImp);
	
	void Init(gplayer_imp *pImp);
	void Reset(gplayer_imp *pImp);
	
};


class player_item_transform : public player_transform
{
private:
	float _energy_drop_speed;
	int _exp_speed;

public:
	player_item_transform(gplayer_imp *imp, unsigned char type, int id, int level, int exp_level, float energy_drop_speed, int exp_speed) 
		: player_transform(imp, type, id, level, exp_level)
	{
		_energy_drop_speed = energy_drop_speed;
		_exp_speed = exp_speed;
	}
	
protected:
	virtual void AddTransformFilter(gplayer_imp *pImp);
	virtual void RemoveTransformFilter(gplayer_imp *pImp);
	virtual bool OnActiveProp(gplayer_imp *pImp);
	virtual bool OnDeActiveProp(gplayer_imp *pImp);
	
};


class player_normal_transform : public player_transform
{
private:
	q_enhanced_param _en_point;	//��������ǿ������
	q_scale_enhanced_param _en_percent;//���ٷֱ���ǿ������
	int _timeout;

protected:
	virtual bool OnActiveProp(gplayer_imp *pImp);
	virtual bool OnDeActiveProp(gplayer_imp *pImp);
	virtual int GetTimeOut(){return _timeout;}
	
public:
	player_normal_transform(gplayer_imp *imp, unsigned char type, int id, int level, int exp_level, int timeout)
	      :	player_transform(imp, type, id, level, exp_level)
	{
		_timeout = timeout;
		memset(&_en_point,0,sizeof(_en_point));
		memset(&_en_percent,0,sizeof(_en_percent));
	}
	
	
	virtual void AddHP(int hp){_en_point.max_hp += hp;} 
	virtual void AddMP(int mp){_en_point.max_mp += mp;} 
	virtual void AddDefense(int defense){_en_point.defense += defense;}
     	virtual void AddAttack(int attack){_en_point.attack += attack;}
	virtual void AddArmor(int armor){_en_point.armor += armor;}
	virtual void AddDamage(int damage){_en_point.damage += damage;}	
	virtual void AddResistance(const int resistance[6]){
		_en_point.resistance[0] += resistance[0];
		_en_point.resistance[1] += resistance[1];
		_en_point.resistance[2] += resistance[2];
		_en_point.resistance[3] += resistance[3];
		_en_point.resistance[4] += resistance[4];
		_en_point.resistance[5] += resistance[5];
	}	
	virtual void AddCrit(int crit_rate, float crit_damage){
		_en_point.crit_rate += crit_rate;
		_en_point.crit_damage += crit_damage;
	}	
	virtual void AddSpeed(float speed){_en_point.run_speed += speed;}	
	virtual void AddAntiCrit(int anti_crit, float anti_crit_damage){
		_en_point.anti_crit += anti_crit;
		_en_point.anti_crit_damage += anti_crit_damage;
	}	
	virtual void AddSkillRate(int skill_attack_rate, int skill_armor_rate){
		_en_point.skill_attack_rate += skill_attack_rate;
		_en_point.skill_armor_rate += skill_armor_rate;
	}	

	virtual void AddAttackRange(float range){
		_en_point.attack_range += range;
	}

	virtual void AddScaleHP(int hp){_en_percent.max_hp += hp;}
	virtual void AddScaleMP(int mp){_en_percent.max_mp += mp;}
	virtual void AddScaleDefense(int defense){_en_percent.defense += defense;}
	virtual void AddScaleAttack(int attack){_en_percent.attack += attack;}
	virtual void AddScaleArmor(int armor){_en_percent.armor += armor;}
	virtual void AddScaleDamage(int damage){_en_percent.damage += damage;}
	virtual void AddScaleSpeed(int speed){_en_percent.speed += speed;}
	virtual void AddScaleResistance(const int resistance[6]){
		_en_percent.resistance[0] += resistance[0];
		_en_percent.resistance[1] += resistance[1];
		_en_percent.resistance[2] += resistance[2];
		_en_percent.resistance[3] += resistance[3];
		_en_percent.resistance[4] += resistance[4];
		_en_percent.resistance[5] += resistance[5];
	}

};


class player_skill_transform : public player_normal_transform
{

public:
	player_skill_transform(gplayer_imp *imp, unsigned char type, int id, int level, int exp_level, int timeout)
	      :	player_normal_transform(imp, type, id, level, exp_level, timeout)
	{
	}
	
protected:
	virtual void AddTransformFilter(gplayer_imp *pImp);
	virtual void RemoveTransformFilter(gplayer_imp *pImp);
	
};

class player_buffarea_transform : public player_normal_transform
{

public:
	player_buffarea_transform(gplayer_imp *imp, unsigned char type, int id, int level, int exp_level, int timeout)
	      :	player_normal_transform(imp, type, id, level, exp_level, timeout)
	{
	}
	
protected:
	virtual void AddTransformFilter(gplayer_imp *pImp);
	virtual void RemoveTransformFilter(gplayer_imp *pImp);
	virtual void ModifyTransformFilter(gplayer_imp* pImp);
	
};

class player_task_transform : public player_normal_transform
{

public:
	player_task_transform(gplayer_imp *imp, unsigned char type, int id, int level, int exp_level, int timeout)
	      :	player_normal_transform(imp, type, id, level, exp_level, timeout)
	{
	}
	
protected:
	virtual void AddTransformFilter(gplayer_imp *pImp);
	virtual void RemoveTransformFilter(gplayer_imp *pImp);
	
};



			       
#endif
