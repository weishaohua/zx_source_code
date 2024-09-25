/*
 * FILE: globaldataman.h
 *
 * DESCRIPTION: global data loader and manager
 *
 * CREATED BY: Hedi, 2005/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef _GLOBALDATAMAN_H_
#define _GLOBALDATAMAN_H_

#include <vector.h>
#include <a3dvector.h>

typedef struct _TRANS_TARGET_SERV
{
	int		id;
	int 		world_tag;
	A3DVECTOR3	vecPos;
	int		domain_id;
} TRANS_TARGET_SERV;

typedef struct _MALL_ITEM_SERV
{
	int	goods_id;
	int	goods_count;
	int 	bonus;
	int 	props;
	int 	discount;
	bool 	has_present;
	bool 	present_bind;
	unsigned int present_id;
	unsigned int present_count;
	unsigned int present_time;

	//-------------------�̳���ʱ������Ʒ-------------------------
	int	type;				//��Чʱ������ 0:������Ч��Ĭ�����1����ʱ��Σ�2��ÿ�ܣ�3��ÿ��
	int	start_time;			//��ʼʱ�䣺��ʱ���ʱ���洢UTC����������ÿ�º�ÿ��ʱ���洢��Сʱ+��+�룩������
	int	end_time;			//��ֹʱ�䣺�洢����ͬ��
	int	param;				//��ʱ���ʱ��1����ʾ������ʼʱ�䣻2����ʾ������ֹʱ��
							//��ÿ��ʱ�������룩0-6λ�ֱ��ʾ������ - ����
							//��ÿ��ʱ�������룩1-31λ�ֱ��ʾ��1-31��
	
	struct __entry
	{
		int 	expire_time;
		int 	cash_need;
	}list[4];
} MALL_ITEM_SERV;

//VIP����
typedef struct _VIP_AWARD_ITEM_SERV
{
	int		award_id;			//�༭����ÿ��������Ʒ��Ӧ��id����id��Ψһ�ģ�������
	short	award_type;			//��Ʒ���ͣ�0����ͨ��1��vip

	int		award_item_id;		//ÿ�������ľ�����Ʒid�����ڽ�����ʱ������Ʒ
								//�����ظ�(��5�����������10��������ǲ�ͬ�Ľ���)
	int		award_item_count;	//��Ʒ����

	short	obtain_level;		//��ȡ��Ʒ����ĵ��Σ�vip���δ�1��ʼ����ͨ��ҵ��θ��ݵȼ��ּ�����1��ʼ
	short	obtain_type;		//��ȡ��Ʒ�����ͣ�0��ÿ����ȡ��1��������ȡ

	int		expire_time;		//ʱ�ޣ���Ʒ�е�����ʱ��
}VIP_AWARD_ITEM_SERV;


bool globaldata_loadserver(const char * trans_data,const char * mall_data, const char * bonusmall_data, const char * zonemall_data, const char * vipaward_data);
bool globaldata_releaseserver();

abase::vector<TRANS_TARGET_SERV> & globaldata_gettranstargetsserver();
abase::vector<MALL_ITEM_SERV> & globaldata_getmallitemservice();
int globaldata_getmalltimestamp();

abase::vector<MALL_ITEM_SERV> & globaldata_getbonusmallitemservice();
int globaldata_getbonusmalltimestamp();

abase::vector<MALL_ITEM_SERV> & globaldata_getzonemallitemservice();
int globaldata_getzonemalltimestamp();

abase::vector<VIP_AWARD_ITEM_SERV> & globaldata_getvipawardservice();
int globaldata_getvipawardtimestamp();

#endif//_GLOBALDATAMAN_H_

