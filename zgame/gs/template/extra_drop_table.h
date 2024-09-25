#ifndef _EXTRA_DROP_TABLE_H_
#define _EXTRA_DROP_TABLE_H_

#include <vector.h>
using namespace abase;

#define EXTRA_DROP_TABLE_VERSION	1

enum EDT_TYPE
{
	EDT_TYPE_REPLACE = 1,
	EDT_TYPE_ADDON,
};

struct EXTRADROPTABLE
{
	char					name[128];		// ���������
	EDT_TYPE				type;			// 1 ��ȫ�滻��2 ͬʱ��Ч
	vector<int>				id_monsters;	// �����б�

	float					drop_num_probability[8];	// ����0~7����Ʒ�ĸ���
	struct 
	{
		unsigned int		id;				// ��ƷID
		float				probability;	// �������

	}  drop_items[256];						// �����
};


///////////////////////////////////////////////////////////////////////////////
// load data from a config file
// return	0 if succeed
//			-1 if failed.
///////////////////////////////////////////////////////////////////////////////
int load_extra_drop_tables(const char * filename);

// get extra drop tables array
vector<EXTRADROPTABLE>& get_extra_drop_table();

#endif//_EXTRA_DROP_TABLE_H_

