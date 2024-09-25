#ifndef __ONLINEGAME_GS_ITEM_DATA_H__
#define __ONLINEGAME_GS_ITEM_DATA_H__
#include <common/packetwrapper.h>

struct item_data 
{
	int type;		//����
	size_t count;		//����
	size_t pile_limit;	//�ѵ�����
	int proc_type;		//��Ʒ�Ĵ���ʽ
	struct 
	{
		int guid1;
		int guid2;
	}guid;			//GUID
	size_t price;		//���� ���������Ϊһ���ο�ֵ,ʵ��ֵ��ģ���е�Ϊ׼
	int expire_date;	//���� ����0��ʾ������
	size_t content_length;	//�������ݵĴ�С
	char * item_content;	//�������� ��item����ʹ��

	bool CanMoveToPocket() const;
};

struct item_data_client
{
	int type;		//����
	size_t count;		//����
	size_t pile_limit;	//�ѵ�����
	int proc_type;		//��Ʒ�Ĵ���ʽ
	struct 
	{
		int guid1;
		int guid2;
	}guid;			//GUID
	size_t price;		//���� ���������Ϊһ���ο�ֵ,ʵ��ֵ��ģ���е�Ϊ׼
	int expire_date;	//���� ����0��ʾ������
	size_t content_length;	//�������ݵĴ�С
	char * item_content;	//�������� ��item����ʹ��
	bool use_wrapper;
	packet_wrapper ar;

	item_data_client()
	{
		use_wrapper = false;
	}
};

#endif

