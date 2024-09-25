#ifndef __ONLINE_GAME_GS_COOLDOWN_CONFIG_H__
#define __ONLINE_GAME_GS_COOLDOWN_CONFIG_H__

//��λΪ����
#define EMOTE_COOLDOWN_TIME		1500								//��ʾ���鶯������ȴʱ��
#define FASHION_COOLDOWN_TIME		4000							//��ʾ����ʱװ����ȴʱ��
#define DROPMONEY_COOLDOWN_TIME		1671							//��ʾ��Ǯ��������ȴʱ��
#define DROPITEM_COOLDOWN_TIME  	321								//��ʾ����Ʒ��������ȴʱ��
#define QUERY_OTHER_EQUIP_COOLDOWN_TIME	20000						//�쿴����װ������ȴʱ��
#define FIREWORKS_COOLDOWN_TIME		800								//ʹ�ñ�׼�̻���ʱ��
#define SKILLTRIGGER_COOLDOWN_TIME	120000							//ʹ�ü�����Ʒ����ȴʱ��
#define COUPLE_JUMP_COOLDOWN_TIME	5000							//����ʹ����Ʒ����ȴʱ��
#define LOTTERY_COOLDOWN_TIME		100								//��Ʊ����ȴʱ��
#define RECORDER_COOLDOWN_TIME		20000							//���������ȴʱ��
#define RECORDER_CHECK_COOLDOWN_TIME	20000						//���¼�������ȴʱ��
#define	GETBATTLEINFO_COOLDOWN_TIME	30000							//��ȡս����Ϣ
#define QUERY_OTHER_ACHIEVMENT_COOLDOWN_TIME    10000   			//�鿴���˵ĳɾ͵���ȴʱ�� 
#define CHANGE_DS_TIME			180000								//����������ȴʱ��

#define GET_MALL_PRICE_COOLDOWN_TIME		30000					//��ȡ�̳�����Щ��ʱ��Ʒ��������
#define GET_VIP_AWARD_INFO_COOLDOWN_TIME	15000					//��ȡVIP������Ϣ��ȴʱ��
#define RESET_SKILL_PROP_COOLDOWN_TIME		(24 * 3600 * 1000) 		//90���������ϴ��ϴ������ȴʱ��
#define ARRANGE_TRASHBOX_COOLDOWN_TIME          5000               	//����ֿ����ȴʱ��
#define ARRANGE_INVENTORY_COOLDOWN_TIME         5000               	//����������ȴʱ��
#define ARRANGE_POCKET_COOLDOWN_TIME         	5000               	//����pocket����ȴʱ��

enum 
{
	COOLDOWN_INDEX_NULL,			//0, ��ʾ����ȴʱ�䣬ͨ�����ڳ�ʼ����ռλ��
	COOLDOWN_INDEX_EMOTE,			//1, ���鶯������ȴʱ�䡣
	COOLDOWN_INDEX_DROP_MONEY,		//2, ��Ǯ��������ȴʱ�䡣
	COOLDOWN_INDEX_DROP_ITEM,		//3, ����Ʒ��������ȴʱ�䡣
	COOLDOWN_INDEX_SWITCH_FASHION,	//4, ����ʱװ����ȴʱ�䡣

	COOLDOWN_INDEX_ITEM_TELEPORT,		//5, ��Ʒ���͵���ȴʱ�䡣
	COOLDOWN_INDEX_QUERY_OTHER_EQUIP,	//6, ��ѯ�������װ������ȴʱ�䡣
	COOLDOWN_INDEX_FIREWORKS,			//7, �̻�ʹ�õ���ȴʱ�䡣
	COOLDOWN_INDEX_FARCRY,				//8, Զ�̺��е���ȴʱ�䡣
	COOLDOWN_INDEX_SKILL_TRIGGER,		//9, ���ܴ�������ȴʱ�䡣

	COOLDOWN_INDEX_DBL_EXP,			//10, ˫���������ȴʱ�䡣
	COOLDOWN_INDEX_SKILL_ITEM_1,	//11, ��ͬ������Ʒ����ȴʱ�䡣
	COOLDOWN_INDEX_SKILL_ITEM_2,	//12, ��ͬ������Ʒ����ȴʱ�䡣
	COOLDOWN_INDEX_SKILL_ITEM_3,	//13, ��ͬ������Ʒ����ȴʱ�䡣
	COOLDOWN_INDEX_PK_RESET,		//14, PK����Ҷ�ս�����õ���ȴʱ�䡣

	COOLDOWN_INDEX_MOUNT,			//15, ����ʹ�õ���ȴʱ�䡣
	COOLDOWN_INDEX_COUPLE_JUMP,		//16, ����ʹ�õ���ȴʱ�䡣
	COOLDOWN_INDEX_LOTTERY,			//17, �齱����ȴʱ�䡣
	COOLDOWN_INDEX_RECORDER,		//18, ��¼����ز�������ȴʱ�䡣
	COOLDOWN_INDEX_RECORDER_CHECK,	//19, ��¼����ز�������ȴʱ�䡣

	COOLDOWN_INDEX_TEAM_RELATION,		//20, �Ŷӹ�ϵ��������ȴʱ�䡣
	COOLDOWN_INDEX_CD_ERASER,			//21, ��ȴʱ���������ȴʱ�䡣
	COOLDOWN_INDEX_PET_COMBINE1,		//22, ����ϳɵ���ȴʱ�䡣
	COOLDOWN_INDEX_PET_COMBINE2,		//23, ����ϳɵ���ȴʱ�䡣
	COOLDOWN_INDEX_PICKUP,				//24, ʰȡ��Ʒ����ȴʱ�䡣

	COOLDOWN_INDEX_SET_FASHION_MASK,	//25, ����ʱװ��ߵ���ȴʱ�䡣
	COOLDOWN_INDEX_GETBATTLEINFO,		//26, ��ȡս����Ϣ����ȴʱ�䡣
	COOLDOWN_INDEX_HEALING_POTION,		//27, ��ͬҩˮ����ȴʱ�䡣
	COOLDOWN_INDEX_MANA_POTION,			//28, ��ͬҩˮ����ȴʱ�䡣
	COOLDOWN_INDEX_REJU_POTION,			//29, ��ͬҩˮ����ȴʱ�䡣

	COOLDOWN_INDEX_STEP_HEALING,		//30, �����ơ������ͻָ�����ȴʱ�䡣
	COOLDOWN_INDEX_STEP_MANA,			//31, �����ơ������ͻָ�����ȴʱ�䡣
	COOLDOWN_INDEX_STEP_REJU,			//32, �����ơ������ͻָ�����ȴʱ�䡣
	COOLDOWN_INDEX_QUERY_OTHER_ACHIEVMENT, //33, ��ѯ������ҳɾ͵���ȴʱ�䡣
	COOLDOWN_INDEX_TRANSFORM,			//34, ���ε���ȴʱ�䡣

	COOLDOWN_INDEX_NEWBIE_CHAT,		//35, �����������ȴʱ�䡣
	COOLDOWN_INDEX_CHANGE_DS,		//36, ����DS����ȴʱ�䡣
	COOLDOWN_INDEX_DEITY_POTION,	//37, ��ҩ����ȴʱ�䡣
	COOLDOWN_INDEX_STEP_DEITY,		//38, ��ҩ����ȴʱ�䡣
	COOLDOWM_INDEX_GET_MALL_PRICE,	//39, ��ȡ�̳Ǽ۸����ȴʱ�䡣

	COOLDOWM_INDEX_GET_VIP_AWARD_INFO,			//40, ��ȡVIP������Ϣ����ȴʱ�䡣
	COOLDOWN_INDEX_RESET_SKILL_PROP_SKILL,		//41, ���ü������Ե���ȴʱ�䡣
	COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT,		//42, ���ü������Ե���ȴʱ�䡣
	COOLDOWN_INDEX_BATTLE_FLAG,					//43, ս����־����ȴʱ�䡣
	COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON,	//44, ����״��˳�ԭ�����
	
	COOLDOWN_INDEX_BOTTLE_HP,		//45, ����ҩˮ����ȴʱ�䡣
	COOLDOWN_INDEX_BOTTLE_MP,		//46, ħ��ҩˮ����ȴʱ�䡣
	COOLDOWN_INDEX_BOTTLE_DEITY,	//47, ����ҩˮ����ȴʱ�䡣
	COOLDOWN_INDEX_SKILL_ITEM_4,	//48, �����ּ�����Ʒ����ȴʱ�䡣
	COOLDOWN_INDEX_SKILL_ITEM_5, 	//49, �����ּ�����Ʒ����ȴʱ�䡣

	COOLDOWN_INDEX_BATH,				//50, ϴ�����ȴʱ�䡣
	COOLDOWN_INDEX_KING_CALL_GUARD,		//51, �����ٻ���������ȴʱ�䡣
	COOLDOWN_INDEX_ARRANGE_INVENTORY,	//52, ����������ȴʱ�䡣
	COOLDOWN_INDEX_ARRANGE_TRASHBOX,	//53, �������������ȴʱ�䡣
	COOLDOWN_INDEX_TELEPORT1,			//54, ף�����͵���ȴʱ�䡣

	COOLDOWN_INDEX_TELEPORT2,			//55, ���䴫�͵���ȴʱ�䡣
	COOLDOWN_INDEX_CHANGE_PUPPET_FORM,	//56, ������̬�л�����ȴʱ�䡣
	COOLDOWN_INDEX_CIRCLE_OF_DOOM,		//57, �������ȴʱ�䡣
	COOLDOWN_INDEX_ARRANGE_POCKET,		//58, ����ڴ�����ȴʱ�䡣

	COOLDOWN_INDEX_PRODUCE_BEGIN 	= 600,	//�������ܿ�ʼ����ȴʱ��������
	COOLDOWN_INDEX_PRODUCE_END 	= 800,		//�������ܽ�������ȴʱ��������

	//��ϼ��༭
	COOLDOWN_INDEX_COMBINE_BEGIN	= 900,	//��ϼ��ܿ�ʼ����ȴʱ��������
	COOLDOWN_INDEX_COMBINE_END	= 950,		//��ϼ��ܽ�������ȴʱ��������

	COOLDOWN_SPEC_ERROR1 = 1023,			//�������1����ȴʱ�䡣
	COOLDOWN_SKILL_START = 1024,			//���ܿ�ʼ����ȴʱ��������
	COOLDOWN_INDEX_CHEATER = 997,			//�����ߵ���ȴʱ�䡣
	COOLDOWN_INDEX_ANTI_CHEAT = 998,		//�����׵���ȴʱ�䡣
};

#endif

