/*
 * FILE: globaldataman.cpp
 *
 * DESCRIPTION: global data loader and manager
 *
 * CREATED BY: Hedi, 2005/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include <vector.h>
#include "globaldataman.h"
#include <string.h>

static abase::vector<TRANS_TARGET_SERV>	global_trans_targets_server;
static abase::vector<MALL_ITEM_SERV>  global_mall_item_service;
static abase::vector<MALL_ITEM_SERV>  global_bonusmall_item_service;
static abase::vector<MALL_ITEM_SERV>  global_zonemall_item_service;
static abase::vector<VIP_AWARD_ITEM_SERV> global_vip_award_service;
static int mall_timestamp = 0;
static int bonusmall_timestamp = 0;
static int zonemall_timestamp = 0;
static int vipaward_timestamp = 0;

abase::vector<TRANS_TARGET_SERV> & globaldata_gettranstargetsserver()
{
	return global_trans_targets_server;
}

static bool load_transdata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	int nCount;
	fread(&nCount, 1, sizeof(int), fpServer);

	// now output a trans target list for server usage
	for(int i=0; i<nCount; i++)
	{
		TRANS_TARGET_SERV target;
		fread(&target, 1, sizeof(target), fpServer);
		global_trans_targets_server.push_back(target);
	}

	fclose(fpServer);
	return true;
}

#pragma pack(push, GSHOP_ITEM_PACK, 1)
typedef struct _GSHOP_ITEM
{
	unsigned int			id;				// object id of this item
	unsigned int			num;				// number of objects in this item
	char					icon[128];		// icon file path

	struct {
		unsigned int			price;			// price of this item
		unsigned int			time;			// time of duration, 0 means forever
	} list[1];

	int				discount;			//���۱���
	int				bonus;
	unsigned int			props;				// mask of all props, currently from low bit to high bit: ��Ʒ���Ƽ�Ʒ������Ʒ
									// ���8λ�ֱ��ʾ���۷������ӵ͵��߷ֱ���1-8�ַ���
									// �θ�8λ�ֱ��ʾ���۷������ӵ͵��߷ֱ���1-8�ַ���

	int					main_type;		// index into the main type array
	int						sub_type;	// index into the sub type arrray
	int						local_id;	// id of this shop item, used only for localization purpose

	unsigned short 			desc[512];			// simple description
	unsigned short			szName[32];			// name of this item to show

	//�¼���Ʒ
	bool		 		bHasPresent;				//�Ƿ�����Ʒ	
	unsigned short 			szPresentName[32];			//��Ʒ����
	unsigned int 			Presentid;				//��ƷID
	unsigned int 			Presentcount;				//��Ʒ����
	unsigned int 			Presenttime;				//��Ʒ���ڵ�ʱ��(��)
	char		 		Present_file_icon[128];			//��Ʒͼ��·��
	bool		 		bPresentBind;				//��Ʒ�Ƿ��
	unsigned short 			Presentdesc[512];			//��Ʒ��������

	//�̳���Ʒ���ϼ���Чʱ��Added 2011-11-07.
	struct
	{
		int		type;				//��Чʱ������ 0:������Ч��Ĭ�����1����ʱ��Σ�2��ÿ�ܣ�3��ÿ��
		int		start_time;			//��ʼʱ�䣺��ʱ���ʱ���洢UTC����������ÿ�º�ÿ��ʱ���洢��Сʱ+��+�룩������
		int		end_time;			//��ֹʱ�䣺�洢����ͬ��
		int		param;				//��ʱ���ʱ��1����ʾ������ʼʱ�䣻2����ʾ������ֹʱ��
									//��ÿ��ʱ�������룩0-6λ�ֱ��ʾ������ - ����
									//��ÿ��ʱ�������룩1-31λ�ֱ��ʾ��1-31��
	}valid_time;
	//Added end.

	unsigned short SearchKey[64];			//�ؼ��֣�","�ָ�
	
	_GSHOP_ITEM()
	{
		bHasPresent = false;
		memset(szPresentName,0,sizeof(short)*32);
		Presentid = 0;
		Presentcount = 0;
		Presenttime = 0;
		memset(Present_file_icon,0,128);
		bPresentBind = false;
		memset(Presentdesc,0,sizeof(short)*512);
		memset(&SearchKey, 0, sizeof(SearchKey));
	}
} GSHOP_ITEM;
#pragma pack(pop, GSHOP_ITEM_PACK)

//-----------------------------VIP����-----------------------------------------
#pragma pack(push, VIP_AWARD_ITEM_PACK, 1)
typedef struct _VIP_AWARD_ITEM
{
	unsigned int	tid;					//ģ��ID
	unsigned short	szName[64];				//������Ʒ������
	unsigned int	item_id;				//������Ʒ��ģ��id
	unsigned int	count;					//������Ʒ�ĸ���
	unsigned int	award_type;				//�������� 0����ͨ������ 1��VIP����
	unsigned int	award_level;			//�������� ��ͨ�����Ľ������������ҵȼ��趨��VIP�ȼ����ݳ�ֵ����趨
	unsigned int	award_obtain_type;		//��ȡ�����ķ�ʽ��0��ÿ����ȡ��1��������ȡ
	int				expire_time;			//���ޣ���Ʒ�е�����ʱ��
}VIP_AWARD_ITEM;
#pragma pack(pop, VIP_AWARD_ITEM_PACK)
//-----------------------------------------------------------------------------


static bool load_vipaward_data(const char * pfile)
{
	FILE * fpServer = fopen(pfile, "rb");
	if(!fpServer) 
	{
		fprintf(stderr, "\n\n��VIPAward.data�ļ�ʧ�ܣ�\n");
		return false;
	}

	bool bRst = true;

	try
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) 
		{
			throw -1;
		}

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) 
		{
			throw -2;
		}

		// now output a trans target list for server usage
		for(int i = 0; i < nCount; i++)
		{
			VIP_AWARD_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}

			VIP_AWARD_ITEM_SERV awards; 
			memset(&awards, 0, sizeof(awards));
			awards.award_id			= data.tid;
			awards.award_type		= data.award_type;
			awards.award_item_id	= data.item_id;
			awards.award_item_count	= data.count;
			awards.obtain_level		= data.award_level;
			awards.obtain_type		= data.award_obtain_type;
			awards.expire_time		= data.expire_time;

			global_vip_award_service.push_back(awards);
		}

		vipaward_timestamp = timestamp;
	}
	catch(...)
	{
		fprintf(stderr, "\n\nVIP�������ݶ������󣬼��gs.conf�е�VipAwardData�\n");
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}


static bool load_malldata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	bool bRst = true;
	try 
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) throw -1;

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) throw -2;

		// now output a trans target list for server usage
		for(int i=0; i<nCount; i++)
		{
			GSHOP_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}
			MALL_ITEM_SERV goods;
			memset(&goods, 0, sizeof(goods));
			goods.goods_id = data.id;
			goods.goods_count = data.num;
			goods.bonus = data.bonus;
			goods.props = data.props;
			goods.discount = data.discount;

			//��Ʒ
			goods.has_present = data.bHasPresent;
			goods.present_bind = data.bPresentBind;
			goods.present_id = data.Presentid;
			goods.present_count = data.Presentcount;
			goods.present_time = data.Presenttime;

			//�̳���ʱ������Ʒ
			goods.type       = data.valid_time.type;
			goods.start_time = data.valid_time.start_time;
			goods.end_time   = data.valid_time.end_time;
			goods.param      = data.valid_time.param;

			for(size_t j = 0;j < 1; j ++)
			{
				goods.list[j].expire_time = data.list[j].time;
				goods.list[j].cash_need = data.list[j].price;
			}

			global_mall_item_service.push_back(goods);
		}

		mall_timestamp = timestamp;
	}
	catch(...)
	{
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}

static bool load_bonusmalldata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	bool bRst = true;
	try 
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) throw -1;

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) throw -2;

		// now output a trans target list for server usage
		for(int i=0; i<nCount; i++)
		{
			GSHOP_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}
			MALL_ITEM_SERV goods;
			memset(&goods, 0, sizeof(goods));
			goods.goods_id = data.id;
			goods.goods_count = data.num;
			goods.bonus = data.bonus;
			goods.props = data.props;
			goods.discount = data.discount;

			//��Ʒ
			goods.has_present = data.bHasPresent;
			goods.present_bind = data.bPresentBind;
			goods.present_id = data.Presentid;
			goods.present_count = data.Presentcount;
			goods.present_time = data.Presenttime;

			for(size_t j = 0;j < 1; j ++)
			{
				goods.list[j].expire_time = data.list[j].time;
				goods.list[j].cash_need = data.list[j].price;
			}

			global_bonusmall_item_service.push_back(goods);
		}

		bonusmall_timestamp = timestamp;
	}
	catch(...)
	{
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}

static bool load_zonemalldata(const char * file)
{
	FILE * fpServer = fopen(file, "rb");
	if(!fpServer) return false;

	bool bRst = true;
	try 
	{
		int timestamp;
		if(fread(&timestamp, 1, sizeof(int), fpServer) != sizeof(int)) throw -1;

		int nCount;
		if(fread(&nCount, 1, sizeof(int), fpServer) != sizeof(int) || nCount < 0 || nCount > 65535) throw -2;

		// now output a trans target list for server usage
		for(int i=0; i<nCount; i++)
		{
			GSHOP_ITEM data;
			if(fread(&data, 1, sizeof(data), fpServer) != sizeof(data))
			{
				throw -3;
			}
			MALL_ITEM_SERV goods;
			memset(&goods, 0, sizeof(goods));
			goods.goods_id = data.id;
			goods.goods_count = data.num;
			goods.bonus = data.bonus;
			goods.props = data.props;
			goods.discount = data.discount;

			//��Ʒ
			goods.has_present = data.bHasPresent;
			goods.present_bind = data.bPresentBind;
			goods.present_id = data.Presentid;
			goods.present_count = data.Presentcount;
			goods.present_time = data.Presenttime;

			for(size_t j = 0;j < 1; j ++)
			{
				goods.list[j].expire_time = data.list[j].time;
				goods.list[j].cash_need = data.list[j].price;
			}

			global_zonemall_item_service.push_back(goods);
		}

		zonemall_timestamp = timestamp;
	}
	catch(...)
	{
		bRst = false;
	}

	fclose(fpServer);
	return bRst;
}


bool globaldata_loadserver(const char * file, const char * file2, const char * file3, const char * file4, const char * file5)
{
	if(!load_transdata(file)) return false;
	if(!load_malldata(file2)) return false;
	if(!load_bonusmalldata(file3)) return false;
	if(!load_zonemalldata(file4)) return false;
	return load_vipaward_data(file5);
}

abase::vector<VIP_AWARD_ITEM_SERV> & globaldata_getvipawardservice()
{
	return global_vip_award_service;
}

int globaldata_getvipawardtimestamp()
{
	return vipaward_timestamp;
}

abase::vector<MALL_ITEM_SERV> & globaldata_getmallitemservice()
{
	return global_mall_item_service;
}

int globaldata_getmalltimestamp()
{
	return mall_timestamp;
}


abase::vector<MALL_ITEM_SERV> & globaldata_getbonusmallitemservice()
{
	return global_bonusmall_item_service;
}

int globaldata_getbonusmalltimestamp()
{
	return bonusmall_timestamp;
}

abase::vector<MALL_ITEM_SERV> & globaldata_getzonemallitemservice()
{
	return global_zonemall_item_service;
}

int globaldata_getzonemalltimestamp()
{
	return zonemall_timestamp;
}


bool globaldata_releaseserver()
{
	global_trans_targets_server.erase(global_trans_targets_server.begin(), global_trans_targets_server.end());

	return true;
}

