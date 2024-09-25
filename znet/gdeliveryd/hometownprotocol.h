#ifndef HOMETOWN_HOMETOWNPROTOCOL_H
#define HOMETOWN_HOMETOWNPROTOCOL_H 
#include <set>
#include <vector>
#include "common/octets.h"

namespace HOMETOWN
{
typedef std::set<int> PACK_SET;
using GNET::Octets;
#pragma pack(1)
	enum										//CMD type
	{
		COMPO_LIST = 0			,				//�о��Լ�����ӵ����,���������c2s_hometown_cmd_header�е�compo_id�ֶ�
		COMPO_ADD   			,				//������
		EXCHG_MONEY				,				//ͨ�ñҺ�������һ���, compo_idָ�����������
		LEVEL_UP				,				//����
		BUY_ITEM				,				//������Ʒ
		USE_ITEM				,				//ʹ����Ʒ
		SELL_ITEM				,				//����Ʒ
		PACKAGE_LIST			,				//�����鿴
		FARM_VIEW     			,				//�鿴
		FARM_VIEW_FIELD			,				//�鿴һ���
		FARM_PLOW_FIELD 		,				//���
		FARM_SOW_FIELD			,				//����
		FARM_WATER_FIELD		,				//��ˮ
		FARM_PEST_FIELD			,				//ɱ��
		FARM_WEED_FIELD			,				//����
		FARM_HARVEST_FIELD		,				//�ջ�
		FARM_STEAL_FIELD		,				//͵��
		FARM_LIST_LOVE_FIELD	,				//�о���Щ���Ѽ������Ҹ��ֵİ��ĵ�
		FARM_LIST_MATURE_FIELD	,				//�о���Щ���Ѽ����г���ĵ�
	};

	enum FARM_TARGET_TYPE						//ũ�������USE_ITEM��Ŀ������
	{
		FARM_TARGET_SELF = 0,					//����
		FARM_TARGET_PLAYER	,					//�������
		FARM_TARGET_FARM,						//������԰
		FARM_TARGET_FIELD,						//����һ��˵�
		FARM_TARGET_CROP,						//����һ��ׯ��
	};

	struct crop_info							//ׯ����Ϣ
	{
		int id;									//ׯ��ģ��id
		int seed_time;							//����ʱ��
		int ripe_time;							//���ж���ʱ�����,ֻ����δ����֮ǰ����Ч,�����ᷢ
		int protect_period;						//ʣ���͵������
		int state;								//״̬
		int fruit_left;							//��ǰʣ���ʵ�� ֻ�ڳ���״̬�²���Ч,�����ᷢ
		int fruit_total;						//��͵�Ĺ�ʵ��, ֻ�ڳ����͵��״̬�²���Ч,�����ᷢ
	};
	struct field_info							//�ؿ���Ϣ
	{
		int field_id;
		char field_type;						//0:δ����, 1:��ͨ�أ�2�����ĵ�
		int state;								//�ؿ鵱ǰ��״̬
		int friend_id;							//���ĵز����߽�ɫid
		char friend_name_size;	
		char friend_name[20];
		struct crop_info crop;					//ׯ����Ϣ,���ڸ���״̬�²Żᷢ���ֶ�
	};
	struct farm_player
	{
		int id;
		int64_t charm; 
		int64_t money; 
		int level; 
		int state;								//�˵�ǰ��״̬
	};
	struct farm_info
	{
		int state;								//����ũ����״̬�б�
		int protect_period;						//����ũ����ʣ�ౣ��ʱ��
		struct farm_player owner;
		std::vector<struct field_info> fields;
		std::vector<Octets> farm_dynamic;
		std::vector<Octets> action_dynamic;
	};
	struct item_data
	{
		unsigned int id;
		int pos;
		int count;
		int max_count;
		Octets content;
	};
	struct pocket_info
	{
		int capacity;					//������С
		std::vector<item_data> items;	//��Ʒ�б�
	};

	namespace DYNAMIC					//�ҵ��ж��Ͳ�԰��̬�ṹ
	{
		struct dynamic_header
		{
			int time;
			int action;
		};

		//ֻ��ʾ�Լ�
		struct exchg_money
		{
			char type;				//�μ�hometownprotocol.h
			int64_t local_money;	//�һ��ı�����
			int64_t foreign_money;	//�һ��������
		};

		//ֻ��ʾ�Լ�
		struct level_up
		{
			int level;				//������ȼ�
		};

		//ֻ��ʾ�Լ�
		struct buy_item
		{
			int item_id;
			int count;
			int64_t cost1;			//����ͬS2C�е�Э��
			int64_t cost2;
		};

		//ֻ��ʾ�Լ�
		struct sell_item
		{
			int item_id;
			int count;
			int64_t earning;
		};

		//�Ժ���ʹ������Ʒ,��ʾ�Լ�����ʾ����
		struct use_item
		{
			int sponsor;				//ʹ���߽�ɫid
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//Ŀ���ɫid
			char target_name_size;	
			char target_name[20];
			int item_id;
			int target_type;
		};

		//�ں��ѵİ��ĵ���أ���ʾ�Լ��ͺ���
		struct plow_field
		{
			int sponsor;				//�����
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//����ص���
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//�ں��ѵİ��ĵز��֣���ʾ�Լ��ͺ���
		struct sow_field
		{
			int sponsor;				//������
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//�����ֵ���
			char target_name_size;	
			char target_name[20];
			int seed_id;
		};

		//�ں��ѵĵ��ｽˮ����ʾ�Լ��ͺ���
		struct water_field
		{
			int sponsor;				//��ˮ��
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//����ˮ����
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//�ں��ѵĵ���׽�棬��ʾ�Լ��ͺ���
		struct pest_field
		{
			int sponsor;				//׽����
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//��׽�����
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//�ں��ѵĵ�����ݣ���ʾ�Լ��ͺ���
		struct weed_field
		{
			int sponsor;				//������
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//�����ݵ���
			char target_name_size;	
			char target_name[20];
			int emotion;
		};

		//�ջ����Լ������ػ���ѵİ��ĵ�,ֻ�������ҵ��ж���
		struct harvest_field
		{
			int sponsor;				//�ջ����
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//���ջ����
			char target_name_size;	
			char target_name[20];
			int fruit_id;
			int count;
		};

		//�Լ��İ��ĵػ����Լ��ֵİ��ĵر������ջ񣬵õ���ʵ��ֻ�����ڲ�԰��̬��
		struct harvest_lovefield
		{
			int sponsor;
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;
			char target_name_size;	
			char target_name[20];
			int fruit_id;
			int gain_count;				//ʵ�ʻ�õĹ�ʵ��
			int drop_count;				//���������ʧ����Ŀ
		};

		//͵����ׯ԰����ʾ�Լ��ͺ���
		struct steal_field
		{
			int sponsor;				//͵�Ե���
			char sponsor_name_size;	
			char sponsor_name[20];
			int target;					//��͵�Ե���
			char target_name_size;	
			char target_name[20];
			int emotion;
			int fruit_id;
			int count;
		};
	};

	namespace S2C
	{
		struct hometown_s2c_cmd_header
		{
			int retcode;			
			int cmd_type;
			int compo_id;						//���id
		};

		namespace CMD
		{
			struct compo_list
			{
				int roleid;
				int64_t money;
				PACK_SET compo_list;		//�����ӵ����ID�б�ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
			};

			struct compo_add
			{
			};
			struct exchg_money
			{
				char type;						//ͬC2S
				int64_t money;					//ͬC2S
				//��������ֻ��HT_ERR_SUCCESS����
				int64_t hometown_money;			//�һ���ɺ�ͨ�ñ���Ŀ
				int64_t compo_money;			//�һ���ɺ��������Ŀ
			};
			struct level_up
			{
				//ֻ��HT_ERR_SUCCESS�Ŵ���
				//���ڲ�ͬ������в�ͬ��ֵ
				//struct farm_info farm;		ũ�����������Ľ��
			};
			struct buy_item
			{       
				int item_id;        			//������ƷID
				int count;          			//��������

				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				//���ѣ���ͬ������ܻ��Ѳ�ͬ�Ļ���
				//ũ�������cost1��ʾ���ĵ�����ֵ, cost2��ʾ���ĵĽ�Ǯ
				int64_t cost1;					
				int64_t cost2;
			};      
			struct use_item
			{
				int pos;			//ʹ��1��λ�ڰ�����pos����Ʒ
				int item_id;		//����У��
				int target_type;	//Ŀ������
									//����ũ��������£�
									//FARM_TARGET_SELF = 0,	����
									//FARM_TARGET_PLAYER,�������
									//FARM_TARGET_FARM,������԰
									//FARM_TARGET_FIELD,����һ��˵�
									//FARM_TARGET_CROP,����һ��ׯ��

				int roleid;			//Ŀ��roleid, ��FARM_TARGET_SELF�������
				int field_id;		//Ŀ��ؿ�id, ֻ��FARM_TARGET_FIELD, FARM_TARGET_CROP��Ч
				char consumed;		//0:�����ģ�1:����
			};
			struct sell_item
			{       
				int pos;            //��λ�ڰ�����pos����Ʒ
				int item_id;		//��ƷID�����ڼ���
				int count;          //��������
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int64_t earning;	//���룬��ͬ������ܻ�ò�ͬ������
									//����ũ�������ũ����
			};  
			struct package_list
			{
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				struct pocket_info pktinfo;		//����������İ�����Ϣ��ֻ��HT_ERR_SUCCESS�Ŵ���
			};
			struct farm_view
			{
				int roleid;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				struct farm_info farm;			//ֻ��HT_ERR_SUCCESS�Ŵ���
			};
			struct farm_view_field
			{
				int roleid;
				int field_id;
				//����ֻ��HT_ERR_SUCCESS�Ŵ���
				struct field_info field;
			};
			struct farm_plow_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_sow_field
			{
				int roleid;
				int field_id;
				int seed_id;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int cost;						//���ӺķѵĽ�Ǯ��
			};
			struct farm_water_field
			{
				int roleid;
				int field_id;
				int emotion;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int charm_change;
			};
			struct farm_pest_field
			{
				int roleid;
				int field_id;
				int emotion;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int charm_change;
			};
			struct farm_weed_field
			{
				int roleid;
				int field_id;
				int emotion;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int charm_change;
			};
			struct farm_harvest_field
			{
				int roleid;
				int field_id;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int charm_add;
				int fruit_id;
				int fruit_count;
			};
			struct farm_steal_field
			{
				int roleid;
				int field_id;
				int emotion;
				//����ֻ��retcode==HT_ERR_SUCCESS�Ŵ���
				int charm_change;
				int fruit_id;
				int fruit_count;
			};
			struct farm_list_love_field
			{
				PACK_SET friends;
			};
			struct farm_list_mature_field
			{
				PACK_SET friends;
			};
		};
	};

	namespace C2S
	{
		struct hometown_c2s_cmd_header
		{
			int cmd_type;
			int compo_id;
		};

		namespace CMD
		{
			struct compo_list
			{
				int roleid;
			};
			struct compo_add
			{
				//û������
			};
			struct exchg_money
			{
				char type;		//0��ʾͨ�ñһ�������ң�1:�෴
				int64_t money;	//�һ��ı�������type==0:ͨ�û�������type==1:�������
			};
			struct level_up
			{
			};
			struct buy_item
			{
				int item_id;		//������ƷID
				int count;			//��������
			};
			struct use_item
			{
				int pos;			//ʹ��1��λ�ڰ�����pos����Ʒ
				int item_id;
				int target_type;	//Ŀ������
									//����ũ��������£�
									//FARM_TARGET_SELF = 0,	����
									//FARM_TARGET_PLAYER,�������
									//FARM_TARGET_FARM,������԰
									//FARM_TARGET_FIELD,����һ��˵�
									//FARM_TARGET_CROP,����һ��ׯ��

				int roleid;
				int field_id;
			};
			struct sell_item
			{
				int pos;			//��λ�ڰ�����pos����Ʒ
				int item_id;		//��ƷID�����ڼ���
				int count;			//��������
			};
			struct package_list
			{
			};
			struct farm_view
			{
				int roleid;
			};
			struct farm_view_field
			{
				int roleid;
				int field_id;
			};
			struct farm_plow_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_sow_field
			{
				int roleid;
				int field_id;
				int seed_id;
			};
			struct farm_water_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_pest_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_weed_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_harvest_field
			{
				int roleid;
				int field_id;
			};
			struct farm_steal_field
			{
				int roleid;
				int field_id;
				int emotion;
			};
			struct farm_list_love_field
			{
			};
			struct farm_list_mature_field
			{
			};
		};
	};
#pragma pack()
};
#endif
