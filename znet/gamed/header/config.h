#ifndef __ONLINEGAME_GS_CONFIG_H__
#define __ONLINEGAME_GS_CONFIG_H__

#define MAX_MATTER_COUNT 	64000		//���ɳ���65536*16
#define MAX_NPC_COUNT 		164000		//���ɳ���65536*16
#define MAX_PLAYER_COUNT 	4096
#define MAX_CS_NUM		1024
#define MAX_GS_NUM		1024


#define ITEM_LIST_SIZE		50
//#define EQUIP_LIST_SIZE		13	//ÿ����λ�Ķ��忴item.h
#define TASKITEM_LIST_SIZE	20

#define PICKUP_DISTANCE		10.f
#define MAGIC_CLASS		5
#define MAX_MAGIC_FACTION	50
#define MAX_MESSAGE_LATENCY	256
#define MAX_AGGRO_ENTRY		50
#define MAX_SOCKET_COUNT	4
#define MAX_SPAWN_COUNT		10240
#define MAX_PLAYER_SESSION	64
#define NORMAL_COMBAT_TIME	5
#define MAX_COMBAT_TIME		30
#define MAX_HURT_ENTRY		512		//�˺��б���������Ŀ��Ŀ
#define LOGOUT_TIME_IN_COMBAT   15		//15��ǳ�ʱ��

#define NPC_IDLE_TIMER		20		//20�����IDLE״̬
#define NPC_IDLE_HEARTBEAT	30		//ÿ60����ͨ��������һ��idle״̬������
#define LEAVE_IDLE_MSG_LIMIT	40		//40��tick����һ�Σ�������Ϊ2��
#define TICK_PER_SEC		20		//һ�����ж��ٸ�tick�����ֵ��������䶯

#define GET_EQUIP_INFO_DIS	150.f		//ȡ��װ����Ϣ�����ƾ��루ˮƽ����
#define TEAM_INVITE_DIS		50.f		//��������ƾ���
#define TEAM_EXP_DISTANCE	100.f		//��Ӿ�������ƾ���
#define TEAM_ITEM_DISTANCE	100.f		//������������Ʒ�ľ���
#define NORMAL_EXP_DISTANCE	100.f		//��Ӿ�������ƾ���
#define TEAM_MEMBER_CAPACITY	10
#define TEAM_WAIT_TIMEOUT	5		//5�룬��5������
#define TEAM_INVITE_TIMEOUT	30		//���볬ʱʱ�� 30 ��, ����ĳ�ʱ
#define TEAM_INVITE_TIMEOUT2	25		//���볬ʱʱ��  �����뷽�ĳ�ʱ,Ӧ��С����������뷽��ʱ
#define TEAM_LEADER_TIMEOUT	30		//��ʮ��memberû���յ����Զӳ�����Ϣ������ʱ
#define TEAM_MEMBER_TIMEOUT	15		//15��û�ܵ���Ա����Ϣ������ʱ
#define TEAM_LEADER_UPDATE_INTERVAL 20		//ÿ��20��leader������������
#define TEAM_LEADER_NOTIFY_TIME	10		//ÿ��10��ӳ���֪ͨ���ж�Ա�Լ����ڵ���Ϣ
#define MAX_PROVIDER_PER_NPC	32
#define MAX_SERVICE_DISTANCE	10.f		//���ķ������Ϊ 10.f ��
#define PURCHASE_PRICE_SCALE	0.3f		//��Ʒ���NPC�ļ۸����
#define DURABILITY_UNIT_COUNT	100		//�ⲿ��ʾ��һ���;öȵ�λ��Ӧ�ڲ���ֵ
#define REPAIR_PRICE_FACTOR	0.1f		//����ʱ�����۸��ϳ��Ե�����
#define TOSSMATTER_USE_TIME	40		//������ʹ��ʱ��̶���1.5��
#define MAX_TOWN_REGION		1024
#define LINK_NOTIFY_TIMER	33
#define MAX_EXTEND_OBJECT_STATE 32		//ÿ���������ͬʱ���ڵ���չ������Ŀ
#define UNARMED_ATTACK_DELAY	12		//���ֵĹ����ӳ�ʱ�����0.3��
#define HELP_RANGE_FACTIOR	2.0f		//��Ⱦ������Ұ���������
#define MAX_AGGRESSIVE_RANGE	10.f		//��������npc���з�Χ
#define MAX_INVADER_ENTRY_COUNT 10		//����¼���ٸ���������

#endif
