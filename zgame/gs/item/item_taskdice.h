#ifndef __ONLINEGAME_GS_RECIPE_TASKDICE_H__
#define __ONLINEGAME_GS_RECIPE_TASKDICE_H__

#include "../item.h"
#include "../config.h"

class item_taskdice : public item_body
{
	//����1�Ǿ������򴥷�������2��Բ�����򴥷�
	enum {TRIGGER_TYPE_CUBE = 1, TRIGGER_TYPE_SPHERE = 2};

	struct
	{
		unsigned int	id;
		float		probability;
	} _task_lists[8];

	//����1�������򴥷�
	struct 
	{
		unsigned int	map_id;			// ��ͼ��
		float			pos_min[3];		// ���½�(x,y,z)
		float			pos_max[3];		// ���Ͻ�(x,y,z)
	} _trigger_cube_point[8];

	//����2Բ�����򴥷�
	struct
	{
		int			map_id;		// ������ͼid
		float		x;			// ������(x,y,z)
		float		y;
		float		z;
		float		radius;		// �����뾶
	} _trigger_sphere_point;

	//����Ʒ�Ƿ������򴥷���
	int _area_trigger_type;


public:
	item_taskdice()
	{
		memset(_task_lists,0,sizeof(_task_lists));
		_area_trigger_type = false;
		memset(_trigger_cube_point, 0, sizeof(_trigger_cube_point));
		memset(&_trigger_sphere_point, 0, sizeof(_trigger_sphere_point));
	}

	bool SetTaskList(const void * value)
	{
		memcpy(_task_lists,value, sizeof(_task_lists));
		float prob = 0.f;
		for(size_t i = 0; i < 8; i ++)
		{
			prob += _task_lists[i].probability;
		}
		return fabs(prob - 1.0f) < 1e-5;
	}

	bool SetTriggerPointList(int area_trigger_type, const void * value, int type2_map_id, float x, float y, float z, float radius);
	
	
public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_RECIPE;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return 0;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;


private:
	bool IsInCubeArea(int world_tag, float x, float y, float z) const;
	bool IsInSphereArea(int world_tag, float x, float y, float z) const;
};
#endif


