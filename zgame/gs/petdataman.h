#ifndef __NETGAME_GS_PET_DATA_MAN_H__
#define __NETGAME_GS_PET_DATA_MAN_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>
#include "petman.h"
#include "playertemplate.h"
#include "property.h"

class itemdataman;

//����������ģ��
struct pet_bedge_data_temp
{
	int tid;
	float body_size;
	int pet_type;
	int immune_type;
	int init_level;			//��ʼ�ȼ�,û����,�ű������
	float sight_range;
	float attack_range;		//���﹥������
	int honor_point_inc_speed;	//���ܶ������ٶ�
	int hunger_point_dec_speed[2];	//��ʳ�������ٶ� 0 ��ս�� 1 ս��
	int hp_gen[2];
	int vp_gen[2];
	float walk_speed;
	float run_speed;
};

class pet_bedge_dataman
{
	typedef abase::hashtab<pet_bedge_data_temp,int,abase::_hash_function> MAP;
	MAP _pt_map;
	
	bool __InsertTemplate(const pet_bedge_data_temp& pt)
	{       
		return _pt_map.put(pt.tid,pt);
	}       

	const pet_bedge_data_temp* __GetTemplate(int tid)
	{       
		return _pt_map.nGet(tid);
	}

	static pet_bedge_dataman& __GetInstance()
	{
		static pet_bedge_dataman __Singleton;
		return __Singleton; 
	}
	
	pet_bedge_dataman():_pt_map(1024) {}
	
public:
	static bool Insert(const pet_bedge_data_temp& pt)
	{
		bool rst = __GetInstance().__InsertTemplate(pt);
		ASSERT(rst);
		return rst;
	}

	static const pet_bedge_data_temp* Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman& dataman);
	static bool GenerateBaseProp(int tid,int level,q_extend_prop& prop);
};

//����װ������ģ��
struct pet_equip_data_temp
{
	int tid;
	unsigned int pet_type_mask;	//�����������
	int level_require;		//��Ҫ����
	int astro_type;			//�������޶�
	int astro_level;		//�ǵ��޶�
	int equip_location;		//װ��λ���޶�
	struct
	{
		int maximum;	// ����
		int minimum;	// ����
	} int_props[12];
	struct
	{
		float maximum;	// ����
		float minimum;	// ����
	} float_props[2];
};

class pet_equip_dataman
{
	typedef abase::hashtab<pet_equip_data_temp,int,abase::_hash_function> MAP;
	MAP _pt_map;
	
	bool __InsertTemplate(const pet_equip_data_temp& pt)
	{       
		return _pt_map.put(pt.tid,pt);
	}       

	const pet_equip_data_temp* __GetTemplate(int tid)
	{       
		return _pt_map.nGet(tid);
	}

	static pet_equip_dataman& __GetInstance()
	{
		static pet_equip_dataman __Singleton;
		return __Singleton; 
	}
	
	pet_equip_dataman():_pt_map(1024) {}
	
public:
	static bool Insert(const pet_equip_data_temp& pt)
	{
		bool rst = __GetInstance().__InsertTemplate(pt);
		ASSERT(rst);
		return rst;
	}

	static const pet_equip_data_temp* Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman& dataman);
	static bool GenerateBaseProp(int tid,int level,q_extend_prop& prop);
};

#endif

