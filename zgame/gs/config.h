#ifndef __ONLINEGAME_GS_CONFIG_H__ 
#define __ONLINEGAME_GS_CONFIG_H__ 

#include "dbgprt.h" 
#define GL_MAX_MATTER_COUNT 	164000		//���ɳ���65536*16
#define GL_MAX_NPC_COUNT 	164000		//���ɳ���65536*16
#define GL_MAX_PLAYER_COUNT 	8192
#define MAX_PLAYER_IN_WORLD	(4096*4)
#define MAX_GS_NUM		1024

#define TEAM_MAX_RACE_COUNT	8		//������ְҵ���� �������Ҫ��elements_data��һ��
#define ITEM_LIST_BASE_SIZE	24		//����İ�������С

//�޸İ�����С��ʱ��һ��Ҫ�޸�gamed/gmailendsync.hpp��ͬ�������Ĵ�С����Ȼ״̬�����
#define ITEM_LIST_MAX_ACTUAL_SIZE	252	//ʵ�����İ�������С
#define ITEM_LIST_MAX_SIZE	252		//���İ�������С
//#define EQUIP_LIST_SIZE		13	//ÿ����λ�Ķ��忴item.h
#define TASKITEM_LIST_SIZE	30		//����������Ĵ�С
#define TRASHBOX_BASE_SIZE	24		//�ֿ�������С
#define TRASHBOX_MAX_SIZE	252		//���Ĳֿ��С
#define MAFIA_TRASHBOX_BASE_SIZE 6		//���ɲֿ�����Ĵ�С
#define MAX_PET_BEDGE_LIST_SIZE	32		//�������ư�������С
#define INIT_PET_BEDGE_LIST_SIZE 2		//��ʼ�������ư�������С
#define PET_EQUIP_LIST_SIZE	12		//����װ����������С
#define POCKET_INVENTORY_BASE_SIZE 0		//���������ʼ��С
#define POCKET_INVENTORY_MAX_SIZE 252		//������������С
#define FASHION_INVENTORY_SIZE 120 		//ʱװ������С
#define REPURCHASE_INVENTORY_SIZE 18	//��Ʒ�ع�������С
#define MOUNT_WING_INVENTORY_BASE_SIZE 0    //����ɽ������ĳ�ʼ��С
#define MOUNT_WING_INVENTORY_MAX_SIZE 18	//����ɽ��������size
#define GIFT_INVENTORY_SIZE	12		//Ԫ�����ְ�����С
#define FUWEN_INVENTORY_SIZE	6		//���İ�����С
#define HOTKEY_MAX_SIZE 10			//�����ȼ��������
#define PET_SUMMON_ALLOWED_COUNT 1		//�����ٻ��ĳ�������
#define MAX_PET_LEVEL		160		//�����Ｖ��
#define DEFAULT_GRID_SIGHT_RANGE 40.f		//Ĭ����Ұ����

#define MAX_REBORN_COUNT	3		//���ת������
#define MIN_REBORN_LEVEL	135		//��Сת���ȼ�

#define MATTER_ITEM_LIFE	120
#define MATTER_ITEM_PROTECT	30
#define PICKUP_DISTANCE		10.f
#define MAGIC_CLASS		5
#define MAX_MAGIC_FACTION	50
#define MAX_MESSAGE_LATENCY	256*20
#define MAX_AGGRO_ENTRY		50
#define MAX_SOCKET_COUNT	4
#define MAX_SPAWN_COUNT		10240
#define MAX_MATTER_SPAWN_COUNT  10240	
#define MAX_PLAYER_SESSION	64
#define NORMAL_COMBAT_TIME	15		//ͨ��ս��ʱ�䣬ʹ���˹������ʱ��
#define MAX_COMBAT_TIME		15		//���ս��ʱ�䣬�ǹ���������������ʱ��	
#define MAX_HURT_ENTRY		512		//�˺��б���������Ŀ��Ŀ
#define LOGOUT_TIME_IN_NORMAL   3		//3����ͨ�ĵǳ�ʱ��
#define LOGOUT_TIME_IN_COMBAT   15		//15��ǳ�ʱ��
#define LOGOUT_TIME_IN_RECONNECT 1		//1��ǳ�ʱ��
#define LOGOUT_TIME_IN_TRAVEL   30		//30�����еǳ�ʱ��
#define SELECT_IGNORE_RANGE	170.f		//ѡ��Ŀ������ƾ���

#define NPC_IDLE_TIMER		20		//20�����IDLE״̬
#define NPC_IDLE_HEARTBEAT	30		//ÿ60����ͨ��������һ��idle״̬������
#define LEAVE_IDLE_MSG_LIMIT	40		//40��tick����һ�Σ�������Ϊ2��
#define TICK_PER_SEC		20		//һ�����ж��ٸ�tick�����ֵ��������䶯

#define GET_EQUIP_INFO_DIS	200.f		//ȡ��װ����Ϣ�����ƾ��루ˮƽ����
#define TEAM_INVITE_DIS		100.f		//��������ƾ���
#define TEAM_EXP_DISTANCE	60.f		//��Ӿ�������ƾ���
#define TEAM_ITEM_DISTANCE	60.f		//������������Ʒ�ľ���
#define NORMAL_EXP_DISTANCE	60.f		//��Ӿ�������ƾ���
#define TEAM_MEMBER_CAPACITY	6		//��ӵ��������
#define TEAM_WAIT_TIMEOUT	5		//5�룬��5������
#define TEAM_INVITE_TIMEOUT	30		//���볬ʱʱ�� 30 ��, ����ĳ�ʱ
#define TEAM_INVITE_TIMEOUT2	25		//���볬ʱʱ��  �����뷽�ĳ�ʱ,Ӧ��С����������뷽��ʱ
#define TEAM_LEADER_TIMEOUT	30		//��ʮ��memberû���յ����Զӳ�����Ϣ������ʱ
#define TEAM_MEMBER_TIMEOUT	15		//15��û�ܵ���Ա����Ϣ������ʱ
#define TEAM_LEADER_UPDATE_INTERVAL 20		//ÿ��20��leader������������
#define TEAM_LEADER_NOTIFY_TIME	10		//ÿ��10��ӳ���֪ͨ���ж�Ա�Լ����ڵ���Ϣ
#define MAX_PROVIDER_PER_NPC	48		//ÿ��NPC����ṩ���������
#define MAX_SERVICE_DISTANCE	10.f		//���ķ������Ϊ 10.f ��
#define DURABILITY_UNIT_COUNT	100		//�ⲿ��ʾ��һ���;öȵ�λ��Ӧ�ڲ���ֵ
#define DURABILITY_DEC_PER_HIT 	25		//����ÿ�α����м��ٵ��;�ֵ
#define DURABILITY_DEC_PER_ATTACK 2		//����ÿ�ι������ٵ��;�ֵ
#define TOSSMATTER_USE_TIME	40		//������ʹ��ʱ��̶���1.5��
#define MAX_TOWN_REGION		1024
#define LINK_NOTIFY_TIMER	33
#define MAX_EXTEND_OBJECT_STATE 32		//ÿ���������ͬʱ���ڵ���չ������Ŀ
#define UNARMED_ATTACK_DELAY	12		//���ֵĹ����ӳ�ʱ�����0.3��
#define HELP_RANGE_FACTOR	1.0f		//��Ⱦ������Ұ���������
#define DEFAULT_AGGRESSIVE_RANGE 15.f		//��׼��������з�Χ����������������趨
#define MAX_INVADER_ENTRY_COUNT 10		//����¼���ٸ���������
#define MAX_PLAYER_ENEMY_COUNT  20		//������ҵ�����Ŀ
#define PARIAH_TIME_PER_KILL    7200		//ÿ��ɱ�˱�ɵĺ����ۼ�ʱ��
#define PARIAH_TIME_REDUCE	72		//ɱһֻ�߼���������pkֵ
#define MAX_PARIAH_TIME		(PARIAH_TIME_PER_KILL*100)	//����PKֵ
#define DEATH_PROTECT_LEVEL	15		//���������ļ���
#define PVP_PROTECT_LEVEL	30		//���Խ���PK�ļ���
#define MATTER_HEARTBEAT_SEC	11		//��Ʒÿ������һ������
#define GATHER_RANGE_LIMIT	6.f		//�ɼ�����ľ�������
#define TRASHBOX_MONEY_CAPACITY	2000000000	//�ֿ�������Ǯ����
#define MONEY_CAPACITY_BASE	2000000000
#define MONEY_CAPACITY_PER_LVL	200000000
#define MAX_ITEM_DROP_COUNT	65535		//������Ʒʱÿ�����������Ŀ
#define MONEY_DROP_RATE		0.7f		//��Ǯ�������
#define MONEY_MATTER_ID		565		//��Ǯ�ĵ�����Ʒid
#define FLEE_SKILL_ID 		40		//�������ܵļ���ID
#define SUICIDE_ATTACK_SKILL_ID	147		//�Ա������ļ���ID
#define WORLD_SPEAKER_ID	2530		//ǧ�ﴫ��������ƷID
#define WORLD_SPEAKER_ID2	33056		//ǧ�ﴫ��������ƷID2
#define ITEM_DESTROYING_ID	2965		//����Ʒ�ݻ��е�ID
#define STAYIN_BONUS		100		//�����ļӳ�
#define PLAYER_BODYSIZE		0.3f		//��ҵ����ʹ�С
#define MAX_MASTER_MINOR_RANGE	400.f		
#define BASE_REBORN_TIME	15		//����5�븴��ʱ��
#define NPC_FOLLOW_TARGET_TIME	0.5f		//��������һ��NPC׷������Сʱ��
#define NPC_FLEE_TIME		0.5f		//��������һ��NPC���ܵ���Сʱ��
#define MAX_FLIGHT_SPEED	20.f		//�������ٶ�
#define MAX_RUN_SPEED		15.f		//����ܶ��ٶ�
#define MAX_WALK_SPEED		8.f		//��������ٶ�
#define MIN_RUN_SPEED		0.1f		//��С�ܶ��ٶ�
#define MIN_WALK_SPEED		0.1f		//��С�����ٶ�
#define MAX_JUMP_SPEED		10.f		//�����Ծ�ٶ�
#define PLAYER_DEATH_EXP_VANISH	0.03f		//����ʱ��Ҫ���ٵľ������
#define NPC_PATROL_TIME		1.0f
#define PLAYER_MARKET_SLOT_CAP	36
#define PLAYER_MAERKET_NAME_LEN 28
#define PVP_DAMAGE_REDUCE	1.0f		//PVP����ʱ�˵�ϵ��
#define MAX_PLAYER_LEVEL	200		//������󼶱𣬲�Ҫ�������
#define MAX_TALISMAN_LEVEL	100		//������󷨱�����
#define MAX_WAYPOINT_COUNT	1024		//����·����Ŀ
#define NPC_REBORN_PASSIVE_TIME 5		//������������������ĵȴ�ʱ��
#define PVP_STATE_COOLDOWN	(10*3600)
#define WATER_BREATH_MARK	3.0f		//ˮ���������²���������
#define MAX_PLAYER_EFFECT_COUNT 32
#define PLAYER_REBORN_PROTECT	5		//��Ҹ�����ü����ӵķ���ʱ��(���ܲ������������ƣ������˶�)
#define CRIT_DAMAGE_BONUS	2.0f		//�ػ��������˺��ӳ�
#define PLAYER_HP_GEN_FACTOR	5		//��һ�Ѫ������
#define PLAYER_MP_GEN_FACTOR	10		//��һ�ħ������
#define MAX_USERNAME_LENGTH	20		//������ֵ���󳤶�
#define PVP_COMBAT_HIGH_TH	60		//PVP��ʱ�������
#define PVP_COMBAT_LOW_TH	45		//PVP��ʱ��С��һ��ֵ�Ż���з���ˢ��
#define MAX_DOUBLE_EXP_TIME	(4000*3600)	//������˫��ʱ��
#define MAX_MULTI_EXP_TIME	(4000*3600)	//������˫��ʱ��
#define MIN_DUEL_LEVEL		10		//��͵ľ�������
#define MIN_PK_VALUE		-999		//��СPKֵ
#define MAX_PK_VALUE		999		//���PKֵ
#define REPUTATION_REGION	32		//��������������Ŀ
#define MAX_REPUTATION		200000000	//�����������������
#define	HELP_BONUS_LEVEL	75		//���ټ����������˴����˵Ľ���
#define	HELP_LEVEL_LIMIT	90		//���ټ����ϵ����˲��ܴ�����
#define PKVALUE_PER_POINT	7200		//ÿ��PKֵ�ĵ�Ч�ڲ�ֵ
#define STD_MOVE_USE_TIME	500		//�ƶ�������Ĳ���ʱ�� ���뵥λ
#define MIN_MOVE_USE_TIME	100		//�ƶ��������������Сʱ�� ���뵥λ
#define MAX_MOVE_USE_TIME	1000		//�ƶ���������������ʱ�� ���뵥λ
#define FAST_MOVE_SKIP_TIME	150		//������������ʱ��
#define SESSION_TIME_OUT	30		//session�����ִ�У��䳬ʱʱ��Ϊ����
#define ID_LIANQIFU		685		//������ID
#define MAX_ARCHER_COUNT	5
#define SECT_MASTER_REP_INDEX	8		//ʦͽϵͳ��ʦ�������Ѻö�����
#define SECT_MASTER_TITLE	11013
#define MAX_DEATH_EXP_PUNISH	250000000	//���������ʧ����
#define SPIRIT_POWER_DEC_PROB	0.005f		//ÿ�ι���/�������������ٵĸ���
#define MAGIC_DURATION_DEC_PROB	0.005f		//ÿ�ι���/����������ʯ�������ٵĸ���

#define TICK_PER_SECOND		20
#define TICK_PER_SECOND_A	TICK_PER_SECOND
#define SECOND_TO_TICK(f) 	((int)((f) * TICK_PER_SECOND_A + 0.5f))
#define TICK_TO_SECOND(t) 	((t) * (1.0f/TICK_PER_SECOND_A)
#define MILLISEC_TO_TICK(f) 	((int)((f) * (TICK_PER_SECOND_A/1000.f) + 0.5f))
#define TICK_MILLISEC		(1000.f/TICK_PER_SECOND_A)
#define TICK_TO_MILLISEC(t) 	(int)((t) * TICK_MILLISEC)

#define MIN_BATTLE_MAPID	101		//��С��ս����ͼID
#define MAX_BATTLE_MAPID	199		//����ս����ͼID

#define LINE_COMMON_DATA_TYPE_BEGIN  1024		//�����ڲ��Լ�ʹ�õ�ȫ�����ݿռ�Ŀ�ʼ���к�
#define MAP_COMMON_DATA_TYPE_BEGIN  2048		//����ͼ�ڲ��Լ�ʹ�õ�ȫ�����ݿռ�Ŀ�ʼ���к�

#define MAX_DEITY_LEVEL		81		//�������ȼ�
#define MAX_DEITY_CLEVEL	9		//����ÿ���׶����ȼ�
#define MAX_DEITY_EXP		(int64_t)1000000000000ULL	//��������������(1����)
#define MAX_DEITY_POWER		10000
#define MAX_GEM_LEVEL		10

#define MAX_TREASURE_REGION	12	
#define MAX_TREASURE_NORMAL_REGION	9	
#define MAX_TREASURE_REGION_LEVEL	5
#define MAX_TREASURE_RESET_CYCLE	86400	

#define MAX_LITTLE_PET_LEVEL	5

#define MAX_TOWER_RAID_LEVEL	100
#define CONSUMPTION_SCALE (100000.0f)  //����ֵ���ñ����������С��֧�ֵ�0.0001�ľ���

#define COLLISION_RAID_ID 535 
#define COLLISION_RAID_ID2 543 

#define HIDE_AND_SEEK_RAID_ID 549

#define CAPTURE_RAID_ID 550

#define MAFIA_BATTLE_RAID_ID 691

#define MAX_XUANYUAN_SPIRITS	3
#define MAX_ASTROLOGY_VALUE 100000

#define MAFIA_BATTLE_RAID_VISITOR_COST 500000 //��ս������ս����ȡ�������ѣ�50��
#define SKILL_COOLINGID_BEGIN 1024
#define MAX_BOTTLE_CAPACITY 2000000000

#define BASE_COLLISION_SCORE	1000

#define REPU_XIAN 9
#define REPU_FO 10
#define REPU_MO 11

#define TASK_CHAT_MESSAGE_ID	24		//��������������Ϣ��ָ��ID
#define LOTTERY_TY_CHAT_MESSAGE_ID	25	//��Բ��Ʊ���纰����ϢID
#define KINGDOM_BUY_ITEM_CHAT_MESSAGE_ID 26	//�������������Ʒ���纰��ID
#define FUWEN_COMPOSE_MESSAGE_ID 27		//������Ƭ�ϳɲ�����Ʒ���ĵ�ʱ�����纰��

#define WEB_PACKAGE_BASE_ID	1000000		//Web�̳�Package��ʼID

enum
{
	FBASE_PROP_GRASS = 1,
	FBASE_PROP_MINE,
	FBASE_PROP_MONSTERFOOD,
	FBASE_PROP_MONSTERCORE,
	FBASE_PROP_CASH,
};

#define MAX_FAC_BASE_MSG_LEN	100

#define MAX_MAFIA_NAME_LENGTH 20
#endif

