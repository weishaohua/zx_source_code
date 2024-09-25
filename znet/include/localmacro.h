#ifndef __GNET_LOCALMACRO_H
#define __GNET_LOCALMACRO_H

#include "groleinventory"
#include "gshoplog"
#include "gmailsyncdata"
#include "familyid"
#include "hostilefaction"
#include "hostileinfo"
#include "gfactioninfo"

#ifdef USE_LOG2 
#include "logger.hpp"
#endif

#define  _ROLE_STATUS_NORMAL    1
#define  _ROLE_STATUS_MUSTDEL   2
#define  _ROLE_STATUS_READYDEL  3
#define  _ROLE_STATUS_FROZEN	4
#define  _ROLE_STATUS_SELLING	5

#define  ERR_NO_LINE_CHGS       0

namespace GNET
{

// Ѱ�������۽�ɫ������

	
// Ѱ������ɫ�Ǽ���
#define WEBTRADE_ROLE_REGISTER_PERIOD 	3*24*3600
// Ѱ������ɫ����ʱ��
#define WEBTRADE_ROLE_POST_TIME_LIMIT 	6*24*3600
// Ѱ��������ת������
#define WEBTRADE_ROLE_SOLD_REBORN_LIMIT  2
// Ѱ�������۵ȼ�����
#define WEBTRADE_ROLE_SOLD_LEVEL_LIMIT  90
// Ѱ���������ߵȼ�
#define WEBTRADE_ROLE_SOLD_BUYER_LEVEL_LIMIT 90

#define LOGICUID_START    1024
#define LOGICUID_MAX      0x7FFFFFFF 
#define LOGICUID(rid)     (int)((rid) & 0xFFFFFFF0)
#define MAX_NAME_SIZE     40
//#define MAX_ROLENAME_RECORD	10	//ÿ����ɫ��������10�θ�����¼
#define ROLENAME_PAGESIZE 100

#define CRSSVRTEAMS_MAX_ROLES 8 // ���ս���������
//#define CRSSVRTEAMS_APPLAY_ROLES 6 // ���ս�ӱ�������
#define CRSSVRTEAMS_INIT_ROLE_SCORE 1000 // ���ս�ӳ�ʼ����
#define CRSSVRTEAMS_CALC_SCORE_LOW 1500 // С������������㲻�۷�

const static int HIDESEEK_MEMBER_LIMIT = 20; // ׽�Բظ�����������
const static int FENGSHEN_MEMBER_LIMIT = 12; // ���񸱱���������

const static int CRSSVRTEAMS_SEASON_CLOSE_DAY = 26; 
const static int CRSSVRTEAMS_SEASON_OPEN_DAY = 1;
const static int CRSSVRTEAMS_SEASON_REOPEN_DAY = 31;

#define NEW_MAX_CASH_IN_POCKET 	2000000000  //������Ǯ��������
#define NEW_MAX_CASH_IN_STOCK	1000000000	//Ǯׯ���ܴ��Ǯ������

#define MAX_USER_CASH           2000000000
#define CASH_BALANCE_THRESHOLD  10000000
#define STOCK_TAX_RATE    1.02
#define STOCK_BALANCE_ID  1

#define UNIQUENAME_ENGAGED      1
#define UNIQUENAME_USED         2
#define UNIQUENAME_OBSOLETE     3  //��ɫ���� ʹ�ù�������
#define UNIQUENAME_PREALLOCATE	4  //Ԥ�ȷ�������� �����������ȡ

#define SYSMAIL_LIMIT		64   //ϵͳ�ʼ���������
const static int OCCUPATION_PLAYER_LIMIT=2; // 6v6��ÿ��ְҵ���ܳ���2��

enum{
	ALGORITHM_MASK_HIGH    = 0x000F0000,
	ALGORITHM_MASK_OPT     = 0x0000FF00,
	ALGORITHM_OPT_NOCACHE  = 0x00000100,
	ALGORITHM_NONE         = 0x0,
	ALGORITHM_CARD         = 0x00010000,
	ALGORITHM_HANDSET      = 0x00020000,
	ALGORITHM_USBKEY2      = 0x00050000,
	ALGORITHM_PHONETOKEN   = 0x00060000,
	ALGORITHM_SINGLE_SING_ON =  0x00001000,
	ALGORITHM_OPT_STUDIO_USER = 0x00100000,
	ALGORITHM_FILL_INFO_MASK  = 0x000000FF, //������ʾ�뽱����Ϣ
};

enum{
	CASH_GETSERIAL_FAILED = -16,
	CASH_ADD_FAILED       = -17,
	CASH_NOT_ENOUGH       = -18
};

enum{
	ITEM_PROC_TYPE_NOTRADE   = 0x00000010,   //��Ҽ䲻�ܽ���
	ITEM_PROC_TYPE_CD_TRADE  = 0x00000400,	 //(Central Deliveryd)����������Ͽ��Խ���
	ITEM_PROC_TYPE_LOCKED    = 0x80000000,   //����
};
#define MASK_ITEM_NOTRADE  (ITEM_PROC_TYPE_NOTRADE|ITEM_PROC_TYPE_LOCKED)

enum{
	SYNC_STOTEHOUSE  = 0x01,
	SYNC_CASHUSED    = 0x02,
	SYNC_CASHTOTAL   = 0x04,
	SYNC_SHOPLOG     = 0x08,
};
#define TOP_DEITY_NUM 4
enum{
	TOP_PERSONAL_LEVEL = 1,
	TOP_PERSONAL_MONEY = 2,
	TOP_PERSONAL_REPUTATION = 3,
	TOP_PERSONAL_CREDIT_START = 10,

	TOP_FACTION_LEVEL = 31,
	TOP_FACTION_MONEY = 32,
	TOP_FACTION_POPULATION = 33,
	TOP_FACTION_PROSPERITY = 34,
	TOP_FACTION_NIMBUS  = 35,
	TOP_FACTION_ACTIVITY = 36,
	TOP_FACTION_CREDIT_START = 40,

	TOP_FAMILY_TASK_START = 60,

	TOP_PERSONAL_CHARM= 80,
	TOP_FRIEND_CHARM = 81,
	TOP_SECT_CHARM = 82,
	TOP_FACTION_CHARM = 83,

	TOP_PERSONAL_DEITY_LEVEL_START = 90,
	TOP_PERSONAL_DEITY_LEVEL_END = TOP_PERSONAL_DEITY_LEVEL_START+TOP_DEITY_NUM,

	TOP_TOWERRAID_LEVEL = 95,
	TOP_CRSSVRTEAMS_SCORE = 96,
	TOP_PERSONAL_CREDIT_START2 = 200,
	TOP_FACTION_CREDIT_START2 = 300,
	TOP_TABLEID_MAX = TOP_FACTION_CREDIT_START2+100, //Ԥ����100������ id
};

//#define FIRST_TOPTABLE_ID     (TOP_PERSONAL_LEVEL);  liuyue
//#define LAST_TOPTABLE_ID      (TOP_FAMILY_TASK_START+8)
#define LAST_TOPTABLE_ID      (TOP_TABLEID_MAX-1)
#define TOP_ITEM_PER_PAGE     20
//#define REGION_COUNT          15
#define TOPTABLE_COUNT        71
#define WEEKLYTOP_BEGIN       1000

enum ERR_TOP_TABLE{
	TOP_DATE_NOTREADY = 1,
	TOP_INVALID_ID = 2,
};

enum ERR_COMBAT
{
        ERR_COMBAT_MASTEROFFLINE = 1,
        ERR_COMBAT_NOPROSPERITY  = 2,
        ERR_COMBAT_COOLING       = 3,
        ERR_COMBAT_BUSY          = 4,
        ERR_COMBAT_LOWLEVEL      = 5,
	ERR_COMBAT_INBATTLE      = 6,
};

enum ERR_STOCK
{
        ERR_STOCK_CLOSED        = 1, 
        ERR_STOCK_ACCOUNTBUSY   = 2, 
        ERR_STOCK_INVALIDINPUT  = 3,
        ERR_STOCK_OVERFLOW      = 4,
        ERR_STOCK_DATABASE      = 5, 
        ERR_STOCK_NOTENOUGH     = 6, 
        ERR_STOCK_MAXCOMMISSION = 7, 
        ERR_STOCK_NOTFOUND      = 8, 
	ERR_STOCK_CASHLOCKED    = 9,
	ERR_STOCK_CASHUNLOCKFAILED= 10,
};
enum ORDER_RESULT
{
        STOCK_ORDER_DONE        = 0, 
        STOCK_ORDER_CANCEL      = 1, 
        STOCK_ORDER_TIMEOUT     = 2,
};

enum OPER_ENEMY{
	ENEMY_REMOVE = 0,
	ENEMY_FREEZE = 1,
	ENEMY_INSERT = 2
};
enum GET_ENEMY{
	ENEMY_ONLINE   = 0,
	ENEMY_FULLLIST = 1,
	ENEMY_NEW      = 2,
	ENEMY_IDLIST   = 3
};
enum
{
	MSG_BIDSTART             = 1,  // ��ʼ����
	MSG_BIDEND               = 2,  // ���۽���
	MSG_BATTLESTART          = 3,  // ��ս��ʼ
	MSG_BATTLEEND            = 4,  // ��ս����
	MSG_BIDSUCCESS           = 5,  // ���۳ɹ�
	MSG_BONUSSEND            = 6,  // �������淢��
	MSG_MARRIAGE             = 10, // ���
	MSG_DIVORCE              = 11, // ���
	MSG_COMBATCHALLENGE      = 12, // Ұս��ս
	MSG_COMBATSTART          = 13, // Ұս��ʼ
	MSG_COMBATREFUSE         = 14, // �ܾ�Ұս����
	MSG_COMBATEND            = 15, // Ұս����
	MSG_COMBATTIMEOUT        = 16, // Ұս���볬ʱ
	MSG_CITYNOOWN            = 17, // ���ɽ������������
	MSG_BATTLE1START         = 18, // �����������������ɳ�ս��ʼ
	MSG_BATTLE2START         = 19, // �����������������ɳ�ս��ʼ 
	MSG_FAMILYDEVOTION       = 20, // ��ü��幱�׶�
	MSG_FAMILYSKILLABILITY   = 21, // ���弼�ܵȼ��仯
	MSG_FAMILYSKILLEVEL      = 22, // ���弼�������ȱ仯
	MSG_FACTIONNIMBUS        = 23, // ���������仯
	MSG_TASK                 = 24, // ���񺰻�
	MSG_BATTLE               = 25, // ��ս����
	MSG_BATTLEWILLSTART      = 26, // ս������һ���Ӻ���
	MSG_SIEGEOWNER		 = 27, // �ط�֪ͨ
	MSG_SIEGEATTACKER	 = 28, // ����֪ͨ
	MSG_SIEGEASSISTANT  	 = 29, // ������֪ͨ
	MSG_SIEGEBUILD  	 = 30, // ս��������Ϣ
	MSG_SIEGEBROADCAST  	 = 31, // ȫ���㲥ս����ʼ
	MSG_SIEGEFACTIONCAST 	 = 32, // ���ɹ㲥ս����ʼ
	MSG_CONTESTTIME          = 33, // ����ʱ��֪ͨ
	MSG_CONTESTEND           = 34, // �������
	MSG_SNSPRESSMSG		 = 35, // ����ƽ̨������Ϣ
	MSG_SNSACCEPTAPPLY	 = 36, // ����ƽ̨�����߽�������
	MSG_TERRI_BID		 = 37, // ����ս��ս���� type
	MSG_TERRI_START		 = 38, // ����ս����
	MSG_TERRI_AWARD		 = 39, // ����ս��������  territoryid itemid
	MSG_UNIQUEAUCTION_BEGIN	 = 40, // Ψһ�۾��Ŀ�ʼǰ30���� win_itemid
	MSG_UNIQUEAUCTION_LAST	 = 41, // Ψһ�۾��������ʾ
	MSG_UNIQUEAUCTION_END	 = 42, // Ψһ�۾��Ľ��� winner_id winner_name price itemid
	MSG_UNIQUEAUCTION_START  = 43, // Ψһ�۾��Ŀ�ʼ win_itemid
	MSG_TERRI_END		 = 44, // ����ս���� territoryid,defener_id(�ط�id,Ϊ0��ʾ���ط�),defender_name,attacker_id,attacker_name,result(int:1:����ʤ2:�ط�ʤ),time(int:0:���ٽ���1:���ٽ���)
	MSG_KINGDOM_FIRSTHALF_END   = 45, //��ս��һ�׶ν��� res(char����1�ط�ʤ2����ʤ),defender(�ط�����id),defender_name,attacker_name1,attacker_name2,attacker_name3,attacker_name4
	MSG_KINGDOM_NEW_WINNER	    = 46, //��ս�ڶ��׶ν��� win_fac_name(��ʤ������),(�¹�����)
	MSG_KINGDOM_ANNOUNCE	    = 47, //������ּ�㲥 msg �ֶ�ֱ��Ϊʥּ���� ���� unmarshal
	MSG_KINGDOM_SET_ATTR	    = 48, //��������������Ȩ�� flag(int:1��������2˫������),cost(���ĵĻ���)
	MSG_KINGDOM_APPOINT	    = 49, //������ְ�� title(char),rolename
	MSG_KINGDOM_OPEN_BATH	    = 50, //��������� queenname
	MSG_KINGDOM_ISSUE_TASK      = 51, //��۷������� kingname, (int)task_type
};

enum CHGS_ERR
{
	ERR_CHGS_SUCCESS         = 0,
	ERR_CHGS_INVALIDGS       = 1,	//�����ڸ�gs��
	ERR_CHGS_MAXUSER         = 2,	//Ŀ��gs�����ﵽ����
	ERR_CHGS_NOTINSERVER     = 3,	//�û��л�gsʱ���ڷ�������
	ERR_CHGS_STATUSINVALID   = 4,	//�û��л�gsʱ״̬����
	ERR_CHGS_NOTGM           = 5,	//�û�����gm
	ERR_CHGS_MAPIDINVALID    = 6,	//��ͼ������
	ERR_CHGS_SCALEINVALID    = 7,	//�Ƿ�����
	ERR_CHGS_DBERROR         = 8,	//���ݿ����
};

enum {
	TITLE_FREEMAN    = 0, //�Ǽ����Ա
	TITLE_SYSTEM     = 1, //������ʾ
	TITLE_MASTER     = 2, //����
	TITLE_VICEMASTER = 3, //������
	TITLE_CAPTAIN    = 4, //����
	TITLE_HEADER     = 5, //����
	TITLE_MEMBER     = 6, //��ͨ��Ա
}; //end of Roles

//ggated related
enum ACTIVATETYPE
{
	ACTIVATE_NONE = 0,
	ACTIVATE_GT = 1,//bit 1
	ACTIVATE_SNS = 2,//bit 2
};
//gt enums
enum ERR_GT
{
	ERR_ROLEREMOVED = 1,
	ERR_HASNOT_ACTIVATED,
	ERR_HAS_ACTIVATED,
	ERR_GT_DB_FAILURE,
	ERR_GT_DATANOTFIND,
};
enum FType
{
	FACTIONTYPE=0,
	FAMILYTYPE,
};
enum GroupType
{
	FRIENDTYPE=0,
};
enum RouteType
{
	FROM_GT=0,
	FROM_GAME,
	TO_GT,
	TO_GAME,
	TO_GT_AND_GAME,
};
/*enum CHAT_SRC_ROUTE
{
	FROM_GAME=0,
	FROM_GT,
};*/
enum FriendOperation
{
	FRIEND_ADD=0,
	FRIEND_DEL,
	FRIEND_MOVE,
};
enum GroupOperation
{
	OP_ADD=0,
	OP_DEL,
	OP_MOD,
};
enum FactionOperation
{
	MEMBER_ADD=0,
	MEMBER_DEL,
	MEMBER_MOVE,
};
enum FactionInfoOperation
{
	FAC_UPDATE_NAME=1,
	FAC_UPDATE_ANNOUNCE=2,
	FAC_UPDATE_EXT=32768,
};
/*enum GameOnlineSyncStatus
{
	SYNC_START = 0,
	SYNC_CONTINUE,
	SYNC_FINISH,
};*/
enum AnnounceRespCode
{
	RET_OK=0,
	RET_INVALID_ZONE,
	RET_DUPLICATE,
	RET_MULTIZONE,
};
enum GT_Role_Change 
{
	UPDATE_NAME=1,
	UPDATE_GENDER=2,
	UPDATE_RACE=4,
	UPDATE_OCCUPATION=8,
	UPDATE_LEVEL =16,
	//UPDATE_ADD_NEW_ROLE=32,
	UPDATE_EXT=64,
	UPDATE_ROLE_CREATE = 32768,
};
/* GT enums end*/
// Circle enums
enum CircleTitle{
	CIRCLE_MEMBER = 0,
	CIRCLE_MASTER = 1,
	CIRCLE_BOY = 2,
	CIRCLE_GIRL = 4,
};
enum Jointype{
	JOIN_CIRCLE = 1,
	LEAVE_AND_JOIN_CIRCLE,
	LEAVE_CIRCLE,
};
enum CircleMsgType{
	CIRCLE_JOIN = 1,
	CIRCLE_GRADUATE,
	CIRCLE_APPOINT_MASTER,
	CIRCLE_APPOINT_BOY,
	CIRCLE_APPOINT_GIRL,
	CIRCLE_POINT_OVER_THRESHOLD,
};

//Circle end
enum ERR_GLOBAL_DROP_COUNTER
{
	ERR_DROP_COUNTER_TIME=1,
	ERR_DROP_COUNTER_ARGU,
	ERR_DROP_COUNTER_NUMBER,
};

//Consign start
enum CONSIGN_TYPE //consist with Post::posttype
{
	CONSIGNTYPE_MONEY=1,
	CONSIGNTYPE_ITEM=2, 
//	CONSIGNTYPE_PET=3, //pet is item
	CONSIGNTYPE_ROLE=4,
};
enum CONSIGN_INFORM //mail type
{
	_CONSIGN_BUY,
	_CONSIGN_SOLD, 
	_CONSIGN_CANCEL,
	_CONSIGN_FORCED_CANCEL,
	_CONSIGN_EXPIRE,
	_CONSIGN_FAIL,
};
enum CONSIGNROLE_INFORM //mail type
{
	_CONSIGNROLE_POST,
};
enum ConsignState
{
	DSTATE_PRE_POST=0,					//Ԥ����,δ��ƽ̨ͬ��
	DSTATE_POST,					//���۳ɹ�,��ƽ̨ͬ�����,�¼�
	DSTATE_PRE_CANCEL_POST,				//Ԥȡ������,δ��ƽ̨ͬ��
//	DSTATE_SHOW,						//��ʾ
	DSTATE_SELL,						//�ϼ�
	DSTATE_SOLD, //tricky,all finish state be put behind this one
	DSTATE_EXPIRE,
	DSTATE_POST_FAIL,
	DSTATE_POST_WEB_CANCEL,
	DSTATE_POST_GAME_CANCEL,
	DSTATE_POST_FORCE_CANCEL,
};
enum ConsignPostType
{
	POST_NORMAL,
	POST_RESTORE,
};
enum CheckStateChangeRes
{
	CHANGE_FORBID = 0,
	CHANGE_PERMIT,
	CHANGE_FORCED,
	CHANGE_REPEAT,
	CHANGE_IGNORE,
};
enum ConsignMailStatus
{
	NOT_SEND = 0,
	BUYER_SENDED,
	SELLER_SENDED,
	ALL_SENDED,
};
enum ERR_WEBTRADE
{
	ERR_WT_UNOPEN = 1,
	ERR_WT_ENTRY_NOT_FOUND,
	ERR_WT_ENTRY_IS_BUSY,
	ERR_WT_SN_ROLEID_MISMATCH,
	ERR_WT_TIMESTAMP_OLD,
	ERR_WT_TIMESTAMP_EQUAL,
	ERR_WT_TIMESTAMP_MISMATCH,
	ERR_WT_TIMESTAMP_ERR_SEQUENCE,
	ERR_WT_BUYER_NOT_EXIST,
	ERR_WT_TOO_MANY_ATTEND_SELL,
	ERR_WT_MAILBOX_FULL,//11
	ERR_WT_BUYERNAME_INVALID,
	ERR_WT_DB_FAILURE,
	ERR_WT_DB_STATEERR,
	ERR_WT_DB_KEYZERO,
	ERR_WT_DB_INVALID_ARGUMENT,
	ERR_WT_TIMEOUT,
	ERR_WT_GCONSIGNSTART_PROCESSING,
	ERR_WT_DB_DUPLICATE_RQST,  //occur when db write success but delivery timeout
	ERR_WT_STATEERR,
	ERR_WT_DUPLICATE_RQST,//21
	ERR_WT_PREPOST_ARG_ERR,
	ERR_WT_BUYER_SAMEUSER,
	ERR_WT_ROLE_IS_BUSY,
	ERR_WT_INVALID_ARGUMENT,
	ERR_WT_SELL_ROLE_WHILE_ITEM_SELLING,
	ERR_WT_SOLD_ROLE_STATUS_ERR,
	ERR_WT_SOLD_ROLE_BUYER_COND,//sect or spouse
	ERR_WT_SOLD_ROLE_SELLER_NOT_EXIST,
	ERR_WT_SELL_ROLE_WHILE_FORBID,
	ERR_WT_SELL_ROLE_HAS_FAC_RELATION, // 31
	ERR_WT_SELL_ROLE_HAS_SECT_RELATION,
	ERR_WT_SELL_ROLE_HAS_SPOUSE_RELATION,
	ERR_WT_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION,
};
enum ERR_WEBTRADE_CLIENT
{
	ERR_WTC_UNOPEN = 1,
	ERR_WTC_ENTRY_NOT_FOUND,
	ERR_WTC_ENTRY_IS_BUSY,
	ERR_WTC_SN_ROLEID_MISMATCH,
	ERR_WTC_TOO_MANY_ATTEND_SELL,
	ERR_WTC_MAILBOX_FULL,
	ERR_WTC_BUYERNAME_INVALID,
	ERR_WTC_TIMEOUT,
	ERR_WTC_GCONSIGNSTART_PROCESSING,
	ERR_WTC_STATEERR,
	ERR_WTC_DUPLICATE_RQST,//11
	ERR_WTC_PREPOST_ARG_ERR,
	ERR_WTC_DB_ERR,
	ERR_WTC_INNER,
	ERR_WTC_BUYER_SAMEUSER,
	ERR_WTC_LISTALL_ARG_ERR,
	ERR_WTC_LISTALL_FINISH,
	ERR_WTC_SELL_ROLE_WHILE_ITEM_SELLING,
	ERR_WTC_SELL_ROLE_WHILE_FORBID,
	ERR_WTC_SELL_ROLE_HAS_FAC_RELATION,
	ERR_WTC_SELL_ROLE_HAS_SECT_RELATION,
	ERR_WTC_SELL_ROLE_HAS_SPOUSE_RELATION,
	ERR_WTC_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION,
};

//Consign end

// Friend Callback start
enum ERR_FRIENDCALLBACK
{
	ERR_CALLBACK_SUCCESS,               // 0.�����ɹ�
	ERR_CALLBACK_TIMEOUT,               // 1.��Ϣ����ʱ
	ERR_CALLBACK_UNVALIDTEMPID,         // 2.�����ٻ��ʼ����ʼ�ģ��ID��Ч
	ERR_CALLBACK_LIMITNOTINIT,          // 3.��ҷ����ٻ��ʼ�����������û�м���
	ERR_CALLBACK_MSGCOOLDOWN,           // 4.�����ٻ��ʼ���ʱ��������
	ERR_CALLBACK_OFFLINE,               // 5.�����߲�����
	ERR_CALLBACK_FRIENDONLINE,          // 6.�ٻ��ʼ��Ľ���������
	ERR_CALLBACK_FDLISTNOTINIT,         // 7.��ҵĺ����б�û�м���
	ERR_CALLBACK_NOTHISFRIEND,          // 8.�����߲��Ƿ����ߵĺ���
	ERR_CALLBACK_MAXMAILNUM,            // 9.�������Ѿ��ﵽ���췢���ٻ��ʼ�������
	ERR_CALLBACK_COOLDOWN,              // 10.�ٻ��ʼ��Ľ����߻�������ȴ��
	ERR_CALLBACK_EXCEPTION,             // 11.���ݿ�����쳣
	ERR_CALLBACK_ACCOUNTFROZEN,         // 12.�ٻ��ʼ��Ľ�����Ϊ�����
	ERR_CALLBACK_MAILBOXFULL,           // 13.��������
	ERR_CALLBACK_NOAWARD,               // 14.û�н���
};

enum FRIENDCALLBACK_AWARDTYPE
{
	RECALL_AWARD = 1, 		// �����ٻ�����Ҷ���õĽ���
	RETURN_AWARD = 2, 		// ����һع齱��
};

enum
{
	BONUS_CIRCLE,
	BONUS_CALLBACK,
};

//Mail sender type, local extention
enum MAIL_SENDER_TYPE_LOCAL{
	_MST_LOCAL_START = 32,
	_MST_LOCAL_CONSIGN = _MST_LOCAL_START, //consign
	_MST_CALLBACK,
	_MST_CALLBACK_AWARD,
	_MST_ROLE_CONSIGN_NOTIFY,
	_MST_GAMEDB,
	_MST_KINGDOM_AWARD,  //�����ǻ� ��Ա����ʱװ
	_MST_FACTION_PK_BONUS_NOTIFY=40,
};

enum FACTION_DEFINE
{
	MAX_HOSTILE_NUMBER = 5,
	HOSTILE_MINIMUM = 3*3600,
	HOSTILE_TIMEOUT = 3*24*3600,
	PROTECT_TIMEOUT = 7*24*3600,
};

enum HOSTILE_DEFINE
{
	HOSTILE_PROTECTED = 0x01,
};

enum HOSTILE_DATAMASK
{
	OP_PROTECT = 0x01,
       	OP_ACTIONPOINT = 0x02, 
};

enum TERRI_SAVE_REASON
{
	TERRITORY_SAVE_BIDBEGIN,
	TERRITORY_SAVE_BIDEND,
};

inline HostileInfo & GetHostileInfo(GFactionInfo &info) { return info.hostileinfo.front();}
inline std::vector<HostileFaction> & Hostiles(GFactionInfo &info) { return GetHostileInfo(info).hostiles;}

enum FAMILY_DEFINE
{
	MAX_FAMILY_MEMBER_NUMBER = 15,
	FAMILYSKILL_BASE         = 440,
	FAMILYSKILL_SUM          = 4,
};


enum ERR_HOSTILE
{
	ERR_FC_INFACTION 		= 125,
	ERR_HOSTILE_COOLING 		= 126,  // ��Ϊ�ж԰��ɲ���10Сʱ�����ܳ����ж�״̬
	ERR_HOSTILE_ALREADY 		= 127,  // �Ѿ��ǵж�״̬
	ERR_HOSTILE_FULL 		= 128,  // �ж԰������ﵽ����
	ERR_HOSTILE_PEER_FULL 		= 129,  // �Է��ж�״̬���ﵽ����
	ERR_HOSTILE_LEVEL_LIMIT 	= 130,  // �������ɵȼ������󣬲��ܽ���ж�״̬
	ERR_HOSTILE_ITEM 		= 131,  // û���ж���
	ERR_HOSTILE_PROTECTED 		= 132,  // ������Ʒ���������ܽ���ж�״̬
	ERR_HOSTILE_PEER_PROTECTED 	= 133,  // �Է���������Ʒ���������ܽ���ж�״̬
	ERR_HOSTILE_PRIVILEGE 		= 134,  // ��Ȩ���д˲���
	ERR_HOSTILE_PROTECTITEM		= 135,  // û���ҵ����Ᵽ����Ʒ
	ERR_FC_MAILBOXFULL 	   	= 136,  //��ȡ��ս����ʱ��������
	ERR_FC_FACTION_INEXIST 	   	= 137,  //û�ж�Ӧ����	  
	/*
	ERR_FC_MSG_COOLDOWN		= 138,  //����������ȴ
	ERR_FC_MSG_ARG			= 139,  //�������Բ�������
	ERR_FC_MSG_LENGTH		= 140,  //���Գ���
	*/
};

enum RAID_TYPE
{
	ROOM_RAID = 0,
	TOWER_RAID = 1,//����
	TEAM_RAID = 2,//6v6
	FACTION_PK_RAID = 3,
	LEVEL_RAID = 4, //�ؿ�����
	CRSSVRTEAMS_RAID = 5, // ���ս��6v6
	TRANSFORM_RAID = 6, //������
	STEP_RAID = 7,
	HIDEANDSEEK_RAID = 8, // ׽�Բظ���
	FENGSHEN_RAID = 9, // ����150-160
};
enum RAID_TEAM_TYPE//TEAM_RAID
{
	RAIDTEAM_NEUTRAL = 0,//not used
	RAIDTEAM_ATTACK,
	RAIDTEAM_DEFEND,
	RAIDTEAM_LOOKER,
	RAIDTEAM_END = RAIDTEAM_LOOKER,
};
enum ERR_RAID
{
	ERR_RAID_MAP_NOTEXIST 			= 1, //��ͼ������
	ERR_RAID_ROOM_NUMBER_FULL 		= 2, //��������������
	ERR_RAID_JOIN_ALREADY                   = 3, //�Ѿ����븱��
	ERR_RAID_ROOMID_EXIST 			= 4, //��������id�ظ�
	ERR_RAID_ROLE_OFFLINE 			= 5, //���������
	ERR_RAID_ROOM_INEXIST 			= 6, //�������䲻����
	ERR_RAID_ARGUMENT_ERR 			= 7, //��������
	ERR_RAID_STARTED 			= 8, //�����ѿ�ʼ
	ERR_RAID_APPLY_TEAM_FULL		= 9, //����������������
	ERR_RAID_ROLE_ALREADY_IN_ROOM 		= 10,//�Ѿ��ڸ���������
	ERR_RAID_OPERATOR_NOT_MASTER 		= 11,//�����˲��Ƿ���
	ERR_RAID_TEAM_FULL			= 12,//������ʽ��������
	ERR_RAID_NOT_IN_APPLYLIST 		= 13,//�������������
	ERR_RAID_JOIN_REFUSED 			= 14,//�����ܾ�������
	ERR_RAID_START_TEAMNUMBER 		= 15,//������������
	ERR_RAID_GET_INSTANCE_FAIL 		= 16,//DELIVER��ȡʵ��ʧ��
	ERR_RAID_ROLE_NOT_IN_ROOM 		= 17,//��Ҳ��ڴ˷���
	ERR_RAID_STATUS 	 		= 18,//���䵱ǰ״̬������˲���
	ERR_RAID_INSTANCE_INEXIST 		= 19,//����ʵ��������
	ERR_RAID_ROOMCLOSE 	 		= 20,//��������ر�
	ERR_RAID_ROLE_NOT_IN_RAID 		= 21,//���δ��������
	ERR_RAID_ITEMLIMIT 			= 22,//��������Ʒ����
	ERR_RAID_STATUS_CHANGE 			= 23,//״̬�л�����
	ERR_RAID_GS_GET_INSTANCE_FAIL 		= 24,//GS��ȡʵ��ʧ��
	ERR_RAID_IS_OPENING 			= 25,//����Ѿ������뿪����
	ERR_RAID_GS_ENTER_CHECK_FAIL 		= 26,//GS�����븱������ʧ��
	ERR_RAID_DB_TIMEOUT 			= 27,//DB����ʱ
//	ERR_RAID_INSTANCE_START 		= 28,//��������ʧ��
//	ERR_RAID_INSTANCE_ENTER 		= 29,//���븱��ʧ��
//	ERR_RAID_INSTANCE_CLOSE 		= 30,//�����ر�
//	ERR_RAID_KICKOUT 			= 31,//�������߳�
	ERR_RAID_ROOMNAME 			= 28,//����������
	ERR_RAID_VOTECOOLDOWN			= 29,//ͶƱ��ȴ��
	ERR_RAID_VOTEING 			= 30,//���ڽ�������ͶƱ
	ERR_RAID_VOTEDROLE_NOT_IN_ROOM 		= 31,//��ͶƱ�˲��ڷ�����
	ERR_RAID_VOTEDROLE_NOT_IN_RAID 		= 32,//��ͶƱ�˲��ڸ�����
	ERR_RAID_VOTER_NOT_IN_ROOM 		= 33,//ͶƱ�˲��ڷ�����
	ERR_RAID_VOTER_NOT_IN_RAID 		= 34,//ͶƱ�˲��ڸ�����
	ERR_RAID_NOT_VOTEING 			= 35,//���ڽ���ͶƱ
	ERR_RAID_VOTEDROLE_MISMATCH		= 36,//��ͶƱ�˲�ƥ��
	ERR_RAID_ROOM_FORBID_VOTE 		= 37,//���䲻����ͶƱ
	ERR_RAID_TYPE 				= 38,//�������ʹ���
	ERR_RAID_TEAM_SIZE 			= 39,//������������
	ERR_RAID_IS_APPLYING 			= 40,//������ӱ���6v6����
	ERR_RAID_NOT_VALID_FACTITLE             = 41,//����ְ�񲻷�
	ERR_RAID_TARGET_NOT_ONLINE 		= 42,//����ս�˲�����
	ERR_RAID_NOT_IN_FACTION 		= 43,//δ�μӰ���
	ERR_RAID_IN_SAME_FACTION 		= 44,//����ͬһ����
	ERR_RAID_FAC_LOWLEVEL 			= 45,//���ɵȼ�����
	ERR_RAID_CHALLENGE_COOLING 		= 46,//������ս��ȴ��
	ERR_RAID_FACTION_COMBAT_BUSY 		= 47,//���ڰ�ս��
	ERR_RAID_TARGET_MASTER_NOT_ONLINE	= 48,//����ս����������
	ERR_RAID_FACTION_NOPROSPERITY  		= 49,//�ﹱ����
	ERR_RAID_FACTION_CHALLENGE_REJECTED 	= 50,//��ս���ܾ�
	ERR_RAID_FACTION_CHALLENGE_TIMEOUT 	= 51,//�������볬ʱ	
	ERR_RAID_FACTION_JOIN_GROUP 		= 52,//������Ӫ����
	ERR_RAID_FACTION_CANCEL_ARU_ERR 	= 53,//ȡ����ս����
	ERR_RAID_FACTION_KILL_INVALID 		= 54,//��Ч��ս��ɱ
	ERR_RAID_TEAMAPPLY_COOLING		= 55,//��Ӹ�����ȴʮ����
	ERR_RAID_NOT_APPLY_TEAM			= 56,//û��������Ӹ���
	ERR_RAID_NOT_TEAMAPPLY_TIME		= 57,//����Ӹ�������ʱ��
	ERR_RAID_TEAM_APPLY_COND		= 58,//ĳ��ְҵ����>2���ܱ���
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_HAVE_OTHER  	= 59,//�����ڵ�С��������ս�ӳ�Ա�����ܽ��б���
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_ROLESIZE 	= 60,//�����ڵ�С��û������6�ˣ����ܽ��б���
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_NOT_IN		= 61,//��������ս�ӳ�Ա�������ս�Ӳſɽ��б���	
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_MUST_TEAM	= 62,//ս�ӱ������������״̬�£������ȸ��Լ�ս�ӳ�Ա���С�ӣ����б���
	ERR_RAID_TEAM_APPLY_CRSSVRTEAM_MUST_ONTIME	= 63,//�����ڵ�ս���Ѿ�������������ǽ������ٿ�ʼ����
};
enum ERR_BATTLE
{
	ERR_BATTLE_TEAM_FULL				= 140,	// ��Ӫ����
	ERR_BATTLE_GAME_SERVER				= 141,	// ����ͬһ����
	ERR_BATTLE_JOIN_ALREADY 			= 142,	// �Ѿ��������
	ERR_BATTLE_MAP_NOTEXIST				= 143,	// û���ҵ���ͼ
	ERR_BATTLE_COOLDOWN					= 144,	// ���ϴ�ս��ʱ�䲻����ȴʱ�䣬���ܱ���
	ERR_BATTLE_NOT_INTEAM				= 145,  // �û����ڶ�����
	ERR_BATTLE_LEVEL_LIMIT				= 146,  // �û�������ս����������
	ERR_BATTLE_CLOSE					= 147,  // ս��û�п���
	ERR_BATTLE_QUEUELIMIT           	= 148,  // ����Ŷӳ�������
	ERR_BATTLE_INFIGHTING           	= 149,  // �Ѿ�����ս���������˳��Ŷ�
	ERR_BATTLE_MONEY           			= 150,  // Ǯ���������ܿ���ս��
	ERR_BATTLE_REBORN_NEED     			= 151,  // ��Ҫ�������ܼ������ս��
	ERR_BATTLE_STARTED     				= 152,  // �ν�ս�����������ٱ���
	ERR_BATTLE_STARTED10     			= 153,  // �ν�ս������10���Ӻ����ٽ���ս��
	ERR_BATTLE_JOIN_REFUSED				= 154,	// ����������ܾ�
	ERR_BATTLE_APPOINT_TARGETNOTFOUND 	= 155,	// ���ܴ�λ���˲�����
	ERR_BATTLE_TEAM_SIZE			 	= 156,	// �Ŷ������ﲻ������Ҫ��
	ERR_BATTLE_NOT_MASTER			 	= 157,	// �������죬û��Ȩ�޿���ս��
	ERR_BATTLE_MONEYLIMIT				= 158,	// ��Ǯ���㼤��ս��
	ERR_BATTLE_ITEMLIMIT				= 159,	// ��Ʒ���㼤��ս��
	ERR_BATTLE_OCCUPATION           	= 160,  // �����Ӫ����
	ERR_BATTLE_ACCEPT_REQUESTERNOTFOUND	= 161,	// �������˲�����
	ERR_BATTLE_ACTIVATECOOLDOWN			= 162,	// ս��������ȴ
	ERR_BATTLE_REBORN_CNT				= 163,	// ��������������Ҫ��
	ERR_BATTLE_MUSTNOT_REBORN			= 164,	// �ѷ�����Ҳ��ܱ���
	ERR_BATTLE_TIMEOUT 				= 165, 	// GS����ʱ
	ERR_BATTLE_CROSSBATTLE 				= 166,  // ��ɫ�п��ս����Ϣ,��Ҫ���ڲ��ÿͻ�����ʾ����ս���ɹ�
	ERR_BATTLE_ENTERCROSSBATTLE 	 		= 167,  // ��ɫ�ڿ������ս��
	ERR_BATTLE_ENTER_MAXTIMES			= 168,  //���������������
	ERR_BATTLE_ENTER_NOT_IN_JOINROOM		= 169,  // �״ν������ڱ�������npc��������ϵGM
};

enum SIEGE_RESULT
{
	SIEGE_ATTACKER_WIN = 1,
	SIEGE_DEFENDER_WIN = 2,
	SIEGE_TIMEOUT = 3,
	SIEGE_CANCEL = 4,
};

enum SIEGE_RELATIVE
{
	SIEGE_QUEUE_MAX = 15,

	MONEY_ONEDAY_SIEGE = 100*10000,
	MONEY_TWODAY_SIEGE = 2*MONEY_ONEDAY_SIEGE,
	MONEY_THREEDAY_SIEGE = 3*MONEY_ONEDAY_SIEGE,

};
enum ERR_SIEGE
{
	ERR_SIEGE_QUEUEFULL 		= 150, // �ŶӰ��ɹ���
	ERR_SIEGE_FACTIONLIMIT 		= 151, // ���ɼ�������
	ERR_SIEGE_NOTFOUND 		= 152, // û���ҵ�ս��
	ERR_SIEGE_CHALLENGED		= 153, // �Ѿ�����
	ERR_SIEGE_SELF			= 154, // ������ս�Լ�
	ERR_SIEGE_CHECKFAILED		= 155, // �������ʧ��
	ERR_SIEGE_TIMEERROR		= 156, // ���ڲ������ø�������
	ERR_SIEGE_MONEYLIMIT		= 157, // ����ʧ�ܣ���Ҳ���
	ERR_SIEGE_ITEMLIMIT		= 158, // ����ʧ�ܣ�ȱ����Ʒ
	ERR_SIEGE_FACTIONNOTFOUND	= 159, // û���ҵ���������
	ERR_SIEGE_CANTENTERNORMAL	= 161, // ���ܽ���
	ERR_SIEGE_CANTENTERBATTLE	= 162, // ���ܽ���
	ERR_SIEGE_NOFACTION		= 163, // ���ܽ���
	ERR_SIEGE_ASSISTANT_TIME 	= 164, // ���������ʱ�䲻��
	ERR_SIEGE_ASSISTANT_LEVEL 	= 165, // �����Ｖ����
	ERR_SIEGE_ASSISTANT_SELF 	= 166, // ������Ϊ������
	ERR_SIEGE_ASSISTANT_OWNER 	= 167, // ������Ϊ������
	ERR_SIEGE_COOLING			= 168,	//��������Ҫ���10����

};

enum ERR_TERRITORY
{
				    //������ͨ�ô�����
	ERR_TERRI_NOTFOUND 		= 1,//��������������
	ERR_TERRI_LOGIC,		    //�߼�����
	ERR_TERRI_OUTOFSERVICE,		    //����������
				   //��������ս������
	ERR_TERRI_BID_TIME,		   //����սʱ��
	ERR_TERRI_BID_REPEAT,		   //�ظ���ս�������Ѷ�����������ս
	ERR_TERRI_BID_BUSY,		   //���ݿⷱæ
	ERR_TERRI_BID_FAC_NOTFOUND,	   //��ս���ɲ�����
	ERR_TERRI_BID_NOTMASTER,	   //�ǰ���
	ERR_TERRI_BID_SELF,	      	   //���Լ�������ս
	ERR_TERRI_BID_ITEMTYPE,		   //��ս��Ʒ���ʹ���
	ERR_TERRI_BID_ITEM_MIN,		   //��ս�����
	ERR_TERRI_BID_LISTSIZE,		   //��ս���ɹ���
	ERR_TERRI_BID_ITEM_MAX,		   //��ս������
	ERR_TERRI_BID_FAC_LEVEL,	   //���ɵȼ�����
	ERR_TERRI_BID_ADJACENT,		   //��ս���ɲ�����
	ERR_TERRI_BID_OCCUPY_NUM,	   //��ռ���������
				   //�����ǽ��������
	ERR_TERRI_ENTER_NOTSTART,	   //ս��δ����			17
	ERR_TERRI_ENTER_NOTBATTLESIDE,	   //��ս��˫����Ա
	ERR_TERRI_ENTER_ALREADYIN,	   //�Ѿ���ս����
	ERR_TERRI_ENTER_NUMLIMIT,	   //��������			20
	ERR_TERRI_ENTER_JOINTIME,	   //������ɲ���һ��
				   //�������뿪������
	ERR_TERRI_LEAVE_NOTIN,		   //���δ������ս��
				   //����������Ʒ������
	ERR_TERRI_GETITEM_NOTMASTER,	   //�ǰ���			23
	ERR_TERRI_GETITEM_NUM,		   //��������
	ERR_TERRI_GETITEM_TIME,		   //�¼�����
	ERR_TERRI_GETITEM_ITEMID,	   //��ȡ��Ʒ���ʹ���
	ERR_TERRI_GETITEM_NOTWINNER,	   //��ʤ����
	ERR_TERRI_GETITEM_PEACE,	   //����û������ս�����Բ�������ս����
	ERR_TERRI_GETITEM_NOTBIDDER,	   //û������վ��������ս��һ���������˻���ս��
	ERR_TERRI_GETITEM_NORAND,	   //������û������ ���������Ѿ�����
	ERR_TERRI_GETITEM_INVALIDTYPE,	   //�������ʹ���
	ERR_TERRI_BID_COOLING,		   //��ս��ȴ       32
	ERR_TERRI_BID_CREATETIME,	   //���ɴ�����һ�ܲſ�����ս
};

enum ERR_SECT
{
	ERR_SECT_OFFLINE        = 1,   //��Ҳ�����
	ERR_SECT_UNAVAILABLE    = 2,   //�Է��Ѿ���ʦ
	ERR_SECT_FULL           = 3,   //ͽ�������Ѿ��ﵽ����
	ERR_SECT_BUSY           = 4,   //ϵͳ��æ�����Ժ�����
	ERR_SECT_REFUSE         = 5,   //�Է��ܾ��������ͽ����
	ERR_SECT_INVALIDLEVEL   = 6,   //�Է���������Ҫ��
	ERR_SECT_COOLING        = 7,   //һ��ֻ������һ����
};

enum USER_STATUS
{
	STATUS_CASHINVISIBLE   =   0x01,
};

class FactionHelper
{
public :
	static unsigned int GetMemberCapacity(int level)
	{
		unsigned int _capacity[10] = {5, 7, 9, 11, 13, 15, 17, 19, 21, 23};
	        return _capacity[level%10];
	}

};

enum WAITDEL_TYPE
{
	TYPE_ROLE        = 1,
	TYPE_FACTION     = 2,
	TYPE_FAMILY      = 3,
};

enum FORBID
{
	FORBID_USER_LOGIN = 100,
};

enum SNS_CHARM_TYPE
{
	SNS_CHARM_TYPE_PERSONAL = 0,
	SNS_CHARM_TYPE_FRIEND,
	SNS_CHARM_TYPE_SECT,
	SNS_CHARM_TYPE_FACTION,
};

enum SNS_MESSAGE_TYPE
{
	SNS_MESSAGE_TYPE_WEDDING = 0,
	SNS_MESSAGE_TYPE_FRIEND,
	SNS_MESSAGE_TYPE_SECT,
	SNS_MESSAGE_TYPE_FACTION,
	SNS_MESSAGE_TYPE_MAX,
};

enum SNS_LEAVEMESSAGE_TYPE
{
	SNS_LEAVEMESSAGE_TYPE_PRESS = 0,
	SNS_LEAVEMESSAGE_TYPE_APPLY,
	SNS_LEAVEMESSAGE_TYPE_LEAVE,
};

enum SNS_OPERATION_TYPE
{
	SNS_OP_PRESSMESSAGE = 0,
	SNS_OP_APPLY,
	SNS_OP_APPLYMSG,
	SNS_OP_AGREE,
	SNS_OP_DENY,
	SNS_OP_ACCEPTAPPLY,
	SNS_OP_REJECTAPPLY,
};

enum SNS_ERRCODE
{
	SNS_ERR_SUCCESS = 0,
	SNS_ERR_PRIVILEGE,
	SNS_ERR_ROLEOFFLINE,
	SNS_ERR_LEVEL,
	SNS_ERR_FACTIONLEVEL,
	SNS_ERR_GENDER,
	SNS_ERR_OCCUPATION,
	SNS_ERR_VOTETIME15MIN,
	SNS_ERR_LENGTH,
	SNS_ERR_REPEAT,
	SNS_ERR_MSGNOTFOUND,
	SNS_ERR_APPLYROLEIDNOTFOUND,
	SNS_ERR_MARRIED,
	SNS_ERR_TITLE,
	SNS_ERR_FORBIDSELF,
	SNS_ERR_NOTREADY,
	SNS_ERR_PRESSTIME24HOUR,
	SNS_ERR_PRESSTIME12HOUR,
	SNS_ERR_APPLYTIME24HOUR,
	SNS_ERR_SECTAPPLYLEVEL,
	SNS_ERR_APPLYCOOLTIME,
	SNS_ERR_LEAVEMSGCOOLTIME,
	SNS_ERR_VOTECOOLTIME,
	SNS_ERR_LEAVEMSGLEVEL,
	SNS_ERR_LEAVEMSGFULL,
	SNS_ERR_ALREADYINSECT,
	SNS_ERR_ALREADYINFACTION,
	SNS_ERR_MARRYMULTIACCEPT,
	SNS_ERR_APPLYMULTIACCEPT,
	SNS_ERR_MARRYALREADYACCEPT,
	SNS_ERR_LEAVEMSGNOTFOUND,
};

enum SNS_LIMIT
{
	SNS_LIMIT_COOLTIME = 15*60,						//����Ӧ����ͶƱҪ���15����
	SNS_LIMIT_LEAVEMSGCOOLTIME = 60,				//��������Ҫ���1����
	SNS_LIMIT_PRESSLEVEL = 15,						//15�����ϲ��ܷ�����Ϣ
	SNS_LIMIT_SECTPRESSLEVEL = 105,					//105�����ϲ��ܷ���ʦ����ļ��Ϣ
	SNS_LIMIT_SECTAPPLYLEVEL = 90,					//90�����ϲ���Ӧ��ʦ��
	SNS_LIMIT_MESSAGESIZE = 60,						//��Ϣ�����Դ�С���ܳ���40�ֽ�
	SNS_LIMIT_VOTETIME = 15*60,						//ͬһ����Ϣ��ͶƱҪ���15����
	SNS_LIMIT_APPLYTIME = 24*3600,					//ͬһ����Ϣ��Ӧ��Ҫ���24Сʱ
	SNS_LIMIT_PRESSTIME_CHARMSTARGE4 = 12*3600,		//����ֵ4�Ǽ����Ϸ���ʱ����12Сʱ
	SNS_LIMIT_PRESSTIME_CHARMSTARLT4 = 24*3600,		//����ֵ4�����·���ʱ����24Сʱ
	SNS_LIMIT_APPLYINCCHARM = 1,					//����ʹ�ø���Ϣ����ֵ��1
	SNS_LIMIT_LEAVEMSGINCCHARM = 1,					//����ʹ�ø���Ϣ����ֵ��1
	SNS_LIMIT_VOTEAGREEINCCHARM = 50,				//ͶƱ֧��һ����Ϣʹ������ֵ��1
	SNS_LIMIT_VOTEDENYDECCHARM = 50,				//ͶƱ����һ����Ϣʹ������ֵ��1
	SNS_LIMIT_MSGCHARM = 9999,						//һ����Ϣ����ֵ����9999
	SNS_LIMIT_MSGLIFETIME = 7*24*3600,				//��Ϣ�ı���ʱ��Ϊһ������
	SNS_LIMIT_MSGNUM = 50,							//ÿ����Ϣ��ౣ��50��
	SNS_LIMIT_LEAVEMSGNUM = 50,						//ͬһ����Ϣ��������ౣ��50��
	SNS_LIMIT_APPLYVOTENUM = 200,					//Ӧ����֧�֡��������б���ౣ��200����
	SNS_LIMIT_CHARMSTAR = 10,						//����ֵ�Ǽ����10��
	SNS_LIMIT_LEAVEMSGMAXLEVEL = 8,					//����������
};

enum REF_ERRCODE	//�����ƹ���ش������
{
	REF_ERR_SUCCESS = 0,
	REF_ERR_NOREFERRALFOUND,						//���û�����ߵ�����
	REF_ERR_REFERRERNOTLOADED,						//������Ϣ��û����
	REF_ERR_REFERRERINTRANSACTION,					//����������ȡ����ֵ����ֵ
	REF_ERR_REFERRERLOGOUT,							//���߲�����
	REF_ERR_NOEXPAVAILABLE,							//����û�п���ȡ�ľ���ֵ
	REF_ERR_NOBONUSAVAILABLE,						//����û�п���ȡ�ĺ���ֵ
	REF_ERR_SUBMITTODB,								//��ȡ�����ύ�����ݿ����
	REF_ERR_INVALIDSPREADCODE,						//�ƹ��벻��ȷ
	REF_ERR_REFERRERNOTINSERVER,					//�ƹ��˲��ڱ�������
	REF_ERR_REFREFERRERLEVEL,						//60�����ϲ����ƹ�����
	REF_ERR_REFERRERNOTINDISTRICT,					//�ƹ��˲��ڱ���
};

enum REF_LIMIT
{
	REF_LIMIT_REFERRALPERPAGE = 15,					//�ͻ���ÿ�λ�ȡ15������
	REF_LIMIT_REFERRERLEVEL	  = 60,					//���߱���ﵽ60�������ƹ�
};

enum CHALLENGE_ALGO
{
	ALGO_NONE = -1,
	ALGO_MD5 = 0,
	ALGO_PLAINTEXT = 1, 
};

enum ERR_UNIQUEAUCTION
{
	ERR_UNIQUE_BID_TIME		= 1, //�Ǿ���ʱ��
	ERR_UNIQUE_BID_MONEY,		     //���ļ۸񲻷�������
	ERR_UNIQUE_BID_TIMES_PERSON,	     //���ĳ���ÿ��ÿ��涨�Ĵ���
	ERR_UNIQUE_BID_TIMES_TOTAL,	     //ȫ�������ܴ�������ÿ��涨�Ĵ���
	ERR_UNIQUE_BID_REPEAT,		     //��������ͬ�ļ۸�
	ERR_UNIQUE_GETITEM_NOTWINNER,	     //�Ǿ��Ļ���
	ERR_UNIQUE_GETITEM_ITEMID,	     //������Ʒ���ʹ�����������������
};

enum ERR_KINGDOM
{
	ERR_KINGDOM_BATTLE_STATE	= 1, //����ս��ʱ�� �޷�����ս��
	ERR_KINGDOM_INVALID_FAC,	     //��ݲ����Ϲ�ս�������޷�����
	ERR_KINGDOM_ROLE_LIMIT,		     //����ս�������ﵽ���ޣ���ʱ�޷�����
	ERR_KINGDOM_INVALID_TAG,	     //��ͼ�Ŵ���
	ERR_KINGDOM_NOTIN,		     //������ս����
	ERR_KINGDOM_FIELD_UNREGISTER,        //��ս�����Ͳ�����
	ERR_KINGDOM_BATTLE_STILL_IN,	     //�쳣�˳� ���Ժ����
	ERR_KINGDOM_ALREADY_FAIL,	     //����������ѱ��ݻ٣��޷������ͼ1
	ERR_KINGDOM_SUB_LIMIT,		     //ս����ͼ�����ﵽ���ޣ���ʱ�޷�����

	ERR_KINGDOM_PRIVILEGE,		     //����ʻȨ��Ȩ�� 10
	ERR_KINGDOM_LOGIC,		     //�߼�����
	ERR_KINGDOM_POINTS_NOTENOUGH,	     //���һ��ֲ���
	ERR_KINGDOM_NOT_INIT,		     //����δ����
	ERR_KINGDOM_SVR_ATTR_REPEAT,	     //�����Ѿ����� �����ظ�����
	ERR_KINGDOM_SVR_ATTR_CD,	     //���������Ը�����ȴ
	ERR_KINGDOM_SVR_ATTR_COND,	     //�����������޸Ŀ�����������
	ERR_KINGDOM_FUNC_SIZE,		     //ְλ��������
	ERR_KINGDOM_INVALID_NAME,	     //��ɫ��������
	ERR_KINGDOM_TITLE_REPEAT,	     //������й���ְλ
	ERR_KINGDOM_INVALID_CHAR,	     //ʥּ�а����Ƿ��ַ�
	ERR_KINGDOM_CALLGUARDS_CD,	     //�����촫�������Ĵ����Ѿ��ﵽ����
	ERR_KINGDOM_NOGUARD_ONLINE,	     //û����������
	ERR_KINGDOM_TIMEOUT,		     //���û��ע�⵽�������� ���Ժ���������
	ERR_KINGDOM_KING_REFUSE,	     //���û��ͬ������ȫ����������
	ERR_KINGDOM_KING_NOTONLINE,	     //����û������
	ERR_KINGDOM_SYS_BATH_TIME,	     //ϵͳ��������ʱ��β��ܿ�������
	ERR_KINGDOM_DATABASE,		     //���ݿ����
	ERR_KINGDOM_TASK_CD,		     //��������������ȴ
	ERR_KINGDOM_TASK_REPEAT,	     //�����Ѿ��������������ظ���
};

enum ERR_TOUCH
{
	ERR_TOUCH_SUCCESS		= 0,
	ERR_TOUCH_COMMUNICATION 	= 1, //��������ͨ�Ŵ���
	ERR_TOUCH_EXCHG_FAIL		= 2, //�һ� touch �� ʧ��
	ERR_TOUCH_GEN_ORDERID		= 3, //���� touch �һ�������ʧ��
	ERR_TOUCH_POINT_NOTENOUGH	= 4, //touch �㲻��
};

class Utility
{
public:
	static void SwapSyncData(GMailSyncData& data1, GMailSyncData& data2)
	{
		/*std::swap(data1.data_mask, data2.data_mask);
		std::swap(data1.cash_total, data2.cash_total);
		std::swap(data1.cash_used, data2.cash_used);
		std::swap(data1.cash_serial, data2.cash_serial);
		std::swap(data1.inventory.capacity, data2.inventory.capacity);
		std::swap(data1.inventory.timestamp, data2.inventory.timestamp);
		std::swap(data1.inventory.money, data2.inventory.money);

		data1.inventory.items.swap(data2.inventory.items);
		data1.inventory.equipment.swap(data2.inventory.equipment);
		data1.inventory.petbadge.swap(data2.inventory.petbadge);
		data1.inventory.petequip.swap(data2.inventory.petequip);

		std::swap(data1.storehouse.capacity, data2.storehouse.capacity);
		std::swap(data1.storehouse.capacity2, data2.storehouse.capacity2);
		std::swap(data1.storehouse.money, data2.storehouse.money);
		data1.storehouse.items.swap(data2.storehouse.items);
		data1.storehouse.items2.swap(data2.storehouse.items2);
		data1.logs.swap(data2.logs);*/
		data1 = data2;
	}
};

enum FUNGAME_LIMIT
{
	FUN_HALL_MEMBER_MAX 	= 600,
};

#ifdef USE_LOG2 
        #define LogProto LOG::Log
#else
        #define LogProto
#endif

#define QPUT_ACHIEVEMENT    0x00000010
#define GET_ACHIEVEMENT	   0x00000040
enum 
{
	GAME_TYPE_NO	= 0, //��Ϸ���
	GAME_TYPE_LANDLORD = 1, //��������Ϸ 
	GAME_TYPE_CARD = 2, //������Ϸ
};

enum //flag value in playerlogin_re.hpp
{
	DS_TO_CENTRALDS = 1,
	CENTRALDS_TO_DS = 2,
	DIRECT_TO_CENTRALDS = 3,
	DIRECT_TO_RAID = 4,//direct login raid line ԭ���������븱��
	DS_TO_BATTLENPC = 5, 	//ֱ�ӽ�����ս��������ͼ
	DS_TO_BATTLEMAP = 6, 	//ֱ�ӽ�����ս����ͼ
	DIRECT_TO_CNETRALMAP = 7,// direct login ����������ս���򸱱�
	//DS_TO_OPENBANQUET = 8, // ԭ����¼�������ˮ?
};

inline bool IsTransToCentralDS(char flag) 
{ 
	return (flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLENPC || flag == DS_TO_BATTLEMAP) ? true : false;
}
inline bool IsTransToNormalDS(char flag)
{
	return (flag == CENTRALDS_TO_DS) ? true : false;
}

enum
{
	KICKOUT_LOCAL	= 1,
	KICKOUT_REMOTE	= 2,
};

enum GS_ATTRIBUTE_MASK
{
	GS_ATTR_HIDE	= 0x01,
	GS_ATTR_CENTRAL = 0x02,
};

enum GATE_ATTR_MASK
{
	GT_SERVER_CONNECTED = 0x01,
};

enum ErrCode_Local  //��../rpc/errcode.h����� enum ErrCode
{
	ERR_IDENTITY_TIMEOUT       = 57, //��� ��֤�볬ʱ
	ERR_IDENTITY_NOTMATCH      = 58, //��� ��֤�벻ƥ��
	ERR_REMOTE_VERIFYFAILED    = 59, //��� �����������ݴ���
	ERR_CHG_GS_STATUS	   = 67, //��ǰ״̬���޷�ִ�д˲���
	ERR_CDS_COMMUNICATION	   = 90, //������������ά��
	ERR_DELROLE_SECT	   = 91, //��ʦͽ��ϵ����²���ɾ��
	ERR_FORBIDROLE_GLOBAL	   = 92, //Ԥע��׶ν�ֹ��½��ɫ

	ERR_FC_CHANGE_KINGDOM      = 126, //�����ﲻ�ܴ�λ���ɢ
	ERR_FC_RECRUIT_FAMILY_CD   = 127, //���������ȴ
};

enum
{
	WEB_ACT_GET_SEND_TOP = 0,
	WEB_ACT_GET_RECV_TOP = 1,
	WEB_ACT_GET_CASH_AVAIL = 2,
};

enum AuReqType
{
	AU_REQ_FASTPAY = 4, //���֧��
	AU_REQ_FASTPAY_BINDINFO = 6, //������Щ���֧���̼�
	AU_REQ_TOUCH_POINT = 8, //��ȡ touch ��
	AU_REQ_TOUCH_POINT_EXCHG = 9, //�һ� touch ��
	AU_REQ_FLOWER_TOP = 10, //��վ����Ϸ��ȡ�ʻ���
	AU_REQ_BROADCAST = 11, //��Ϸ��ȫ��ȫ���㲥��Э��
	AU_REQ_PASSPORT_CASHADD = 12, //��ȡͨ��֤���ۻ���ֵ��
};

enum 
{
	USER_TABLE_CRUSHED = 0x01,
	STATUS_TABLE_CRUSHED = 0x02,
};

enum ETopFlowerStatus
{
	TOPFLOWER_OPT_OK = 0, // ִ�гɹ�
	TOPFLOWER_NOTREADY = 1,// δ׼����
	TOPFLOWER_READY = 2,// ׼����
};

enum ETopFLowerErr
{
	TOPFLOWER_LOAD_TOP_SUCCESS = 0,// ����top500��ɹ�
	TOPFLOWER_LOAD_ADD_SUCCESS = 1,// ���ظ��Ӱ�ɹ�
	TOPFLOWER_LOAD_TOP_ERR = 2, // ����top500ʧ��
	TOPFLOWER_LOAD_ADD_ERR = 3, // ���ظ��Ӱ�ʧ��
	TOPFLOWER_LOAD_END = 4, // ���ؽ���	

	TOPFLOWER_FLOWER_TAKEOFF_SUCCESS,// ���ʻ��ɹ�
	TOPFLOWER_FLOWER_TAKEOFF_ERR,//���ʻ�ʧ��
	TOPFLOWER_FLOWER_GIFT_TAKE_SUCCESS, // ��ȡ��Ʒ�ɹ�
	TOPFLOWER_FLOWER_GIFT_TAKE_ERR,// ��ȡ��Ʒʧ��

	TOPFLOWER_GETROLEID_SUCCESS, // �õ�roleid�ɹ�
	TOPFLOWER_GETROLEID_ERR_OLDNAME, // ʹ�õ���������
	TOPFLOWER_ROLEID_NOT_EXIST,// roleid ������
	TOPFLOWER_ROLEID_EXIST, // roleid ����
	TOPFLOWER_UPDATA_DATA_SUCCESS,// �����ʻ��ɹ�
	TOPFLOWER_UPDATA_DATA_ERR, // �����ʻ�ʧ��
};

enum EClientSrvFlowerErr
{
	S2C_TOPFLOWER_NAME_ERR = 0, // ���ֲ���
	S2C_TOPFLOWER_GENDER_ERR = 1, // �Ա𲻶�
	S2C_TOPFLOWER_TOP_NOTREADY = 2, // ��û��׼����
	S2C_TOPFLOWER_SUCESS = 3, // �ɹ�
	S2C_TOPFLOWER_DB_ERR = 4, // ���ݿ�ִ�й����в����Ĵ���,δ֪��
	S2C_TOPFLOWER_ROLEID_NOT_EXIST = 5, // roleid������
	S2C_TOPFLOWER_GIFT_TAKE_SUCESS = 6,// ��ȡ����ɹ�
	S2C_TOPFLOWER_GIFT_TAKE_ERR = 7, // ��ȡ����ʧ��
	S2C_TOPFLOWER_GIFT_NOT_IN_TOP = 8, // δ�����а���
	S2C_TOPFLOWER_GIFT_HAS_TAKE = 9, // �Ѿ���ȡ����
	S2C_TOPFLOWER_GIFT_TIME_NOT = 10, // �콱ʱ��δ��
};

enum EClientSrvOpenBanquetEnterNotify
{
	S2C_OPENBANQUET_ENTER_OK = 0, // ����ɹ�
	S2C_OPENBANQUET_ENTER = 1, // ʧ��
};

enum EClientSrvOpenBanquetJoinNotify
{
	S2C_OPENBANQUET_JOIN = 0, // ֱ�ӽ���
	S2C_OPENBANQUET_QUEUE = 1, // ��Ҫ�Ŷ�
};


enum KINGDOM_TITLE
{
	KINGDOM_TITLE_NONE = 0,
	KINGDOM_TITLE_KING,
	KINGDOM_TITLE_QUEEN,
	KINGDOM_TITLE_GENERAL,
	KINGDOM_TITLE_MINISTER,
	KINGDOM_TITLE_MEMBER,
	KINGDOM_TITLE_GUARD,
};

enum
{
	KINGDOM_DEFENDER_WIN = 1,
	KINGDOM_ATTACKER_WIN,
};

enum
{
	KINGDOM_TASK_NONE = 0,		//δ�������� task_type=0 issu_time��ȷ��
	KINGDOM_TASK_OPEN,		//���񷢲��� task_type!=0 issue_time!=0
	KINGDOM_TASK_WAIT_BALANCE,	//��������ȴ����� task_type=0 issue_time!=0
};

enum
{
	KINGDOM_PRIVILEGE_ANNOUNCE = 0x01,  //��ּ	
	KINGDOM_PRIVILEGE_APPOINT  = 0x02,  //����ְλ
	KINGDOM_PRIVILEGE_SET_GAMEATTR = 0x04, //������������
	KINGDOM_PRIVILEGE_OPEN_BATHPOOL = 0x08, //��������

	KING_PRIVILEGE_INIT = KINGDOM_PRIVILEGE_ANNOUNCE|KINGDOM_PRIVILEGE_APPOINT|KINGDOM_PRIVILEGE_SET_GAMEATTR,
	QUEEN_PRIVILEGE_INIT = 0x0, //�ʺ�
	FUNC_PRIVILEGE_INIT = 0x0, //���� �� 
};

enum //���ݿ� config ���и��� key ��ʾ����
{
	//100 ��ռ�� DBConfig
	CONFIG_KEY_TOUCH_ORDER = 101,  //��¼ touch �һ�������
};

enum //����ȫ�ֱ���ID
{
	COMMON_DATA_KINGTASK_WINPOINTS = 30353,  //�ݴ��������������Ϊ����Ӯ�õĹ��һ��� 
	COMMON_DATA_KINGTASK_ISSUETIME = 10070, //��¼�������������ʱ��
};

enum
{
	KINGDOM_TASK_POINTS_LIMIT = 10000,  //�����������������Ϊ����Ӯȡ�Ļ�������
	KINGDOM_REWARD_WIN_TIMES = 6, //ÿ 6 �����λ��ý���
};
enum
{
	WEEK_SECONDS = 604800,  //һ�ܵ�����
	DAY_SECONDS =   86400,	//һ�������
};
enum //������Ϣ�����ʽ������ channel=CHANNEL_SPECIAL_TYPE ���
{
	OFFLINE_MSG_ACACCUSE_REP = 1, //��Ҿٱ���ҷ���
};

enum ERR_OPENBANQUET
{
	ERR_OPB_NOT_OPEN				= 1,	// ս��δ����
	ERR_OPB_TEAM_INFO_FAIL				= 2,	// �������Ʋ���ȷ(1-6��)
	ERR_OPB_MAP_NOTEXIST				= 3,	// û���ҵ���ͼ
	ERR_OPB_LEVEL_LIMIT				= 4,    // �û��ȼ�������ս������
	ERR_OPB_APPOINT_TARGETNOTFOUND 			= 5,	// ���ܴ�λ���˲�����
	ERR_OPB_ACCEPT_REQUESTERNOTFOUND		= 6,	// �������˲�����
	ERR_OPB_ENTER_MAXTIMES				= 7,    // ���������������
	ERR_OPB_HAS_QUEUE				= 8,    // �Ѿ����Ŷ���
	
	ERR_OPB_TIMEOUT 				= 20, 	// GS����ʱ
	ERR_OPB_ROLE_NOT_ONLINE				= 21,   // ��ɫδ����
	ERR_OPB_ROLE_NOT_FIND_WAIT			= 22,   // ����ս��ʱ����ɫδ�ڵȴ�������
	ERR_OPB_ROLE_NOT_IN_BATTLE			= 23,	// ���δ��ս����
};

enum ERR_IWEB_CMD
{
	ERR_IWEBCMD_SUCCESS		= 0, 	//�ɹ�
	ERR_IWEBCMD_INVALID_OP_TYPE	= 1,	//����Ĳ�������
	ERR_IWEBCMD_DATA_NOTREADY	= 2,	//���ݼ���δ���
	ERR_IWEBCMD_UNMARSHAL		= 3,    //������ unmarsal ʧ��
	ERR_IWEBCMD_CMDSIZE		= 4, 	//�����볤�ȴ���
	ERR_IWEBCMD_INTEGRITY		= 5, 	//�����벻����
	ERR_IWEBCMD_PARSE		= 6, 	//���������ʧ��
	ERR_IWEBCMD_TIMESTAMP		= 7,	//�������ѹ���
	ERR_IWEBCMD_COMMUNICATION	= 8,	//�����ڼ�ͨ�Ŵ���
	ERR_IWEBCMD_DB			= 9, 	//���ݿ����
};

enum SERVICE_FIRBID_TYPE
{
	SERVICE_FORBID_DELIVERY_PRO = 101, //gdeliveryd ���� glinkd ������Э��
};
enum
{
	WEB_ORDER_UNPROCESSED = 0,
	WEB_ORDER_PROCESSED = 1,
};
enum
{
	WEB_ORDER_PAY_COUPON = 1, //��ȯ֧�� web �̳Ƕ���
	WEB_ORDER_PAY_CASH = 2, //Ԫ��֧�� web �̳Ƕ���
};
enum
{
	ERR_WEBORDER_ARG_INVALID = 101, //��������
	ERR_WEBORDER_CASH_NOTENGOUTH = 102, //Ԫ������
	ERR_WEBORDER_COMMUNICATION = 103, //�����ڼ�ͨ�Ŵ���
	ERR_WEBORDER_INVALID_ROLE = 104, //��ɫ������
	ERR_WEBORDER_REPEAT = 105, //��Ϸ�ж��ظ����� roleid һ��
	ERR_WEBORDER_ORDERID = 106, //��Ϸ�ж϶������ظ� roleid ��һ��
	ERR_WEBORDER_USERID = 107, //��ɫ���˺Ų�ƥ��
	ERR_WEBORDER_MAXSIZE = 108, //�������Ѵ�����
	ERR_WEBORDER_ROLE_STATUS = 109, //��ɫ״̬����
};
enum
{
	WEB_ORDER_MAXSIZE = 30, //���������� 30 ��δ��ȡ����
};

// User �� data_group��ö��  Key ֵһ������ �����޸�
enum USER_DATA_GROUP
{
	USER_DATA_CASHADD2	= 1,
};
//deliveryd ʹ��;  GRoleBase �� datagroup��ö��; Key ֵһ������ �����޸�
enum BASE_DATA_GROUP
{
	KEY_DATAGOURP_ROLE_CT_COOLTIME = 1, // ���ս���뿪ս����ȴcd
	KEY_DATAGOURP_ROLE_CT_TEAMID = 2, // ���ս��id
	KEY_DATAGOURP_ROLE_CT_SCORE = 3, // ���ս�Ӹ��˻���
	KEY_DATAGOURP_ROLE_CT_MAX_SCORE = 4, // ��ǰ��߻���,������GRoleInfo
	KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE = 5, // ������߻���
	KEY_DATAGOURP_ROLE_CT_SCORE_UPDATETIME = 6, //��ǰ���ָ���ʱ��,������GRoleInfo
	KEY_DATAGOURP_ROLE_CT_LAST_PUNISH_TIME = 7, // ���ĳͷ�����ʱ��
	KEY_DATAGOURP_ROLE_CT_LAST_BATTLE_COUNT = 8, // ���ܴ�ó���
};
//GFactionInfo.datagroup �е�ö�� Key ֵһ������ �����޸�
enum FAC_DATA_GROUP
{
	FAC_DATA_MULTI_EXP = 1,		//���ɶ౶���鱶��
	FAC_DATA_MULTI_EXP_ENDTIME,	//���ɶ౶�������ʱ��
};
enum
{
	CT_DIRTY_ROLE_DB = 0x01, //��ʶս�������Ϣд��
	CT_DIRTY_TEAM_DB = 0x02,//��ʶս�Ӷ�����Ϣд��
	CT_DIRTY_SCORE_DB = 0x04,   //��ʶ��ҵ�ǰ����д��
	CT_DIRTY_ROLE_CENTRAL = 0x08,   // ��ʶս�����ͬ�����
	CT_DIRTY_TEAM_CENTRAL = 0x10,   //��ʶս�Ӷ���ͬ�����
	CT_DIRTY_SCORE_CENTRAL= 0x20,	// ��ʶ��һ���ͬ����� 
	CT_DIRTY_ROLE_ONLINE_CENTRAL = 0x40, // �������״̬ͬ��,���⴦��
};

/*
enum
{
	CT_DIRTY_ROLE_SCORE = 0x01,
	CT_DIRTY_ROLE_JOINBATTLE_TIME = 0x02,// ���һ�β���ս����ʱ�� 
	CT_DIRTY_ROLE_BATTLE_TIMES = 0x04, // ��ó���

};
*/
//gs ʹ��; GRoleBase2 �� datagroup��ö��; Key ֵһ������ �����޸�
enum BASE2_DATA_GROUP
{
	BASE2_DATA_FAC_COUPON_ADD = 1,  //���ɽ�ȯ�ۼ�ֵ
	BASE2_DATA_FAC_COUPON,  //���ɽ�ȯ
	BASE2_DATA_USED_TITLE,
};

enum
{
	// CrssvrTeams 300-340
	ERR_CRSSVRTEAMS_NOT_OPEN 		= 300, // ս��δ����
	ERR_CRSSVRTEAMS_TEAMID_INVAILD 		= 301, // teamid ���� 
	ERR_CRSSVRTEAMS_DATA_IN_LOADING 	= 302, // ս��������loading�� 
	ERR_CRSSVRTEAMS_NOTEXIST 		= 303, // ս�Ӳ����� 
	ERR_CRSSVRTEAMS_CHANGENAME_CD		= 304, // ��������̫Ƶ����cd��
	ERR_CRSSVRTEAMS_CREATE_DUP 		= 305, // �����ظ�
	ERR_CRSSVRTEAMS_INVALIDNAME		= 306, // ���ֲ��Ϸ�
	ERR_CRSSVRTEAMS_NOT_MASTER              = 307, // ���Ƕӳ������ܽ��д˲���
	ERR_CRSSVRTEAMS_FULL			= 308, // ս������
	ERR_CRSSVRTEAMS_CHECKCONDITION     	= 309, // ����������������ʽ𲻹�,�ڿ����
	ERR_CRSSVRTEAMS_OFFLINE			= 310, // ��Ҳ�����
	ERR_CRSSVRTEAMS_LEAVE_COOLDOWN		= 311, // ����뿪ս�ӣ�1�������ټ���
	ERR_CRSSVRTEAMS_ROLE_NOT_IN		= 312, // ��Ҳ�����ս����
	ERR_CRSSVRTEAMS_JOIN_REFUSE		= 313, // �ܾ�����ս��
	ERR_CRSSVRTEAMS_DBFAILURE       	= 314, // ���ݿ�IO����
	ERR_CRSSVRTEAMS_DB_TIMEOUT              = 315, // ���ݿ�ִ�г�ʱ
	ERR_CRSSVRTEAMS_DB_INTEAMS              = 316, // ����Ѿ���ս����
	ERR_CRSSVRTEAMS_DB_NOTIN_TEAMS		= 317, // �����ս�����ݿ���δ�ҵ�
	ERR_CRSSVRTEAMS_JOIN_INVITEE_IN 	= 318, // ���������Ѿ��ڶ�����
	ERR_CRSSVRTEAMS_JOIN_NOT_REBORN 	= 319, // ��ҵȼ�û�дﵽ����135
	ERR_CRSSVRTEAMS_JOIN_OCCUP_CONT		= 320, // ĳ��ְҵ����>2������ļ

	ERR_CRSSVRTEAMS_DISMISS_OK		= 321, // ���Ķ����ѽ�ɢ
	ERR_CRSSVRTEAMS_CROSS_CREATE_FAIL	= 322, // ���ͬ�����ݣ�����ս��ʧ��
	ERR_CRSSVRTEAMS_CROSS_UPDATE_ROLE_FAIL  = 323, // ���ͬ�����ݣ�����ս�������Ϣʧ��
	ERR_CRSSVRTEAMS_NAME_NOT_FIND		= 324, // ��Ҳ����߻�������������
	ERR_CRSSVRTEAMS_CROSS_DISCONNECT	= 325, // ���δ���ӣ����ܽ��д������
	ERR_CRSSVRTEAMS_INVITE_JOIN_OK		= 326, // ����ս�ӳɹ�
	ERR_CRSSVRTEAMS_QUIT_TEAM		= 327, // XX�˳�ս��
	ERR_CRSSVRTEAMS_KICK_TEAM		= 328, // ���ѱ��ӳ��߳�ս��
	ERR_CRSSVRTEAMS_CHG_CAPTAIN		= 329, // **���ӳ�ת�Ƹ���
};

enum
{
	CRSSVRTEAMS_OFFLINE_STATUS = 0, // ������
	CRSSVRTEAMS_SRC_ONLINE_STATUS = 1, // ԭ������
	CRSSVRTEAMS_CROSS_ONLINE_STATUS = 2, // �������

};

enum
{
	CRSSVRTEAMS_DAILY_TOP_TYPE   = 1, // �����а�����
	CRSSVRTEAMS_WEEKLY_TOP_TYPE  = 2, // �����а�����
	CRSSVRTEAMS_SEASON_TOP_TYPE  = 3, // ����
};

enum
{
	RAID_ATTARCKER_WIN = 1, // ����ʤ
	RAID_DEFENDER_WIN = 2, // �ط�ʤ
	RAID_DRAW = 3, // ƽ
};

enum
{
	FAC_DIRTY_NORMAL = 0x01, //��ʶ��ͨ�ֶε�д��
	FAC_DIRTY_DYNAMIC = 0x02,//��ʶ���ɶ�̬��Ϣ��д��
//	FAC_DIRTY_MSGS = 0x04,   //��ʶ�������Ե�д��
	FAC_DIRTY_ACT = 0x08,    //��ʶ���ɻ�Ծ�ȵ�д��
};
enum
{
	FAC_ACT_START_LEV = 3, //4���￪ʼ�����Ծ��
//	FAC_ACT_LIMIT = 100000,
};
enum
{
	ST_FAC_BASE_OPEN = 0x01,
	ST_FAC_BASE_START = 0x02, //���״̬���������db�� ��ʾ�����ѷ���gs ���Խ���
};

enum
{
	ERR_FAC_BASE_ALREADY_OPEN = 1, //���ɻ����Ѿ����� �����ظ�����
	ERR_FAC_BASE_CREATE_LEV = 2, //�����ļ� ���ܴ�������
	ERR_FAC_BASE_CREATE_DB = 3, //�������� ���ݿ����
	ERR_FAC_BASE_START_STATUS = 4, //��������״̬����
	ERR_FAC_BASE_START_FULL = 5, //�����Ѵ����� ���ܿ���
	ERR_FAC_BASE_ENTER_NOTOPEN = 6, //����δ���� �޷�����
	ERR_FAC_BASE_LOGIC = 7, //�߼�����
	ERR_FAC_BASE_CREATE_DELETING = 8, //����Ϊɾ��״̬ ���ܴ�������
};

enum CHG_GS_REASON //�� gs player_imp.h �е� CHANGE_GS_REASON ͬ���޸�
{
	CHG_GS_REASON_NORMAL = 0, //��ͨ�л�gs
	CHG_GS_REASON_VOTE_LEAVE_RAID = 1, //ͶƱ�߳�����
	CHG_GS_REASON_COLLISION_RAID = 2, //������ײ����
	CHG_GS_REASON_LEAVE_RAID = 3, //�뿪����
	CHG_GS_REASON_ENTER_FBASE = 4, //������ɻ��� ���ͻ�����
};

enum TopicSiteType
{
	TS_ADD_FACTION_TYPE = 1,
	TS_ADD_FAMILY_TYPE = 2,
	TS_DELETE_FACTION_TYPE = 3,
	TS_DELETE_FAMILY_TYPE = 4,
	//TS_ADDFRIEND_TYPE = 5,
};

// ׽�Բ��ั��
enum HideSeekType
{
	HST_HIDESEEK = 1, // ׽�Բظ���
	HST_FENGSHEN = 2, // ����150-160����
};




//by 2022 add

//	Move mode
enum
{
	GP_MOVE_WALK	= 0,
	GP_MOVE_RUN		= 1,
	GP_MOVE_STAND	= 2,
	GP_MOVE_FALL	= 3,
	GP_MOVE_SLIDE	= 4,
	GP_MOVE_PUSH	= 5,
	GP_MOVE_FLYFALL	= 6,
	GP_MOVE_RETURN	= 7,
	GP_MOVE_JUMP	= 8,
	GP_MOVE_QINGGONG= 9,
	GP_MOVE_PULLED	= 10,		//	����״̬���ƶ� Added 2011-07-29.
	GP_MOVE_MASK	= 0xf,

	GP_MOVE_TURN	= 0x10,		//	Turnaround
	GP_MOVE_DEAD	= 0x20,

	GP_MOVE_AIR		= 0x40,
	GP_MOVE_WATER	= 0x80,
	GP_MOVE_ENVMASK	= 0xc0,
};

//	Leave team reason
enum
{
	GP_LTR_LEAVE = 0,		//	Normal leave
	GP_LTR_KICKEDOUT,		//	Was kicked out
	GP_LTR_LEADERCANCEL,	//	Leader cancel the team
};

//	Team member pickup flag
enum
{
	GP_TMPU_RANDOM = 0,
	GP_TMPU_FREEDOM,
};

//	NPC service type
enum
{
	GP_NPCSEV_BUY = 1,			//	1, NPC buy from player
	GP_NPCSEV_SELL,				//	NPC sell to player
	GP_NPCSEV_REPAIR,
	GP_NPCSEV_HEAL,
	GP_NPCSEV_TRANSMIT,			//	Transmit to somewhere
	
	GP_NPCSEV_TASK_RETURN,		//	6, Return task
	GP_NPCSEV_TASK_ACCEPT,		//	Accept task
	GP_NPCSEV_TASK_MATTER,		//	Task matter
	GP_NPCSEV_LEARN,			//	Learn skill
	GP_NPCSEV_EMBED,			//	Embed stone

	GP_NPCSEV_CLEAR_TESSERA,	//	11, Clear tessear
	GP_NPCSEV_MAKEITEM,
	GP_NPCSEV_RESET_PK_VALUE,
	GP_NPCSEV_TRASHPSW,			//	Change trash password
	GP_NPCSEV_OPENTRASH,		//	Open trash

	GP_NPCSEV_OPENFACTIONTRASH,	//	16
	GP_NPCSEV_IDENTIFY,			//	Identify item
	GP_NPCSEV_FACTION,			//	About faction
	GP_NPCSEV_BOOTHSELL,		//	Player booth sell		
	GP_NPCSEV_TRAVEL,			//	Travel

	GP_NPCSEV_BOOTHBUY,			//	21, Player booth buy
	GP_NPCSEV_WAYPOINT,
	GP_NPCSEV_FORGETSKILL,
	GP_NPCSEV_FACECHANGE,
	GP_NPCSEV_MAIL,

	GP_NPCSEV_VENDUE,			//	26
	GP_NPCSEV_DBLEXPTIME,
	GP_NPCSEV_HATCHPET,
	GP_NPCSEV_RESTOREPET,
	GP_NPCSEV_BATTLE,
	
	GP_NPCSEV_BUILDTOWER,		//	31
	GP_NPCSEV_LEAVEBATTLE,
	GP_NPCSEV_RETURNSTATUSPT,
	GP_NPCSEV_ACCOUNTPOINT,
	GP_NPCSEV_BINDITEM,

	GP_NPCSEV_DESTROYBINDITEM,	//	36
	GP_NPCSEV_CANCELDESTROY,
	GP_NPCSEV_STOCK_TRANSACTION,
	GP_NPCSEV_STOCK_OPERATION,
	GP_NPCSEV_TALISMAN_REFINE,

	GP_NPCSEV_TALISMAN_UPGRADE,	// 41
	GP_NPCSEV_TALISMAN_REMOVE_OWNER,
	GP_NPCSEV_TALISMAN_COMBINE,
	GP_NPCSEV_TALISMAN_ENCHANT,
	GP_NPCSEV_WAR_CHALLENGE,

	GP_NPCSEV_WAR_CONSTRUCTION, // 46
	GP_NPCSEV_WAR_ARCHER,
	GP_NPCSEV_ADOPT_PET,
	GP_NPCSEV_FREE_PET,
	GP_NPCSEV_COMBINE_PET,
	
	GP_NPCSEV_RENAME_PET,		// 51
	GP_NPCSEV_VEHICLE_UPGRADE,
	GP_NPCSEV_ITEM_TRADE,
	GP_NPCSEV_LOCK_ITEM,
	GP_NPCSEV_UNLOCK_ITEM,

	GP_NPCSEV_REPAIR_DAMAGED_ITEM,
	GP_NPCSEV_BLEED_SACRIFICE,		//װ��Ѫ��
	GP_NPCSEV_EMBED_SOUL,			//������Ƕ
	GP_NPCSEV_CLEAR_SOUL_TESSERA,	//���ǲ��
	GP_NPCSEV_REFINE_SOUL,			//�ָ�����

	GP_NPCSEV_BREAKDOWN_EQUIP,		//61      //װ����� 
	GP_NPCSEV_MELD_SOUL,			//�����ں�
	GP_NPCSEV_PET_EQUIP_ENCHANT,	//����װ��ǿ��
	GP_NPCSEV_CHANGE_FACE,			//��������
	GP_NPCSEV_REFINE_SHAPE_CARD,	//�����û�ʯ����������������Ԫ��ǿ�������ϣ��ֻ�

	GP_NPCSEV_RECOVER_CARD_WAKAN,	//66 �ָ��û�ʯ��������������ֵ
	GP_NPCSEV_ARENA_JOIN,			//��������������ǰ�ߵ���Э��battle_jion����Э���delivery, ��ΪҪ�ڱ�����ʱ��۱���ȯ���ĳɷ���Э���GS
	GP_NPCSEV_TERRITORY_CHALLENGE,	//����ս��ս
	GP_NPCSEV_TERRITORY_ENTER,		//��������սս��

	GP_NPCSEV_TERRITORY_AWARD,		//70 ����ս����
	GP_NPCSEV_TELEPORTATION_CHARGE,	//���̳��ܷ���
	GP_NPCSEV_REPAIR_DAMAGED_ITEM2,	//�޸�������Ʒ���������
	GP_NPCSEV_UPGRADE_EQUIP,		//װ����������
	GP_NPCSEV_ONLINE_TRADE,			//���Ͻ���

	GP_NPCSEV_TRANSMIT_TO_CROSS,	//75 ���͵����������
	GP_NPCSEV_TRANSMIT_TO_NORMAL,	//�ӿ�����������ͻ���ͨ������
	GP_NPCSEV_IDENTIFY_GEM_SLOTS,	//��ʯ��ۼ�������
	GP_NPCSEV_REBUILD_GEM_SLOTS,	//��ʯ�����������
	GP_NPCSEV_CUSTOMIZE_GEM_SLOTS,	//��ʯ��۶��Ʒ���

	GP_NPCSEV_EMBED_GEMS,			//80 ��ʯ��Ƕ����
	GP_NPCSEV_REMOVE_GEMS,			//��ʯ�������
	GP_NPCSEV_UPGRADE_GEMS,			//��ʯ��Ʒ����
	GP_NPCSEV_REFINE_GEMS,			//��ʯ��������
	GP_NPCSEV_EXTRACT_GEMS,			//��ʯ��ȡ����

	GP_NPCSEV_SMELT_GEMS,			//85 ��ʯ��������
	GP_OPEN_RAID_ROOM,				//��������������� Added 2011-07-19.
	GP_JOIN_RAID_ROOM,				//�������븱��������� Added 2011-07-19.
	GP_PET_UPGRADE_WITH_XIANDOU,	//ֱ�����ɶ���������
	GP_CHANGE_NAME,					//��������

	GP_NPCSEV_CHANGE_FAMILYGUILD_NAME,	//90 ���ɡ��������
	GP_NPCSEV_TALISMAN_HOLEYLEVELU,		//��������
	GP_NPCSEV_TALISMAN_EMBEDSKILL,		//����������Ƕ
	GP_NPCSEV_TALISMAN_SKILLREFINE,		//���������ں�
	GP_NPCSEV_TALISMAN_SKILLREFINE_RESULT,	//���������ں��Ƿ����	���ã���ΪЭ��

	GP_NPCSEV_SHOP_REPUTATION,     //95  �����̵����
	GP_NPCSEV_EQUIP_UPDATE2,		//װ������Ϊ����װ, ����
	GP_NPCSEV_EQUIP_SLOT2,			//����װ�����
	GP_NPCSEV_ASTROLOGY_INSTALL,	//������Ƕ
	GP_NPCSEV_ASTROLOGY_UNINSTALL,	//����ժ��

	GP_NPCSEV_ASTROLOGY_IDENTIFY,	//100��������, ����
	GP_NPCSEV_ASTROLOGY_UPGRADE,	//��������, ����
	GP_NPCSEV_ASTROLOGY_DESTORY,	//��������, ����
	GP_NPCSEV_ACTIVE_UI_TRANSFER,   //�����󼤻�
	GP_NPCSEV_USE_UI_TRANSER,		//ʹ�ô�������

	GP_NPCSEV_TRANSFER_SERVE = 105,		// ���ͷ���
	GP_NPCSEV_TRANSFER_SERVE_BATTLE_LEAVE,	// ��ս���뿪����

	GP_NPCSEV_KINGDOM_ENTER = 107, //�����ս
	GP_NPCSEV_PRODUCE_JINFASHEN,	// �����������
	GP_NPCSEV_PET_FLY,				// �������
};

//	Player and NPC state
enum
{
    GP_STATE_SHAPE				= 0x00000001,
	GP_STATE_EMOTE				= 0x00000002,
	GP_STATE_INVADER			= 0x00000004,
	GP_STATE_FLY				= 0x00000008,
	GP_STATE_FAMILY				= 0x00000010,
    GP_STATE_SITDOWN			= 0x00000020,
    GP_STATE_EXTEND_PROPERTY	= 0x00000040,
    GP_STATE_CORPSE				= 0x00000080,

	//	Used only by player
	GP_STATE_TEAM				= 0x00000100,
	GP_STATE_TEAMLEADER			= 0x00000200,
	GP_STATE_ADV_MODE			= 0x00000400,
	GP_STATE_FACTION			= 0x00000800,
	GP_STATE_BOOTH				= 0x00001000,
	GP_STATE_FASHION			= 0x00002000,
	GP_STATE_GMFLAG				= 0x00004000,
	GP_STATE_PVP_ENABLE			= 0x00008000,
	GP_STATE_EFFECT				= 0x00010000,
	GP_STATE_INPVPCOMBAT		= 0x00020000,
	GP_STATE_IN_DUEL			= 0x00040000,	//	�Ƿ����ھ�����
	GP_STATE_IN_MOUNT			= 0x00080000,	//	���������
	GP_STATE_IN_BIND			= 0x00100000,	//	�ͱ��˰���һ��
	GP_STATE_BC_INVADER			= 0x00200000,	//	Battle camp: invader
	GP_STATE_BC_DEFENDER		= 0x00400000,	//	Battle camp: defender
	GP_STATE_TITLE				= 0x00800000,	//	�����һ����ǰ��νid, short����
	GP_STATE_SPOUSE				= 0x01000000,
	GP_STATE_SECT_MASTER_ID		= 0x08000000,
	GP_STATE_IN_SJBATTLE		= 0x10000000,	//  ���ν�ս����
	GP_STATE_IN_VIPSTATE		= 0x20000000,	//	ʹ����vip��Ƭ
	GP_STATE_IS_OBSERVER		= 0x40000000,	//  ���pk�Ĺ�ս��ģʽ
	GP_STATE_TRANSFIGURE		= 0x80000000,	//  ����״̬
	
	//	Used only by NPC
	GP_STATE_NPC_DELAYDEAD		= 0x00000008,
	GP_STATE_NPC_ADDON1			= 0x00000100,
	GP_STATE_NPC_ADDON2			= 0x00000200,
	GP_STATE_NPC_ADDON3			= 0x00000400,
	GP_STATE_NPC_ADDON4			= 0x00000800,
	GP_STATE_NPC_ALLADDON		= 0x00000F00,
	GP_STATE_NPC_PET			= 0x00001000,	//	Pet flag
	GP_STATE_NPC_NAME			= 0x00002000,
	GP_STATE_NPC_SUMMON			= 0x00004000,	//  �����ٻ���
	GP_STATE_NPC_OWNER			= 0x00008000,	//  �й����Ĺ���
	GP_STATE_NPC_CLONE			= 0x00010000,	//  NPC�Ƿ����ͻ�����ʱ�������״̬
	GP_STATE_NPC_DIM			= 0x00020000,	//  NPC����
	GP_STATE_NPC_FROZEN			= 0x00040000,	//	NPC����״̬
	GP_STATE_NPC_DIR_VISIBLE	= 0x00080000,	//	ʮ�ַ�����Ч��״̬
	GP_STATE_NPC_INVISBLE       = 0x00100000,   //  NPC����
	GP_STATE_NPC_TELEPORT1      = 0x00200000,   //  ������NPC��ף����
	GP_STATE_NPC_TELEPORT2      = 0x00400000,   //  ������NPC�����䣩
	GP_STATE_NPC_PHASE			= 0x00800000,	//  ��λ���NPC
};

enum
{
	GP_STATE_MATTER_OWNER		= 0x00000004,	// �й����Ŀ���
	GP_STATE_MATTER_PHASE       = 0x00000008,   // ����λ��ǵ�MATTER
	GP_STATE_COMBINE_MINE       = 0x00000020,   // ���Ͽ�
};

enum
{
	GP_EXTRA_STATE_CARRIER			= 0x00000001,	//  �������������
	GP_EXTRA_STATE_ANONYMOUS		= 0x00000002,	//  ��Ҵ���������ͼ
	GP_EXTRA_STATE_INVISIBLE		= 0x00000004,	//  ��������Լ����Լ��ǰ�͸���ģ���������ܿ����Լ��Ļ������˿��Լ�Ҳ�ǰ�͸���ģ�
	GP_EXTRA_STATE_DIM				= 0x00000008,	//  �������ʣ�Ч���ǰ�͸�����Լ������߱��˿�Ч�����ǰ�͸����
	GP_EXTRA_STATE_CLONE			= 0x00000010,	//  ����һ������
	GP_EXTRA_STATE_CIRCLE			= 0x00000020,	//  Ȧ����Ϣ
	GP_EXTRA_STATE_CROSSZONE		= 0x00000040,	//  ����
	GP_EXTRA_STATE_IGNITE			= 0x00000080,	//  ��ȼ״̬
	GP_EXTRA_STATE_FROZEN			= 0x00000100,   //  ����״̬
	GP_EXTRA_STATE_FURY				= 0x00000200,	//  ��ʥ֮ŭ״̬
	GP_EXTRA_STATE_DARK				= 0x00000400,	//  �ڰ�֮��
	GP_EXTRA_STATE_COLDINJURE		= 0x00000800,	//  ����״̬
	GP_EXTRA_STATE_CIRCLEOFDOOM		= 0x00001000,	//	��״̬��ֻ�������ۣ��󷨷����ߣ����ϲ��� Added 2011-06-22.
	GP_EXTRA_STATE_BE_DRAGGED		= 0x00010000,	//	��ǣ��״̬ //Added 2011-08-26.���漸���������ķֱ��ǣ���ת���棬��ת���棬��Ѫ״̬
	GP_EXTRA_STATE_BE_PULLED		= 0x00020000,	//	������״̬ // Added 2011-08-26.
	GP_EXTRA_STATE_VIP_AWARD		= 0x00040000,	//	VIP����״̬ Added 2012-01-16.
	GP_EXTRA_STATE_COLLISION_RAID	= 0x00080000,	//	������ײ����ʱ��״̬	Added 2012-08-01.
	GP_EXTRA_STATE_XY_DARK_LIGHT	= 0x00100000,	//	��ԯ�ⰵ״̬	Added 2012-08-16.
	GP_EXTRA_STATE_XY_SPIRITS		= 0x00200000,	//	��ԯ����
	GP_EXTRA_STATE_MIRROR_IMAGE		= 0x00400000,	//	����
	GP_EXTRA_STATE_CHANGE_WING_COLOR= 0x00800000,	//	�ɽ���ɫ�仯 Added 2012-08-20.
	GP_EXTRA_STATE_DIR_VISIBLE_STATE= 0x01000000,	//	ʮ�ַ�����Ч��״̬
	GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT = 0X02000000,	//	װ�������Ч
	GP_EXTRA_STATE_FLOW_BATTLE = 0X04000000,		//	��ˮϯ״̬
	GP_EXTRA_STATE_PUPPET			= 0x08000000,   //  ������̬
	GP_EXTRA_STATE_INTERACT			= 0x20000000,	//	����״̬
	GP_EXTRA_STATE_ACTIVE_EMOTE		= 0x40000000,	//	��������
	GP_EXTRA_STATE_TRAVEL			= 0x80000000,	//	���ɷ���
};
//	Chat channel
enum
{
	//���ö�ٶ�������Ķ��Ļ���Ҫ֪ͨ�����ˣ���������������Ա
	//��Ǳ�Ҫ���������ں�����ӣ�����ɾ���͸���ǰ��channel��˳��
	GP_CHAT_LOCAL = 0,
	GP_CHAT_FARCRY,
	GP_CHAT_TEAM,
	GP_CHAT_FACTION,
	GP_CHAT_WHISPER,
	GP_CHAT_DAMAGE,
	GP_CHAT_FIGHT,
	GP_CHAT_TRADE,
	GP_CHAT_SYSTEM,
	GP_CHAT_BROADCAST,
	GP_CHAT_MISC,
	GP_CHAT_FAMILY,
	GP_CHAT_CIRCLE,
	GP_CHAT_ZONE,
	GP_CHAT_SPEAKER,
	GP_CHAT_RUMOR,		// ���������ȰƵ�
	GP_CHAT_MAX,
};

//	The reason of droping item
enum
{
	GP_DROP_GM,
	GP_DROP_PLAYER,
	GP_DROP_TAKEOUT,
	GP_DROP_TASK,
	GP_DROP_RECHARGE,
	GP_DROP_DESTROY,
	GP_DROP_DEATH,
	GP_DROP_PRODUCE,
	GP_DROP_DECOMPOSE,
	GP_DROP_TRADEAWAY,
	GP_DROP_RESURRECT,
	GP_DROP_USED,
	GP_DROP_CONSUMERUNE,
	GP_DROP_EXPIRED,
	GP_DROP_BIND_DISAPPEAR,
	GP_DROP_DAMAGED,
};

//	Cool time index
enum
{
	GP_CT_NULL = 0,					//	�գ�����
	GP_CT_EMOTE,                   	//	���������ȴʱ�� 
	GP_CT_DROP_MONEY,              	//	��������Ǯ����ȴʱ��
	GP_CT_DROP_ITEM,               	//	����������Ʒ����ȴʱ��
	GP_CT_SWITCH_FASHION,          	//	�л�ʱװģʽ����ȴʱ��
	// 5
	GP_CT_TRANSMIT_ROLL,			//	���;���
	GP_CT_VIEWOTHEREQUIP,			//	�鿴���˵�װ��
	CP_CT_FIREWORKS,				//	ʩ���̻�
	CP_CT_FARCRY,					//	far cry Ƶ��˵��
	CP_CT_SKILLMATTER,				//  ������Ʒ
	// 10
	CP_CT_DOUBLEEXPMATTER,			//  ˫���������
	GP_CT_SKILLMATTER1,				//	������Ʒ��ȴʱ������1
	GP_CT_SKILLMATTER2,				//	������Ʒ��ȴʱ������2
	GP_CT_SKILLMATTER3,				//	������Ʒ��ȴʱ������3
	GP_CT_RESERVED,					//	����
	// 15
	GP_CT_VEHICLE,					//	�ٻ���ͨ����
	GP_CT_COUPLE_JUMPTO,			//	����ͬ�Ľ�
	GP_CT_LOTTERY,					//	��Ʊ
	GP_CT_CAMRECORDER,				//	¼�����   
	GP_CT_CAMRECORDCHECK,			//	¼����֤			
	// 20
	GP_CT_TEAM_RELATION,
	GP_CT_CD_ERASER,
	GP_CT_PET_COMBINE1,				//	����ͨ��			
	GP_CT_PET_COMBINE2,				//	��������			
	GP_CT_PET_PICKUP,				//	ʰȡ
	// 25
	GP_CT_SET_FASHION_MASK,			//	
	GP_CT_GET_BATTLEINFO,			//	��ȡս����Ϣ����ȴ
	GP_CT_HP,						//  ˲��hpҩƷ��ȴʱ��
	GP_CT_MP,
	GP_CT_HP_MP,
	// 30
	GP_CT_SLOW_HP,					//  ������hpҩƷ��ȴʱ��
	GP_CT_SLOW_MP,
	GP_CT_SLOW_HP_MP,
    GP_CT_QUERY_OTHER_ACHIEVMENT,	//  �鿴������ҳɾ���ȴ
    GP_CT_TRANSFIGURE,				//  ������ȴ
	// 35
	GP_CT_NEWBIE_CHAT,				//  30��������ͨƵ��˵����ȴ   
	GP_CT_CHANGE_DS,				//  ������ȴ
 	GP_CT_DP,						//  ˲��dpҩƷ��ȴʱ��
	GP_CT_SLOW_DP,					//	������dpҩƷ��ȴʱ��
	GP_CT_GET_MALL_PRICE,			//  ȡ�̳���������
	// 40
	GP_CT_GET_VIP_AWARD_INFO,		//	��ȡVIP�����б���Ϣ��ȴʱ�� Added 2012-01-04.
	COOLDOWN_INDEX_RESET_SKILL_PROP,		// 90�����¼���������ȴ
	COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT,	// 90����������������ȴ
	COOLDOWN_INDEX_BATTLE_FLAG,				// ս��ʹ����ȴ
	COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON,	// ����״��˳���Ϸ��ȴ
	// 45
	COOLDOWN_INDEX_BOTTLE_HP,
	COOLDOWN_INDEX_BOTTLE_MP,
	COOLDOWN_INDEX_BOTTLE_DP,
	GP_CT_SKILLMATTER4,
	GP_CT_SKILLMATTER5,
	//50
	COOLDOWN_INDEX_BATH,
	COOLDOWN_INDEX_KING_CALL_GUARD,
	COOLDOWN_INDEX_ARRANGE_INVENTORY,
	COOLDOWN_INDEX_ARRANGE_TRASHBOX,
	COOLDOWN_INDEX_TELEPORT1,      //ף������

	//55
	COOLDOWN_INDEX_TELEPORT2,		//���䴫��
	COOLDOWN_INDEX_CHANGE_PUPPET_FIGHT_FORM,//����ս��״̬ת��

	GP_CT_MAX,
	GP_CT_PRODUCE_START = 600,
	GP_CT_PRODUCE_END = 800,
	
	GP_CT_COMBINE_EDIT_START	= 900,
	GP_CT_COMBINE_EDIT_END		= 950,

	GP_CT_SKILL_START = 1024,
};

//	PVP mask
enum
{
	GP_PVPMASK_WHITE	= 0x0001,
	GP_PVPMASK_RED		= 0x0002,
	GP_PVPMASK_BLUE		= 0x0004,
	GP_PVPMASK_MAFIA	= 0x0008,
	GP_PVPMASK_ZONE		= 0x0010,	// ����󹥻�ͬһ�������������
	GP_PVPMASK_ALL		= 0x001F,
};

enum
{
	GP_PET_SKILL_NUM	= 16
};

//	Pet type
enum
{   
	GP_PET_CLASS_INVALID = -1,
	GP_PET_CLASS_MOUNT = 0, //	���
	GP_PET_CLASS_COMBAT,    //	ս������
	GP_PET_CLASS_FOLLOW,    //	�������
	GP_PET_CLASS_MAX,
};

//	Player camp in battle
enum
{
	GP_BATTLE_CAMP_NONE = 0,
	GP_BATTLE_CAMP_INVADER,
	GP_BATTLE_CAMP_DEFENDER,
	GP_BATTLE_CAMP_OBSERVER,	// �۲���
	GP_BATTLE_CAMP_ARENA,		// ������������Ӫ������ӪͳһΪ4
};



enum
{
    BATTLE_TYPE_NORMAL         = 0, //��ͨս��
    BATTLE_TYPE_NAMED_CRSSVR   = 1, //�������Ŀ��ս��
	BATTLE_TYPE_INSTANCING	   = 2, //����ս��	
    BATTLE_TYPE_REBORN         = 3, //����ս��
    BATTLE_TYPE_SONJIN         = 4, //�ν�ս��
    BATTLE_TYPE_SONJINREBORN   = 5, //�ν����ս��
	BATTLE_TYPE_ARENA		   = 6, //������
	BATTLE_TYPE_ARENAREBORN	   = 7, //����������
	BATTLE_TYPE_CRSSVR		   = 8,	//�������ս��
	BATTLE_TYPE_CHALLENGE	   = 9, //��սս��
	BATTLE_TYPE_CRSSVRTEAM	   = 10,//���С��pkս�� Added 2011-05-19
	BATTLE_TYPE_FLPW_CRSSVR   = 11,	//�����ˮϯ		
};
enum
{
    BATH_TYPE_NORMAL = 0, //�����ͼ 2013-04-19
};
enum COLLISION_SKILL_TYPE				//��ײ��������
{
	COLLISION_SKILL_MAXSPEED	= 0,	//���ټ���
	COLLISION_SKILL_HALFSPEED,			//���ټ���
	COLLISION_SKILL_ASHILL,				//������ɽ����
	COLLISION_SKILL_ROOT,				//������
	COLLISION_SKILL_CHAOS,				//���Ҽ���
	COLLISION_SKILL_VOID,				//���޼���
};

enum									//��ײ�����м��ܺ�״̬
{
	COLLISION_STATE_ASHILL		= 0x00000001,	//������ɽ
	COLLISION_STATE_ROOT		= 0x00000002,	//����״̬
	COLLISION_STATE_CHAOS		= 0x00000004,	//����״̬
	COLLISION_STATE_VOID		= 0x00000008,	//����״̬
};

enum
{
	MAX_WING_COLOR_COUNT		= 6,			//�ɽ���ɫ�����������Ŀ
	COLLISION_MAX_SPEED			= 15,			//��ײ����������ƶ��ٶ�
	COLLISION_PLAYER_MAX_MASS	= 1000000,		//��ײ������������������ڲ�����ɽ����
	MAX_MULTILINE_SKILL_GFX_NUM	= 6,			//�����Լ��ܹ���ʱ�����������
};

enum RAID_FACTION
{
     RF_NONE,                         //����Ӫ
     RF_ATTACKER,                     //������
     RF_DEFENDER,                     //���ط�
	 RF_VISITOR,                      //�۲���
};

enum ERR_PHONE_TOKEN
{
	ERR_PHONE_TOKEN_FAILURE = 29, //�ֻ�������֤�벻��ȷ�����
};

enum STATUS_TOKEN
{
	_STATUS_DISCONNECT,
	_STATUS_TRY_RECONNECT,
};


}; //end of GNET

#endif
