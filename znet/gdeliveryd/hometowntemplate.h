#ifndef _HOMETOWN_TEMPLATE_H_
#define _HOMETOWN_TEMPLATE_H_

#include <vector>
#include <map>

#ifdef WIN32
#pragma warning(disable:4996)
typedef signed __int64 int64_t;
typedef wchar_t namechar;
#else
typedef unsigned short namechar;
#endif

enum ID_SPACE
{
	ID_SPACE_ESSENCE = 0,
	ID_SPACE_FUNC,
	ID_SPACE_CONFIG,
	ID_SPACE_MAX,
};

enum DATA_TYPE
{
	DT_CROP_ESSENCE = 0,
	DT_FRUIT_ESSENCE,
	DT_TOOL_FUNC,
	DT_TOOL_ESSENCE,
	DT_MAX,
	DT_INVALID = DT_MAX,
};

struct CROP_ESSENCE
{
	unsigned int 	id;
	namechar 		name[32];
	char 			type;				//0:��ͨ��1����ϡ
	int 			required_level;		//����ȼ�
	int 			fruit_max;			//��������
	int 			fruit_min;			//��������
	int 			fruit_id;			//��ʵid
	int 			seed_price;			//���Ӽ۸�
	int 			grow_period;		//������
	int 			protect_period;		//��͵������
	int 			steal_limit;		//��͵�ı������ٷ���
	int 			icon_seed;			//����ͼ��
	int 			icon_sprout;		//����ͼ��
	int 			icon_immature;		//������ͼ��
	int 			icon_premature;		//��������ͼ��
	int 			icon_mature;		//����ͼ��
	int 			icon_wilt;			//��ήͼ��
	int 			icon_harvest;		//���ջ�ͼ��
	int 			icon_stolen;		//��͵��ͼ��
};

struct FRUIT_ESSENCE
{
	unsigned int 	id;
	namechar 		name[32];
	int 			price;				//��ʵ���ۼ۸�
	int 			icon;				//��ʵͼ��
	int				pile_limit;			//�ѵ�����
};

enum TOOL_FUNC_TYPE
{
	TFT_FARM_PROTECT = 0,				//������ũ���ӱ�����
	TFT_FIELD_PROTECT,					//��һ��ؼӱ�����
	TFT_FIELD_SPEEDGROW,				//��һ��ؼӿ�����
	TFT_FARM_SPEEDGROW,					//������ũ���ӿ�����
	TFT_FARM_HIDE,						//ũ��������

	TFT_MAX,
};

struct TOOL_FUNC						//���߹���ģ��
{
	unsigned int	id;
	namechar		name[32];
	int 			type;				//��������
	int				num_params;			//���ܲ�������
	int				param1;				//����1
	int 			param2;				//����2
	int 			param3;				//����3
};

struct TOOL_ESSENCE
{
	unsigned int	id;
	namechar		name[32];
	int 			required_level;		//��Ҫ������ȼ�
	int				charm_price;		//������Ҫ������ֵ
	int				money_price;		//������Ҫ���������
	unsigned int 	func_ids[5];		//ÿ���������5������
	int				cool_type;			//������ȴ���ͣ���1��ʼ���
	int 			cool_time;			//��ȴʱ��
	int				icon;				//ͼ��
	int				pile_limit;			//�ѵ�����
};

struct LEVEL_CONFIG
{
	int level;							//����
	int normal_field_count;				//�ü���ӵ����ͨ������
	int love_field_count;				//�ü���ӵ�а��ĵ���
	int plant_love_field;				//�ü���ɸ��ֱ��˵İ��ĵ���
};

struct FARM_CONFIG							//͵��������ýṹ
{
	unsigned int 	id;
	namechar		name[32];
	int 			max_level;				//���ȼ�
	int 			max_field_count;		//ÿ��������������Ŀ
	int				init_pocket_caps;		//��ʼ��������
	std::vector<LEVEL_CONFIG> level_config;	//��ͬ�����������Ŀ����,Ҫ�󰴵ȼ���С��������
	std::vector<int64_t> upgrade_config;	//�����������������ֵ,�����СӦ����max_level, upgrade_config[i]��ʾ��level i����i+1��Ҫ������ֵ
	std::vector<int> crop_list;				//������ֲ������ID�б�,Ҫ���С����
	std::vector<int> tool_list;				//�ɹ���ĵ���ID�б�, Ҫ���С����
	int 			money_local;			//������ҵ���Ŀ�һ�������Ŀ��ͨ�û���
	int				money_universal;		//ͨ�û��ҵ���Ŀ��Ӧ������Ŀ���������
};

struct HOMETOWN_CONFIG						//������԰���ýṹ
{
	unsigned int	id;
	namechar		name[32];
	std::vector<int> compo_list;			//�Ѿ����ŵ����ID�б�
};

class HometownTemplate
{

public:
	static HometownTemplate *GetInstance()
	{
		static HometownTemplate inst;
		return &inst;
	}
	unsigned int get_first_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_next_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_data_num(ID_SPACE idspace);
	DATA_TYPE get_data_type(unsigned int id, ID_SPACE idspace);

	template <typename T>
	const T *get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE & datatype, const T *&data)
	{
		if (idspace >= ID_SPACE_MAX) 
		{
			data = NULL;
			return NULL;
		}
		IDToLOCATIONMap &id_loc_map = _id_loc_map[idspace];
		IDToLOCATIONMap::iterator it = id_loc_map.find(id);
		if (it == id_loc_map.end()) 
		{
			data = NULL;
			return NULL;
		}
		datatype = it->second.type;
		std::vector<T> &v = *(std::vector<T> *)_vec_start_ptr[datatype];
		data = &v[it->second.pos];
		return data;
	}
	const HOMETOWN_CONFIG *get_hometown_config() { return &_HOMETOWN_CONFIG; }
	const FARM_CONFIG *get_farm_config() { return &_FARM_CONFIG; }

	template <typename T>
	void add_structure(int data_type, const T &data)
	{
		if (data_type>=0 && data_type<DT_MAX)
		{
			std::vector<T> &v = *(std::vector<T> *)_vec_start_ptr[data_type];
			v.push_back(data);
		}
	}
	void add_structure(const HOMETOWN_CONFIG &cfg) { _HOMETOWN_CONFIG = cfg; }
	void add_structure(const FARM_CONFIG &cfg) { _FARM_CONFIG = cfg; }
	void finish_add_structure() { _setup_location(); }		//������һϵ��add_structure��Ҫ����finish_add_structure���¼���_id_loc_map

	int save_data(const char * pathname);
	int load_data(const char * pathname);

private:
	HometownTemplate() { _setup_start(); }
	HometownTemplate(const HometownTemplate &){}
	HometownTemplate & operator = (const HometownTemplate &){ return *this; }

	int _save_data(FILE *file);
	int _load_data(FILE *file);
	void _setup_location(); 		//���_id_loc_map
	void _setup_start();			//���_vec_start_ptr

	struct LOCATION
	{
		DATA_TYPE type;
		size_t pos;
	};
	typedef std::map<unsigned int, LOCATION> IDToLOCATIONMap;

	std::vector<CROP_ESSENCE>		_CROP_ESSENCE_vector;
	std::vector<FRUIT_ESSENCE>		_FRUIT_ESSENCE_vector;
	std::vector<TOOL_FUNC>			_TOOL_FUNC_vector;
	std::vector<TOOL_ESSENCE>		_TOOL_ESSENCE_vector;
	HOMETOWN_CONFIG					_HOMETOWN_CONFIG;
	FARM_CONFIG						_FARM_CONFIG;

	std::vector<void *>				_vec_start_ptr;

	IDToLOCATIONMap					_id_loc_map[ID_SPACE_MAX];
	IDToLOCATIONMap::iterator 		_id_loc_itr[ID_SPACE_MAX];
};
#endif
