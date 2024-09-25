#ifndef __ONLINEGAME_GS_MOB_ACTIVE_DATA_MAN_H__
#define __ONLINEGAME_GS_MOB_ACTIVE_DATA_MAN_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>

class itemdataman;


struct mob_active_statement
{
	int tid;
	char active_mode; // �������: 0 - ��Ʒ,1 - ����
	char active_point_num; //�ܹҵ���
	char active_times_max; //�ɻ�����������
	bool active_no_disappear; //�ﵽ�������������Ƿ���ʧ
	int active_point[6]; // �ҵ�ģ��
	int need_equipment;
	int task_in;
	int level;
	char require_gender;
	int64_t character_combo_id;
	int64_t character_combo_id2;
	int eliminate_equipment;
	int active_ready_time;
	int active_running_time;
	int active_ending_time;
	float speed;
	int path_id;
	int new_mob_active_id;
	int move_can_active; //�ƶ�ʱ�ܷ񻥶�
	bool path_end_finish_active; //����·���յ��������
};

struct mob_active_mount_point_template
{
	int tid;
	int ready_task_out;
	int activate_ready_controller_id;
	int deactivate_ready_controller_id;
	int end_task_out;
	int activate_end_controller_id;
	int deactivate_end_controller_id;
	int player_can_be_finish; // ������Ʒ����Ƿ���Ե���������
	int interrupt_condition; // ��λ 0 �ܹ����������� 1 �����ƶ���������

	struct
	{
		unsigned int id_obj;
		float prop;
	} drop_table[5];

	A3DVECTOR goal_pos[2];
};

class mob_active_mount_point_dataman
{
	typedef abase::hashtab<mob_active_mount_point_template,int,abase::_hash_function> MAP;
	MAP _mampt_map;

	bool __InsertTemplate(const mob_active_mount_point_template& mampt)
	{
		return _mampt_map.put(mampt.tid,mampt);
	}

	const mob_active_mount_point_template* __GetTemplate(int tid)
	{
		return _mampt_map.nGet(tid);
	}

	static mob_active_mount_point_dataman& __GetInstance()
	{
		static mob_active_mount_point_dataman __Singleton;
		return __Singleton;
	}

	mob_active_mount_point_dataman():_mampt_map(1024) {}

public:
	static bool Insert(const mob_active_mount_point_template& mampt)
	{
		bool rst = __GetInstance().__InsertTemplate(mampt);
		ASSERT(rst);
		return rst;
	}

	static const mob_active_mount_point_template* Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman& dataman);
};

#endif
