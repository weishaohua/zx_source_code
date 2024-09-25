#ifndef _ITEMDATAMAN_H_
#define _ITEMDATAMAN_H_

#include <vector.h>
#include <hashmap.h>

#ifdef LINUX
#include <arandomgen.h>
#include <amemory.h>
#include "exptypes.h"
#include "../itemdata.h"
#include "../item/item_addon.h"
#else	// Windows
//#include "../zelementdata/exptypes.h"
#include "exptypes.h"
#include <Windows.h>
namespace abase
{
	inline int Rand(int lower, int upper)	
	{ 
		if(upper==lower)
			return lower;
		else	
			return rand()%(upper-lower)+lower; 
	}
	
	inline float Rand(float lower, float upper) 
	{ return lower+(upper-lower)*rand()/(float)RAND_MAX; }
	
	inline int RandNormal(int lower, int upper) { return Rand(lower, upper); }
	inline float RandUniform() { return Rand(0.f, 1.f); }
	
	inline int RandSelect(const void * option, int stride, int num) 
	{ 
		const char * tmp = (const char *)option;
		float op = RandUniform();
		for(int i =0;i < num; i ++)
		{
			float prob = *(float*)tmp;
			if(op < prob ) 
				return i;
			op -= prob;
			tmp += stride;
		}
		assert(false);
		return 0;
	}
	inline void * fastalloc(size_t size) { return malloc(size); }
	inline void   fastfree(void * buf, size_t size) {free(buf); }
};

struct item_data
{
	unsigned int type;   			//��Ʒ��ģ��ID
	size_t count;  				//��Ʒ������
	size_t pile_limit;			//��Ʒ�Ķѵ�����
	int proc_type;				//��Ʒ�Ĵ���ʽ
	struct
	{ 
		int guid1;
		int guid2;
	} guid;   				//��Ʒ��GUID
	int price;   				//��Ʒ�ļ۸�
	int expire_date;			//����ʱ��
	size_t content_length;
	char * item_content;
};

#endif

class elementdataman;
namespace element_data
{
	enum ITEM_MAKE_TAG
	{	
		IMT_NULL,
		IMT_CREATE,
		IMT_DROP,
		IMT_SHOP,
		IMT_PRODUCE,
	};
#pragma pack(1)
	struct item_tag_t
	{
		char type;
		char size;
	};
#pragma pack()
}

void generate_item_guid(item_data * item);
//  move into class itemdataman?
class itemdataman
{
protected:
	
#define ELEMENTDATAMAN_EQUIP_MASK_WEAPON       0x0001
#define ELEMENTDATAMAN_EQUIP_MASK_HEAD         0x0002
#define ELEMENTDATAMAN_EQUIP_MASK_NECK         0x0004
#define ELEMENTDATAMAN_EQUIP_MASK_SHOULDER     0x0008
#define ELEMENTDATAMAN_EQUIP_MASK_BODY         0x0010
#define ELEMENTDATAMAN_EQUIP_MASK_WAIST        0x0020
#define ELEMENTDATAMAN_EQUIP_MASK_LEG          0x0040
#define ELEMENTDATAMAN_EQUIP_MASK_FOOT         0x0080
#define ELEMENTDATAMAN_EQUIP_MASK_WRIST        0x0100
#define ELEMENTDATAMAN_EQUIP_MASK_FINGER1      0x0200
#define ELEMENTDATAMAN_EQUIP_MASK_FINGER2      0x0400
#define ELEMENTDATAMAN_EQUIP_MASK_PROJECTILE   0x0800
#define ELEMENTDATAMAN_EQUIP_MASK_FLYSWORD	   0x1000
#define ELEMENTDATAMAN_EQUIP_MASK_DAMAGERUNE	0x20000
	
	enum
	{
		ELEMENTDATAMAN_EQUIP_INDEX_WEAPON          = 0,
			ELEMENTDATAMAN_EQUIP_INDEX_HEAD        = 1,
			ELEMENTDATAMAN_EQUIP_INDEX_NECK        = 2,
			ELEMENTDATAMAN_EQUIP_INDEX_SHOULDER    = 3,
			ELEMENTDATAMAN_EQUIP_INDEX_BODY        = 4,
			ELEMENTDATAMAN_EQUIP_INDEX_WAIST       = 5,
			ELEMENTDATAMAN_EQUIP_INDEX_LEG         = 6,
			ELEMENTDATAMAN_EQUIP_INDEX_FOOT        = 7,
			ELEMENTDATAMAN_EQUIP_INDEX_WRIST       = 8,
			ELEMENTDATAMAN_EQUIP_INDEX_FINGER1     = 9,
			ELEMENTDATAMAN_EQUIP_INDEX_FINGER2     = 10,
			ELEMENTDATAMAN_EQUIP_INDEX_PROJECTILE  = 11,
			ELEMENTDATAMAN_EQUIP_INDEX_FLYSWORD    = 12,
	};
	
	struct _equipment_essence
	{		
		unsigned short reserverd1;
		unsigned short reserverd2;
		unsigned short reserverd3;
		unsigned short reserverd4;
		unsigned short reserverd5;
		unsigned short reserverd6;		
	};
	
	public:
#pragma pack(1)
	// ������
	struct _pet_bedge_essence
	{
		enum
		{
			//�����ƻ�÷�ʽ
			PET_BEDGE_GAIN_TYPE_NULL = 1,
			PET_BEDGE_GAIN_TYPE_QUEST,
			PET_BEDGE_GAIN_TYPE_DROP,
			PET_BEDGE_GAIN_TYPE_NPCSHOP,
			PET_BEDGE_GAIN_TYPE_GSHOP,
			PET_BEDGE_GAIN_TYPE_LOTTERY,
			PET_BEDGE_GAIN_TYPE_REFINE,
		};
		enum
		{
			//��Щ�����˾Ͳ��ܸ���,�����ݿ��
			MAX_PET_SKILL_COUNT = 16,
			MAX_PET_TANLENT_COUNT = 16,
			MAX_OWNER_NAME_LENGTH = 20,
			MAX_PET_NAME_LENGTH = 20,
		};

		enum
		{
			PET_MAGIC_NUMBER = 0xFFFFFFFF,
			PET_VERSION_NUMBER = 1,
		};
		int magic_number;
		char version_number;
		int owner_id;                   //����id,ѱ����־
		char owner_name[MAX_OWNER_NAME_LENGTH];
		char name[MAX_PET_NAME_LENGTH];
		unsigned char origin;           //��Դ
		//�����ǽű������� �����ǽű����ܵ�����
		unsigned char level;            //����
		int exp;                        //����
		int cur_hp;                     //��ǰѪ��
		int cur_vigor;                  //��ǰ����
		unsigned char star;             //�ǵ�
		unsigned char main_type;        //����������
		unsigned char element;          //��������
		unsigned char identify;         //������־
		unsigned char race;             //����
		unsigned char horoscope;        //����
		enum
		{
			MAX_HP,         //����
			MAX_VIGOR,      //����
			MIN_ATTACK,     //��С����
			MAX_ATTACK,     //��󹥻�
			DEFENCE,        //����
			HIT,            //����
			JOUK,           //����
			RESISTANCE1,    //����
			RESISTANCE2,
			RESISTANCE3,
			RESISTANCE4,
			RESISTANCE5,
			RESISTANCE6,
			MAX_COMBAT_ATTR,
		};
		struct combat_attr_t
		{
			unsigned int init;              //��ʼֵ
			unsigned int potential; //Ǳ��ֵ
			unsigned int cur_value; //��ǰֵ
		};
		combat_attr_t attr[MAX_COMBAT_ATTR];    //��������

		unsigned char extra_damage;             //�����˺�
		unsigned char extra_damage_reduce;      //�����˺�����
		float crit_rate;                //������
		float crit_damage;              //�����˺�
		float anti_crit_rate;           //���Ⱪ����
		float anti_crit_damage;         //���Ⱪ���˺�
		float skill_attack_rate;        //����������
		float skill_armor_rate;         //���ܶ�������
		int damage_reduce;              //�˺�����
		float damage_reduce_percent;    //�˺�����ٷֱ�

		unsigned short max_hunger_point;//���ʳ��
		unsigned short cur_hunger_point;//��ǰ��ʳ��
		unsigned short max_honor_point; //������ܶ�
		unsigned short cur_honor_point; //��ǰ���ܶ�
		unsigned short age;             //����
		unsigned short life;            //����
		struct skill_t
		{
			unsigned short id;
			unsigned char level;
		};
		skill_t skills[MAX_PET_SKILL_COUNT];    //����
		unsigned char talents[MAX_PET_TANLENT_COUNT];   //�츳
		enum
		{
			ACUITY, //����
			STAMINA, //����
			SAWY, //����
			DOWRY, //����
			WISDOM, //ѧʶ
			CNR, //���� courageous and resolute
			MAX_MAKE_ATTR,
		};
		unsigned char make_attr[MAX_MAKE_ATTR];
		unsigned char face;
		unsigned char shape;
		enum
		{
			PET_MAIN_STATUS_FIGHT,
			PET_MAIN_STATUS_COLLECT,
			PET_MAIN_STATUS_MAKE,
			PET_MAIN_STATUS_REST,
		};
		unsigned char main_status;      //��״̬
		enum
		{
			PET_SUB_STATUS_PLANTING,
			PET_SUB_STATUS_CUTTING,
			PET_SUB_STATUS_HUNTING,
			PET_SUB_STATUS_FISHING,
			PET_SUB_STATUS_MINING,
			PET_SUB_STATUS_ARCHAEOLOGY,
		};
		unsigned char sub_status;       //��״̬
		enum
		{
			PET_RANK_BASE,
			PET_RANK_COMBAT1,
			PET_RANK_COMBAT2,
			PET_RANK_COMBAT3,
			PET_RANK_COMBAT4,
			PET_RANK_COMBAT5,
			PET_RANK_COMBAT6,
			PET_RANK_WORKSHOP1,
			PET_RANK_WORKSHOP2,
			PET_RANK_WORKSHOP3,
			PET_RANK_WORKSHOP4,
			PET_RANK_WORKSHOP5,
			PET_RANK_WORKSHOP6,
			MAX_PET_RANK,
		};
		unsigned char rank;
		int last_die_timestamp;         //�ϴ�����ʱ��,���������ȴ�
		int last_feed_timestamp;        //�ϴ�ι��ʱ��,����ι����ȴ
		int reborn_cnt;
		int reborn_star;				
		float reborn_prop_add[14];
		int reserved[8];
	}; 

	// ����װ��
	struct _pet_armor_essence
	{
		unsigned short maxhp;
		unsigned short maxmp;
		unsigned short defence;
		unsigned short attack;
		unsigned short resistance[6];
		unsigned short hit;
		unsigned short jouk;
		float crit_rate;
		float crit_damage;
		int reserved;
	};
#pragma pack()
	
	public:
		itemdataman();
		~itemdataman();
		
		int load_data(const char * pathname);
		
		unsigned int get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE& datatype);
		unsigned int get_first_data_id(ID_SPACE idspace, DATA_TYPE& datatype);
		unsigned int get_next_data_id(ID_SPACE idspace, DATA_TYPE& datatype);
		
		unsigned int get_data_num(ID_SPACE idspace);
		DATA_TYPE get_data_type(unsigned int id, ID_SPACE idspace);
		
		const void * get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE& datatype);
		
		int generate_item_from_monster(unsigned int id, int * list,  size_t size);
		void  get_monster_drop_money(unsigned int id,int & low, int &high);
		int get_monster_drop_times(unsigned int id);
		int generate_item_from_droptable(unsigned int id, int * list, size_t size);		

		const void* get_item_for_sell(unsigned int id);
		
		item_data * generate_item(unsigned int id,const void * tag, size_t tag_size, int name_id =0);
		int generate_item(unsigned int id, item_data ** item, size_t& size, const void * tag, size_t tag_size, int name_id =0);
		int get_item_sell_price(unsigned int id);
		int get_item_shop_price(unsigned int id);
		int get_item_pile_limit(unsigned int id);
		int get_item_proc_type(unsigned int id);

	
#include "generate_item_temp.h"

	protected:		
		int generate_item_for_sell();
		int duplicate_static_item(unsigned int id, char ** list,  size_t& size);
		int generate_talisman_stamina_potion(unsigned int id, ID_SPACE idspace, char ** data, size_t& size);
		
		
		elementdataman * _edm;
	public:
		elementdataman * GetElementDataMan() { return _edm;}
		
	private:
		template<class T>
			class array
		{
		public:
			inline size_t size() { return _v.size(); }
			inline void push_back(const T & x) { _v.push_back(x); }
			inline const T & operator [](size_t pos) const { return _v[pos]; }
			inline T & operator [](size_t pos) {return _v[pos];}
		protected:
			abase::vector<T> _v;
		};
		
		array<void *>				sale_item_ptr_array;
		array<size_t>				sale_item_size_array;
		
		struct LOCATION
		{
			DATA_TYPE		type;
			unsigned int	pos;
		};
		
		typedef abase::hash_map<unsigned int, LOCATION> IDToLOCATIONMap;
		IDToLOCATIONMap				sale_item_id_index_map;
};
#endif
