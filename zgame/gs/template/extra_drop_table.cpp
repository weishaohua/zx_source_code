#include <stdio.h>
#include <stdlib.h>
#include "extra_drop_table.h"

static vector<EXTRADROPTABLE>		extra_drop_tables;

int load_extra_drop_tables(const char * filename)
{
	FILE *pFile = fopen(filename,"rb");
	if( pFile==NULL )
		return -1;

	unsigned int nVersion;
	fread(&nVersion, sizeof(unsigned int), 1, pFile);//�ļ��汾
	if( nVersion!= EXTRA_DROP_TABLE_VERSION )
		return -1;

	int count = 0;
	fread(&count, sizeof(int), 1, pFile); //��ĸ���
	for(int i=0; i<count; i++)
	{
		EXTRADROPTABLE newTable;
		int num = 0;
		unsigned int id;
		fread(&num, sizeof(int), 1, pFile);	
		for(int i=0; i<num; i++)
		{
			fread(&id, sizeof(unsigned int), 1, pFile); //�����ID
			newTable.id_monsters.push_back(id);
		}

		// 256�ݵ�����Ʒ��ID�ͻ���
		fread(&newTable.drop_items, sizeof(int) + sizeof(float), 256, pFile);
		fread(newTable.name, sizeof(char) * 128, 1, pFile);
		fread(&newTable.type, sizeof(int), 1, pFile);
		// 0~7��������ĸ���
		fread(&newTable.drop_num_probability, sizeof(float), 8, pFile);
		extra_drop_tables.push_back(newTable);
	}

	fclose(pFile);
	return 0;
}

vector<EXTRADROPTABLE>& get_extra_drop_table()
{
	return extra_drop_tables;
}
