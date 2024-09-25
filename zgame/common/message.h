#ifndef __ONLINEGAME_COMMON_MESSAGE_H__
#define __ONLINEGAME_COMMON_MESSAGE_H__

#include <stdlib.h>
#include <amemory.h>
#include <ASSERT.h>
#include "types.h"
struct MSG 
{
	int 	message;	//��Ϣ������
	struct XID target;	//����Ϣ��Ŀ�꣬�����Ƿ���������ң���Ʒ��NPC��
	struct XID source;	//�����﷢�����ģ����ܵ�id������һ��
	A3DVECTOR pos;		//��Ϣ����ʱ��λ�ã��е���Ϣ����λ�ò�������
	int	param;		//һ���������������������ã���ô��ʹ���������
	int 	param2;		//�ڶ���������Ϊ����������Ҫ�������
	size_t 	content_length;	//��Ϣ�ľ������ݳ���
	const void * content;	//��Ϣ�ľ������� �����ϴ���ʱ����ֶ���Ч
private:
	enum {FAST_ALLOC_LEN = 128};
	friend void * SerializeMessage(const MSG &);
	friend void FreeMessage(MSG *);
};

inline void * SerializeMessage(const MSG & msg)
{
	void * buf;
	size_t length = msg.content_length;
	if(length <= MSG::FAST_ALLOC_LEN)
	{
	//	printf("%d %dalloced\n",sizeof(MSG) + length,msg.message);
		buf = abase::fast_allocator::align_alloc(sizeof(MSG) + length);		//������룬���Ƕ��msg
		memcpy(buf,&msg,sizeof(MSG));
		if(length)
		{
			memcpy((char*)buf + sizeof(MSG),msg.content,length);
		}
	}
	else
	{
		buf = abase::fast_allocator::raw_alloc(sizeof(MSG) + length);
		memcpy(buf,&msg,sizeof(MSG));
		memcpy((char*)buf + sizeof(MSG),msg.content,msg.content_length);
	}
	return buf;
}

inline MSG * DupeMessage(const MSG & msg)
{
	MSG * pMsg = (MSG*)SerializeMessage(msg);
	pMsg->content = ((char*)pMsg) + sizeof(MSG);
	return pMsg;
}

inline void FreeMessage(MSG * pMsg)
{
	ASSERT(pMsg->content == ((char*)pMsg) + sizeof(MSG));
	size_t length = pMsg->content_length;
	if(length <= MSG::FAST_ALLOC_LEN)
	{
		abase::fast_allocator::align_free(pMsg, sizeof(MSG) + length);
	}
	else
	{
		abase::fast_allocator::raw_free(pMsg);
	}
}
inline void BuildMessage(MSG & msg, int message, const XID &target, const XID & source,
			const A3DVECTOR & pos,int param = 0,
			const void * content = NULL,size_t content_length = 0)
{
	msg.message = message;
	msg.target = target;
	msg.source = source;
	msg.pos = pos;
	msg.param = param;
	msg.param2 = 0;
	msg.content_length = content_length;
	msg.content = content;
}

inline void BuildMessage2(MSG & msg, int message, const XID &target, const XID & source,
			const A3DVECTOR & pos,int param = 0,int param2 = 0,
			const void * content = NULL,size_t content_length = 0)
{
	msg.message = message;
	msg.target = target;
	msg.source = source;
	msg.pos = pos;
	msg.param = param;
	msg.param2= param2;
	msg.content_length = content_length;
	msg.content = content;
}

enum
{
//	normal message
	GM_MSG_NULL,				//����Ϣ
	GM_MSG_FORWARD_USERBC,			//ת�����û��㲥
	GM_MSG_FORWARD,				//ת������Ϣ������Ӧ����Ϊһ���µ���Ϣ����������
	GM_MSG_FORWARD_BROADCAST,		//ת������Ϣ�㲥��Ϣ,content������һ����������Ϣ
	GM_MSG_USER_GET_INFO,			//�û�ȡ�ñ�Ҫ������

//5
	GM_MSG_IDENTIFICATION,			//��������֪�Լ������,ԭ�����ͱ�����server����id�����ķ���
	GM_MSG_SWITCH_GET,			//ȡ���û�����,�������л���ȡ���û����� param�� tag,content��key
	GM_MSG_SWITCH_START,			//�û�֪ͨ�������Լ���ʼת��
	GM_MSG_SWITCH_NPC,			//NPC�л�������
	GM_MSG_USER_MOVE_OUTSIDE,		//�û��ڱ߽��ƶ�

//10	
	GM_MSG_USER_NPC_OUTSIDE,		//NPC�ڱ߽紦�ƶ�����֮ͬ������NPC����Ҫȡ���¿�������Ķ���
	GM_MSG_ENTER_WORLD,			//��controller�ģ���ʾ�û��Ѿ�����������
	GM_MSG_ATTACK,				//Ŀ���Դ�������Ǹ���
	GM_MSG_SKILL,				//Ŀ���Դ�������Ǹ���
	GM_MSG_PICKUP,				//������Ʒ,Ŀ��һ������Ʒ content��msg_pickup_t param���Ƿ���������

//15
	GM_MSG_FORCE_PICKUP,			//ǿ�Ƽ�����Ʒ����У��������ID�����ID
	GM_MSG_PICKUP_MONEY,			//��Ʒ֪ͨ�û���Ǯ param��Ǯ��  content��˭������
	GM_MSG_PICKUP_TEAM_ITEM,		//��Ʒ֪ͨ��Ӽ���Ʒ  param �Ǹ���Ʒ��world_tag
	GM_MSG_RECEIVE_MONEY,			//֪ͨ��ҵõ����ʱ�������Ľ�Ǯ ��Ҫ���ǽ�Ǯ�ӳ�
	GM_MSG_PICKUP_ITEM,			//��Ʒ֪ͨ�û�����Ʒ param�� palyer_id | 0x80000000(�����ӣ�

//20
	GM_MSG_ERROR_MESSAGE,			//��player����һ��error message
	GM_MSG_NPC_SVR_UPDATE,			//NPC�����˷������л��������Ϣֻ������������״̬��ԭ��NPC
	GM_MSG_EXT_NPC_DEAD,			//�ⲿ��NPC��������Ϣ(����ɾ��)�������Ϣֻ������������״̬��ԭ��NPC
	GM_MSG_EXT_NPC_HEARTBEAT,		//�ⲿNPC�������������ж��Ƿ�ʱ 
	GM_MSG_WATCHING_YOU,			//�������Ｄ�����Ϣ,����һ�npc������������һ��watching_t�Ľṹ

//25
//	AGGRO  message 
	GM_MSG_GEN_AGGRO,			//����aggro�����渽����һ��aggro_info_t�Ľṹ
	GM_MSG_TRANSFER_AGGRO,			//aggro�Ĵ��� Ŀǰֻ���͵�һλ content��һ��XID,�����XID��idΪ-1    ����ճ���б� param�Ǹ��˳��ֵ
	GM_MSG_AGGRO_ALARM,			//aggro���������ܵ�����ʱ�ᷢ�ͣ����渽����һ��aggro_alarm_tδʹ��
	GM_MSG_AGGRO_WAKEUP,			//aggro�����������ߵĹ��ﾪ��,���渽����һ��aggro_alarm_tδʹ��
	GM_MSG_AGGRO_TEST,			//aggro����,ֻ�е���������aggro�б��У��Ż������µ�aggro�����渽����һ��aggro_info_tδʹ��
	
//30
	GM_MSG_OBJ_SESSION_END,			//�����session���
	GM_MSG_OBJ_SESSION_REPEAT,		//��ʾsessionҪ����ִ�� 
	GM_MSG_OBJ_ZOMBIE_END,			//��ʾҪ������ʬ״̬
	GM_MSG_EXPERIENCE,			//�õ�����ֵ	content ��һ��msg_exp_t
	GM_MSG_GROUP_EXPERIENCE,		//�õ���Ӿ���ֵ conennt �Ƕ��msg_grp_exp_t , param ��ɵ����˺�
	
//35
	GM_MSG_TEAM_EXPERIENCE,			//�õ���Ӿ���ֵ conennt ��msg_exp_t �������뾭��ֵ�ᱻ���� param ��ɱ����npcid ��Ϊ0���Ǳ�����ɱ����
	GM_MSG_QUERY_OBJ_INFO00,		//ȡ�ö����info00 param�Ƿ����ߵ�sid ,content��һ��int����cs_index
	GM_MSG_HEARTBEAT,			//�����Լ���������Ϣ  ���������Heartbeat������
	GM_MSG_HATE_YOU,
	GM_MSG_TEAM_INVITE,			//����ĳ�˼������param��teamseq, content��һ��int ��ʾpickup_flag param2��faction

//40	
	GM_MSG_TEAM_AGREE_INVITE,		//��������ͬ�������� content��һ��int(��ʾְҵ)+ team_mutable_prop
	GM_MSG_TEAM_REJECT_INVITE,		//�ܾ���������
	GM_MSG_JOIN_TEAM,			//�ӳ�ͬ��ĳ�˼������ param��λ�Ǽ�ȡ��ʽ param��λ�Ƕ�Ա������content��member_entry�ı� 
	GM_MSG_JOIN_TEAM_FAILED,		//�����޷�������飬Ӧ�ôӶ�����ȥ��
	GM_MSG_MEMBER_NOTIFY_DATA,		//��ӳ�Ա֪ͨ�������Լ��Ļ�����Ϣ content ��һ��team_mutable_prop

//45	
	GM_MSG_NEW_MEMBER,			//leader֪ͨ�³�Ա���룬content��һ��member_entry list param������
	GM_MSG_LEAVE_PARTY_REQUEST,
	GM_MSG_LEADER_CANCEL_PARTY,
	GM_MSG_MEMBER_NOT_IN_TEAM,
	GM_MSG_LEADER_KICK_MEMBER,

//50	
	GM_MSG_MEMBER_LEAVE,
	GM_MSG_LEADER_UPDATE_MEMBER,
	GM_MSG_GET_MEMBER_POS,			//Ҫ����ѷ���λ�� param�Ƿ����ߵ�sid ,content��һ��int����cs_index
	GM_MSG_QUERY_PLAYER_EQUIPMENT,		//ȡ���ض���ҵ����ݣ�Ҫ��ƽ�������һ����Χ֮��param�Ƿ����ߵ�sid ,content��һ��int����cs_index
	GM_MSG_TEAM_PICKUP_NOTIFY,		//���ѷ��䵽��Ʒ֪ͨ�� param �� type, content ��count

//55	
	GM_MSG_TEAM_CHAT___,			//������� param ��channel, content ������ �Ѿ����ϣ�����ֱ�ӷ���
	GM_MSG_SERVICE_REQUEST,			//playerҪ��������Ϣ param �Ƿ������� content �Ǿ������� �
	GM_MSG_SERVICE_DATA,			//��������ݵ��� param �Ƿ�������  content �� ��������
	GM_MSG_SERVICE_HELLO,			//player ��������ʺ�  param �� player�Լ���faction
	GM_MSG_SERVICE_GREETING,		//�����̽��лػ� ������Ҫ�����淵�ط����б�$$$$(����δ��)

//60	
	GM_MSG_SERVICE_QUIERY_CONTENT,		//ȡ�÷������� 	 param �Ƿ�������, content�ɿ���pair<cs_index,sid>
	GM_MSG_EXTERN_OBJECT_APPEAR_N,		//content ��extern_object_manager::object_appear [����]
	GM_MSG_EXTERN_OBJECT_DISAPPEAR_N,	//��ʧ����    [����]
	GM_MSG_EXTERN_OBJECT_REFRESH_N,		//����λ�ú�Ѫֵ��param�б������Ѫֵ  [����]
	GM_MSG_USER_APPEAR_OUTSIDE,		//�û���������֣�Ҫ���ͱ�Ҫ�����ݸ�����ң�content ����sid,param��linkd id

//65
	GM_MSG_FORWARD_BROADCAST_SPHERE_N,	//ת������Ϣ�㲥��Ϣ,content������һ����������Ϣ [����]
	GM_MSG_FORWARD_BROADCAST_CYLINDER_N,	//ת������Ϣ�㲥��Ϣ,content������һ����������Ϣ [����]
	GM_MSG_PRODUCE_MONSTER_DROP,		//֪ͨϵͳ�������������Ʒ�ͽ�Ǯ�� ����Դ�������ߣ�param��money�� content �� struct { int team_id; int team_seq;int npc_id;int item_count; int item[];}
	GM_MSG_ENCHANT,				//ʹ�ø���ħ��
	GM_MSG_ENCHANT_ZOMBIE,			//ʹ�ø���ħ��,ר�Ÿ������õ�

//70
	GM_MSG_OBJ_SESSION_REP_FORCE,		//��ʾsessionҪrepeat �����漴ʹ������ҲҪ����ִ��
	GM_MSG_NPC_BE_KILLED,			//��Ϣ����ɱ��npc����ң�param ��ʾ��ɱ��npc������ content��NPC�ļ���
	GM_MSG_NPC_CRY_FOR_HELP,		//npc ������Ȳ���
	GM_MSG_PLAYER_TASK_TRANSFER,		//������player֮����д��ͺ�ͨѶ�ĺ���
	GM_MSG_PLAYER_BECOME_INVADER,		//��Ϊ���� msg.param �����ӵ�ʱ��

//75
	GM_MSG_PLAYER_KILL_PLAYER,		//ɱ����ҵı�־ msg.param����Է���PKֵ
	GM_MSG_FORWARD_CHAT_MSG,		//ת�����û�������Ϣ,param��rlevel,source��XID(-channel,self_id)
	GM_MSG_QUERY_SELECT_TARGET,		//ȡ�ö���ѡ��Ķ���
	GM_MSG_NOTIFY_SELECT_TARGET,		//ȡ�ö���ѡ��Ķ���
	GM_MSG_SUBSCIBE_TARGET,			//Ҫ����һ������

//80
	GM_MSG_UNSUBSCIBE_TARGET,		//Ҫ����һ������
	GM_MSG_SUBSCIBE_CONFIRM,		//ȷ�϶����Ƿ����
	GM_MSG_MONSTER_MONEY,			//֪ͨ����յ��������Ľ�Ǯ	param�ǽ�Ǯ ��Ҫ���ǽ�Ǯ�ӳ�
	GM_MSG_MONSTER_GROUP_MONEY,		//֪ͨ����յ������Ǯ  param�ǽ�Ǯ 
	GM_MSG_GATHER_REQUEST,			//�����ռ�ԭ�ϣ�  param ����ҵ�faction, content �ֱ�����Ҽ��𡢲ɼ����ߺ�����ID

//85
	GM_MSG_GATHER_REPLY,			//֪ͨ���Խ��вɼ�  param �ǲɼ���Ҫ��ʱ��
	GM_MSG_GATHER_CANCEL,			//ȡ���ɼ�
	GM_MSG_GATHER,				//���вɼ���Ҫ��ȡ����Ʒ
	GM_MSG_GATHER_RESULT,			//�ɼ���ɣ�param ���ǲɼ�������Ʒid, content������ �Ϳ��ܸ��ӵ�����ID
	GM_MSG_EXTERN_HEAL,			//��ĳĳ�����Ѫ����Ϣ

//90
	GM_MSG_INSTANCE_SWITCH_GET,		//ȡ���û�����,�������л���ȡ���û����� ���ڸ�������л� param��key
	GM_MSG_INSTANCE_SWITCH_USER_DATA,	//�û�����,SWITCH_SWITCH_GET�Ļ�Ӧ
	GM_MSG_EXT_AGGRO_FORWARD,		//֪ͨԭ��npc���г��ת�� param ��rage��С�� content�ǲ�����޵�id
	GM_MSG_TEAM_APPLY_PARTY,		//����������ѡ��      param2��faction
	GM_MSG_TEAM_APPLY_REPLY,		//����ɹ��ظ� ���е�param��seq	

//95
	GM_MSG_QUERY_INFO_1,			//��ѯINFO1�����Է�����һ���NPC,param��������cs_index,content��sid
	GM_MSG_CON_EMOTE_REQUEST_NULL,		//����Эͬ���������� param �� action      �Ѿ�����
	GM_MSG_CON_EMOTE_REPLY_NULL,		//����Эͬ�����Ļ�Ӧ param ��action��ͬ�����������ֽڵ� �Ѿ�����
	GM_MSG_TEAM_CHANGE_TO_LEADER,		//֪ͨ����Ҫ��Ϊleader
	GM_MSG_TEAM_LEADER_CHANGED,		//֪ͨ���Ѷӳ��ĸı�

//100
	GM_MSG_OBJ_ZOMBIE_SESSION_END,		//���������session�Ĳ��������������������session����һ��
	GM_MSG_QUERY_MARKET_NAME,		//ȡ�ð�̯�����֣�param�Ƿ����ߵ�sid ,content��һ��int����cs_index
	GM_MSG_HURT,				//��������˺� content��attacker_info_1, param1��damage, param2����˹���ģʽ���Ƿ�Ƿ�����
	GM_MSG_DEATH,				//ǿ���ö�������
	GM_MSG_PLANE_SWITCH_REQUEST,		//����ʼ���ͣ�content��key��������д��ͣ��򷵻� SWITCH_REPLAY

//105
	GM_MSG_PLANE_SWITCH_REPLY,		//��������ȷ�ϣ�content��key
	GM_MSG_SCROLL_RESURRECT,		//���Ḵ��  param��ʾ�������Ƿ�����pvpģʽ1��ʾ������
	GM_MSG_LEAVE_COSMETIC_MODE,		//��������״̬
	GM_MSG_DBSAVE_ERROR,			//���ݿⱣ�����
	GM_MSG_SPAWN_DISAPPEAR,			//֪ͨNPC����Ʒ��ʧ param��condition

//110
	GM_MSG_PET_CTRL_CMD,			//��ҷ����Ŀ�����Ϣ���������Ϣ��������
	GM_MSG_ENABLE_PVP_DURATION,		//����PVP״̬
	GM_MSG_PLAYER_KILLED_BY_NPC,		//��ұ�NPCɱ����NPC���߼�
	GM_MSG_PLAYER_DUEL_REQUEST,             //��ҷ���Ҫ��duel������
	GM_MSG_PLAYER_DUEL_REPLY,               //��һ�Ӧduel������param���Ƿ��Ӧduel

//115
	GM_MSG_PLAYER_DUEL_PREPARE,      	//����׼����ʼ 3�뵹��ʱ��ʼ
	GM_MSG_PLAYER_DUEL_START,               //������ʼ 
	GM_MSG_PLAYER_DUEL_CANCEL,		//ֹͣ����
	GM_MSG_PLAYER_DUEL_STOP,		//��������
	GM_MSG_DUEL_HURT,			//PVP��������˺�content ������

//120
	GM_MSG_PLAYER_BIND_REQUEST,		//�������ڱ�������
	GM_MSG_PLAYER_BIND_INVITE,		//������������Լ�����
	GM_MSG_PLAYER_BIND_REQ_REPLY,		//������Ļ�Ӧ
	GM_MSG_PLAYER_BIND_INV_REPLY,		//������Ļ�Ӧ
	GM_MSG_PLAYER_BIND_PREPARE,		//׼����ʼ����

//125
	GM_MSG_PLAYER_BIND_LINK,		//���ӿ�ʼ
	GM_MSG_PLAYER_BIND_STOP,		//ֹͣ����
	GM_MSG_PLAYER_BIND_FOLLOW,		//Ҫ����Ҹ���
	GM_MSG_QUERY_EQUIP_DETAIL,		//param Ϊfaction, content Ϊcs_index ��cs_sid
	GM_MSG_PLAYER_RECALL_PET,		//�����ǿ�������ٻ�״̬

//130
	GM_MSG_CREATE_BATTLEGROUND,		//Ҫ��ս������������һ��ս������Ϣ����Ҫ���ڲ���
	GM_MSG_BECOME_TURRET_MASTER,		//��Ϊ���ǳ���master,param��tid, content ��faction
	GM_MSG_REMOVE_ITEM,			//ɾ��һ����Ʒ����Ϣ�����ڹ��ǳ����ƺ����Ʒ���� param��tid
	GM_MSG_NPC_TRANSFORM,			//NPC����Ч����content�ﱣ�� �м�״̬���м�ʱ�� �м��־ ���״̬
	GM_MSG_NPC_TRANSFORM2,			//NPC����Ч��2��param ��Ŀ��ID ��������ͺ�Ŀ��IDһ���ˣ���ô�Ͳ�������

//135
	GM_MSG_TURRET_NOTIFY_LEADER,		//���ǳ�֪ͨleader�Լ����ڣ������޷��ٴν����ٻ�
	GM_MSG_PET_RELOCATE_POS,		//����Ҫ�����¶�λ����
	GM_MSG_PET_CHANGE_POS,			//�����޸��˳��������
	GM_MSG_PET_DISAPPEAR,			//���ݲ���ȷ,�����������,����Ҫ�������ʧ
	GM_MSG_PET_NOTIFY_HP_VP,		//����֪ͨ����,��֪�Լ���Ѫ�� ����

//140
	GM_MSG_PET_NOTIFY_DEATH,		//����֪ͨ�����Լ�������
	GM_MSG_MASTER_INFO,			//����֪ͨ�����Լ�������
	GM_MSG_PET_LEVEL_UP,			//����֪ͨ���������� ,content�� level
	GM_MSG_PET_HONOR_MODIFY,		//����֪ͨ������ҳ϶ȷ����仯
	GM_MSG_MASTER_ASK_HELP,			//����Ҫ��������
	
//145
	GM_MSG_REPU_CHG_STEP_1,			//����ת����1�� ��ɱ�ߴ����Լ��ĵ�ǰ����
	GM_MSG_REPU_CHG_STEP_2,			//����ת����2�� ɱ���ߴ��ؽ��  ��ɱ���޸��Լ�������
	GM_MSG_REPU_CHG_STEP_3,			//����ת����3�� ��ɱ�߼��������� ֪ͨɱ�����޸�����
	GM_MSG_TEAM_MEMBER_LVLUP,		//�ӳ��յ����͵ȼ���Ա����ʱΪ���ӳ�����������Э�� paramΪ����ǰ����
	GM_MSG_KILL_PLAYER_IN_BATTLEGROUND,	// �������ս��ɱ���з����ʱ�����յ����߷��������Ϣ

//150
	GM_MSG_MODIFIY_BATTLE_DEATH,		// �����Ҫ�޸��Լ�����������Ϣ�����͸�world
	GM_MSG_MODIFIY_BATTLE_KILL,		// �����Ҫ�޸��Լ���ɱ������Ϣ�����͸�world
	GM_MSG_PET_SET_COOLDOWN,		//���＼����ȴ
	GM_MSG_PET_SET_AUTO_SKILL,		//���＼���Զ�ʩ��
	GM_MSG_FEED_PET,			//���˸������ҩ

//155
	GM_MSG_PET_CAST_SKILL,			//����ʹ�ü���
	GM_MSG_PET_HONOR_LEVEL_CHANGED,		//��������ܶȼ������
	GM_MSG_PET_HUNGER_LEVEL_CHANGED,	//����ı�ʳ�ȼ������
	GM_MSG_MASTER_START_ATTACK,			//���˹���
	GM_MSG_PET_INFO_CHANGED,		//������Ϣ�仯

//160
	GM_MSG_NPC_DISAPPEAR,			//NPC��ʧ
	GM_MSG_TASK_AWARD_TRANSFOR,
	GM_MSG_PLAYER_KILL_PET,			//ɱ����ҳ���
	GM_MSG_PLAYER_LINK_RIDE_INVITE,
	GM_MSG_PLAYER_LINK_RIDE_INV_REPLY,

//165
	GM_MSG_PLAYER_LINK_RIDE_START,
	GM_MSG_PLAYER_LINK_RIDE_STOP,
	GM_MSG_PLAYER_LINK_RIDE_MEMBER_JOIN,
	GM_MSG_PLAYER_LINK_RIDE_FOLLOW,
	GM_MSG_PLAYER_LINK_RIDE_LEAVE_REQUEST,

//170
	GM_MSG_PLAYER_LINK_RIDE_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_RIDE_LEADER_LEAVE,
	GM_MSG_PLAYER_LINK_RIDE_KICK_MEMBER,
	GM_MSG_KILL_MONSTER_IN_BATTLEGROUND,	//ս��������ﱻ���ɱ��
	GM_MSG_PLAYER_CATCH_PET,		//���ץ����

//175
	GM_MSG_NPC_BE_CATCHED,			//���ﱻץ��
	GM_MSG_NPC_BE_CATCHED_CONFIRM,
	GM_MSG_SYNC_BATTLE_INFO,
	GM_MSG_QUERY_ACHIEVEMENT,		//��ѯ������ҳɾ��б�
	GM_MSG_BATTLE_INFO_CHANGE,		//ս����Ϣ�仯

//180
	GM_MSG_PLAYER_KILL_SUMMON,		//ɱ������ٻ���
	GM_MSG_SUMMON_NOTIFY_DEATH,		//����֪ͨ�����Լ�������
	GM_MSG_SUMMON_RELOCATE_POS,
	GM_MSG_SUMMON_CHANGE_POS,
	GM_MSG_EXTERN_ADD_MANA,			//֪ͨĿ�����

//185
	GM_MSG_PLAYER_ENTER_CARRIER,		//��������ϴ�
	GM_MSG_PLAYER_LEAVE_CARRIER,		//��������´�
	GM_MSG_ENTER_CARRIER,			//֪ͨ����ϴ�
	GM_MSG_LEAVE_CARRIER,			//֪ͨ����´�
	GM_MSG_CARRIER_SYNC_POS,		//���ƶ�ʱͬ���Լ���λ�úͷ�������ϵĶ���

//190	
	GM_MSG_BATTLE_NPC_DISAPPEAR,		//ս��NPC��ʧ,����ս������
	GM_MSG_SUMMON_HEARTBEAT,		//�ٻ��޸���ҷ���������Ϣ
	GM_MSG_NPC_BORN,			//���ﴴ�������ڲ��� 
	GM_MSG_MONSTER_TRANSFORM2,		//�Թ���ʹ��������,ԭ��Ϊ��
	GM_MSG_MINE_TRANSFORM2,			//�Թ���ʹ��������,ԭ��Ϊ����

//195
	GM_MSG_USE_COMBO_SKILL,			//������
	GM_MSG_TRY_CLEAR_AGGRO,			//���������
	GM_MSG_DEC_HP,				//�۳��Է���HP, ����ӳ��
	GM_MSG_EXCHANGE_STATUS,			//����hp,mp
	GM_MSG_EXCHANGE_POS,			//����λ��

//200
	GM_MSG_HEAL_CLONE,			//�������hp
	GM_MSG_MANA_CLONE,			//�������mp
	GM_MSG_EXCHANGE_SUBSCIBE,		//���������б��
	GM_MSG_MINE_DISAPPEAR,			//������п���
	GM_MSG_QUERY_CLONE_EQUIPMENT,		//��ѯ�������˵�װ����Ϣ

//205
	GM_MSG_PROTECTED_NPC_NOTIFY,		//֪ͨ������NPC״̬
	GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER,	//ͬ��ս����Ϣ�����
	GM_MSG_MASTER_DUEL_STOP,		//֪ͨ������ٻ������˾�������
	GM_MSG_TRANSFER_ATTACK,			//ת�ƹ�����Ϣ
	GM_MSG_ROUND_START_IN_BATTLE,   //ս����һС�ֵĿ�ʼ

//210
	GM_MSG_ROUND_END_IN_BATTLE,   //ս����һС�ֵĽ���
	GM_MSG_ATTACK_FEED_BACK,	  //�򹥻��߷��͹����������, paramΪmask��param2Ϊ�������ߵȼ�(FEEDBACK_KILL),  ���˺�����(FEEDBACK_DAMAGE)
	GM_MSG_CIRCLE_OF_DOOM_PREPARE,  //�󷨼���ʹ�ã���ʼ�ռ�����
	GM_MSG_CIRCLE_OF_DOOM_STARTUP,  //�󷨽���ɹ�������Ϣ�����ڵĳ�Ա
	GM_MSG_CIRCLE_OF_DOOM_STOP,     //����ֹ������

//215
	GM_MSG_CIRCLE_OF_DOOM_ENTER,    //��Աͬ�������
	GM_MSG_CIRCLE_OF_DOOM_LEAVE,    //��Ա�뿪��
	GM_MSG_CIRCLE_OF_DOOM_QUERY,    //��ѯ����Ϣ	
	GM_MSG_REMOVE_PERMIT_CYCLE_AREA,//���������Ϣ����ʩ�ӵ��ƶ���������
	GM_MSG_CANCEL_BE_PULLED,		//���������״̬

//220
	GM_MSG_CANCEL_BE_CYCLE,         //�����Ч��
	GM_MSG_QUERY_BE_SPIRIT_DRAGGED,	//��ѯ��ǣ����Ϣ
	GM_MSG_QUERY_BE_PULLED,			//��ѯ��������Ϣ
	GM_MSG_TASK_SHARE_NPC_BE_KILLED,//��Ϣ����ɱ��npc����ң�param ��ʾ��ɱ��npc������ content��NPC�ļ���
	GM_MSG_TASK_CHECK_STATE,	//��̯���߽��׽��������Ƿ��б���ס������

//225
	GM_MSG_SUMMON_CAST_SKILL,		//�ٻ���ʹ�ü���
	GM_MSG_SPIRIT_SESSION_END,		//��ԯ��ʹ�õ�session����
	GM_MSG_SPIRIT_SESSION_REPEAT,	//��ԯ��ʹ�õ�session�ظ�
	GM_MSG_SPIRIT_SESSION_REP_FORCE,//��ԯ��ʹ�õı�ʾsessionҪrepeat �����漴ʹ������ҲҪ����ִ��
	GM_MSG_TALISMAN_SKILL,			//�������к�֪ͨʩ�����ͷŷ�������

//230	
	GM_MSG_KINGDOM_BATTLE_HALF,	//����ս�볡����
	GM_MSG_KINGDOM_UPDATE_KEY_NPC,	//����ս����ս��ɱ��NPC
	GM_MSG_REQUEST_BUFFAREA_BUFF,	//����BUFF����BUFF
	GM_MSG_KINGDOM_BATTLE_END,
	GM_MSG_LEVEL_RAID_INFO_CHANGE,	//level�ั���и�����Ϣ�����ı�

//235
	GM_MSG_LEVEL_RAID_START,
	GM_MSG_LEVEL_RAID_END,
	GM_MSG_KINGDOM_SYNC_KEY_NPC,
	GM_MSG_KINGDOM_CHANGE_KEY_NPC,
	GM_MSG_KINGDOM_QUERY_KEY_NPC,

//240
	GM_MSG_KINGDOM_KEY_NPC_INFO,
	GM_MSG_KINGDOM_CHANGE_NPC_FACTION,
	GM_MSG_KINGDOM_CALL_GUARD,
	GM_MSG_KILL_PLAYER_IN_CSFLOW,
	GM_MSG_SYNC_CSFLOW_PLAYER_INFO,

//245
	GM_MSG_PLAYER_LINK_BATH_INVITE,
	GM_MSG_PLAYER_LINK_BATH_INV_REPLY,
	GM_MSG_PLAYER_LINK_BATH_START,
	GM_MSG_PLAYER_LINK_BATH_STOP,
	GM_MSG_PLAYER_LINK_BATH_LEAVE_REQUEST,


//250
	GM_MSG_PLAYER_LINK_BATH_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_BATH_LEADER_LEAVE,
	GM_MSG_FORBID_NPC,
	GM_MSG_FLOW_TEAM_SCORE,
	GM_MSG_NPC_TELEPORT_INFO,

//255
	GM_MSG_SUMMON_TELEPORT_REQUEST,
	GM_MSG_SUMMON_TELEPORT_REPLY,
	GM_MSG_SUMMON_TELEPORT_SYNC,
	GM_MSG_SUMMON_TRY_TELEPORT,
	GM_MSG_SUMMON_AGREE_TELEPORT,

//260
	GM_MSG_MOB_ACTIVE_START,
	GM_MSG_MOB_ACTIVE_STATE_START,
	GM_MSG_MOB_ACTIVE_STATE_FINISH,
	GM_MSG_MOB_ACTIVE_STATE_CANCEL,
	GM_MSG_MOB_ACTIVE_TELL_POS,

//265
	GM_MSG_MOB_ACTIVE_COUNTER_SUC,
	GM_MSG_MOB_ACTIVE_SYNC_POS,
	GM_MSG_PLAYER_LINK_QILIN_INVITE,
	GM_MSG_PLAYER_LINK_QILIN_INV_REPLY,
	GM_MSG_PLAYER_LINK_QILIN_START,

//270
	GM_MSG_PLAYER_LINK_QILIN_STOP,
	GM_MSG_PLAYER_LINK_QILIN_FOLLOW,
	GM_MSG_PLAYER_LINK_QILIN_LEAVE_REQUEST,
	GM_MSG_PLAYER_LINK_QILIN_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_QILIN_LEADER_LEAVE,

//275
	GM_MSG_FAC_BUILDING_COMPLETE,
	GM_MSG_PLAYER_ACTIVE_EMOTE_INVITE,
	GM_MSG_PLAYER_ACTIVE_EMOTE_INV_REPLY,
	GM_MSG_PLAYER_ACTIVE_EMOTE_STOP,
	GM_MSG_PLAYER_ACTIVE_EMOTE_LINK,

//280
	GM_MSG_GET_RAID_TRANSFORM_TASK, //��������
	GM_MSG_PET_SAVE_COOLDOWN,
	GM_MSG_MOB_ACTIVE_START_MOVE,
	GM_MSG_MOB_ACTIVE_STOP_MOVE,
	GM_MSG_GET_STEP_RAID_TASK,

//285	
	GM_MSG_MOB_ACTIVE_RECLAIM,
	GM_MSG_MOB_ACTIVE_PATH_END,
	GM_MSG_MASTER_GET_PET_PROP,
	GM_MSG_CS6V6_CHEAT_INFO,
	GM_MSG_CONTROL_TRAP,

//290	
	GM_MSG_SEEK_PREPARE,
	GM_MSG_SEEK_START,
	GM_MSG_SEEK_STOP,
	GM_MSG_SEEK_TRANSFORM,
	GM_MSG_SEEK_RAID_KILL,

//295	
	GM_MSG_SEEK_RAID_SKILL_LEFT,
	GM_MSG_GET_CAPTURE_MONSTER_AWARD,
	GM_MSG_CLEAN_FLAG_TRANSFORM,
	GM_MSG_CAPTURE_FORBID_MOVE,
	GM_MSG_CAPTURE_ALLOW_MOVE,

//300
	GM_MSG_CAPTURE_SYNC_FLAG_POS,

//GM�����õ���Ϣ	
	GM_MSG_GM_GETPOS=600,			//ȡ��ָ����ҵ����� param �� cs_index, content ��sid
	GM_MSG_GM_MQUERY_MOVE_POS,		//GMҪ���ѯ���� ������һ����ת����Ҵ� 
	GM_MSG_GM_MQUERY_MOVE_POS_RPY,		//GMҪ���ѯ����Ļ�Ӧ,����GM����ת���� content�ǵ�ǰ��instance key
	GM_MSG_GM_RECALL,			//GMҪ�������ת
	GM_MSG_GM_CHANGE_EXP,			//GM����exp ��sp , param �� exp , content ��sp
	GM_MSG_GM_ENDUE_ITEM,			//GM������������Ʒ ��param ��item id, content ����Ŀ 
	GM_MSG_GM_ENDUE_SELL_ITEM,		//GM�������̵���������Ʒ������ͬ��
	GM_MSG_GM_REMOVE_ITEM,			//GMҪ��ɾ��ĳЩ��Ʒ��param ��item id, content ����Ŀ
	GM_MSG_GM_ENDUE_MONEY,			//GM���ӻ��߼��ٽ�Ǯ
	GM_MSG_GM_RESURRECT,			//GMҪ�󸴻�
	GM_MSG_GM_OFFLINE,			//GMҪ������ 
	GM_MSG_GM_DEBUG_COMMAND,		//GMҪ������ 
	GM_MSG_GM_RESET_PP,			//GM����ϴ�����

	GM_MSG_MAX,

};

struct msg_usermove_t	//�û��ƶ����ҿ�Խ�߽����Ϣ
{
	int cs_index;
	int cs_sid;
	int user_id;
	A3DVECTOR newpos;	//��Ϣ������oldpos
	size_t leave_data_size;	//�뿪���͵���Ϣ��С������Ϣ�����ں���)
	size_t enter_data_size;	//�뿪���͵���Ϣ��С������Ϣ�����ں���)
};

struct msg_aggro_info_t
{
	XID source;		//˭��������Щ���
	int aggro;		//��޵Ĵ�С
	int aggro_type;		//��޵�����
	int faction;		//�Է���������ϵ
	int level;		//�Է��ļ���
};

struct msg_watching_t
{
	int faction;		//Դ����ϵ
	int invisible_rate;	//��������
	int family_id;		//����id
	int mafia_id;		//����id
	int zone_id;		//������id
};

struct msg_aggro_list_t
{
	int count;
	struct 
	{
		XID id;
		int aggro;
	}list[1];
};

struct msg_cry_for_help_t
{
	XID attacker;
	int lamb_faction;
	int helper_faction;
};

struct msg_aggro_alarm_t
{
	XID attacker;	//������
	int rage;	
	int faction;	//�����ߵ���ϵ
	int target_faction;	//Ŀ��Ľ����������
};

struct msg_exp_t
{
	int level;
	int64_t exp;
	bool no_exp_punish;
};

struct msg_grp_exp_t
{
	int level;
	int64_t exp;
	float rand;
};

struct msg_grpexp_info
{
	int64_t exp;
	int64_t team_damage;
	int team_seq;
	int npc_level;
	int npc_id;
	int npc_tag;
	float r;
	bool no_exp_punish;
};

struct msg_grpexp_t
{
	XID who;
	int64_t damage;
};

struct gather_reply
{
	int can_be_interrupted;
	int eliminate_tool;	//���Ĺ��ߵ�ID
};

struct gather_result
{
	int amount;
	int task_id;
	int eliminate_tool;		//���ɾ����Ʒ�򸽼Ӵ�ID
};

struct msg_pickup_t
{
	XID who;
	int team_id;
	int team_seq;
};

struct msg_gen_money
{
	int team_id;
	int team_seq;
};

struct msg_npc_transform
{
	int id_in_build;
	int time_use;
	int flag;
	int id_buildup;
	enum 
	{
		FLAG_DOUBLE_DMG_IN_BUILD = 1,
	};
};

struct msg_player_kill_player
{
	bool bpKilled;		//�Ƿ�����ɱ�������ǣ���Ӧ����PKֵ���������������
	bool noDrop;		//�Ƿ񲻵�������(��ս����Ż����) 
	int mafia_id;		//��ɱ���ߵİ��� ���ڰ�սʱ
	int pkvalue;		//��ɱ���ߵ�PKVALUE
	
};

struct msg_task_transfor_award
{
	int gold;
	int64_t exp;
	int reputation;
	int region_rep_idx;
	int region_rep_val;
	int common_item_id;
	int common_item_count;
	bool common_item_bind;
	int common_item_period;
	int task_item_id;
	int task_item_count;
	int title;
public:
	msg_task_transfor_award():	gold(0),exp(0),reputation(0),region_rep_idx(0),region_rep_val(0),
					common_item_id(0),common_item_count(0),common_item_bind(0),common_item_period(0),
					task_item_id(0),task_item_count(0),title(0)
	{}

	void SetGold(int __gold) { gold = __gold;}
	void SetExp(int64_t __exp) { exp = __exp;}
	void SetReputation(int __rep) { reputation = __rep;}
	void SetRegionReputation(int __idx,int __rep) { region_rep_idx = __idx; region_rep_val = __rep;}
	void SetCommonItem(int __id, int __count, bool __bind, int __period) { common_item_id = __id; common_item_count = __count; common_item_bind = __bind; common_item_period = __period;}
	void SetTaskItem(int __id, int __count) { task_item_id = __id; task_item_count = __count;}
	void SetTitle(int __title) { title = __title;}

};

struct msg_catch_pet
{
	int monster_id;
	int monster_level;
	int monster_raceinfo;
	int monster_catchdifficulty;
	float monster_hp_ratio;	
	
};

struct msg_player_enter_carrier
{
	A3DVECTOR rpos;
	unsigned char rdir;
};

struct msg_player_leave_carrier
{
	A3DVECTOR pos;
	unsigned char dir;
};

struct msg_enter_carrier
{
	A3DVECTOR rpos;
	unsigned char rdir;
	A3DVECTOR carrier_pos;
	unsigned char carrier_dir;
};

struct msg_leave_carrier
{
	A3DVECTOR pos;
	unsigned char dir;
	unsigned char reason;	//0 - ������Ҫ  1 - ����
};

struct msg_apply_party
{
	int faction;
	int family_id;
	int mafia_id;
	int level;
	int sectid;
	int referid;
};

struct msg_team_invite
{
	int pickup_flag;
	int family_id;
	int mafia_id;
	int zone_id;
	int level;
	int sectid;
	int referid;
};

struct msg_combo
{
	int combo_type;
	int combo_color;
	int combo_color_num;
};

struct msg_get_clone_equipment
{
	int cs_sid;
	int cs_index;
};

struct msg_transfer_attack
{
	char force_attack;
	int dmg_plus;
	int radius;
	int total;
};

struct msg_summon_cast_skill
{
	XID target;
	int skillid;
	int skilllevel;
};

struct msg_talisman_skill
{	
	int level;
	float range;
	char force;
	int skill_var[16];
	short skillList[8];
};

struct msg_level_raid_info
{
	char level;
	short matter_cnt;
	int start_time;
};

struct msg_kingdom_update_key_npc
{
	int cur_hp;
	int max_hp;
	int cur_op_type;
	int change_hp;
};

struct msg_kingdom_sync_key_npc
{
	int cur_hp;
	int max_hp;
	int hp_add;
	int hp_dec;
};

struct msg_kingdom_key_npc_info
{
	int cur_hp;
	int max_hp;
};


struct msg_kingdom_call_guard
{
	char kingname[20];
	size_t kingname_len;
	int line_id;
	int map_id;	
	A3DVECTOR pos;
};

struct msg_sync_flow_info 
{
	int score;
	int c_kill_cnt;
	int kill_cnt;
	int death_cnt;
	int monster_kill_cnt;
	int max_ckill_cnt;
	int max_mkill_cnt;
	bool battle_result_sent;
};

struct msg_npc_teleport_info
{
	int npc_id;
	A3DVECTOR pos;
	int lifetime;
};

struct msg_fac_building_complete
{
	int index;
	int tid;
	int level;
};

struct msg_master_get_pet_prop
{
	int cs_index;
	int uid;
	int sid;
	int pet_index;
};

struct msg_cs6v6_cheat_info
{
	bool is_cheat;
	int cheat_counter;
};

#endif

