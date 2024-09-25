#include <octets.h> 
#include <set>

namespace GNET
{
// Youshuang add for new equip essence
enum
{
	EQUIP_ESSENCE_VERSION_OLD = 0,
	EQUIP_ESSENCE_VERSION_SPIRIT = 1,
	EQUIP_ESSENCE_VERSION_GEM = 2,
	EQUIP_ESSENCE_VERSION_ASTROLOGY = 3,
	MAX_ENCHANT_SLOT = 5,
	MAX_GEM_SLOT = 3,
};

#pragma pack(1)
struct astrology_prop
{
        int id; 
        int value;
};

struct astrology_slot 
{
        int astrology_id;
        int level;
        int score;
        int energy_consume;
        astrology_prop prop[6];
        bool is_lock;                   //����������������Ϣ
        int reserver1;
        int reserver2;
};

struct equip_essence_new
{
	int version;
	
	// old essence
	int rf_stone;
	short rf_stone_level;
	int pr_stone;
	short pr_stone_level;
	int sk_stone;
	unsigned int sk_stone_level; 

	// add spirit
	int master_id;				//����ID
	char master_name[20];			//��������
	int reserved1;
	int reserved2;
	int cur_spirit_power;			//��ǰ����
	int max_spirit_power;			//��������
	int soul_used_spirit;			//�����Ѿ�ռ�õĻ���
	int soul[MAX_ENCHANT_SLOT];		//��ľˮ����5������
	int soul_rune_word;			//����������Ե�CRC��ֻ�з�0����Ч

	// add gem
	char gem_slot_shape[MAX_GEM_SLOT]; 	//װ����ʯ����״, -1Ϊ�òۻ�û�б�����
	int gem_embeded[MAX_GEM_SLOT];		//�Ѿ���Ƕ�ı�ʯ��-1��ʾ�òۻ�û����Ƕ��ʯ
	int colorant_id;
	int reserved3;
	int reserved4;
	
	// add astrology
	int cur_hole;				//��ǰ�Ŀ���
	int reserved5;
	int reserved6;
	astrology_slot hole[3];

	int reserved[32];
};
#pragma pack()

//Note: ʹ�������ǰӦ��ȷ��gs�й��������ֶεĶ���û�иı�
#define TALISMAN_ESSENCE_SIZE 92		//����װ���ṹ��С
#define PETBEDGE_OLD_ESSENCE_SIZE 236 		//�ϳ������ݽṹ
#define PETBEDGE_ESSENCE_SIZE 450 		//�³������ݽṹ
#define EQUIP_OLD_ESSENCE_SIZE 12		//������װ���ṹ��С
#define EQUIP_SPIRIT_ESSENCE_SIZE 80		//��Ѫ���汾��װ���ṹ��С
#define EQUIP_GEM_ESSENCE_SIZE 107		//��ʯ�汾��װ���ṹ��С
#define EQUIP_ASTROLOGY_ESSENCE_SIZE 335 	//�����汾��װ���ṹ��С
#define TRANS_STONE_ESSENCE_SIZE 128 		//����ʯ(����ʯװ���ṹ��С)
#define ITEM_BASE_SIZE 9			//item�ṹ�Ļ�����С(9��int)
#define ITEM_UNLOCKING_ID 21119			//�����ֵ���Ʒ
#define ITEM_DESTROY_ID	4544			//�ݻ��е���Ʒ
#define ITEM_BROKEN_ID	21695			//����������Ʒ	
	
// ��ͬ��Gs�е�Item.h��ö��
enum
{
	EQUIP_INDEX_WEAPON		= 0,
	EQUIP_INDEX_HEAD		= 1,
	EQUIP_INDEX_BODY		= 2,
	EQUIP_INDEX_FOOT		= 3,
	EQUIP_INDEX_NECK		= 4,
	EQUIP_INDEX_FINGER1		= 5,
	EQUIP_INDEX_FINGER2		= 6,
	EQUIP_INDEX_FASHION_EYE		= 7,	
	EQUIP_INDEX_MAGIC		= 8, // ����ʯ
	EQUIP_INDEX_FASHION_LIP		= 9, // ����
	EQUIP_INDEX_FASHION_HEAD	= 10,
	EQUIP_INDEX_FASHION_BODY	= 11,
	EQUIP_INDEX_FASHION_LEG		= 12,
	EQUIP_INDEX_FASHION_FOOT	= 13,
	EQUIP_INDEX_FASHION_BACK	= 14,		// ����
	EQUIP_INDEX_WING		= 15,
	EQUIP_INDEX_TALISMAN1		= 16,		// ����
	EQUIP_INDEX_TALISMAN2		= 17,
	EQUIP_INDEX_TALISMAN3		= 18,
	EQUIP_INDEX_BUGLE		= 19,		// ������
	EQUIP_INDEX_XITEM1		= 20,		//����
	EQUIP_INDEX_XITEM2		= 21,		//���
	EQUIP_INDEX_XITEM3		= 22,		//���
	EQUIP_INDEX_RUNE		= 23,		//Ԫ��
	EQUIP_INDEX_XITEM4		= 24,
	EQUIP_INDEX_XITEM5		= 25,
	EQUIP_INDEX_FASHION_WEAPON	= 26,		// Youshuang add for fashion weapon
	EQUIP_INVENTORY_COUNT,

	EQUIP_VISUAL_START	= EQUIP_INDEX_WEAPON,
	//EQUIP_VISUAL_END	= EQUIP_INDEX_RUNE + 1, 
	EQUIP_VISUAL_END	= EQUIP_INDEX_FASHION_WEAPON + 1,  // Youshuang add

	EQUIP_MASK_ALL  	= 0xFFFFFF,
};

struct SEquipUnit
{
	unsigned int id;
	short rf_stone_level;
};

struct SEquipsRole
{
	SEquipUnit equip_head; // ñ��
	SEquipUnit equip_body; // �·�
	SEquipUnit equip_foot; // Ь��
	SEquipUnit equip_weapon; // ����

	unsigned int talisman1_id; 	// ����
	unsigned int neck_id; // ����
	unsigned int finger1_id; //	���ָ
	unsigned int finger2_id; //	�ҽ�ָ
	unsigned int xitem3_id; //	���
	unsigned int back_id;//	����
	unsigned int xitem2_id; //	���
	unsigned int xitem1_id; //	����
	unsigned int lip_id;//	����/����
/*
	unsigned int eye_id;
	unsigned int lip_id;
	unsigned int xitem4_id;
	unsigned int xitem5_id;
*/
};

//typedef std::map<int /*pos*/, SEquipRoleScan> TEquipRoleScanMap;

enum ITEM_TYPE{ITEMTYPE_EQUIPMENT=0, ITEMTYPE_TALISMAN, ITEMTYPE_PETBADGE, ITEMTYPE_TRANS_STONE, ITEMTYPE_DELETE, ITEMTYPE_MAX};

int GetOwnerID(int item_type, const Octets & item_data);
bool SetOwnerID(int item_type, Octets & item_data, int owner_id);
bool SetOwnerName(int item_type, Octets & item_data, Octets & owner_name);
int GetSpecialItemID(int item_id, const Octets & item_data);
int GetSpecialItemOwnerID(int item_id, int special_item_type, const Octets & item_data);
bool SetSpecialItemOwnerName(int item_id, int special_item_type, Octets & item_data, Octets & owner_name);
bool SetSpecialItemOwnerID(int item_id, int special_item_type, Octets & item_data, int owner_id);
bool TestTalismanData(const Octets & item_data);
bool TestPetbedgeData(const Octets & item_data, char & data_type);
bool TestTransStoneData(const Octets & item_data);

class ItemIdMan
{
private:
	std::set<int> m_setItemId[ITEMTYPE_MAX];
public:
	bool LoadItemId(const char *filename);
	bool LoadItemId();
	int GetItemType(int itemid)
	{
		int item_type = -1;
		for (int i = 0; i < ITEMTYPE_MAX; ++i)
		{
			if (m_setItemId[i].find(itemid) != m_setItemId[i].end())
			{
				item_type = i;
				break;
			}
		}
		return item_type;
	}
};
};
