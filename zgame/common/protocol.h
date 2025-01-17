#ifndef __ONLINEGAME_COMMON_PROTOCOL_H__
#define __ONLINEGAME_COMMON_PROTOCOL_H__

#include "types.h"

#pragma pack(1)

namespace S2C
{

	namespace INFO
	{
		struct player_info_1
		{
			int cid;
			A3DVECTOR pos;
			unsigned short equip_crc;
			unsigned char  dir;		//256的定点数表示度数
			unsigned char  cls;		//职业
			unsigned char  level;		//修真级别
			unsigned char level2;
			char  pk_level;			//pk级别 -1 0 1 2 3 4 5 6
			char  cultivation;		//仙魔佛造化
			int   reputation;		//威望
			unsigned int   object_state;	//当前状态，第0位表示是否死亡
			unsigned int   extra_state;	//玩家的另外状态
			unsigned int   extra_state2;    //玩家的另外状态2
		};

		struct player_info_2			//name ,customize data 不常改变
		{
			unsigned char size;
			char data[];
		};

		struct player_info_3			//viewdata of equipments and special effect
		{
			unsigned char size;
			char  data[];
		};

		struct player_info_4			//detail
		{
			unsigned short size;
			char	data[];
		};
		struct player_info_00			//自己会经常改变的内容
		{
			short lvl;
			short dt_lvl;
			unsigned char combat_state;
			unsigned char sec_level;
			int hp;
			int max_hp;
			int mp;
			int max_mp;
			int dp;
			int max_dp;
			int rage;
		};
		struct self_info_00
		{
			short lvl;
			short dt_lvl;
			unsigned char combat_state;
			unsigned char sec_level;
			int hp;
			int max_hp;
			int mp;
			int max_mp;
			int dp;
			int max_dp;
			int talisman_stamina;
			double exp;
			double dt_exp;
			int rage;
		};

		struct npc_info
		{
			int nid;			//npc id
			int tid;			//template id
			A3DVECTOR pos;
			unsigned short seed;		// seed of customize data
			unsigned char dir;
			unsigned int  object_state;
		};

		struct npc_info_00
		{
			int hp;
			int max_hp;
		};

		struct matter_info_1
		{
			int mid;
			int tid;
			int name_id;			 //name owner id
			A3DVECTOR pos;
			unsigned char dir0;
			unsigned char dir1;
			unsigned char rad;
			unsigned char state;
			unsigned char custom;
		};

		struct matter_info_2
		{
			int mid;
			unsigned char size;
			unsigned char data[];
		};

		struct self_info_1
		{
			double exp;
			int cid;
			A3DVECTOR pos;
			unsigned short crc;
			unsigned char  dir;		//256的定点数表示度数
			unsigned char  cls;		//256的定点数表示度数
			unsigned char  level;		//修真级别
			unsigned char  level2;		//修真级别
			char  pk_level;			//pk level
			char cultivation;		//仙魔佛造化
			int   reputation;		//威望
			unsigned int state;
		};

		struct move_info
		{
			int cid;
			A3DVECTOR dest;
			unsigned short use_time;
			unsigned short speed;
			unsigned char  move_mode;
		};
		
		union  market_goods
		{
			struct 
			{
				int type;		//物品类型  如果是0 表示没有内容了
			}empty_item;
			struct 
			{
				int type;		//物品类型
				int count;		//剩余多少个 负数表示是买
				size_t price;		//单价
				int expire_date;	//过期时间
				int state;		//特殊状态
				unsigned short content_length;
				char content[];
			}item;

			struct 
			{
				int type;		//物品类型
				int count;		//剩余多少个 负数表示是买
				size_t price;		//单价
			}order_item;
		};

		struct player_fac_field
		{
			int index;
			int tid;
			int level;
			int status;
		};

		struct fac_base_prop
		{
			int grass;
			int mine;
			int monster_food;
			int monster_core;
			int cash;
			int task_id;
			int task_count;
			int task_need;
		};

		struct fac_base_auc_item
		{
			enum
			{
				ST_NILL = 0, //无效状态
				ST_BIDDING,  //拍卖进行状态
				ST_END,	     //拍卖结束状态
			};
			int idx;
			int itemid;
			int winner;
			int cost;
			int name_len;
			char playername[20];
			int status;
			int end_time;
		};

		struct fac_base_auc_history
		{
			int event_type;	
			int name_len;
			char playername[20];
			int cost;
			int itemid;
		};
	}

	enum		//CMD
	{
		PLAYER_INFO_1,
		PLAYER_INFO_2,
		PLAYER_INFO_3,
		PLAYER_INFO_4,
		PLAYER_INFO_1_LIST,

		PLAYER_INFO_2_LIST,			//5
		PLAYER_INFO_3_LIST,
		PLAYER_INFO_23_LIST,
		SELF_INFO_1,
		NPC_INFO_LIST,

		MATTER_INFO_LIST,			//10
		NPC_ENTER_SLICE,
		PLAYER_ENTER_SLICE,
		OBJECT_LEAVE_SLICE,
		OBJECT_NOTIFY_POS,		//这个命令时暂时的？
		
		OBJECT_MOVE,				//15
		NPC_ENTER_WORLD,
		PLAYER_ENTER_WORLD,
		MATTER_ENTER_WORLD,
		PLAYER_LEAVE_WORLD,
		
		NPC_DEAD,				//20
		OBJECT_DISAPPEAR,
		OBJECT_START_ATTACK,			
		SELF_STOP_ATTACK,			//
		SELF_ATTACK_RESULT,			//作废
		
		ERROR_MESSAGE,				//25	
		BE_ATTACKED,
		PLAYER_DEAD,
		BE_KILLED,
		PLAYER_REVIVAL,
		
		PICKUP_MONEY,				//30
		PICKUP_ITEM,
		PLAYER_INFO_00,
		NPC_INFO_00,
		OUT_OF_SIGHT_LIST,
		
		OBJECT_STOP_MOVE,			//35
		RECEIVE_EXP,
		LEVEL_UP,
		SELF_INFO_00,
		UNSELECT,

		SELF_ITEM_INFO,				//40
		SELF_ITEM_EMPTY_INFO,
		SELF_INVENTORY_DATA,
		SELF_INVENTORY_DETAIL_DATA,
		EXCHANGE_INVENTORY_ITEM,

		MOVE_INVENTORY_ITEM,			//45
		PLAYER_DROP_ITEM,
		EXCHANGE_EQUIPMENT_ITEM,
		EQUIP_ITEM,
		MOVE_EQUIPMENT_ITEM,	

		SELF_GET_EXT_PROPERTY,			//50
		SET_STATUS_POINT_NULL,			//已经作废
		SELECT_TARGET,
		PLAYER_EXTPROP_BASE,
		PLAYER_EXTPROP_MOVE,

		PLAYER_EXTPROP_ATTACK,			//55
		PLAYER_EXTPROP_DEFENSE,
		TEAM_LEADER_INVITE,
		TEAM_REJECT_INVITE,
		TEAM_JOIN_TEAM,

		TEAM_MEMBER_LEAVE,			//60
		TEAM_LEAVE_PARTY,
		TEAM_NEW_MEMBER,
		TEAM_LEADER_CANCEL_PARTY,
		TEAM_MEMBER_DATA,
							
		TEAMMATE_POS,				//65
		EQUIPMENT_DATA,			//用户的装备数据，影响表现
		EQUIPMENT_INFO_CHANGED,	
		EQUIPMENT_DAMAGED,		//装备损坏
		TEAM_MEMBER_PICKUP,		//队友捡起装备

		NPC_GREETING,				//70
		NPC_SERVICE_CONTENT,
		PURCHASE_ITEM,
		ITEM_TO_MONEY,	
		REPAIR_ALL,

		REPAIR,					//75
		RENEW,
		SPEND_MONEY,
		PICKUP_MONEY_IN_TRADE,
		PICKUP_ITEM_IN_TRADE,

		PICKUP_MONEY_AFTER_TRADE,		//80
		PICKUP_ITEM_AFTER_TRADE,
		GET_OWN_MONEY,
		OBJECT_ATTACK_ONCE,			//已经作废 ，完全没有用
		SELF_START_ATTACK,			//已经作废
	
		OBJECT_CAST_SKILL,			//85
		SKILL_INTERRUPTED,
		SELF_SKILL_INTERRUPTED,
		SKILL_PERFORM,
		OBJECT_BE_ATTACKED,			//已经作废 ，完全没有用

		SKILL_DATA,				//90
		PLAYER_USE_ITEM,
		EMBED_ITEM,
		CLEAR_EMBEDDED_CHIP,
		COST_SKILL_POINT,

		LEARN_SKILL,				//95
		OBJECT_TAKEOFF,
		OBJECT_LANDING,
		FLYSWORD_TIME_CAPACITY,
		OBTAIN_ITEM,

		PRODUCE_START,				//100
		PRODUCE_ONCE,
		PRODUCE_END,
		DECOMPOSE_START,
		DECOMPOSE_END,

		TASK_DATA,				//105
		TASK_VAR_DATA,
		OBJECT_START_USE,
		OBJECT_CANCEL_USE,
		OBJECT_USE_ITEM,

		OBJECT_START_USE_WITH_TARGET,		//110
		OBJECT_SIT_DOWN,
		OBJECT_STAND_UP,
		OBJECT_DO_EMOTE,
		SERVER_TIMESTAMP,

		NOTIFY_ROOT,				//115
		DISPEL_ROOT,
		PK_LEVEL_NOTIFY,
		PK_VALUE_NOTIFY,
		PLAYER_CHANGE_CLASS,

		OBJECT_ATTACK_RESULT,			//120
		BE_HURT,
		HURT_RESULT,
		SELF_STOP_SKILL,
		UPDATE_VISIBLE_STATE,

		OBJECT_BUFF_NOTIFY,			//125
		PLAYER_GATHER_START,
		PLAYER_GATHER_STOP,
		TRASHBOX_PASSWD_CHANGED,
		TRASHBOX_PASSWD_STATE,

		TRASHBOX_OPEN,				//130
		TRASHBOX_CLOSE,	
		TRASHBOX_WEALTH,
		EXCHANGE_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,
		
		EXCHANGE_TRASHBOX_INVENTORY,		//135
		INVENTORY_ITEM_TO_TRASH,
		TRASH_ITEM_TO_INVENTORY,
		EXCHANGE_TRASH_MONEY,
		ENCHANT_RESULT,

		SELF_NOTIFY_ROOT,			//140
		OBJECT_DO_ACTION,
		RECORDER_START,
		OBJECT_SKILL_ATTACK_RESULT,
		BE_SKILL_ATTACKED,

		PLAYER_SET_ADV_DATA,			//145
		PLAYER_CLR_ADV_DATA,
		PLAYER_IN_TEAM,		
		TEAM_APPLY_REQUEST,
		OBJECT_DO_EMOTE_RESTORE,

		CONCURRENT_EMOTE_REQUEST,		//150 //$$$$$$$$$$
		DO_CONCURRENT_EMOTE,			      //$$$$$$$$$$
		MATTER_PICKUP,
		MAFIA_INFO_NOTIFY,
		MAFIA_TRADE_START,
		
		MAFIA_TRADE_END,			//155
		TASK_DELIVER_ITEM,
		TASK_DELIVER_REPUTATION,
		TASK_DELIVER_EXP,
		TASK_DELIVER_MONEY,

		TASK_DELIVER_LEVEL2,			//160
		PLAYER_REPUTATION,
		IDENTIFY_RESULT,
		PLAYER_CHANGE_SHAPE,
		PLAYER_ENTER_SANCTUARY,

		PLAYER_LEAVE_SANCTUARY,			//165
		PLAYER_OPEN_MARKET,
		SELF_OPEN_MARKET,
		PLAYER_CANCEL_MARKET,
		PLAYER_MARKET_INFO,

		PLAYER_MARKET_TRADE_SUCCESS,		//170
		PLAYER_MARKET_NAME,
		PLAYER_START_TRAVEL,
		SELF_START_TRAVEL,
		PLAYER_COMPLETE_TRAVEL,

		GM_TOGGLE_INVINCIBLE,			//175
		GM_TOGGLE_INVISIBLE,
		SELF_TRACE_CUR_POS,
		OBJECT_CAST_INSTANT_SKILL,	
		ACTIVATE_WAYPOINT,

		PLAYER_WAYPOINT_LIST,			//180
		UNLOCK_INVENTORY_SLOT,
		TEAM_INVITE_PLAYER_TIMEOUT,
		PLAYER_PVP_NO_PROTECT,
		PLAYER_DISABLE_PVP,

		PLAYER_PVP_COOLDOWN,			//185
		COOLDOWN_DATA,
		SKILL_ABILITY_NOTFIY,
		PERSONAL_MARKET_AVAILABLE,
		BREATH_DATA,

		PLAYER_STOP_DIVE,			//190
		TRADE_AWAY_ITEM,
		PLAYER_ENABLE_FASHION_MODE,
		ENABLE_FREE_PVP_MODE,
		OBJECT_IS_INVALID,

		PLAYER_ENABLE_EFFECT,			//195
		PLAYER_DISABLE_EFFECT,
		ENABLE_RESURRECT_STATE,		
		SET_COOLDOWN,
		CHANGE_TEAM_LEADER,		

		KICKOUT_INSTANCE,			//200
		PLAYER_COSMETIC_BEGIN,
		PLAYER_COSMETIC_END,
		COSMETIC_SUCCESS,
		OBJECT_CAST_POS_SKILL,

		CHANGE_MOVE_SEQ,			//205
		SERVER_CONFIG_DATA,
		PLAYER_RUSH_MODE,
		TRASHBOX_CAPACITY_NOTIFY,
		NPC_DEAD_2,

		PRODUCE_NULL,				//210
		ACTIVE_PVP_COMBAT_STATE,
		DOUBLE_EXP_TIME,			//作废
		AVAILABLE_DOUBLE_EXP_TIME,		//作废
		DUEL_RECV_REQUEST,

		DUEL_REJECT_REQUEST,			//215
		DUEL_PREPARE,
		DUEL_CANCEL,
		DUEL_START,
		DUEL_STOP,
		
		DUEL_RESULT,				//220
		PLAYER_BIND_REQUEST,
		PLAYER_BIND_INVITE,
		PLAYER_BIND_REQUEST_REPLY,
		PLAYER_BIND_INVITE_REPLY,

		PLAYER_BIND_START,			//225
		PLAYER_BIND_STOP,
		PLAYER_MOUNTING,
		PLAYER_EQUIP_DETAIL,
		ELSE_DUEL_START,

		PARIAH_DURATION,			//230
		PLAYER_GAIN_PET,
		PLAYER_FREE_PET,
		PLAYER_SUMMON_PET,
		PLAYER_RECALL_PET,

		PLAYER_START_PET_OP,			//235
		PLAYER_STOP_PET_OP,
		PLAYER_PET_RECV_EXP,
		PLAYER_PET_LEVELUP,
		PLAYER_PET_ROOM,

		PLAYER_PET_ROOM_CAPACITY,		//240
		PLAYER_PET_HONOR_POINT,
		PLAYER_PET_HUNGER_POINT,
		ENTER_BATTLEGROUND,
		TURRET_LEADER_NOTIFY,

		BATTLE_RESULT,				//245
		BATTLE_INFO,
		PLAYER_PET_DEAD,
		PET_REVIVE,
		PLAYER_PET_HP_NOTIFY,

		PLAYER_PET_AI_STATE,			//250
		PLAYER_RECIPE_DATA,
		PLAYER_LEARN_RECIPE,
		PLAYER_PRODUCE_SKILL_INFO,
		PLAYER_REFINE_RESULT,

		PLAYER_START_GENERAL_OPERATION,		//255
		PLAYER_STOP_GENERAL_OPERATION,
		PLAYER_CASH,
		PLAYER_SKILL_ADDON,
		PLAYER_SKILL_COMMON_ADDON,

		PLAYER_EXTRA_SKILL,			//260
		PLAYER_TITLE_LIST,
		PLAYER_ADD_TITLE,
		PLAYER_DEL_TITLE,
		PLAYER_REGION_REPUTATION,

		PLAYER_CHANGE_REGION_REPUTATION,	//265
		PLAYER_CHANGE_TITLE,		
		PLAYER_CHANGE_INVENTORY_SIZE,
		PLAYER_BIND_SUCCESS,
		PLAYER_CHANGE_SPOUSE,
		
		PLAYER_INVADER_STATE,			//270
		PLAYER_MAFIA_CONTRIBUTION,
		LOTTERY_BONUS,
		RECORDER_CHECK_RESULT,
		PLAYER_USE_ITEM_WITH_ARG,

		OBJECT_USE_ITEM_WITH_ARG,		//275
		MAFIA_TRASHBOX_CAPACITY,
		NPC_SERVICE_RESULT,
		BATTLE_FLAG_CHANGE,
		BATTLE_START,

		BATTLE_FLAG_CHANGE_STATUS,		//280
		TALISMAN_EXP_NOTIFY,
		TALISMAN_COMBINE,
		TALISMAN_VALUE_NOTIFY,
		AUTO_BOT_BEGIN,

		PLAYER_WALLOW_INFO,			//285
		ANTI_CHEAT_BONUS,
		BATTLE_SCORE,
		GAIN_BATTLE_SCORE,
		SPEND_BATTLE_SCORE,

		BATTLE_PLAYER_INFO,			//290
		TALENT_NOTIFY,
		LEAVE_BATTLEGROUND,
		NOTIFY_SAFE_LOCK,
		ENTER_BATTLEFIELD,

		LEAVE_BATTLEFIELD,			//295
		BATTLEFIELD_INFO,
		BATTLEFIELD_START,
		BATTLEFIELD_END,
		BATTLEFIELD_CONSTRUCTION_INFO,
		
		BATTLEFIELD_CONTRIBUTION_INFO,		//300
		PLAYER_EQUIP_PET_BEDGE,
		PLAYER_EQUIP_PET_EQUIP,
		PLAYER_SET_PET_STATUS,
		PLAYER_COMBINE_PET,

		PLAYER_UNCOMBINE_PET,			//305
		PLAYER_PET_AGE_LIFE,
		PLAYER_PET_SET_AUTO_SKILL,
		PLAYER_PET_SET_SKILL_COOLDOWN,
		BATTLEFIELD_BUILDING_STATUS_CHANGE,

		PLAYER_SET_PET_RANK,			//310
		PLAYER_REBORN_INFO,
		SCRIPT_MESSAGE,
		PLAYER_PET_CIVILIZATION,
		PLAYER_PET_CONSTRUCTION,

		PLAYER_MOVE_PET_BEDGE,			//315
		PLAYER_START_FLY,
		PLAYER_PET_PROP_ADDED,
		SECT_BECOME_DISCIPLE,
		CULTIVATION_NOTIFY,

		OFFLINE_AGENT_BONUS,			//320
		OFFLINE_AGENT_TIME,
		SPIRIT_POWER_DECREASE,
		PLAYER_BIND_RIDE_INVITE,
		PLAYER_BIND_RIDE_INVITE_REPLY,

		PLAYER_BIND_RIDE_JOIN,			//325
		PLAYER_BIND_RIDE_LEAVE,
		PLAYER_BIND_RIDE_KICK,
		EXCHANGE_POCKET_ITEM,
		MOVE_POCKET_ITEM,

		EXCHANGE_INVENTORY_POCKET_ITEM,		//330
		MOVE_POCKET_ITEM_TO_INVENTORY,
		MOVE_INVENTORY_ITEM_TO_POCKET,
		PLAYER_CHANGE_POCKET_SIZE,
		MOVE_ALL_POCKET_ITEM_TO_INVENTORY,

		SELF_BUFF_NOTIFY,			//335	
		NOTIFY_FASHION_HOTKEY,
		EXCHANGE_FASHION_ITEM,
		EXCHANGE_INVENTORY_FASHION_ITEM,
		EXCHANGE_EQUIPMENT_FASHION_ITEM,

		EXCHANGE_HOTKEY_EQUIPMENT_FASHION,	//340
		PEEP_INFO,
		PLAYER_BONUS,
		SELF_KILLINGFIELD_INFO,
		KILLINGFIELD_INFO,

		ARENA_INFO,				//345
		RANK_CHANGE,
		CHANGE_KILLINGFIELD_SCORE,
		PLAYER_CHANGE_STYLE,	
		PLAYER_CHANGE_VIPSTATE,

		RECEIVE_BONUS_EXP,			//350
		CROSSSERVER_BATTLE_INFO,
		PLAYER_ACHIEVEMENT,
		PLAYER_ACHIEVEMENT_FINISH,
		PLAYER_ACHIEVEMENT_ACTIVE,

		PLAYER_PREMISS_DATA,			//355
		PLAYER_PREMISS_FINISH,		
		PLAYER_ACHIEVEMENT_MAP,			
		ACHIEVEMENT_MESSAGE,			
		SELF_INSTANCE_INFO,	

		INSTANCE_INFO,				//360
		SKILL_CONTINUE,
		PLAYER_START_TRANSFORM,	
		MAGIC_DURATION_DECREASE,
		MAGIC_EXP_NOTIFY,

		TRANSFORM_SKILL_DATA,			//365
		PLAYER_ENTER_CARRIER,
		PLAYER_LEAVE_CARRIER,
		PLAYER_MOVE_ON_CARRIER,
		PLAYER_STOP_MOVE_ON_CARRIER,

		SKILL_PROFICIENCY,			//370	
		NOTIFY_MALL_PROP,
		SEND_PK_MESSAGE,
		UPDATE_COMBO_STATE,		
		ENTER_DIM_STATE,

		ENTER_INVISIBLE_STATE,			//375
		OBJECT_CHARGE,	
		ENTER_TERRITORY,
		LEAVE_TERRITORY,
		LAST_LOGOUT_TIME,	

		COMBINE_SKILL_DATA,			//380
		SEND_CLONE_EQUIPMENT_INFO,
		CIRCLE_INFO_NOTIFY,
		DELIVERY_EXP_NOTIFY,
		DEITY_LEVELUP,

		OBJECT_SPECIAL_STATE,			//385	
		LOTTERY2_BONUS,
		GEM_NOTIFY,
		CRSSVR_TEAM_BATTLE_INFO,
		NOTIFY_CIRCLE_OF_DOOM_START,

		NOTIFY_CIRCLE_OF_DOOM_INFO,     //390
		NOTIFY_CIRCLE_OF_DOOM_STOP,
		PLAYER_SCALE,		
		PLAYER_MOVE_CYCLE_AREA,
		ENTER_RAID,
		
		LEAVE_RAID,						//395
		OBJECT_NOTIFY_RAID_POS,		//liuyue 废弃了此协议
		PLAYER_RAID_COUNTER,
		OBJECT_BE_MOVED,
		PLAYER_PULLING,

		PLAYER_BE_PULLED,				//400
		LOTTERY3_BONUS,
		RAID_INFO,
		TRIGGER_SKILL_TIME,
		PLAYER_STOP_SKILL,

		MALL_ITEM_PRICE,				//405
		PLAYER_SINGLE_RAID_COUNTER,
		PLAYER_VIP_AWARD_INFO,
		VIP_AWARD_CHANGE,
		ONLINE_AWARD_INFO,

		ONLINE_AWARD_PERMIT,			//410
		ADDON_SKILL_PERMANENT,
		ADDON_SKILL_PERMANENT_DATA,
		ZX_FIVE_ANNI_DATA,
		TREASURE_INFO,

		TREASURE_REGION_UPGRADE,		//415
		TREASURE_REGION_UNLOCK,	
		TREASURE_REGION_DIG,
		INC_TREASURE_DIG_COUNT,
		RAID_GLOBAL_VAR,				//副本全局变量

		RANDOM_TOWER_MONSTER,			//420 通知客户端弹出爬塔副本界面
		RANDOM_TOWER_MONSTER_RESULT,		//服务器通知爬塔副本随机怪物结果
		PLAYER_TOWER_DATA,			//玩家爬塔副本相关的数据
		PLAYER_TOWER_MONSTER_LIST,		//玩家爬塔副本怪物图鉴列表
		PLAYER_TOWER_GIVE_SKILLS,		//爬塔副本技能手牌

		SET_TOWER_GIVE_SKILL,			//425 单个手牌使用后发送通知客户端改变状态
		ADD_TOWER_MONSTER,			//获得爬塔副本一个怪物图鉴
		SET_TOWER_REWARD_STATE,			//爬塔副本奖励领取成功后发送给客户端状态协议
		LITTLEPET_INFO,				//诛小仙信息
		RUNE_INFO,				//元魂装备位激活

		RUNE_REFINE_RESULT,			//430	元魂洗练结果		
		TOWER_LEVEL_PASS,			//通过爬塔副本的一关
		FILL_PLATFORM_MASK,			//补填的平台信息 mask
		PK_BET_DATA,				//PK竞猜相关数据
		PUT_ITEM,				//将物品放入包裹指定位置

		OBJECT_START_SPECIAL_MOVE,		//435	玩家开始特殊移动
		OBJECT_SPECIAL_MOVE,			//玩家特殊移动
		OBJECT_STOP_SPECIAL_MOVE,		//玩家停止特殊移动
		PLAYER_CHANGE_NAME,
		ENTER_COLLISION_RAID,

		COLLISION_RAID_SCORE,				//440
		REPURCHASE_INV_DATA,				//物品回购包裹信息
		COLLISION_SPECIAL_STATE,		
		EXCHANGE_MOUNT_WING_ITEM,			//坐骑飞剑包裹里交换物品
		EXCHANGE_INVENTORY_MOUNTWING_ITEM,	//普通包裹 <-> 坐骑飞剑包裹

		EXCHANGE_EQUIPMENT_MOUNTWING_ITEM,	//445 //装备栏 <-> 坐骑飞剑包裹 
		PLAYER_CHANGE_MOUNTWING_INV_SIZE,
		PLAYER_DARKLIGHT,					//轩辕光暗度
		PLAYER_DARKLIGHT_STATE,				//轩辕光暗状态
		PLAYER_DARKLIGHT_SPIRIT,			//轩辕职业的灵
		
		MIRROR_IMAGE_CNT,					//450, 镜像数量
		CHANGE_FACTION_NAME,
		PLAYER_WING_CHANGE_COLOR,		//飞剑改变颜色
		UI_TRANSFER_OPENED_NOTIFY,		//UI传送开启的传送点通告
		NOTIFY_ASTROLOGY_ENERGY,

		ASTROLOGY_UPGRADE_RESULT,		//455 星座升级结果
		ASTROLOGY_DESTROY_RESULT,		//星座粉碎结果
		TALISMAN_REFINESKILL,			//通知客户端法宝技能洗练结果
		COLLISION_RAID_RESULT,			//离开碰撞副本结果
		COLLISION_RAID_INFO,			//碰撞战场信息

		DIR_VISIBLE_STATE,				//460, 设置朝向的状态效果
		PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL,	//玩家弥补未满级飞升数值
		COLLISION_PLAYER_POS,
		LIVENESS_NOTIFY,				//通告活跃度信息
		ENTER_KINGDOM,

		LEAVE_KINGDOM,		//465
		RAID_LEVEL_START,	//关卡开始
		RAID_LEVEL_END,		//关卡结束
		RAID_LEVEL_RESULT,	//关卡通过结果
		RAID_LEVEL_INFO,	//关卡统计的动态信息

		KINGDOM_HALF_END,	//470 国王战半场结束
		KINGDOM_END_RESULT,	//国王战结束结果
		KINGDOM_MAFIA_INFO,	//国王战帮派积分,杀敌数，死亡数
		SALE_PROMOTION_INFO,	//台历物品前提条件通告(任务及成就)
		EVENT_START,		//事件开始

		EVENT_STOP,		//475 事件结束
		EVENT_INFO,		//事件信息，启动的时候发送
		CASH_GIFT_IDS,		//商城买赠积分ID
		NOTIFY_CASH_GIFT,	//通知领取礼包
		PLAYER_PROPADD,	//物品增加的玩家属性
		
		PLAYER_GET_PROPADD_ITEM,	//480, 玩家获得属性增加物品
		PLAYER_EXTRA_EQUIP_EFFECT,	//玩家额外武器光效
		KINGDOM_KEY_NPC_INFO,
		NOTIFY_KINGDOM_INFO,
		CSFLOW_BATTLE_PLAYER_INFO,	//战场

		PLAYER_BATH_INVITE,	//485  邀请搓澡
		PLAYER_BATH_INVITE_REPLY,	//邀请搓澡回复
		PLAYER_BATH_START,		//开始搓澡
		PLAYER_BATH_STOP,		//结束搓澡
		ENTER_BATH_WORLD,		//进入搓澡地图

		LEAVE_BATH_WORLD,	//490	离开搓澡地图
		BATH_GAIN_ITEM,		//搓澡获得物品
		FLOW_BATTLE_INFO,
		KINGDOM_GATHER_MINE_START,
		FUWEN_COMPOSE_INFO,


		FUWEN_COMPOSE_END,	//495 符文碎片合成结束
		FUWEN_INSTALL_RESULT,
		FUWEN_UNINSTALL_RESULT,
		MULTI_EXP_MODE,
		EXCHANGE_ITEM_LIST_INFO, //服务器端交换整理包裹

		WEB_ORDER_INFO,		//500 web订单信息
		CS_BATTLE_OFF_LINE_INFO,
		CONTROL_TRAP_INFO,
		NPC_INVISIBLE,
		PUPPET_FORM,

		TELEPORT_SKILL_INFO,	//505
		MOB_ACTIVE_READY_START, //互动开始
		MOB_ACTIVE_RUNNING_START, //互动过程开始
		MOB_ACTIVE_END_START, //互动结束开始
		MOB_ACTIVE_END_FINISH, //互动结束完成
		
		MOB_ACTIVE_CANCEL, //互动取消, 510
		FASHION_COLORANT_PRODUCE,  // Youshuang add
		FASHION_COLORANT_USE,   // Youshuang add
		BLOW_OFF,		// 击飞
		PLAYER_QILIN_INVITE,	//  邀请搓澡

		PLAYER_QILIN_INVITE_REPLY,	//515 邀请搓澡回复
		PLAYER_QILIN_START,		//开始搓澡
		PLAYER_QILIN_STOP,		//结束搓澡
		PLAYER_QILIN_DISCONNECT,
		PLAYER_QILIN_RECONNECT,

		PHASE_INFO,			//520
		PHASE_INFO_CHANGE,		
		GET_ACHIEVEMENT_AWARD,  // Youshuang add
		FAC_BUILDING_ADD,
		FAC_BUILDING_UPGRADE,

		FAC_BUILDING_REMOVE,	//525
		FAC_BUILDING_COMPLETE,
		PLAYER_ACTIVE_EMOTE_INVITE,
		PLAYER_ACTIVE_EMOTE_INVITE_REPLY,
		PLAYER_ACTIVE_EMOTE_START,

		PLAYER_ACTIVE_EMOTE_STOP,  // 530
		PLAYER_FAC_BASE_INFO,
		RAID_BOSS_BEEN_KILLED,	// Youshuang add
		FAC_BASE_PROP_CHANGE,
		BE_TAUNTED2,

		COMBINE_MINE_STATE_CHANGE,  // 535, Youshuang add
		GET_FACBASE_MALL_INFO, // Youshuang add
		GET_FACTION_COUPON,	// Youshuang add
		GET_FACBASE_CASH_ITEMS_INFO, // Youshuang add
		PLAYER_CHANGE_PUPPET_FORM,

		STEP_RAID_SCORE_INFO, //540
		NOTIFY_BLOODPOOL_STATUS,
		PLAYER_FACBASE_AUCTION,	//返回给客户端的基地拍卖列表
		FACBASE_AUCTION_UPDATE, //基地拍卖信息发生变化 会通过deliveryd给所有帮众发送
		FAC_COUPON_RETURN,	//更新基地拍卖退回的金券数量

		OBJECT_TRY_CHARGE,	//545	
		OBJECT_BE_CHARGE_TO,
		PLAYER_START_TRAVEL_AROUND,
		PLAYER_STOP_TRAVEL_AROUND,
		PLAYER_GET_SUMMON_PETPROP,

		GET_FACBASE_CASH_ITEM_SUCCESS,	// 550, Youshuang add
		FACBASE_AUCTION_ADD_HISTORY,	//增加拍卖纪录
		FACBASE_MSG_UPDATE,		//更新基地留言消息
		CS6V6_CHEAT_INFO,
		SEND_TALISMAN_ENCHANT_ADDON,	// 554, Youshuang add

	 	HIDE_AND_SEEK_RAID_INFO, //555	
		HIDE_AND_SEEK_ROUND_INFO,
		HIDE_AND_SEEK_PLAYERS_INFO,
		HIDE_AND_SEEK_SKILL_INFO,
		HIDE_AND_SEEK_BLOOD_NUM,

		HIDE_AND_SEEK_ROLE, //560
		HIDE_AND_SEEK_ENTER_RAID,
		HIDE_AND_SEEK_LEAVE_RAID,
		HIDE_AND_SEEK_HIDER_TAUNTED,
		MOVE_MAX_POCKET_ITEM_TO_INVENTORY,

		GET_NEWYEAR_AWARD_INFO, // 565, Youshuang add
		CAPTURE_RAID_PLAYER_INFO,
		CAPTURE_BROADCAST_SCORE,
		CAPTURE_SUBMIT_FLAG,
		CAPTURE_RESET_FLAG,
		
		CAPTURE_GATHER_FLAG, //570
		CAPTURE_MONSTER_FLAG,
		PLAYER_USE_TITLE,  // 572, Youshuang add
		CAPTURE_BROADCAST_TIP,
		CAPTURE_BROADCAST_FLAG_MOVE,

	}; //S2C


	enum 
	{
		//0
		ERR_SUCCESS,
		ERR_INVALID_TARGET,
		ERR_OUT_OF_RANGE,
		ERR_FATAL_ERR,
		ERR_COMMAND_IN_ZOMBIE,
		//5
		ERR_ITEM_NOT_IN_INVENTORY,
		ERR_ITEM_CANT_PICKUP,
		ERR_INVENTORY_IS_FULL,
		ERR_ITEM_CANNOT_EQUIP,
		ERR_CANNOT_ATTACK,
		//10
		ERR_TEAM_CANNOT_INVITE,
		ERR_TEAM_JOIN_FAILED,
		ERR_TEAM_ALREADY_INVITE,
		ERR_TEAM_INVITE_TIMEOUT,
		ERR_SERVICE_UNAVILABLE,
		//15
		ERR_SERVICE_ERR_REQUEST,
		ERR_OUT_OF_FUND,
		ERR_CANNOT_LOGOUT,
		ERR_CANNOT_USE_ITEM,
		ERR_TASK_NOT_AVAILABLE,
		//20
		ERR_SKILL_NOT_AVAILABLE,
		ERR_CANNOT_EMBED,
		ERR_CANNOT_LEARN_SKILL,
		ERR_CANNOT_HEAL_IN_COMBAT,
		ERR_CANNOT_RECHARGE,
		//25
		ERR_NOT_ENOUGH_MATERIAL,
		ERR_PRODUCE_FAILED,
		ERR_DECOMPOSE_FAILED,
		ERR_CANNOT_SIT_DOWN,
		ERR_CANNOT_EQUIP_NOW,
		//30
		ERR_MINE_HAS_BEEN_LOCKED,
		ERR_MINE_HAS_INVALID_TOOL,
		ERR_MINE_GATHER_FAILED,
		ERR_OTHER_SESSION_IN_EXECUTE,
		ERR_INVALID_PASSWD_FORMAT,
		//35
		ERR_PASSWD_NOT_MATCH,
		ERR_TRASH_BOX_NOT_OPEN,
		ERR_ENOUGH_MONEY_IN_TRASH_BOX,
		ERR_TEAM_REFUSE_APPLY,
		ERR_CONCURRENT_EMOTE_REFUSED,		//$$$$$$$$$$$$$$$$$$
		//40
		ERR_EQUIPMENT_IS_LOCKED,
		ERR_CANNOT_OPEN_PLAYER_MARKET,
		ERR_INVALID_ITEM,
		ERR_YOU_HAS_BEEN_BANISHED,
		ERR_CAN_NOT_DROP_ITEM,
		//45
		ERR_INVALID_PRIVILEGE,
		ERR_PLAYER_NOT_EXIST,
		ERR_CAN_NOT_DISABLE_PVP_STATE,
		ERR_CAN_NOT_UNLEARN_SKILL,
		ERR_COMMAND_IN_IDLE,
		//50
		ERR_COMMAND_IN_SEALED,
		ERR_LEVEL_NOT_MATCH,
		ERR_CANNOT_ENTER_INSTANCE,
		ERR_SKILL_IS_COOLING,
		ERR_OBJECT_IS_COOLING,
		//55
		ERR_CANNOT_FLY,
		ERR_CAN_NOT_RESET_INSTANCE,
		ERR_INVENTORY_IS_LOCKED,
		ERR_TOO_MANY_PLAYER_IN_INSTANCE,
		ERR_TOO_MANY_INSTANCE,
		//60
		ERR_FACTION_BASE_NOT_READY,
		ERR_FACTION_BASE_DENIED,
		ERR_CAN_NOT_JUMP_BETWEEN_INSTANCE,
		ERR_NOT_ENOUGH_REST_TIME,
		ERR_CANNOT_DUEL_TWICE,
		//65
		ERR_CREATE_DUEL_FAILED,
		ERR_INVALID_OPERATION_IN_COMBAT,
		ERR_INVALID_GENDER,
		ERR_INVALID_BIND_REQUEST,
		ERR_INVALID_BIND_REPLY,
		//70
		ERR_FORBIDDED_OPERATION,
		ERR_PET_IS_ALEARY_ACTIVE,
		ERR_PET_IS_NOT_EXIST,
		ERR_PET_IS_NOT_ACTIVE,
		ERR_PET_FOOD_TYPE_NOT_MATCH,
		//75
		ERR_BATTLEFIELD_IS_CLOSED,
		ERR_PET_CAN_NOT_BE_HATCHED,
		ERR_PET_CAN_NOT_BE_RESTORED,
		ERR_FACTION_IS_NOT_MATCH,
		ERR_CANNOT_QUERY_ENEMY_EQUIP,
		//80
		ERR_NPC_SERVICE_IS_BUSY,
		ERR_PET_CAN_NOT_MOUNT,
		ERR_CAN_NOT_RESET_PP,
		ERR_BATTLEFIELD_IS_FINISHED,
		ERR_HERE_CAN_NOT_DUEL,
		//85
		ERR_SUMMON_PET_INVALID_POS,
		ERR_CONTROL_TOO_MANY_TURRETS,
		ERR_CANNOT_SUMMON_DEAD_PET,
		ERR_PET_IS_NOT_DEAD,
		ERR_CANNOT_BIND_HERE,
		//90
		ERR_INVALID_PLAYER_CALSS,
		ERR_RUNE_IS_IN_EFFECT,
		ERR_REINFORCE_FAILED,
		ERR_LEARN_RECIPE_FAILED,
		ERR_CAN_NOT_REFINE,
		//95
		ERR_CAN_NOT_UNINSTALL,
		ERR_GSHOP_INVALID_REQUEST,
		ERR_CAN_NOT_FIND_SPOUSE,
		ERR_NO_SKILL_TO_FORGET,
		ERR_RESTORE_DESTROY,
		//100
		ERR_USE_ITEM_FAILED,
		ERR_SKILL_IS_NOT_COOLING,
		ERR_CAN_TRANSMIT_IN,
		ERR_CAN_TRANSMIT_OUT,
		ERR_NOT_MAFIA_MEMBER,
		//105
		ERR_MAFIA_TIME_NOT_ENOUGH,
		ERR_ITEM_NEED_BATTLEGROUND,
		ERR_TALISMAN_REFINE_ERROR,
		ERR_TALISMAN_LEVEL_UP,
		ERR_TALISMAN_RESET,
		//110
		ERR_TALISMAN_COMBINE,
		ERR_TALISMAN_ENCHANT,
		ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK,
		ERR_DO_NOT_HAVE_SIEGE_TOCKEN, //113
		ERR_ARCHER_MAX,
		//115
		ERR_NOT_MAFIA_LEADER_CAN_NOT_CONSTRUCT,
		ERR_NOT_ENOUGH_RES_TO_CONSTRUCT,
		ERR_CONSTRUCT_MAX_LEVEL,
		ERR_NOT_ENOUGH_RES_TO_CONTRIBUTE,
		ERR_PET_BEDGE_IS_LOCKED,
		//120
		ERR_PET_EQUIP_IS_LOCKED,
		ERR_NEED_PET_BEDGE,
		ERR_PET_ALREADY_ADOPTED,
		ERR_PET_LEVEL_TO_HIGH,
		ERR_PET_NOT_ADOPTED,
		//125
		ERR_NOT_ENOUGH_ROOM_IN_PACKBACK,
		ERR_PET_ACTIVE_TOO_MUCH,
		ERR_NEED_EQUIP_PET_BEDGE,
		ERR_PET_NOT_IN_BATTLE_STATUS,
		ERR_PET_SKILL_IN_COOLDOWN,
		//130
		ERR_NOT_HAVE_BUY_ARCHER_ITEM,
		ERR_CAN_NOT_SET_MORE_PET_FOR_COMBET_STATUS,
		ERR_PET_NOT_SUMMON,
		ERR_PET_NOT_COMBINE,
		ERR_NOT_HAVE_ENOUGH_PET_CONSTRUCTION,
		//135
		ERR_NOT_HAVE_ENOUGH_PET_ABILITY,
		ERR_NOT_ENOUGH_CIVILIZATION,
		ERR_NOT_ENOUGH_PET_LEVEL,
		ERR_CUR_PET_RANK_MAX,
		ERR_FLY_IN_SHAPING,
		//140
		ERR_FLY_IN_COMBAT,
		ERR_PET_LEVEL_TOO_HIGH_CAN_NOT_USE_ITEM,
		ERR_PET_COMBINE1_COOLDOWN,
		ERR_PET_COMBINE2_COOLDOWN,
		ERR_AGE_LIFE_INVALID,
		//145
		ERR_PRODUCE_IN_COOLDOWN,
		ERR_REQUIRE_REINFORCE_LEVEL,
		ERR_ONLINE_AGENT_POS,
		ERR_PET_IS_LOCKED,
		ERR_BLOOD_ENCHANT_FAILED,
		//150
		ERR_SPIRIT_ADDON_FAILED,
		ERR_NOT_ENOUGH_SPIRIT_POWER,
		ERR_RESTORE_SPIRIT_POWER,
		ERR_SPIRIT_DECOMPOSE_FAILED,
		ERR_SPIRIT_MASTER_NOT_MATCH,
		//155
		ERR_SPIRIT_ADDON_REMOVE_FAILED,
		ERR_INVALID_LINK_RIDE_REQUEST,
		ERR_CANNOT_LINK_RIDE_HERE,
		ERR_LINK_RIDE_INVITE_FULL,
		ERR_LINK_RIDE_ALREADY_INVITE,
		//160
		ERR_CANNOT_MOVE_TO_POCKET,
		ERR_CANNOT_MOVE_ALL_TO_INVENTORY,	
		ERR_CANNOT_LOCK_EXPIRE_ITEM,
		ERR_REBORN_COUNT_NOT_MATCH,
		ERR_CAN_NOT_REFINE_PETEQUIP,
		//165
		ERR_CULT_NOT_MATCH,
		ERR_MAGIC_REFINE_ERROR,
		ERR_MAGIC_RESTORE_ERROR,
		ERR_MINE_NOT_OWNER,
		ERR_NOT_TERRITORY_OWNER,
		//170
		ERR_COMBINE_SKILL_IS_COOLING,
		ERR_CAN_NOT_BID,
		ERR_CAN_NOT_UPGRADE_EQUIPMENT,
		ERR_NEWBIE_CHAT_IS_COOLING,
		ERR_FAILED_IDENTIFY_GEMSLOT,
		//175
		ERR_FAILED_REBUILD_GEMSLOTS,
		ERR_FAILED_CUSTOMIZE_GEMSLOTS,
		ERR_FAILED_EMBED_GEMS,
		ERR_FAILED_REMOVE_GEMS,
		ERR_FAILED_UPGRADE_GEM_LEVEL,
		//180
		ERR_FAILED_UPGRADE_GEM_QUALITY,
		ERR_FAILED_EXTRACT_GEM,
		ERR_FAILED_SMELT_GEM,
		ERR_SAME_GEM_SLOT_SHAPE,
		ERR_ITEM_CAN_NOT_GEM_SLOT_IDENTIFY,

		//185
		ERR_PLAYER_KEEP_ALL_SLOTS_WHEN_REBUILD,
		ERR_GEM_UPGRADE_LEVEL_PROB_NOT_ENOUGH,
		ERR_VIPAWARD_INVALID_REQUEST,
		ERR_ONLINEAWARD_INVALID_REQUEST,
		ERR_CONSIGN_ROLE_REDNAME,

		//190
		ERR_CONSIGN_ROLE_LOGOUT,
		ERR_CONSIGN_IN_COMBAT,
		ERR_TOWERWARD_INVALID_REQUEST,
		ERR_PET_CAN_NOT_BE_SUMMONED,
		ERR_PET_CAN_NOT_BE_COMBINED,

		//195
		ERR_PET_CAN_NOT_BE_RECALLED,
		ERR_PET_CAN_NOT_BE_UNCOMBINED,
		ERR_CIRCLE_CANCEL_TEAM_CHANGE,
		ERR_DBEXP_TIME_LIMIT,
		ERR_HAS_SUMMON_CAN_NOT_CONSIGNROLE,

		//200
		ERR_TALISMAN_HOLYLEVELUP,			//法宝飞升失败
		ERR_TALISMAN_EMBEDSKILL,			//法宝技能镶嵌失败
		ERR_TALISMAN_SKILLREFINE,			//法宝融合失败
		ERR_TALISMAN_SKILLREFINERESULT,
		ERR_LACK_OF_REPUTATION,

		//205
		ERR_FAILED_MERGE_POTION,			//药品灌注失败
		ERR_FAILED_DELIVERYGIFTBOX,			//多次开启礼包领奖失败
		ERR_FAILED_GEN_PROPADDITEM,		//属性丹生成失败
		ERR_FAILED_GEN_PROPADDITEM_MATERIAL_NOT_ENOUGH,		//属性丹生成失败材料不足
		ERR_FAILED_REBUILD_PROPADDITEM,	//重炼属性丹失败

		//210
		ERR_FAILED_REBUILD_PROPADDITEM_MATERIAL_NOT_ENOUGH,	//重炼属性丹失败, 材料不足
		ERR_RESURRECT_POS_NOT_EXIST,	//复活点不存在，无法复活
		ERR_BATH_INVALID_REQUEST,	//无效的搓澡邀请
		ERR_BATH_INVITE_FULL,		//邀请队列已满
		ERR_BATH_ALREADY_INVITE,	//已经邀请了该玩家

		//215
		ERR_BATH_MAX_LIMIT,		//超过澡堂最多邀请次数
		ERR_BATH_NOT_OPEN,		//澡堂未开放，不能进入
		ERR_BATH_CANNOT_IN,		//当前地图不能进入澡堂
		ERR_WORLD_CANNOT_FLY_BATH_POS,  //该地图不能传送到澡堂进入点
		ERR_MULTI_EXP_TIME_LIMIT,	//多倍经验达到上限

		//220
		ERR_KING_CALL_GUARD_IS_COOLING, //亲卫召唤冷却
		ERR_CHANGE_INV_TRASHBOX_SIZE,	//改变包裹和仓库大小错误
		ERR_CHANGE_INV_TRASHBOX_SIZE_NOT_ENOUGH_ITEM,	//物品不足
		ERR_BATH_IS_COOLDOWN,		//洗澡操作正在冷却
		ERR_KING_CALL_GUARD_NOT_ALLOWED, //当前状态下不允许发起亲卫传送

		//225
		ERR_KING_CALL_GUARD_WORLD_NOT_ALLOWED, //该地图不允许发起亲卫传送
		ERR_FORBID_SKILL,		//该技能暂时禁止使用
		ERR_FORBID_ITEM,		//该物品暂时禁止使用
		ERR_FORBID_SERVICE,		//该服务暂时停止使用
		ERR_FORBID_PROTOCOL,		//该功能暂时停止使用

		//230
		ERR_INVENTORY_HAS_SORT,  //背包已经有序
		ERR_TRASHBOX_HAS_SORT,  //仓库已经有序
		ERR_CROSS_SEVER_MAP_ID,
		ERR_BATTLE_READY,
		ERR_SUMMON_TELEPORT1_IS_COOLING,
		
		//235
		ERR_MOB_ACTIVE_INVALID,
		ERR_MOB_ACTIVE_LOCKED,
		ERR_MOB_ACTIVE_POINT_MAX,
		ERR_MOB_ACTIVE_HAS_INVALID_TOOL,
		ERR_MOB_ACTIVE_HAS_INVALID_TASK,

		//240
		ERR_MOB_ACTIVE_TIMES_MAX,
		ERR_MOB_ACTIVE_FINISH_GET_ITEM_FAILED,
		ERR_QILIN_INVALID_REQUEST,
		ERR_QILIN_INVITE_FULL,
		ERR_QILIN_ALREADY_INVITE,

		//245
		ERR_FBASE_LOGIC,	//帮派基地逻辑错误
		ERR_FBASE_BUILD_REPEAT, //已经有建筑在建设中 不能建设新建筑
		ERR_FBASE_FIELD_INDEX,  //地块错误
		ERR_FBASE_FIELD_REPEAT, //地块重复建筑
		ERR_FBASE_BUILDING_UNAVAIL, //此地块不能建设此建筑

		//250
		ERR_FBASE_BUILDING_TID, //非法建筑
		ERR_FBASE_BUILDING_COND, //建设条件不满足
		ERR_FBASE_PRIVILEGE, //玩家无权限操作基地
		ERR_CANNOT_SUMMON_REBORN_PET,	//只有飞升玩家能够召唤飞升宠物
		ERR_PET_SUMMON_COOLDOWN,	//飞升宠物在战斗状态下两次召唤的冷却时间为30s

		//255
		ERR_INVALID_ACTIVE_EMOTE_INVITE,
		ERR_CANNOT_ACTIVE_EMOTE,
		ERR_FBASE_UPGRADE_LEV, //升级建筑 建筑等级不匹配
		ERR_FBASE_UPGRADE_FULL, //已达满级 不能再升级
		ERR_FBASE_REMOVE_INDEX, //该地块没有建筑 不能拆除

		//260
		ERR_FBASE_UPGRADE_INDEX, //该地块没有建筑 不能升级
		ERR_FBASE_UPGRADE_TID, //建筑 tid 不匹配
		ERR_FBASE_REMOVE_WHILE_INBUILDING, //建设中的建筑不能拆除
		ERR_FBASE_NOT_ENOUGH_CASH,	// Youshuang add
		ERR_FBASE_CASH_ITEM_IN_COOLDOWN,  // Youshuang add

		//265
		ERR_FBASE_AUC_NOT_EXIST, //拍卖品不存在
		ERR_FBASE_AUC_NOT_WINNER, //非拍卖获胜者 不能领取
		ERR_FBASE_STATUS,	//基地状态错误
		ERR_FBASE_AUC_SIZE,	//基地拍卖条数已达上限
		ERR_CS6V6_CANNOT_GET_AWARD, //当前不能领取该奖励

		// 270
		ERR_POCKET_HAS_SORT,  // Youshuang add
		ERR_MINE_CANNOT_GATHER, //不能采集本派的矿
		ERR_HAS_NOT_CAPTURE_RAID_FLAG, //包裹里没有军旗
		ERR_CANNOT_SUBMIT_CAPTURE_RAID_FLAG, //对方手上也有军旗不能交
	};

	enum
	{
	//0
		GOP_INSTALL,
		GOP_UNINSTALL,
		GOP_BIND,
		GOP_BIND_DESTORY,
		GOP_BIND_DESTORY_RESTORE,

	//5
		GOP_MOUNT,
		GOP_TALISMAN_REFINE,
		GOP_TALISMAN_LVLUP,
		GOP_TALISMAN_RESET,
		GOP_TALISMAN_COMBINE,

	//10
		GOP_TALISMAN_ENCHANT,
		GOP_FLY,
		GOP_LOCK,
		GOP_UNLOCK,
		GOP_SPIRIT_OPERATION,

	//15
		GOP_GIFT,
		GOP_PETEQUIP_REFINE,
		GOP_CHANGE_STYLE,
		GOP_MAGIC_REFINE,
		GOP_MAGIC_RESTORE,

	//20
		GOP_TRANSFORM,
		GOP_TELESTATION,
		GOP_REPAIR_DAMAGE_ITEM,
		GOP_UPGRADE_EQUIPMENT,
		GOP_CROSS_SERVER,

	//25
		GOP_TOWNSCROLL,
		GOP_GEM_OPERATION,
		GOP_RUNE_IDENTIFY,	//元魂鉴定
		GOP_RUNE_COMBINE,	//元魂血炼
		GOP_RUNE_REFINE,	//元魂洗练

	//30	
		GOP_RUNE_RESET,		//元魂归元
		GOP_RUNE_DECOMPOSE,	//元魂拆解
		GOP_RUNE_OPEN_SLOT,	//元魂打孔
		GOP_RUNE_CHANGE_SLOT,	//元魂改孔
		GOP_RUNE_ERASE_SLOT,	//元魂擦孔

	//35
		GOP_RUNE_INSTALL_SLOT,	//元魂镶嵌
		GOP_TALISMAN_HOLYLEVELUP,	//法宝飞升
		GOP_TALISMAN_EMBEDSKILL,	//镶嵌技能
		GOP_TALISMAN_SKILLREFINE,	//技能洗炼
		GOP_UPGRADE_EQUIPMENT2,		//升级成封神装备

	//40
		GOP_EQUIPMENT_SLOT,	//装备打孔
		GOP_EQUIPMENT_INSTALL_ASTROLOGY, //镶嵌星座
		GOP_EQUIPMENT_UNINSTALL_ASTROLOGY, //摘除星座
		GOP_ASTROLOGY_IDENTIFY,	//星座鉴定
		GOP_TALISMAN_SKILLREFINERESULT,	//技能洗炼

	//45
		GOP_BATTLEFLAG,
		GOP_ASTROLOGY_UPGRADE,
		GOP_ASTROLOGY_DESTROY,
		GOP_BATH,
		GOP_FUWEN_COMPOSE,

	//50
		GOP_PET_REBORN,		//宠物飞升
		GOP_ENTER_FAC_BASE,	//进入帮派基地
	};

	enum
	{
		DROP_TYPE_GM,
		DROP_TYPE_PLAYER,
		DROP_TYPE_TAKEOUT,
		DROP_TYPE_TASK,
		DROP_TYPE_RECHARGE,
		DROP_TYPE_DESTROY,
		DROP_TYPE_DEATH,
		DROP_TYPE_PRODUCE,
		DROP_TYPE_DECOMPOSE,
		DROP_TYPE_TRADEAWAY,
		DROP_TYPE_RESURRECT,
		DROP_TYPE_USE,
		DROP_TYPE_RUNE,
		DROP_TYPE_EXPIRE,
		DROP_TYPE_DEATH_DESTROY,
		DROP_TYPE_DEATH_DAMAGE,

	};

	enum OBJECT_STATE
	{
		STATE_IGNITE,
		STATE_FROZEN,
		STATE_FURY,
		STATE_DARK,
		STATE_CYCLE_SKILL,	//旋风斩
	};

	enum COLLISION_SKILL_TYPE
	{
		COLLISION_SKILL_MAXSPEED,	//加速
		COLLISION_SKILL_HALFSPEED,	//减速
		COLLISION_SKILL_ASHILL,		//不动如山
		COLLISION_SKILL_NOMOVE,		//定身
		COLLISION_SKILL_CHAOS,		//混乱
		COLLISION_SKILL_VOID,		//虚无
	};

	enum GEM_NOTIFY_MSG
	{
		//客户端弹框的消息
		GEM_UPGRADE_SUCC = 0,
		GEM_UPGRADE_FAIL,
		GEM_SMELT_FAIL_SAME_QUALITY,

		//客户端聊天框显示的消息
		GEM_SLOT_IDENTIFY_SUCC = 50,
		GEM_SLOT_REBUILD_SUCC,
		GEM_SLOT_CUSTOMIZE_SUCC,
		GEM_EMBED_SUCC,
		GEM_REMOVE_SUCC,
		GEM_UPGRADE_QUALITY_SUCC,
		GEM_EXTRACT_SUCC,
		GEM_SMELT_SUCC,	
	};

	namespace CMD
	{
		using namespace INFO;

		struct player_enter_slice	//player进入可见区域
		{	
			single_data_header header;
			player_info_1 data;
		};

		struct npc_enter_slice
		{
			single_data_header header;
			npc_info data;
		};

		struct leave_slice	//player离开可见区域
		{	
			single_data_header header;
			int id;
		};

		struct notify_pos		//player更新位置
		{	
			single_data_header header;
			A3DVECTOR pos;
			int tag;
		};

		struct self_info_1
		{
			single_data_header header;
			INFO::self_info_1 info;
		};

		struct	player_info_1_list 		//player list
		{
			multi_data_header header;
			player_info_1 list[];
		};

		struct player_info_2_list 
		{
			multi_data_header header;
			/*
			   struct		//假设的代码，实际上由于info2是变长的结构，所以无法这样组织
			   {
			   int cid;
			   player_info_2 info;
			   }list[];
			 */
			char data[1];
		};

		struct player_info_3_list 
		{
			multi_data_header header;
			/*
			   struct		//假设的代码，实际上由于info3是变长的结构，所以无法这样组织
			   {
			   int cid;
			   player_info_3 info;
			   }list[];
			 */
			char data[1];
		};

		struct player_info_23_list 
		{
			multi_data_header header;
			/*
			   struct		//假设的代码，实际上由于info2/3是变长的结构，所以无法这样组织
			   {
			   int cid;
			   player_info_2 info2;
			   player_info_3 info3;
			   }list[];
			 */
			char data[1];
		};

		struct	npc_info_list 		// npc list
		{
			multi_data_header header;
			npc_info list[1];
		};

		struct matter_info_list
		{
			multi_data_header header;
			matter_info_1 list[1];
		};

		struct matter_enter_world
		{
			single_data_header header;
			matter_info_1 data;
		};

		struct npc_enter_world
		{
			single_data_header header;
			npc_info info;
		};

		struct player_info_00
		{
			single_data_header header;
			int pid;
			INFO::player_info_00 info;
		};

		struct self_info_00
		{
			single_data_header header;
			INFO::self_info_00 info;
			
		};

		struct npc_info_00
		{
			single_data_header header;
			int nid;
			INFO::npc_info_00 info;
		};
		
		struct object_move
		{
			single_data_header header;
			move_info data;
		};
		struct player_enter_world
		{
			single_data_header header;
			player_info_1	info;
		};
		struct player_leave_world
		{
			single_data_header header;
			int	   cid;
		};
		struct player_select_target
		{
			single_data_header header;
			int	id;
		};

		struct npc_dead
		{
			single_data_header header;
			int	   nid;
			int 	   attacker;
		};
		struct object_disappear
		{
			single_data_header header;
			int	   nid;
		};
		struct object_start_attack
		{
			single_data_header header;
			int	oid;
			int	tid;	//target id
			unsigned char   attack_stamp;
		};

		struct self_stop_attack
		{	
			single_data_header header;
			int	flag;
		};

		struct hurt_result //hurt_result
		{
			single_data_header header;
			int 	target_id;
			int	damage;
		};

		struct object_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int damage;
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			unsigned char attack_stamp;	//攻击的时间戳
			
		};

		struct object_skill_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int skill_id;
			int damage;
			int dt_damage;
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			unsigned char attack_stamp;
			
		};

		struct error_msg
		{
			single_data_header header;
			int  msg;
		};

		struct be_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	damage;
			unsigned char eq_index; //高位代表这次攻击是不是应该变橙色
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			char speed;			//攻击速度
		};

		struct be_skill_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	skill_id;
			int 	damage;
			unsigned char eq_index; //高位代表这次攻击是不是应该变橙色
			unsigned char attack_flag;	//标记该攻击是否有攻击优化符和防御优化符和重击发生
			char speed;			//攻击速度
		};


		struct be_hurt
		{
			single_data_header header;
			int 	attacker_id;
			int 	damage;
			unsigned char invader;
		};
		
		struct player_dead
		{
			single_data_header header;
			int  killer;
			int  player;
		};
		struct be_killed
		{
			single_data_header header;
			int  killer;
			A3DVECTOR pos;
			int time;	//复活需要的时间， 缺省为0
		};

		struct player_revival
		{
			single_data_header header;
			int id;
			short type;	//复活的类型 0 回城复活，1 开始复活 2：复活完全完成
			A3DVECTOR pos;
		};
		struct player_pickup_money
		{
			single_data_header header;
			int amount;
		};
		struct player_pickup_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
			int state;
		};

		struct player_purchase_item
		{
			single_data_header header;
			int trade_id;
			size_t cost;
			unsigned char type;		//表示顾客买入还是卖出
			unsigned short item_count;
			struct
			{
				int item_id;
				unsigned short count;
				unsigned short inv_index;
				unsigned char  stall_index;
				int state;
			} item_list[];
		};
		
		struct OOS_list
		{
			single_data_header header;
			unsigned int count;
			int id_list[1];
		};
		struct object_stop_move
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;
		};
		struct receive_exp
		{
			single_data_header header;
			int64_t exp;
		};
		
		struct receive_bonus_exp
		{
			single_data_header header;
			double exp;
		};

		struct level_up
		{
			single_data_header header;
			int id;
		};

		struct unselect 
		{
			single_data_header header;
		};
		
		struct self_item_info
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int type;
			int expire_date;
			size_t count;
			unsigned short crc;
			unsigned short content_length;
			char content[];
		};
		struct self_item_empty_info
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
		};
		struct self_inventory_data
		{
			single_data_header header;
			unsigned char where;
			unsigned char inv_size;
			size_t content_length;
			char content[];
		};

		struct self_inventory_detail_data
		{
			single_data_header header;
			unsigned char where;
			unsigned char inv_size;
			size_t content_length;
			char content[];
		};

		/**
		 * @brief 物品回购包裹信息，发的是detail_data详细信息
		 */
		struct repurchase_inventory_data
		{
			single_data_header header;
			unsigned char inv_size;
			size_t content_length;
			char content[];
		};

		struct  exchange_inventory_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};
		
		struct move_inventory_item
		{
			single_data_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short count;
		};

		struct player_drop_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			unsigned short count;
			int type;
			unsigned char drop_type;
		};
		struct exchange_equipment_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		// Youshuang 
		struct send_talisman_enchant_addon
		{
			single_data_header header;
			int id;
			int index;
		};
		
		struct fashion_colorant_produce
		{
			single_data_header header;
			int index;
			int color;
			int quality;
		};
		
		struct fashion_colorant_use
		{
			single_data_header header;
			int where;
			int index;
			int color;
			int quality;
		};

		struct get_achieve_award
		{
			single_data_header header;
			unsigned short achieve_id;
		};
		
		struct raid_boss_been_killed
		{
			single_data_header header;
			unsigned short boss_cnt;
			struct
			{
				unsigned char is_killed;
				unsigned char achievement;
				unsigned char cur_num;
				unsigned char max_num;
			}boss[1];
		};

		struct facbase_mall_item_info
		{
			single_data_header header;
			unsigned short items_cnt;
			int actived_items[1];
		};

		struct combine_mine_state_change
		{
			single_data_header header;
			int matter_id;
			int state;
		};

		struct get_faction_coupon
		{
			single_data_header header;
			int faction_coupon;
		};

		struct get_facbase_cash_items_info
		{
			single_data_header header;
			unsigned short items_cnt;
			struct
			{
				int idx;
				int cooldown;
			}item_cooldown[1];
		};

		struct player_change_puppet_form 
		{
			single_data_header header;
			int player_id;
		};

		struct get_facbase_cash_item_success
		{
			single_data_header header;
			int item_index;
		};

		struct get_newyear_award_info
		{
			single_data_header header;
			int score;
			int cnt;
			char award_level[1];
		};
		
		struct player_use_title
		{
			single_data_header header;
			short title;
		};
		// end
		
		struct equip_item
		{
			single_data_header header;
			unsigned char index_inv;
			unsigned char index_equip;
			unsigned short count_inv;
			unsigned short count_equip;
		};

		struct move_equipment_item
		{
			single_data_header header;
			unsigned char index_inv;
			unsigned char index_equip;
			unsigned short amount;
		};

		struct self_get_property
		{
			single_data_header header;
			unsigned int index_inv;
		//	q_extend_prop prop;		//不想引用头文件，所以注释掉了
		};


		struct player_extprop_base
		{
			single_data_header header;
			int id;
			int max_hp;		//最大hp
			int max_mp;		//最大mp
			int max_dp;
		};

		struct player_extprop_move
		{
			single_data_header header;
			int id;
			float walk_speed;	//行走速度 单位  m/s
			float run_speed;	//奔跑速度 单位  m/s
		};

		struct player_extprop_attack
		{
			single_data_header header;
			int id;
			int attack;		//攻击率 attack rate
			int damage_low;		//最低damage
			int damage_high;	//最大物理damage
			float attack_range;	//攻击范围
			int crit_rate;		//致命一击概率 千分数
			float crit_damage;	//致命一击时造成的额外伤害

		};

		struct player_extprop_defense
		{
			single_data_header header;
			int id;
			int resistance[6];	//魔法抗性
			int defense;		//防御力
			int armor;		//闪躲率（装甲等级）
		};
		
		struct team_leader_invite
		{
			single_data_header header;
			int leader;
			int seq;
			short pickup_flag;
			int familyid;
			int mafiaid;
			int level;
			int sectid;
			int referid;
		};

		struct team_reject_invite
		{
			single_data_header header;
			int member;
		};

		struct team_join_team
		{
			single_data_header header;
			int leader;
			short pickup_flag;
		};

		struct  team_member_leave
		{
			single_data_header header;
			int leader;
			int member;
			short type;
		};
		
		//表明自己离开了队伍
		struct team_leave_party
		{
			single_data_header header;
			int leader;
			short type;
		};

		struct team_new_member
		{
			single_data_header header;
			int member;
		};

		struct team_leader_cancel_party
		{
			single_data_header header;
			int leader;
		};

		struct team_member_data
		{
			single_data_header header;
			unsigned char member_count;
			unsigned char data_count;
			int leader;
			struct 
			{
				int id;
				short level;
				short dt_level;
				unsigned char combat_state;
				unsigned char pk_level;
				unsigned char wallow_level;
				unsigned char cls_type;
				int hp;
				int mp;
				int dp;
				int max_hp;
				int max_mp;
				int max_dp;
				int family_id;
				int master_id;
				char reborn_cnt;
				int rage;
			} data[1];
		};

		struct teammate_pos
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int tag;
		};

		struct send_equipment_info
		{
			single_data_header header;
			unsigned short crc;
			int id;		//who
			unsigned int mask;
			struct 
			{
				short item;
				short mask;
			}data[];	//0 ~ 16
		};

		struct send_clone_equipment_info
		{
			single_data_header header;
			int id;		//who
			player_info_1 info;
			unsigned short crc;
			unsigned int mask;
			struct 
			{
				short item;
				short mask;
			}data[];	//0 ~ 16
		};
		
		struct equipment_info_changed
		{
			single_data_header header;
			unsigned short crc;
			int id;		//who
			unsigned int mask_add;
			unsigned int mask_del;
			struct 
			{
				short item;
				short mask;
			}data_add[];	//0 ~ 16
		};

		struct equipment_damaged
		{
			single_data_header header;
			unsigned char index;
		};

		struct team_member_pickup
		{
			single_data_header header;
			int id;
			int type;
			int count;
		};

		struct npc_greeting
		{
			single_data_header header;
			int id;
		};

		struct npc_service_content
		{
			single_data_header header;
			int id;
			int type;	//服务的类型
			size_t length;
			char data[];
		};

		struct item_to_money
		{
			single_data_header header;
			unsigned short index;		//在包裹栏里的索引号码
			int type;			//物品的类型
			size_t count;
			size_t money;
		};

		struct repair_all
		{
			single_data_header header;
			size_t cost;
		};
		
		struct repair
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			size_t cost;
		};

		struct renew
		{
			single_data_header header;
		};

		struct spend_money
		{
			single_data_header header;
			size_t cost;
		};

		struct player_pickup_money_in_trade
		{
			single_data_header header;
			int amount;
		};
		struct player_pickup_item_in_trade
		{
			single_data_header header;
			int type;
			unsigned short amount;
		};

		struct player_pickup_money_after_trade
		{
			single_data_header header;
			size_t amount;
		};
		struct player_pickup_item_after_trade
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned short index;
			int state;
		};
		struct get_own_money
		{	
			single_data_header header;
			size_t amount;
			size_t capacity;
		};
		struct object_attack_once
		{
			single_data_header header;
			unsigned char arrow_dec;
		};

		struct object_cast_skill
		{
			single_data_header header;
			int caster;
			int skill;
			unsigned short time;
			unsigned char level;
			unsigned char attack_stamp;
			unsigned char state;
			short cast_speed_rate;
			A3DVECTOR skillpos;
			char spirit_index; //0-2, 灵的index
			int target_cnt;
			int targets[];
		};

		struct skill_interrupted
		{
			single_data_header header;
			int caster;
			char spirit_index; //0-2, 灵的index
		};

		struct self_skill_interrupted
		{
			single_data_header header;
			unsigned char reason;
			char spirit_index; //0-2, 灵的index
		};

		struct skill_perform
		{
			single_data_header header;
			char spirit_index; //0-2, 灵的index
		};

		struct object_be_attacked
		{
			single_data_header header;
			int id;
		};
		
		struct skill_data
		{
			single_data_header header;
			char content[];
		};

		struct player_use_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int item_id;
			unsigned short use_count;
		};

		struct embed_item
		{
			single_data_header header;
			unsigned char chip_idx;
			unsigned char equip_idx;
		};

		struct clear_embedded_chip
		{
			single_data_header header;
			unsigned short equip_idx;
			size_t  cost;
		};

		struct cost_skill_point
		{
			single_data_header header;
			int skill_point;
		};

		struct learn_skill
		{
			single_data_header header;
			int skill_id;
			int skill_level;
		};

		struct object_takeoff
		{
			single_data_header header;
			int object_id;
		};

		struct object_landing
		{
			single_data_header header;
			int object_id;
		};

		struct flysword_time_capacity
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int cur_time;
		};

		struct player_obtain_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
			int state;
		};

		struct produce_start
		{
			single_data_header header;
			unsigned short use_time;
			unsigned short count;
			int type;
		};

		struct produce_once
		{
			single_data_header header;
			int type;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
			int state;
		};
		
		struct produce_end
		{
			single_data_header header;
		};

		struct decompose_start
		{
			single_data_header header;
			unsigned short use_time;
			int type;
		};

		struct decompose_end
		{
			single_data_header header;
		};

		struct task_data
		{
			single_data_header header;
			size_t active_list_size;
			char active_list[1];
			size_t finished_list_size;
			char finished_list[1];
			size_t finished_time_size;
			char finished_time[1];
		};

		struct task_var_data
		{
			single_data_header header;
			size_t size;
			char data[1];
		};

		struct object_start_use
		{
			single_data_header header;
			int user;
			int item;
			unsigned short time;
		};
		
		struct object_cancel_use
		{
			single_data_header header;
			int user;
		};

		struct object_use_item
		{
			single_data_header header;
			int user;
			int item;
		};
		
		struct object_start_use_with_target
		{
			single_data_header header;
			int user;
			int target;
			int item;
			unsigned short time;
		};

		struct object_sit_down
		{
			single_data_header header;
			int id;
		//		A3DVECTOR pos; $$$$$$$$$$$$$
		};

		struct object_stand_up
		{
			single_data_header header;
			int id;
		};

		struct object_do_emote
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct server_timestamp
		{
			single_data_header header;
			int timestamp;
			int timezone_bias;
			int lua_version;
		};

		struct notify_root
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
		};

		struct self_notify_root
		{
			single_data_header header;
			A3DVECTOR pos;
			unsigned char type;
		};

		struct dispel_root
		{
			single_data_header header;
			unsigned char type;
		};

		struct pk_level_notify
		{
			single_data_header header;
			int id;
			char pk_level;
		};

		struct pk_value_notify
		{
			single_data_header header;
			int id;
			int value;
		};

		struct player_change_class
		{
			single_data_header header;
			int id;
			unsigned char cls;
		};

		struct self_stop_skill
		{
			single_data_header header;
			char spirit_index; //0-2, 灵的index
		};

		struct update_visible_state
		{
			single_data_header header;
			int oid;
			int64_t newstate;
		};
	
		struct object_buff_notify               //OBJECT_BUFF_NOTIFY
		{       
			single_data_header header;
			int oid;
			unsigned short count;           
			struct
			{
				unsigned short state;
				unsigned short level;
				char overlay_cnt;
			}list[];
		};

		struct self_buff_notify			//SELF_BUFF_NOTIFY
		{       
			single_data_header header;
			int oid;
			unsigned short count;
			struct
			{
				unsigned short state;
				unsigned short level;
				int buff_endtime;
				char overlay_cnt;
			}list[];
		};
	
		struct player_gather_start
		{
			single_data_header header;
			int pid;		//player id
			int mid;		//mine id
			unsigned char use_time; //use time in sec;
		};

		struct player_gather_stop
		{
			single_data_header header;
			int pid;		//player id
		};

		struct trashbox_passwd_changed
		{
			single_data_header header;
			unsigned char has_passwd;
		};

		struct trashbox_passwd_state
		{
			single_data_header header;
			unsigned char has_passwd;
		};
		
		struct trashbox_open
		{
			single_data_header header;
			unsigned char where;
			unsigned short trashbox_size;
		};

		struct trashbox_close
		{
			single_data_header header;
		};

		struct trashbox_wealth
		{
			single_data_header header;
			size_t money;
		};

		struct exchange_trashbox_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char idx1;
			unsigned char idx2;
		};
		struct move_trashbox_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char src;
			unsigned char dest;
			size_t amount;
		};
		
		struct exchange_trashbox_inventory
		{
			single_data_header header;
			unsigned char where;
			unsigned char idx_tra;
			unsigned char idx_inv;
		};
		
		struct trash_item_to_inventory
		{
			single_data_header header;
			unsigned char where;
			unsigned char src;
			unsigned char dest;
			size_t amount;
		};
		
		struct inventory_item_to_trash
		{
			single_data_header header;
			unsigned char where;
			unsigned char src;
			unsigned char dest;
			size_t amount;
		};
		
		struct exchange_trash_money
		{
			single_data_header header;
			int inv_delta;
			int tra_delta;
		};

		struct enchant_result
		{
			single_data_header header;
			int 	caster;
			int 	target;
			int 	skill;
			char 	level;
			char	orange_name;
			char 	at_state;
			char	attack_stamp;
			int     enchant_value;
		};
		
		struct object_do_action
		{
			single_data_header header;
			int id;
			unsigned char emotion;
		};

		struct player_set_adv_data
		{
			single_data_header header;
			int id;
			int data1;
			int data2;
			
		};

		struct player_clr_adv_data 
		{
			single_data_header header;
			int id;
		};

		struct player_in_team
		{
			single_data_header header;
			int id;
			unsigned char state;	// 0 no team 1, leader, 2 member
		};

		struct team_apply_request
		{
			single_data_header header;
			int id;			//who
			int familyid;
			int mafiaid;
			int level;
			int sectid;
			int referid;
		};
		
		struct object_do_emote_restore
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct concurrent_emote_request
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct do_concurrent_emote 
		{
			single_data_header header;
			int id1;
			int id2;
			unsigned short emotion;
		};

		struct matter_pickup
		{
			single_data_header header;
			int matter_id;
			int who;
		};

		struct mafia_info_notify
		{
			single_data_header header;
			int pid;
			int mafia_id;
			int family_id;
			char mafia_rank;
		};

		struct mafia_trade_start
		{
			single_data_header header;
		};

		struct mafia_trade_end
		{
			single_data_header header;
		};
		
		struct task_deliver_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//最后部分放在哪个位置
			int state;                  //item状态：绑定、装备后绑定、锁定
		};

		struct task_deliver_reputaion
		{
			single_data_header header;
			int delta;
			int cur_reputaion;
		};

		struct task_deliver_exp
		{
			single_data_header header;
			int64_t exp;
			int sp;
			int dt_exp;	//封神经验
		};

		struct task_deliver_money
		{
			single_data_header header;
			size_t amount;
			size_t cur_money;
		};

		struct task_deliver_level2
		{
			single_data_header header;
			int level2;
		};

		struct player_reputation
		{
			single_data_header header;
			int who;
			int reputation;
		};

		struct identify_result
		{
			single_data_header header;
			char index;
			char result;	//0 	
		};

		struct player_change_shape
		{
			single_data_header header;
			int pid;
			char shape;
		};

		struct player_enter_sanctuary
		{
			single_data_header header;
		};

		struct player_leave_sanctuary
		{
			single_data_header header;
		};

		struct player_open_market
		{
			single_data_header header;
			int item_id; //不使用道具摆摊时为-1
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];		//最大28
		};

		struct self_open_market
		{
			single_data_header header;
			int index;    //不使用道具摆摊时为-1
			int item_id;  //不使用道具摆摊时为-1
			unsigned short count;
			struct 
			{
				int type;		//物品类型
				unsigned short index;	//如果是0xFFFF，表示是购买
				unsigned short count;	//卖多少个
				size_t price;		//单价
			} item_list;
			
		};

		struct player_cancel_market
		{
			single_data_header header;
			int pid;
		};

		struct player_market_info
		{
			single_data_header header;
			int pid;
			int market_id;
			size_t count;
			market_goods item_list[];
		};

		struct player_market_trade_success
		{
			single_data_header header;
			int trader;
		};

		struct player_market_name
		{
			single_data_header header;
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];	//最大28字节
		};

		struct player_start_travel
		{
			single_data_header header;
			int pid;
			unsigned char vehicle;
		};

		struct self_start_travel
		{
			single_data_header header;
			float speed;
			A3DVECTOR dest;
			int line_no;
			unsigned char vehicle;
		};

		struct player_complete_travel
		{
			single_data_header header;
			int pid;
			unsigned char vehicle;
		};

		struct gm_toggle_invisible
		{
			single_data_header header;
			unsigned char is_visible;
		};

		struct gm_toggle_invincible
		{
			single_data_header header;
			unsigned char is_invincible;
		};

		struct self_trace_cur_pos
		{
			single_data_header header;
			A3DVECTOR pos;
			unsigned short seq;
		};

		struct object_cast_instant_skill
		{
			single_data_header header;
			int id;
			int skill;
			unsigned char level;
			A3DVECTOR skillpos;
			char spirit_index; //0-2, 灵的index
			int target_cnt;
			int targets[];
		};

		struct activate_waypoint
		{
			single_data_header header;
			unsigned short waypoint;
		};

		struct player_waypoint_list
		{
			single_data_header header;
			size_t count;
			unsigned short list[];
		};
		
		struct unlock_inventory_slot
		{
			single_data_header header;
			unsigned char where;
			unsigned short index;
		};

		struct team_invite_timeout
		{
			single_data_header header;
			int who;
		};

		struct player_pvp_no_protect
		{
			single_data_header header;
			int who;
		};

		struct cooldown_data
		{
			single_data_header header;
			unsigned short count;
			struct
			{
				unsigned short idx;
				int cooldown;
			}list[1];
		};

		struct skill_ability_notify
		{
			single_data_header header;
			int skill_id;
			int skill_ability;
		};

		struct personal_market_available
		{
			single_data_header header;
			int index;    //不使用摆摊道具时该值为-1
			int item_id;  //不使用摆摊道具时该值为-1
		};

		struct breath_data
		{
			single_data_header header;
			int breath;
			int breath_capacity;
		};

		struct player_stop_dive
		{
			single_data_header header;
		};

		struct trade_away_item
		{
			single_data_header header;
			short inv_index;
			int item_type;
			size_t item_count;
			int buyer;
		};

		struct player_enable_fashion_mode
		{
			single_data_header header;
			int who;
			int fashion_mask;
			unsigned char is_enable;
		};

		struct enable_free_pvp_mode
		{
			single_data_header header;
			unsigned char enable_type;	//0 close 1 free pvp 2 cultivation pvp 3 race pvp
		};

		struct object_is_invalid
		{
			single_data_header header;
			int id;
		};

		struct player_enable_effect
		{
			single_data_header header;
			short effect;
			int id;
		};

		struct player_disable_effect
		{
			single_data_header header;
			short effect;
			int id;
		};
		
		struct enable_resurrect_state
		{
			single_data_header header;
			float exp_reduce;
		};

		struct set_cooldown
		{
			single_data_header header;
			int cooldown_index; 
			int cooldown_time;
		};

		struct change_team_leader
		{
			single_data_header header;
			int old_leader; 
			int new_leader;
		};

		struct kickout_instance
		{
			single_data_header header;
			int instance_tag; 
			int timeout;		//如果是-1表示取消此次踢出
		};

		struct player_cosmetic_begin
		{
			single_data_header header;
			unsigned short index;
		};

		struct player_cosmetic_end
		{
			single_data_header header;
			unsigned short index;
		};

		struct cosmetic_success
		{
			single_data_header header;
			unsigned short crc;
			int id;
		};

		struct object_cast_pos_skill
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int skill;
			unsigned short time;
			unsigned char level;
		};

		struct change_move_seq
		{
			single_data_header header;
			unsigned short seq;
		};

		struct server_config_data
		{
			single_data_header header;
			int world_tag;
			int region_time;
			int precinct_time;
			int mall_time;
		};

		struct player_rush_mode
		{
			single_data_header header;
			char is_active;
		};

		struct trashbox_capacity_notify
		{
			single_data_header header;
			int capacity;
		};

		struct npc_dead_2
		{
			single_data_header header;
			int nid;
			int attacker;
		};

		struct produce_null
		{
			single_data_header header;
			int recipe_id;
		};

		struct active_pvp_combat_state
		{
			single_data_header header;
			int who;
			char is_active;
		};

		struct duel_recv_request
		{
			single_data_header header;
			int player_id;
		};

		struct duel_reject_request
		{
			single_data_header header;
			int player_id;
			int reason;
		};

		struct duel_prepare
		{
			single_data_header header;
			int player_id;
			int delay;		//sec
		};
		
		struct duel_cancel
		{
			single_data_header header;
			int player_id;
		};

		struct duel_start
		{
			single_data_header header;
			int player_id;
		};
		
		struct duel_stop
		{
			single_data_header header;
			int player_id;
		};
		
		struct duel_result
		{
			single_data_header header;
			int id1;
			int id2;
			char result;	//1 id1 win 2 draw
		};

		struct player_bind_request
		{
			single_data_header header;
			int who;
		};

		struct player_bind_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bind_request_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_start
		{
			single_data_header header;
			int mule;
			int rider;
		};

		struct player_bind_stop
		{
			single_data_header header;
			int who;
		};

		struct player_mounting
		{
			single_data_header header;
			int id;
			int mount_id;
			char mount_level;
			char mount_type; // 0 - 默认类型   1 - 多人骑乘类型 
		};

		struct player_equip_detail
		{
			single_data_header header;
			int id;
			size_t content_length;
			char content[];
		};
		// skill
		// pet
		// pet_equip

		struct else_duel_start
		{
			single_data_header header;
			int player_id;
		};

		struct pariah_duration
		{
			single_data_header header;
			int time_left;
		};

		struct enter_battleground
		{
			single_data_header header;
			unsigned char battle_faction;	// 0 中立 1 攻方 2 守方 3 旁观 4 混战
			unsigned char battle_type;
			int battle_id;			// 战场id
			int end_timestamp;		// 结束时间

		};
		
		struct leave_battleground
		{
			single_data_header header;
		};

		struct enter_battlefield
		{
			single_data_header header;
			int faction_id_attacker;
			int faction_id_defender;
			int faction_id_attacker_assistant;
			int battle_id;			// 战场id
			int end_timestamp;		// 结束时间
		};
		
		struct leave_battlefield
		{
			single_data_header header;
		};

		struct battlefield_info
		{
			single_data_header header;
			unsigned char count;
			struct structure_info_t
			{
				unsigned char key;
				unsigned char status; //0 death
				A3DVECTOR pos;
				float hp_factor;
			}nodes[0];
		};

		struct battlefield_start
		{
			single_data_header header;
		};

		struct battlefield_end
		{
			single_data_header header;
			unsigned char result; // 1:攻方获胜 2:守方获胜
		};

		struct turret_leader_notify
		{
			single_data_header header;
			int turret_id;
			int turret_tid;
			int leader_id;
		};

		struct battle_result
		{
			single_data_header header;
			int result;
			int attacker_score;
			int defender_score;
		};

		struct player_recipe_data
		{
			single_data_header header;
			unsigned short count;
			unsigned short buf[];
		};

		struct player_learn_recipe
		{
			single_data_header header;
			unsigned short recipe_id;
		};
		
		struct player_produce_skill_info
		{
			single_data_header header;
			int level;
			int exp;
		};

		struct player_refine_result
		{
			single_data_header header;
			int item_index;
			int item_id;
			int result; // 0 成功 1 失败 删除材料 2失败删除材料 删除属性 3失败 装备爆掉 4 拆卸属性石 
				    // 5拆卸技能石
		};

		struct player_start_general_operation
		{
			single_data_header header;
			int player_id;
			int op_id;
			int duration;	//以0.05秒为单位的tick
		};

		struct player_stop_general_operation
		{
			single_data_header header;
			int player_id;
			int op_id;
		};

		struct player_cash
		{
			single_data_header header;
			int cash_amount;
			int cash_used;
			int cash_add2;
		};

		struct player_bonus
		{
			single_data_header header;
			int bonus_amount;
			int bonus_used;
		};

		struct player_skill_addon
		{
			single_data_header header;
			int skill;
			int level;
		};

		struct player_skill_common_addon
		{
			single_data_header header;
			int addon;
		};
		struct player_extra_skill
		{
			single_data_header header;
			int skill;
			int level;
		};

		struct player_title_list
		{
			single_data_header header;
			size_t count;	//不是 byte size
			short title[];
		};
		struct player_add_title
		{
			single_data_header header;
			short title;
		};
		struct player_del_title
		{
			single_data_header header;
			short title;
		};

		struct player_region_reputation
		{
			single_data_header header;
			int count;
			int reputation[];
		};

		struct player_change_region_reputation
		{
			single_data_header header;
			int index;
			int new_reputation;
		};

		struct player_change_title
		{
			single_data_header header;
			int who;
			short title;
		};

		struct player_change_inventory_size
		{
			single_data_header header;
			int new_size;
		};

		struct player_change_mountwing_inv_size
		{
			single_data_header header;
			int new_size;
		};

		struct player_bind_success
		{
			unsigned short inv_index;
			int item_id;
			int item_state;	 
		};

		struct player_change_spouse
		{
			single_data_header header;
			int who;
			int id;
		};

		struct player_invader_state
		{
			single_data_header header;
			int who;
			char bState;
		};

		struct player_mafia_contribution
		{
			single_data_header header;
			int contribution;
			int family_contribution;
		};

		struct lottery_bonus
		{
			single_data_header header;
			int lottery_id;
			int bonus_level;
			int bonus_item;
			int bonus_count;
			int bonus_money;
		};

		struct recorder_start
		{
			single_data_header header;
			int timestamp;
		};

		struct recorder_check_result
		{
			single_data_header header;
			char value[16];
		};

		struct player_use_item_with_arg
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int item_id;
			unsigned short use_count;
			unsigned short arg_size;
			char arg[];
		};

		struct object_use_item_with_arg
		{
			single_data_header header;
			int user;
			int item;
			unsigned short arg_size;
			char arg[];
		};

		struct mafia_trashbox_capacity_notify
		{
			single_data_header header;
			int cap;
		};

		struct npc_service_result
		{
			single_data_header header;
			int type;
		};

		// 只能发给附近的人
		struct battle_flag_change
		{
			single_data_header header;
			int matter_id;
			char origin_state;
			char new_state;
		};

		struct battle_player_info
		{
			single_data_header header;
			int id;
			unsigned char battle_faction;
			unsigned int battle_score;
			unsigned short kill;
			unsigned short death;
		};

		struct battle_info
		{
			struct player_info_in_battle
			{
				int id;
				unsigned char battle_faction;
				unsigned int battle_score;
				unsigned short kill;
				unsigned short death;
			};
			single_data_header header;
			unsigned char attacker_building_left;
			unsigned char defender_building_left;
			unsigned char player_count;
			player_info_in_battle info[0];
		};
		// 战场开始
		struct battle_start
		{
			single_data_header header;
		};

		// 广播使用
		struct battle_flag_change_status
		{
			single_data_header header;
			int flag_id;
			char origin_state;
			char new_state;
		};

		struct player_wallow_info               
		{       
			single_data_header header;
			unsigned char anti_wallow_active;
			unsigned char wallow_level;
			int msg;
			int play_time;
			int light_timestamp;
			int heavy_timestamp;
		};

		struct talisman_exp_notify
		{
			single_data_header header;
			unsigned short where;
			unsigned short index;
			int new_exp;
		};

		struct talisman_combine
		{
			single_data_header header;
			int id;
			int type1;
			int type2;
			int type3;
		};

		struct talisman_value
		{
			single_data_header header;
			int value;
		};

		struct auto_bot_begin
		{
			single_data_header header;
			unsigned char active;
		};

		struct anti_cheat_bonus
		{
			single_data_header header;
			unsigned short bonus_type;
		};
		
		// 玩家的战场积分
		struct battle_score
		{	
			single_data_header header;
			size_t score;
		};
		struct gain_battle_score
		{
			single_data_header header;
			size_t inc;
		};
		struct spend_battle_score
		{
			single_data_header header;
			size_t dec;
		};

		struct talent_notify
		{
			single_data_header header;
			int value;
		};

		struct notify_safe_lock
		{
			single_data_header header;
			unsigned char active;
			int time;
			int max_time;
		};

		struct battlefield_construction_info
		{
			single_data_header header;
			int res_a;
			int res_b;
			struct node_t
			{
				int type;
				int key;
				struct attr_t
				{
					unsigned char attack_level;
					unsigned char defence_level;
					unsigned char range_level;
					unsigned char ai_level;
					int maxhp_level;
				};
				attr_t attr;
			};
			unsigned char count;
			node_t nodes[0];
		};

		struct battlefield_contribution_info
		{
			single_data_header header;
			struct node_t
			{
				int player_id;
				int res_a;
				int res_b;
			};
			unsigned char page;		//当前页号,0开头
			unsigned char max_page;		//最大页号,0开头
			unsigned char count;		//个数
			node_t nodes[0];
		};

		struct player_equip_pet_bedge
		{
			single_data_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};
		
		struct player_equip_pet_equip
		{
			single_data_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};

		struct player_set_pet_status
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char main_status;	// 0 战斗 1 采集 2 制造 3 休息
			unsigned char sub_status;	// 只有main_status为 1时有用 ,0 种植 1 伐木 2 狩猎 3 钓鱼 4 采矿 5 考古
		};

		struct player_summon_pet
		{
			single_data_header header;
			unsigned char pet_index;        
			int pet_tid;			//宠物的模版id
			int pet_id;			//宠物的实际id
		};

		struct player_recall_pet
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;			//宠物的模版id
			int pet_id;			//宠物的实际id
		};

		struct player_combine_pet
		{
			single_data_header header;
			unsigned char pet_index;        //宠物索引
			int pet_tid;			//宠物的模版id
			unsigned char type;		//0 通灵,1 御宝
		};

		struct player_uncombine_pet
		{
			single_data_header header;
			unsigned char pet_index;	//宠物索引
			int pet_tid;			//宠物的模版id
		};

		struct player_start_pet_op
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int delay;			//延迟时间,单位是50ms的tick
			unsigned char operation;	//操作类型 0:放出 1:召回 2:通灵 3:解除通灵 4:御宝 5:解除御宝
		};

		struct player_stop_pet_op
		{
			single_data_header header;
			unsigned char operation;
		};

		struct player_pet_recv_exp
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int cur_exp;			//增加的经验
		};

		struct player_pet_levelup
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int new_level;			//新级别
			int cur_exp;			//当前的经验值 
		};
		
		struct player_pet_honor_point
		{
			single_data_header header;
			unsigned char pet_index;
			int cur_honor_point;
			int max_honor_point;
		};

		struct player_pet_hunger_point
		{
			single_data_header header;
			unsigned char pet_index;
			int cur_hunge_point;
			int max_hunge_point;
		};

		struct player_pet_age_life
		{
			single_data_header header;
			unsigned char pet_index;
			int age;
			int life;
		};

		struct player_pet_hp_notify
		{
			single_data_header header;
			unsigned char pet_index;
			int cur_hp;
			int max_hp;
			int cur_vp;
			int max_vp;
		};

		struct player_pet_dead
		{
			single_data_header header;
			unsigned char pet_index;
		};

		struct player_pet_ai_state
		{
			single_data_header header;
			unsigned char pet_index;
			unsigned char stay_state;
			unsigned char aggro_state;
		};

		struct player_pet_room_capacity
		{
			single_data_header header;
			unsigned char capacity;
		};

		struct player_pet_set_auto_skill
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int skill_id;
			unsigned char set_flag; //1设置 0取消
		};

		struct player_pet_set_skill_cooldown
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_id;
			int cooldown_index;
			int cooldown_time;
		};

		struct battlefield_building_status_change
		{
			single_data_header header;
			unsigned char key;
			unsigned char status; //0 death
			A3DVECTOR pos;
			float hp_faction;
		};

		struct player_set_pet_rank
		{
			single_data_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char rank;
		};

		struct player_reborn_info
		{
			single_data_header header;
			unsigned short count;	//转了几次，不是字节数
			struct
			{
				short level;	//本次转生时的等级
				short prof;	//本次转生时的职业
			} info[0/*count*/];
		};

		struct script_message
		{
			single_data_header header;
			int player_id;
			int pet_id;
			unsigned short channel_id;
			unsigned short msg_id;
		};

		struct player_pet_civilization
		{
			single_data_header header;
			int civilization;	//宠物文明度
		};

		struct player_pet_construction
		{
			single_data_header header;
			int construction;	//宠物工坊建设度
		}; 

		struct player_move_pet_bedge
		{
			single_data_header header;
			unsigned char src_index;
			unsigned char dst_index;
		};

		struct player_start_fly
		{
			single_data_header header;
			int who;
			char active;
			char type; //0:飞剑, 1:技能
		};

		/**
		 * @brief 该结构与GMSV::role_pet_prop_added的结构相对应，后者用于角色交易
		 *        因此player_pet_prop_added修改时注意角色寄售是否需要改。
		 */
		struct player_pet_prop_added
		{
			single_data_header header;
			unsigned char pet_index;
			unsigned short maxhp;
			unsigned short maxmp;
			unsigned short defence;
			unsigned short attack;
			unsigned short resistance[6];
			unsigned short hit;
			unsigned short jouk;
			float crit_rate;
			float crit_damage;
			int equip_mask;
		};

		struct sect_become_disciple
		{
			single_data_header header;
			int who;
			int master_id;
		};

		struct cultivation_notify
		{
			single_data_header header;
			int who;
			char new_cult;
		};

		struct offline_agent_bonus
		{
			single_data_header header;
			int bonus_time;		//min
			int left_time; 		//min
			double bonus_exp;
		};

		struct offline_agent_time
		{
			single_data_header header;
			int left_time; 		//min
		};
		
		struct spirit_power_decrease
		{
			single_data_header header;
			short index;
			short dec_amount;
			int result;
		};

		struct player_bind_ride_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bind_ride_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_ride_join
		{
			single_data_header header;
			int leader;
			int member_id;
			char bind_pos;
		};

		struct player_bind_ride_leave
		{
			single_data_header header;
			int leader;
			int member_id;
			char bind_pos;
		};
		
		struct player_bind_ride_kick
		{
			single_data_header header;
			int leader;
			char bind_pos;
		};
		
		struct exchange_pocket_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_pocket_item
		{
			single_data_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct exchange_inventory_pocket_item
		{
			single_data_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
		};

		struct move_pocket_item_to_inventory
		{
			single_data_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
			unsigned short amount;
		};

		struct move_inventory_item_to_pocket
		{
			single_data_header header;
			unsigned char idx_inv;
			unsigned char idx_poc;
			unsigned short amount;
			
		};

		struct move_all_pocket_item_to_inventory
		{
			single_data_header header;
		};

		struct move_max_pocket_item_to_inventory
		{
			single_data_header header;
		};

		struct player_change_pocket_size
		{
			single_data_header header;
			int new_size;
		};

		struct notify_fashion_hotkey
		{
			single_data_header header;
			int count;
			struct key_combine
			{
				int index;
				int id_head;
				int id_cloth;
				int id_shoe;
			} key[];
		};

		struct exchange_fashion_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};
		
		struct exchange_inventory_fashion_item
		{
			single_data_header header;
			unsigned char idx_inv;
			unsigned char idx_fas;
		};

		struct exchange_equipment_fashion_item
		{
			single_data_header header;
			unsigned char idx_equ;
			unsigned char idx_fas;
		};

		/**
		 * @brief 坐骑飞剑包裹里交换物品
		 */
		struct exchange_mount_wing_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		/**
		 * @brief 普通包裹 <-> 坐骑飞剑包裹 
		 */
		struct exchange_inventory_mountwing_item
		{
			single_data_header header;
			unsigned char idx_inv;
			unsigned char idx_mw;
		};

		/**
		 * @brief 装备栏 <-> 坐骑飞剑包裹
		 */
		struct exchange_equipment_mountwing_item
		{
			single_data_header header;
			unsigned char idx_equ;
			unsigned char idx_mw;
		};

		struct exchange_hotkey_equipment_fashion
		{
			single_data_header header;
			unsigned char idx_key;
			unsigned char idx_head; //0xff代表那个部位不需要替换
			unsigned char idx_cloth;
			unsigned char idx_shoe;  	
		};
		
		struct peep_info
		{
			single_data_header header;
			int id;	
		};

		struct self_killingfield_info
		{
			single_data_header header;
			int battle_score;
			int rank;
			int conKill;
			int postion;
		};

		struct killingfield_info
		{
			struct player_info_in_killingfield
			{
				int id;
				unsigned char faction;
				int score;
				int rank;
				int conKill;
			};
			single_data_header header;
			int attacker_score;
			int defender_score;
			int attacker_count;
			int defender_count;
			int player_count;
			player_info_in_killingfield info[0];
		};

		struct arena_info
		{
			struct player_info_in_arena
			{
				int score;
				int kill_count;
			};
			single_data_header header;
			int kill_count;
			int score;
			int apply_count;
			int remain_count;
			int player_count;
			player_info_in_arena info[0];
		};

		struct rank_change  //玩家军衔变化
		{
			single_data_header header;
			int player_id;
			char new_rank;
		};

		struct change_killingfield_score
		{
			single_data_header header;
			int old_score;
			int new_score;
		};

		struct player_change_style
		{
			single_data_header header;
			int player_id;
			unsigned char faceid;
			unsigned char hairid;
			unsigned char earid;
		    unsigned char tailid;	
			unsigned char fashionid;
		};
		
		struct player_change_vipstate
		{
			single_data_header header;
			int player_id;
			unsigned char new_state;
		};

		struct crossserver_battle_info
		{
			single_data_header header;
			int attacker_score; //当前红方分数
			int defender_score; //当前蓝方分数
			int kill_count;
			int death_count;
			int attacker_off_line_score; //当前红方离线惩罚分数
			int defender_off_line_score; //当前蓝方离线惩罚分数
			unsigned char attacker_count; //当前红方人数
			unsigned char defender_count; //当前蓝方人数
			unsigned char min_player_deduct_score; //离线惩罚人数限制
			int end_timestamp;
		};

		struct crossserver_team_battle_info
		{
			struct history_score
			{
				int attacker_score;
				int defender_score;
			};

			single_data_header header;
			unsigned char battle_state; //战场当前状态，0表示休息，1表示战斗
			int cur_state_end_time;     //当前状态的结束时间，该值为绝对时间
			int attacker_score;         //当前红方分数
			int defender_score;         //当前蓝方分数
			int best_killer_id;         //每小局累计击杀数最高的player

			unsigned char history_round_count;  //历史局数 
			history_score score_info[0];
		};

		/**
		 * @brief 阵法开始启动，想客户端广播一条消息
		 */
		struct notify_circleofdoom_start
		{
			single_data_header header;
			int  sponsor_id; //阵法发起者的id
			char faction;    //1表示按组队收集，2表示按帮派收集，3表示按家族收集
		};
		
		/**
		 * @brief 阵法发起成功后，定时向客户端同步阵法内成员信息
		 */
		struct notify_circleofdoom_info
		{
			single_data_header header;
			int skill_id;     //阵法发起者所使用的阵法技能id
			int sponsor_id;   //阵法发起者的id 
			int member_count; //阵法成员的个数，不包含阵法发起者
			int member_id[0];
		};

		/**
		 * @brief 通知客户端哪个阵法停止
		 */
		struct notify_circleofdoom_stop
		{
			single_data_header header;
			int sponsor_id;  //阵法发起者的id
		};

		/**
		 * @brief 查询哪些限时商品正在销售 
		 */
		struct mall_item_price
		{
			struct saleitem_list
			{
				int   good_index;     //在商城里的index
				int   good_id;        //物品id
				int   remaining_time; //限时销售的剩余的秒数，-1代表没有时限
			};

			single_data_header header;
			int good_count;
			saleitem_list list[0];
		};

		/**
		 * @brief 通知客户端有哪些奖励可以领取
		 */
		struct vip_award_info
		{
			enum{IS_NOT_OBTAINED = 0, IS_OBTAINED = 1,};

			struct award_data
			{
				int		award_id;			//奖励id
				char	is_obtained;		//本奖励是否已经领取：0表示未领取, 1表示已经领取
			};

			single_data_header header;
			int vip_end_time;				//vip特殊物品领取的截止时间
			int recent_total_cash;			//近期（30天）的累计充值数
			int award_count;				//奖励数量
			award_data list[0];				//奖励id列表
		};

		/**
		 * @brief vip奖励等级发生变化时，广播通知该区域内的玩家
		 */
		struct vip_award_change
		{
			single_data_header header;
			int roleid;
			unsigned char vipaward_level;
			char is_hide;
		};

		/**
		 * @brief 通知客户端玩家身上有哪些在线倒计时奖励
		 */
		struct online_award_info
		{
			struct award_data
			{
				int		award_id;			//在线倒计时奖励id
				int		index;				//奖励倒计时到第几轮
				int		small_giftbag_id;	//本轮小礼包id
				int		remaining_time;		//本轮倒计时剩余多少秒，单位是秒
			};

			single_data_header header;
			char force_flag;				//强制客户端更新奖励信息标志，非0表示强制更新
			int  award_count;				//本条消息奖励的个数
			award_data list[0];
		};

		/**
		 * @brief 通知客户端是否已经可以领取在线奖励
		 */
		struct online_award_permit
		{
			single_data_header header;
			int  award_id;
			int  index;
			char permit_code;			//0  表示可以领取奖励, 1表示成功进包裹,可以进行下一轮的倒计时
										//-1 该礼包没激活, -2 该礼包已经领取, -3 没有找到该礼包
										//-4 该礼包对应的index没有小礼包信息, -5 无效的player，不能领取
										//-6 无效的index（index表示倒计时到那一轮）
										//-7 本轮的倒计时还没有结束
		};
		
		struct achievement
		{
			unsigned short achieve_id;	// 成就id 

			unsigned short premiss_mask;	// 以完成前提位图
			char premiss_count;		// 前提数量
			struct _premiss			// 前提的数据结构
			{
				char premiss_id;	// 前提id;
				char data[1];		// 前提数据
			} premiss[1];
		};

		struct achievement_spec_info
		{
			unsigned short id;
			int finish_time;
		};

		struct player_achievement
		{
			single_data_header header;

			size_t 	map_count;			//成就map大小
			char 	map_data[1];			//成就map数据

			unsigned short  active_count;		//激活中的成就数量
			achievement achieve[1];			//激活中的成就数据

			int achieve_point;			//成就点
			int current_achieve_point;		//剩余的成绩点

			size_t spec_count;			//特殊成就数量
			achievement_spec_info info[1];		//特殊成就数据

			// Youshuang add
			size_t award_map_count;
			unsigned char award_map_data[1];
			// end
		};

		struct player_achievement_finish
		{
			single_data_header header;
			unsigned short achieve_id;	
			int achieve_point;	
			int finish_time;
		};

		struct player_achievement_active
		{
			single_data_header header;
			achievement achieve;
		};

		struct player_premiss_data
		{
			single_data_header header;
			unsigned short achieve_id;	
			char premiss_id;
			char data[1];
		};
		struct player_premiss_finish
		{
			single_data_header header;
			unsigned short achieve_id;	
			char premiss_id;
		};
		struct player_achievement_map
		{
			single_data_header header;
			int 	target;
			int achieve_point;	
			size_t 	map_count;			//成就map大小
			char 	map_data[1];			//成就map数据
		};
		struct achievement_message
		{
			single_data_header header;
			unsigned short achieve_id;
			int param;
			unsigned char name_len;
			char name[];		
			int finish_time;
		};
		struct self_instance_info
		{
			single_data_header header;
			int level_score;
			int monster_score;
			int time_score;	
			int death_penalty_score;
		};
		struct instance_info
		{
			single_data_header header;
			int cur_level; 
			size_t level_count;
			char level_status[0];
		};
		struct skill_continue
		{
			single_data_header header;
			int caster_id;
			int skill_id;
			char spirit_index; //0-2, 灵的index
		};
		struct player_start_transform
		{
			single_data_header header;
			int who;
			int template_id;
			char active;
			char type;
			char level;
			char expLevel;
		};
		struct magic_duration_decrease
		{
			single_data_header header;
			short index;
			short dec_amount;
			int result;
		};
		struct magic_exp_notify
		{
			single_data_header header;
			unsigned short where;
			unsigned short index;
			int new_exp;
		};
		
		struct transform_skill_data
		{
			single_data_header header;
			char content[];
		};
		
		struct player_enter_carrier  
		{
			single_data_header header;
			int player_id;
			int carrier_id; 		//想要乘坐的交通工具id
			A3DVECTOR rpos;			//相对坐标和方向
			unsigned char rdir;
			unsigned char success;
		};

		struct player_leave_carrier 
		{
			single_data_header header;
			int player_id;
			int carrier_id; 		//想要离开的交通工具id
			A3DVECTOR pos;			//绝对坐标和方向
			unsigned char dir;
			unsigned char success;
		};
		
		struct player_move_on_carrier	
		{
			single_data_header header;
			move_info info;
		};

		struct player_stop_move_on_carrier
		{
			single_data_header header;
			int id;
			A3DVECTOR rpos;
			unsigned short speed;
			unsigned char rdir;
			unsigned char move_mode;
		};

		struct skill_proficiency
		{
			single_data_header header;
			int id;
			int proficiency;
		};

		struct notify_mall_prop
		{
			single_data_header header;
			int mall_prop; 
		};

		struct send_pk_message
		{
			single_data_header header;
			int killer;
			int deader;
		};

		struct update_combo_state
		{
			single_data_header header;
			char is_start;
		       	char color[5];	
		};

		struct enter_dim_state
		{
			single_data_header header;
			int id;
			bool state;
		};
		
		struct enter_invisible_state
		{
			single_data_header header;
			int id;
			bool state;
		};

		struct object_charge
		{
			single_data_header header;
			int id;
			char type;//0, 向目标瞬移 1, 向目标冲锋, 2, 向面向闪现 3, 弹飞  4 吸引  5 击退 6 背摔
			int target_id;
			A3DVECTOR destPos;
		};

		//服务器端发起的对人生效的瞬移
		struct object_try_charge
		{
			single_data_header header;
			char type;//0, 向目标瞬移 1, 向目标冲锋, 2, 向面向闪现 3, 弹飞  4 吸引  5 击退 6 背摔
			int target_id;
			A3DVECTOR destPos;
		};

		struct object_be_charge_to
		{
			single_data_header header;
			int id;
			char type;//0, 向目标瞬移 1, 向目标冲锋, 2, 向面向闪现 3, 弹飞  4 吸引  5 击退 6 背摔
			int target_id;
			A3DVECTOR destPos;
		};
		
		struct enter_territory
		{
			single_data_header header;
			unsigned char battle_faction;	// 0 中立 1 攻方 2 守方 3 旁观 4 混战
			unsigned char battle_type;
			int battle_id;			// 战场id
			int end_timestamp;		// 结束时间

		};

		struct leave_territory
		{
			single_data_header header;
		};

		struct last_logout_time
		{
			single_data_header header;
			int time;
		};
		
		struct combine_skill_data
		{
			single_data_header header;
			char content[];
		};
		
		struct circle_info_notify
		{
			single_data_header header;
			int roleid;
			unsigned int circle_id;
			unsigned char circle_title;
		};

		struct receive_deliveryd_exp
		{
			single_data_header header;
			int64_t exp;
			unsigned int award_type;
		};

		struct deity_levelup
		{
			single_data_header header;
			int id;
			short cur_lvl;	//当前等级
			char success;	//0 - 失败  1-成功
		};

		struct object_special_state
		{
			single_data_header header;
			int roleid;
			int type;
			char on;	//0- 停止 1-开始 
		};
		
		struct lottery2_bonus
		{
			single_data_header header;
			int lottery_id;
			int bonus_index;
			int bonus_level;
			int bonus_item;
			int bonus_count;
			int bonus_money;
		};

		struct gem_notify	
		{
			single_data_header header;
			int gem_id;
			int msg; 	//Define by enum GEM_NOTIFY_MSG
		};
		
		struct player_scale
		{
			single_data_header header;
			int roleid;
			char on;				//0, 进行缩放， //1，还原
			int scale_ratio;	 	//缩放比例, 百分数
		};

		struct player_move_cycle_area
		{
			single_data_header header;
			char mode;	//0, Add; 1, remove; 2, clear
			char type;	//0, permit area; 1, limit area;
			int idx;
			float radius;		//半径
			A3DVECTOR center;	//中心点
		};

		struct enter_raid
		{
			single_data_header header;
			unsigned char raid_faction;	// 0 中立 1 攻方 2 守方 3 旁观
			unsigned char raid_type;
			int raid_id;			// 副本id
			int end_timestamp;		// 结束时间
		};
		
		struct leave_raid
		{
			single_data_header header;
		};
		/* liuyue-facbase
		struct notify_raid_pos
		{
			single_data_header header;
			A3DVECTOR pos;
			int raid_map_id;		//副本地图号, 客户端加载资源应该用这个ID
			int tag;				//副本实例tag
		};
		*/
		struct player_raid_counter
		{
			single_data_header header;
			int cnt;
			struct
			{
				int raid_map_id;	//副本地图号
				int counter;		//当日进入次数
			} raid_counter[];
		};

		struct player_single_raid_counter
		{
			single_data_header header;
			int map_id;
			short counter;
		};

		struct object_be_moved
		{
			single_data_header header;
			move_info data;
		};

		struct player_pulling
		{
			single_data_header header;
			bool on;
		};

		struct player_be_pulled
		{
			single_data_header header;
			int player_pulling_id;
			int player_pulled_id;
			bool on;
			unsigned char type;		//0, 拉扯，1.牵引
		};

		struct lottery3_bonus
		{
			single_data_header header;
			int lottery_id;
			int bonus_index;
			int bonus_level;
			int bonus_item;
			int bonus_count;
			int bonus_money;
		};

		struct raid_info
		{
			struct player_info_in_raid
			{
				int id;
				short cls;
				short level;
				short reborn_cnt;
			};

			single_data_header header;
			unsigned char player_count;
			player_info_in_raid info[0];
		};

		struct trigger_skill_time
		{
			single_data_header header;
			short skill_id;
			short time;//秒
		};

		struct player_stop_skill
		{
			single_data_header header;
			int player_id;
			char spirit_index; //0-2, 灵的index
		};

		struct addon_skill_permanent
		{
			single_data_header header;
			int skill_id;
			int skill_level;
		};

		struct addon_skill_permanent_data
		{
			single_data_header header;
			char content[];
		};

		struct zx_five_anni_data
		{
			single_data_header header;
			char star;
			char state; //0, 初始；1，点击过恭喜按钮；2，点击过领奖按钮
		};

		struct treasure_info
		{
			single_data_header header;

			int remain_dig_count;	//剩余探索次数
			int region_count;	//区域数量
			struct 
			{
				char type;	//0-无效 1-普通 2-隐藏
				char status;	//0-无效 1-可探索 2-关闭(普通区域) 3-锁定(隐藏区域)
				char level;
				int id;
			}treasure_region[];
		};

		struct treasure_region_upgrade
		{
			single_data_header header;
			int region_id;
			int region_index;
			bool success;
			int new_level;
		};

		struct treasure_region_unlock
		{
			single_data_header header;
			int region_id;
			int region_index;
			bool success;
		};

		struct treasure_region_dig
		{
			single_data_header header;
			int region_id;
			int region_index;
			bool success;
			char event_id;

			int award_item_num;
			struct 
			{
				int item_id;
				int item_count;
			}award_item_list[];
		};

		struct inc_treasure_dig_count
		{
			single_data_header header;
			int inc_count;			//增加的次数
			int total_dig_count;		//现在总共剩余的探索次数
		};
		
		struct raid_global_var
		{       
			single_data_header header;
			int count;           
			struct
			{
				int key;
				int value;
			}var_list[];
		};	

		struct random_tower_monster
		{
			single_data_header header;
			int level;			//当前层
			char reenter;		//0, 否，1，是
		};

		struct random_tower_monster_result
		{
			single_data_header header;
			int _monste_tid;	//-1表示为空
			char _client_idx;
		};	

		struct player_tower_data
		{
			single_data_header header;
			int tower_level;
			int start_level_today;	 //当日玩家选择开始层
			int end_level_today;	 //当日玩家达到的最高层
			int best_pass_times[10]; //每10层的时间, 没有时间为0
			int level_cnt;
			struct
			{
				char daily_reward_delivered;
				char lifelong_reward_delivered;
			}tower_reward_state[];
		};

		struct player_tower_monster_list
		{
			single_data_header header;
			int _tower_monster_list_cnt;
			int _tower_monster_list[]; 
		};

		struct player_tower_give_skills
		{
			single_data_header header;
			int cnt;
			struct 
			{
				int skill_id;
				int skill_level;
				bool used;
			}skills[];
		};

		struct set_tower_give_skill
		{
			single_data_header header;
			int skill_id;
			int skill_level;
			bool used;
		};

		struct add_tower_monster
		{
			single_data_header header;
			int monster_tid;
		};

		struct set_tower_reward_state
		{
			single_data_header header;
			char type;	//0, 每日; 1, 终身
			int level;
			bool deliveried; 	//是否发送成功
		};

		struct littlepet_info
		{
			single_data_header header;
			char level;
			char cur_award_lvl;	//当前领取到几级奖励
			int cur_exp;
		};

		//元魂信息
		struct rune_info
		{
			single_data_header header;
			bool active;			//元魂装备位是否激活
			int rune_score;			//元魂洗练值
			int customize_info;		//元魂定制信息 (0x01 0x02 0x04 0x08)
		};

		struct rune_refine_result
		{
			single_data_header header;
			char type; // 0 -洗练  1-接受洗炼结果
		};

		struct tower_level_pass
		{
			single_data_header header;
			int level;			//通过关卡的层数
			int best_time;		//最好时间
		};

		struct fill_platform_mask
		{
			single_data_header header;
			int mask;
		};
		

		struct pk_bet_data
		{
			struct pk_bet
			{
				int bet_1st_num;
				bool top3;
			} _pk_bets[8];
			bool _top3_bet;
			bool _1st_bet_reward_deliveried;
			bool _1st_bet_reward_result_deliveried;
			bool _top3_bet_reward_deliveried;
			bool _top3_bet_reward_result_deliveried[3];
		};
		
		struct put_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned char index;		//放在哪个位置
			unsigned short amount;
			unsigned short slot_amount;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			int state;
		};

		struct object_start_special_move
		{
			single_data_header header;
			int id;
			A3DVECTOR velocity;
			A3DVECTOR acceleration;
			A3DVECTOR cur_pos;
			char collision_state;		//0: 没有碰撞   1:发生碰撞 
			int timestamp;
		};

		struct object_special_move
		{
			single_data_header header;
			int id;
			A3DVECTOR velocity;
			A3DVECTOR acceleration;
			A3DVECTOR cur_pos;
			char collision_state;		//0: 没有碰撞   1:发生碰撞 
			int timestamp;
		
		};

		struct object_stop_special_move
		{
			single_data_header header;
			int id;
			unsigned char dir;
			A3DVECTOR cur_pos;
		};

		struct player_change_name
		{
			single_data_header header;
			int id;
			int name_len;
			char name[0];
		};

		struct enter_collision_raid
		{
			single_data_header header;
			int roleid;
			unsigned char raid_faction;	// 0 中立 1 攻方 2 守方 3 旁观
			unsigned char raid_type;
			int raid_id;			// 副本id
			int end_timestamp;		// 结束时间
		};

		struct collision_raid_score
		{
			single_data_header header;
			int attacker_score;
			int defender_score;
			int max_killer_id;
			int count;
			struct
			{
				int roleid;
				int faction;
				int kill_count;
				int death_count;
			}member_info[];
		};	
		
		struct collision_special_state
		{
			single_data_header header;
			int roleid;
			int type;
			char on;	//0- 停止 1-开始 
		};
		
		struct change_faction_name
		{
			single_data_header header;
			int id;
			int fid;
			char type;	//0 -帮派 1-家族
			int name_len;
			char name[0];
		};

		struct player_darklight
		{
			single_data_header header;
			int roleid;					//玩家ID
			int dark_light_value;		//-100 to 100
		};

		struct player_darklight_state
		{
			single_data_header header;
			int id;					//玩家ID
			char dark_light_state;	//0, 光; 1. 暗;
		};

		struct player_darklight_spirit
		{
			single_data_header header;
			int id;
			char darklight_spirits[3];	//0: 暗灵；1：光灵；2：合一灵；
		};

		struct mirror_image_cnt
		{
			single_data_header header;
			int id;
			char mirror_image_cnt;		//当前镜像的数量
		};

		struct player_wing_change_color
		{
			single_data_header header;
			int id;
			unsigned char wing_color;
		};

		// 上古世界传送，通告客户端当前开启的传送点
		// by sunjunbo 2012-8-20
		struct ui_transfer_opened_notify
		{
			single_data_header header;
			unsigned short length;						// 长度
			int opened_ui_transfer_indexs[];    		// 打开的传送点索引	
		};

		// 通告活跃度相关信息
		// by sunjunbo 2012-11-07
		struct liveness_notify
		{
			single_data_header header;
			int cur_point;       // 当前的分数		
			char is_taken[4];    // 今天是否领取过了	
			int length;	         // 完成的id数目
			int achieved_ids[];  // 完成的ids
		};

		struct notify_astrology_energy
		{
			single_data_header header;
			bool active;					//当前是否显示精力条
			int cur_energy; 				//当前星座精力值
		};

		struct astrology_upgrade_result
		{
			single_data_header header;
			bool success;					//false - 失败  1-成功
			int new_level;				
		};

		struct astrology_destroy_result
		{
			single_data_header header;
			int gain_energy;				//获得的精力值
		};

		struct talisman_refineskill
		{
			single_data_header header;
			int id;
			int index;
		};

		struct collision_raid_result
		{
			single_data_header header;
			char result;	//0-平局  1-赢 2-输
			int score_a;	//A积分
			int score_b;	//B积分
		};

		struct collision_raid_info
		{
			single_data_header header;
			int score_a;	//A积分
			int score_b;	//B积分
			int win_num;	//胜场
			int lost_num;	//败场
			int draw_num;	//平局
			int daily_award_timestamp;	//上次领取每日奖励的时间戳
			int last_collision_timestamp;	//上次参加碰撞战场的时间戳
			int day_win;		//最近一次参加战场当天的胜场数
			int cs_personal_award_timestamp;  //跨服6v6个人跨服每周分档奖励 
			int cs_exchange_award_timestamp; //跨服6v6每周兑换币奖励
			int cs_team_award_timestamp;	//跨服6v6战队赛季奖励

			int exchange_award_info_count;
			struct exchange_info
			{
				int item_id;
				int exchange_count;
			};
			exchange_info exchange_award_info[0];
		};

		struct dir_visible_state
		{
			single_data_header header;
			int player_id;	//玩家ID
			int id;		//效果ID
			int dir;	//0-360度
			bool on;	//开关
		};

		struct player_remedy_metempsychosis_level
		{
			single_data_header header;
		};

		struct collision_player_pos
		{
			single_data_header header;
			int roleid;
			A3DVECTOR pos;
		};

		struct enter_kingdom
		{
			single_data_header header;
			unsigned char battle_faction;	//0 中立 1 攻方 2 守方 3 旁观 4 混战
			char kingdom_type;	//1-主战场 2-辅战场
			char cur_phase;		//当前阶段
			int end_timestamp;		// 结束时间
			int defender_mafia_id;		//防守方帮派id
			int attacker_mafia_count;
			int attacker_mafia_ids[];

		};

		struct leave_kingdom
		{
			single_data_header header;
			char kingdom_type;	//1-主战场 2-辅战场
		};

		struct raid_level_start
		{
			single_data_header header;
			char level; // -1表示总挑战，0-9表示其他关卡
		};

		struct raid_level_end
		{
			single_data_header header;
			char level; // -1表示总挑战，0-9表示其他关卡
			bool result;
		};

		struct raid_level_result
		{
			single_data_header header;
			bool final;
			bool level_result[10];
			bool final_award_deliveried;
			bool level_award_deliveried[10];
		};

		struct raid_level_info
		{
			single_data_header header;
			char level; // -1表示总挑战，0-9表示其他关卡
			short matter_cnt;
			int start_time;
		};
		
		struct kingdom_half_end 
		{
			single_data_header header;
			int win_faction;	//1-防守方胜利  2-进攻方胜利
			int end_timestamp;	//下一场结束的时间(如果守方获胜则直接结束，改值为0)
		};

		struct kingdom_end_result
		{
			single_data_header header;
			int win_mafia_id;	//胜利方的帮派id
		};

		struct kingdom_mafia_info
		{
			struct player_info_in_kingdom
			{
				int roleid;
				int mafia_id;
				int kill_count;
				int death_count;
			};
			
			single_data_header header;
			int player_count;
			player_info_in_kingdom info[0];
			
		};
		struct sale_promotion_info
		{
			single_data_header header;
			int task_count;
			int data[0]; // task ids
		};

		struct event_start
		{
			single_data_header header;
			int event_id;
		};

		struct event_stop
		{
			single_data_header header;
			int event_id;
		};

		struct event_info
		{
			single_data_header header;
			int event_count;
			int events[0];
		};

		struct cash_gift_ids
		{
			single_data_header header;
			int gift_id1;			//1个积分对应的id
			int gift_id2;			//10000个积分对应的id
		};

		struct notify_cash_gift
		{
			single_data_header header;
			int gift_id;			//礼包id
			int gift_index;			//礼包位置
			int gift_score;			//礼包积分
		};

		struct player_propadd
		{
			single_data_header header;
			int add_prop[30];
		};

		struct player_get_propadd_item
		{
			single_data_header header;
			int item_id;
			int item_idx;
		};

		struct player_extra_equip_effect
		{
			single_data_header header;
			int player_id;
			char weapon_effect_level;
		};

		struct kingdom_key_npc_info
		{
			single_data_header header;
			int cur_hp;
			int max_hp;
			int hp_add;
			int hp_dec;
		};

		struct notify_kingdom_info
		{
			single_data_header header;
			int kingdom_title;		//国家职位 1:国王 2:王后 3:将军 4:大臣 5:成员
			int kingdom_point;		//国家积分
		};


		struct cs_flow_player_info
		{
			single_data_header header;
			int player_id;	//玩家ID
			int score;		//玩家当前分
			int c_kill_cnt;	//连续击杀次数
			bool m_kill_change;
			int m_kill_cnt;	//多杀数
			short kill_cnt;
			short death_cnt; 
			short max_ckill_cnt;
			short max_mkill_cnt;
		};

		struct player_bath_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bath_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bath_start
		{
			single_data_header header;
			int invitor;	//邀请者(搓澡的人)
			int invitee;	//被邀请的(被搓的)
		};

		struct player_bath_stop
		{
			single_data_header header;
			int who;
		};

		struct enter_bath_world 
		{
			single_data_header header;
			int remain_bath_count;
		};

		struct leave_bath_world 
		{
			single_data_header header;
		};

		struct bath_gain_item
		{
			single_data_header header;
			int roleid;
			int item_id;
			int item_count;
		};

		struct flow_battle_info
		{
			struct player_info_in_flow_battle
			{
				int   id;
				short level;
				int   cls;
				unsigned char battle_faction;
				unsigned short kill;
				unsigned short death;
				unsigned int  zone_id;
				unsigned int  flow_score;
			};
			single_data_header header;
			unsigned char player_count;
			player_info_in_flow_battle info[0];
		};

		struct kingdom_gather_mine_start
		{
			single_data_header header;
			int roleid;
			int mafia_id;
		};

		struct fuwen_compose_info
		{
			single_data_header header;
			int compose_count;	//今天已经合成多少次
			int extra_compose_count;	//今天使用额外道具合成的次数
		};

		struct fuwen_compose_end
		{
			single_data_header header;
		};

		struct fuwen_install_result
		{
			single_data_header header;
			int src_index;
			int dst_index;
		};

		struct fuwen_uninstall_result
		{
			single_data_header header;
			int fuwen_index;
			int inv_index;
		};

		struct multi_exp_mode
		{
			single_data_header header;
			int mulit_exp_factor;	//多倍经验倍数
			int timestamp;		//到期时间
		};

		struct exchange_item_list_info
		{
			single_data_header header;
			unsigned char where;
			size_t content_length;
			char content[];
		};
		
		//Web商城对应的商品信息
		struct web_order_goods
		{
			int _goods_id;
			int _goods_count;
			int _goods_flag;
			int _goods_time;
		};

		//Web商城对应的一个礼包
		struct web_order_package
		{
			int _package_id;
			size_t _name_len;
			char _package_name[0];
			int _count;
			int _price;
			int _goods_count;
			web_order_goods * _order_goods;
		};

		//Web商城对应的一次订单
		struct web_order_info
		{
			int64_t _order_id;
			int _pay_type;	//1:coupon 2:cash
			int _status;	//0:未领取 1:已经领取
			int _timestamp;
			int _package_count;
			web_order_package * _order_package;
		};

		struct web_order_list
		{
			int web_order_count;
			web_order_info * _order_info;
		};

		struct cs_battle_off_line_info
		{
			single_data_header header;
			struct player_off_line_in_cs_battle
			{
				unsigned char battle_faction;
				int _timestamp; //过了多少秒
				unsigned char off_line_count;
				unsigned char off_line_score;
			};
			unsigned char off_line_count;
			player_off_line_in_cs_battle off_line[0];
		};

		struct control_trap_info
		{
			single_data_header header;
			int id;
			int tid;
			int time;
		};

		struct step_raid_score_info
		{    
			single_data_header header;
			char tower_index;
			char monster_index;
			char award_icon;
			int award_id;
			int score;
		};  

		struct npc_invisible
		{
			single_data_header header;
			int npc_id;
			bool on;
		};

		struct puppet_form
		{
			single_data_header header;
			bool on;
			int player_id;
			int puppet_id;
		};

		struct teleport_skill_info
		{
			single_data_header header;
			int player_id;
			int npc_tid;
			int skill_id;
			int teleport_count;
		};
		
		struct mob_active_ready_start
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
		};

		struct mob_active_running_start
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
		};

		struct mob_active_end_start
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
		};

		struct mob_active_end_finish
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
			A3DVECTOR player_pos;
			A3DVECTOR mobactive_pos;
		};

		struct mob_active_cancel
		{
			single_data_header header;
			int player_id;
			int mobactive_id;
			int active_pos;
			A3DVECTOR player_pos;
			A3DVECTOR mobactive_pos;
		};

		struct blow_off
		{
			single_data_header header;
			int roleid;
		};

		struct player_qilin_invite
		{
			single_data_header header;
			int who;
		};

		struct player_qilin_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_qilin_start
		{
			single_data_header header;
			int invitor;	//麒麟
			int invitee;	//被邀请的
		};

		struct player_qilin_stop
		{
			single_data_header header;
			int who;
		};

		struct player_qilin_disconnect
		{
			single_data_header header;
			int who;
		};

		struct player_qilin_reconnect
		{
			single_data_header header;
			int who;
			A3DVECTOR newpos;
		};

		struct phase_info
		{
			single_data_header header;
			int phase_count;

			struct phase
			{
				int phase_id;
				bool on;
			};
			phase phase_list[0];
		};

		struct phase_info_change
		{
			single_data_header header;
			int phase_id;
			bool on;
		};

		struct fac_building_add
		{
			single_data_header header;
			int field_index; //地块索引 从 1 开始编号
			int building_tid; //建筑的模板 id
		};

		struct fac_building_upgrade
		{
			single_data_header header;
			int field_index; //地块索引 从 1 开始编号
			int building_tid; //建筑的模板 id
			int new_level;
		};

		struct fac_building_remove
		{
			single_data_header header;
			int field_index; //地块索引 从 1 开始编号
		};

		struct fac_building_complete //建筑建设或升级成功
		{
			single_data_header header;
			int field_index; //地块索引 从 1 开始编号
			int building_tid; //建筑的模板 id
			int level;
		};

		struct fac_base_prop_change
		{
			single_data_header header;
			fac_base_prop value;
			int noti_roleid;
		};

		struct player_active_emote_invite
		{
			single_data_header header;
			int who;
			int type;
		};

		struct player_active_emote_invite_reply
		{
			single_data_header header;
			int who;
			int type;
			int param;
		};

		struct player_active_emote_start
		{
			single_data_header header;
			int who;
			int who1;
			int type;
		};

		struct player_active_emote_stop
		{
			single_data_header header;
			int who;
			int who1;
			int type;
		};

		struct player_fac_base_info
		{
			single_data_header header;
			fac_base_prop prop;
			int msg_size; //基地 留言 size
			char msg[0];  //基地 留言
			unsigned char field_count;
			player_fac_field fields[0];
		};

		struct be_taunted2
		{
			single_data_header header;
			int id;
			int time;
		};

		struct notify_bloodpool_status
		{
			single_data_header header;
			int player_id;
			bool on;
			int cur_hp;
			int max_hp;
		};

		struct player_facbase_auction
		{
			single_data_header header;
			int return_coupon;		//退回的金券数
			int size;
			fac_base_auc_item list[];
			int history_size;
			fac_base_auc_history history[];
		};

		struct facbase_auction_update
		{
			single_data_header header;
			enum
			{
				ADD = 1, //新拍卖上架
				END,	 //拍卖结束
				DEL, 	 //拍卖删除
				REFRESH, //拍卖刷新
			};
			int type;
			fac_base_auc_item item;
		};

		struct fac_coupon_return
		{
			single_data_header header;
			int coupon_return;
		};

		struct player_start_travel_around
		{
			single_data_header header;
			int player_id;
			int travel_vehicle_id;
			float travel_speed;
			int travel_path_id;
		};

		struct player_stop_travel_around
		{
			single_data_header header;
			int player_id;
		};

		struct player_get_summon_petprop
		{
			single_data_header header;
			int pet_index;
		//	q_extend_prop prop;		//不想引用头文件，所以注释掉了
		};

		struct facbase_auction_add_history
		{
			single_data_header header;
			fac_base_auc_history entry;
		};

		struct facbase_msg_update
		{
			single_data_header header;
			int msg_size; //基地 留言 size
			char msg[0];  //基地 留言
		};

		struct cs6v6_cheat_info
		{
			single_data_header header;
			bool is_cheat;
			int cheat_counter;	//倒计时
		};

		struct hide_and_seek_raid_info
		{
			single_data_header header;
			int last_hide_and_seek_timestamp;
			int day_has_get_award; // 0不能领取,1可以领取,2当日已经领取过
		};

		struct hide_and_seek_round_info
		{
			single_data_header header;
			char round;
			char round_status;
			int end_timestamp;
		};

		struct hide_and_seek_players_info
		{
			single_data_header header;
			int total_num;
			struct seek_player_info
			{
				int playerid;
				short score;
				char status;
				int occupation;
			};
			seek_player_info player_info[1];
		};

		struct hide_and_seek_skill_info
		{
			single_data_header header;
			char skill_left_num;
			char skill_buy_num;
		};

		struct hide_and_seek_blood_num
		{
			single_data_header header;
			int seeker_id;
			int hider_id;
			char blood_num;
		};

		struct hide_and_seek_role
		{
			single_data_header header;
			int player_id;
			char role_type;
		};

		struct hide_and_seek_enter_raid
		{
			single_data_header header;
		};

		struct hide_and_seek_leave_raid
		{
			single_data_header header;
		};

		struct hide_and_seek_hider_taunted
		{
			single_data_header header;
			int playerid;
			A3DVECTOR pos;
			bool isstart;
		};

		struct capture_raid_player_info
		{
			single_data_header header;
			int attacker_score; // 攻方积分
			int defender_score; // 守方积分
			char attacker_flag_count; //攻方夺旗数
			char defender_flag_count; //守方夺旗数
			int max_attacker_killer_id; //攻方杀神 
			int max_defender_killer_id; //守方杀神
			short max_attacker_kill_count; //攻方杀神杀人数
			short max_defender_kill_count; //守方杀神啥人数
			int score_info_count;
			struct capture_score_info
			{
				int roleid;
				short kill_count;
				short death_count;
				char faction;
				char flag_count;
			};
			capture_score_info info[0];
		};

		struct capture_broadcast_score
		{
			single_data_header header;
			char faction;
			char score;
		};

		struct capture_submit_flag
		{
			single_data_header header;
			char faction;
			int player_id;
			int name_len;
			char name[];
		};

		struct capture_reset_flag
		{
			single_data_header header;
			char faction;
		};

		struct capture_gather_flag
		{
			single_data_header header;
			char faction;
			int player_id;
			int name_len;
			char name[];
		};

		struct capture_monster_flag
		{
			single_data_header header;
			char faction;
			int monster_id;
		};

		struct capture_broadcast_tip
		{
			single_data_header header;
			char tip; // 1,格子� 2让移动 
		};

		struct capture_broadcast_flag_move
		{
			single_data_header header;
			char faction;
			A3DVECTOR pos;
		};
	}
}

namespace C2S
{
	enum MOVE_MODE
	{
		MOVE_MODE_WALK		= 0x00,
		MOVE_MODE_RUN		= 0x01,
		MOVE_MODE_STAND		= 0x02,
		MOVE_MODE_FALL		= 0x03,
		MOVE_MODE_SLIDE		= 0x04,
		MOVE_MODE_KNOCK		= 0x05,
		MOVE_MODE_FLY_FALL 	= 0x06,
		MOVE_MODE_RETURN	= 0x07,
		MOVE_MODE_JUMP		= 0x08,
		MOVE_MODE_DODGE		= 0x09,
		MOVE_MODE_PULL		= 0x0A,
		MOVE_MASK_DEAD		= 0x20,
		MOVE_MASK_SKY		= 0x40,
		MOVE_MASK_WATER		= 0x80

	};

	enum FORCE_ATTACK_MASK
	{	
		FORCE_ATTACK_WHITE	= 0x01,		//攻击白名
		FORCE_ATTACK_RED	= 0x02,		//攻击红名
		FORCE_ATTACK_FAMILY	= 0x04,		//攻击家族
		FORCE_ATTACK_MAFIA	= 0x08,		//攻击帮派成员
		FORCE_ATTACK_ZONE	= 0x10,		//攻击跨服玩家
		FORCE_ATTACK_ALL	= 0x1F,		//攻击所有
	};
	
	namespace INFO
	{
		struct move_info
		{
			A3DVECTOR cur_pos;
			A3DVECTOR next_pos;
			unsigned short use_time;	//使用的时间 单位是ms
							//使用的时间对于逻辑服务器来说，只是一个参考值
							//同时用于检测用户的指令是否正确，无论如何，
							//用户的移动都会在固定的0.5秒钟以后进行
			unsigned short speed;
			unsigned char  move_mode;		//walk run swim fly .... walk_back run_back
		};
	}

	enum
	{
		PLAYER_MOVE,
		LOGOUT,
		SELECT_TARGET,
		NORMAL_ATTACK,
		RESURRECT_IN_TOWN,	//城镇复生
//5		
		RESURRECT_BY_ITEM,	//物品复生
		PICKUP,			//检起物品或者金钱
		STOP_MOVE,
		UNSELECT,		//中止当前选定的目标
		GET_ITEM_INFO,		//取得物品特定位置的信息

//10
		GET_INVENTORY,		//取得某个位置上的所有物品列表
		GET_INVENTORY_DETAIL,	//取得某个位置上的所有物品列表，包含详细的物品数据
		EXCHANGE_INVENTORY_ITEM,
		MOVE_INVENTORY_ITEM,
		DROP_INVENTORY_ITEM,

//15		
		DROP_EQUIPMENT_ITEM,
		EXCHANGE_EQUIPMENT_ITEM,
		EQUIP_ITEM,		//装备物品，将物品栏上和装备栏上的两个位置进行调换
		MOVE_ITEM_TO_EQUIPMENT,
		GOTO,

//20		
		DROP_MONEY,		//扔出钱到地上
		SELF_GET_PROPERTY,
		LEARN_SKILL,		//学习技能
		GET_EXTPROP_BASE,
		GET_EXTPROP_MOVE,

//25
		GET_EXTPROP_ATTACK,
		GET_EXTPROP_DEFENSE,
		TEAM_INVITE,
		TEAM_AGREE_INVITE,
		TEAM_REJECT_INVITE,
//30
		TEAM_LEAVE_PARTY,
		TEAM_KICK_MEMBER,
		TEAM_GET_TEAMMATE_POS,
		GET_OTHERS_EQUIPMENT,
		CHANGE_PICKUP_FLAG,

//35		
		SERVICE_HELLO,
		SERVICE_GET_CONTENT,
		SERVICE_SERVE,
		GET_OWN_WEALTH,
		GET_ALL_DATA,
//40		
		USE_ITEM,
		CAST_SKILL,
		CANCEL_ACTION,
		RECHARGE_EQUIPPED_FLYSWORD,
		RECHARGE_FLYSWORD,
//45		
		USE_ITEM_WITH_TARGET,
		SIT_DOWN,
		STAND_UP,
		EMOTE_ACTION,
		TASK_NOTIFY,
//50
		ASSIST_SELECT,
		CONTINUE_ACTION,
		STOP_FALL,	//终止跌落
		GET_ITEM_INFO_LIST,
		GATHER_MATERIAL,
//55		
		GET_TRASHBOX_INFO,
		EXCHANGE_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,
		EXHCANGE_TRASHBOX_INVENTORY,
		MOVE_TRASHBOX_ITEM_TO_INVENTORY,
//60		
		MOVE_INVENTORY_ITEM_TO_TRASHBOX,
		EXCHANGE_TRASHBOX_MONEY,
		TRICKS_ACTION,
		SET_ADV_DATA,
		CLR_ADV_DATA,
//65		
		TEAM_LFG_REQUEST,
		TEAM_LFG_REPLY,
		QUERY_PLAYER_INFO_1,
		QUERY_NPC_INFO_1,
		SESSION_EMOTE_ACTION,
//70 	
		CONCURRECT_EMOTE_REQUEST_NULL,
		CONCURRECT_EMOTE_REPLY_NULL,
		TEAM_CHANGE_LEADER,
		DEAD_MOVE,
		DEAD_STOP_MOVE,

//75
		ENTER_SANCTUARY,
		OPEN_PERSONAL_MARKET,
		CANCEL_PERSONAL_MARKET,
		QUERY_PERSONAL_MARKET_NAME,
		COMPLETE_TRAVEL,

//80
		CAST_INSTANT_SKILL,
		DESTROY_ITEM,
		ENABLE_PVP_STATE,
		DISABLE_PVP_STATE,
		TEST_PERSONAL_MARKET,

//85
		SWITCH_FASHION_MODE,
		REGION_TRANSPORT,
		RESURRECT_AT_ONCE,
		NOTIFY_POS_TO_MEMBER,
		CAST_POS_SKILL,

//90	
		ACTIVE_RUSH_MODE,
		QUERY_DOUBLE_EXP_INFO,
		DUEL_REQUEST,
		DUEL_REPLY,
		BIND_PLAYER_REQUEST,

//95
		BIND_PLAYER_INVITE,
		BIND_PLAYER_REQUEST_REPLY,
		BIND_PLAYER_INVITE_REPLY,
		BIND_PLAYER_CANCEL,
		QUERY_OTHER_EQUIP_DETAIL,

//100
		SUMMON_PET,	//召唤宠物
		RECALL_PET,	//收回宠物
		NO_USE_BANISH_PET,
		PET_CTRL_CMD,
		PRODUCE_ITEM,

//105
		MALL_SHOPPING,
		SELECT_TITLE,
		DEBUG_DELIVERY_CMD,
		DEBUG_GS_CMD,
		LOTTERY_CASHING,
//110
		CHECK_RECORDER,
		MALL_SHOPPING2,
		START_MOVE,
		USE_ITEM_WITH_ARG,
		AUTO_BOT_BEGIN,

//115
		AUTO_BOT_CONTINUE,
		GET_BATTLE_SCORE,
		GET_BATTLE_INFO,
		EQUIP_PET_BEDGE,	//装备宠物牌,将宠物牌栏上和装备栏上的两个位置进行调换
		EQUIP_PET_EQUIP,	//装备宠物装备,将宠物装备栏上和装备栏上的两个位置进行调换

//120
		COMBINE_PET,		//宠物合体
		UNCOMBINE_PET,		//解除合体
		SET_PET_STATUS,		//设置宠物状态
		SET_PET_RANK,		//设置宠物阶级
		MOVE_PET_BEDGE,		//调换宠物牌的位置

//125
		START_FLY,
		STOP_FLY,
		SET_FASHION_MASK,
		START_ONLINE_AGENT,
		JOIN_INSTANCE,
		
//130
		BIND_RIDE_INVITE, 		//多人骑乘邀请
		BIND_RIDE_INVITE_REPLY, 	//多人骑乘邀请的回复
		BIND_RIDE_CANCEL, 		//离开多人骑乘
		BIND_RIDE_KICK,             	//主骑踢人
		POST_SNS_MESSAGE,		//玩家发布信息

//135
		APPLY_SNS_MESSAGE,		//玩家应征信息
		VOTE_SNS_MESSAGE,		//玩家投票
		RESPONSE_SNS_MESSAGE, 		//玩家留言		
		EXCHANGE_POCKET_ITEM,
		MOVE_POCKET_ITEM,

//140
		EXCHANGE_INVENTORY_POCKET_ITEM,
		MOVE_POCKET_ITEM_TO_INVENTORY,
		MOVE_INVENTORY_ITEM_TO_POCKET,
		MOVE_ALL_POCKET_ITEM_TO_INVENTORY,
		CAST_ITEM_SKILL,
	
//145		
		UPDATE_FASHION_HOTKEY,
		EXCHANGE_FASHION_ITEM,
		EXCHANGE_INVENTORY_FASHION_ITEM,
		EXCHANGE_EQUIPMENT_FASHION_ITEM,
		EXCHANGE_HOTKEY_EQUIPMENT_FASHION,

//150
		BONUSMALL_SHOPPING,
		QUERY_OTHERS_ACHIEVEMENT,		// 查看其他人的成就
		UPDATE_PVP_MASK,
		START_TRANSFORM,			//开始变身
		STOP_TRANSFORM,				//停止变身
		
//155	
		CAST_TRANSFORM_SKILL,
		ENTER_CARRIER,
		LEAVE_CARRIER,
		MOVE_ON_CARRIER,
		STOP_MOVE_ON_CARRIER,

//160
		EXCHANGE_HOMETOWN_MONEY,
		GET_SERVER_TIMESTAMP,
		TERRITORY_LEAVE,
		CAST_CHARGE_SKILL,
		UPDATE_COMBINE_SKILL,

//165
		UNIQUE_BID_REQUEST,				//唯一最低价拍卖竞价
		UNIQUE_BID_GET,					//唯一最低价获取奖励物品
		GET_CLONE_EQUIPMENT,				//获取分身主人的装备信息
		TASK_FLY_POS,
		ZONEMALL_SHOPPING,

		
//170
		ACTIVITY_FLY_POS,
		DEITY_LEVELUP,
		QUERY_CIRCLE_OF_DOOM_INFO,      //查询阵法成员信息，用于客户端显示
		RAID_LEAVE,						//玩家主动离开
		CANCEL_PULLING,					//取消拉扯

//175
		GET_RAID_INFO,					//获得副本信息
		QUERY_BE_SPIRIT_DRAGGED,		//获取某个玩家被灵力牵引的信息
		QUERY_BE_PULLED,				//获取某个玩家被拉扯的信息
		GET_MALL_SALETIME_ITEM,         //获取商城当前正在销售的限时销售物品
		GET_RAID_COUNT,					//获取副本进入次数

//180
		GET_VIP_AWARD_INFO,				//获取VIP奖励信息
		GET_VIP_AWARD_BY_ID,			//获取VIP奖励对应的奖励物品
		TRY_GET_ONLINE_AWARD,			//尝试获取在线奖励
		GET_ONLINE_AWARD,				//获取在线奖励
		FIVE_ANNI_REQUEST,				//5周年签到请求
		
//185
		UPGRADE_TREASURE_REGION,		//升级挖宝区域等级
		UNLOCK_TREASURE_REGION,			//解锁挖宝区域(仅对隐藏区)
		DIG_TREASURE_REGION,			//挖宝
		START_RANDOM_TOWER_MONSTER, 	//开始随机爬塔副本怪物
		ADOPT_LITTLEPET,		//领养诛小仙

//190
		FEED_LITTLEPET,			//喂养诛小仙
		GET_LITTLEPET_AWARD,		//领取诛小仙奖励
		RUNE_IDENTIFY,			//元魂鉴定
		RUNE_COMBINE,			//元魂合成
		RUNE_REFINE,			//元魂洗练

//195
		RUNE_RESET,			//元魂归元
		RUNE_DECOMPOSE,			//元魂分解
		RUNE_LEVELUP,			//元魂升级
		RUNE_OPEN_SLOT,			//元魂开启符文 (作废)
		RUNE_CHANGE_SLOT,		//元魂改变孔位

//200
		RUNE_ERASE_SLOT,		//元魂擦写符语
		RUNE_INSTALL_SLOT,		//元魂镶嵌符文
		RUNE_REFINE_ACTION,		//元魂洗炼操作 (接受或者不接受洗练结果)
		TOWER_REWARD,					//领取爬塔副本奖励
		RESET_SKILL_PROP,       //90级前免费洗点洗天书

//205
		GET_TASK_AWARD,
		PK_1ST_GUESS,
		PK_TOP3_GUESS,
		PK_1ST_GUESS_REWARD,
		PK_1ST_GUESS_RESULT_REWARD,

//210
		PK_TOP3_GUESS_REWARD,
		PK_TOP3_GUESS_RESULT_REWARD,
		GET_PLAYER_BET_DATA,	
		START_SPECIAL_MOVE,
		SPECIAL_MOVE,

//215
		STOP_SPECIAL_MOVE,
		COLLISION_RAID_APPLY,
		GET_REPURCHASE_INV_DATA,
		EXCHANGE_MOUNT_WING_ITEM, //坐骑飞剑包裹里交换位置
		EXCHANGE_INVENTORY_MOUNTWING_ITEM, //普通包裹 <-> 坐骑飞剑包裹

//220
		EXCHANGE_EQUIPMENT_MOUNTWING_ITEM, //装备栏 <-> 坐骑飞剑包裹 
		HIDE_VIP_LEVEL, //隐藏VIP等级信息
		CHANGE_WING_COLOR,		//改变飞剑颜色
		ASTROLOGY_IDENTIFY,		//星座鉴定
		ASTROLOGY_UPGRADE,		//星座升级

//225
		ASTROLOGY_DESTROY,		//星座破碎
		JOIN_RAID_ROOM, 
		TALISMAN_REFINESKILL_RESULT, //客户端是否接受法宝技能洗练结果
		GET_COLLISION_AWARD,
		CANCEL_ICE_CRUST,

//230
		PLAYER_FIRST_EXIT_REASON,
		REMEDY_METEMPSYCHOSIS_LEVEL,	//弥补飞升等级 (未满级飞升有数值损失)
		MERGE_POTION,
		KINGDOM_LEAVE,
		GET_COLLISION_PLAYER_POS,

//235
		TAKE_LIVENESS_AWARD,
		RAID_LEVEL_AWARD,
		GET_RAID_LEVEL_RESULT,
		OPEN_TRASHBOX,
		DELIVERY_GIFT_BAG,

//240
		GET_CASH_GIFT_AWARD,
		GEN_PROP_ADD_ITEM,
		REBUILD_PROP_ADD_ITEM,
		GET_PROPADD,
		BUY_KINGDOM_ITEM,

//245
		GET_TOUCH_AWARD,		//兑换touch奖励
		FLOW_BATTLE_LEAVE,
		KING_TRY_CALL_GUARD,		//亲卫传送
		BATH_INVITE,			//搓澡邀请
		BATH_INVITE_REPLY,		//搓澡奖励

//250
		DELIVER_KINGDOM_TASK,		//发布国王任务
		RECEIVE_KINGDOM_TASK,		//接受国王任务
		KINGDOM_FLY_BATH_POS,
		FUWEN_COMPOSE,			//符文合成
		FUWEN_UPGRADE,			//符文升级

//255
		FUWEN_INSTALL,			//符文镶嵌
		FUWEN_UNINSTALL,			//符文拆除
		RESIZE_INV,
		REGISTER_FLOW_BATTLE,
		ARRANGE_INVENTORY,                //整理背包

//260		
		ARRANGE_TRASHBOX,                 //整理仓库
		GET_WEB_ORDER,			  //领取web订单物品
		CUSTOMIZE_RUNE,			  //定制元婴
		GET_CROSS_SERVER_BATTLE_INFO,
		CONTROL_TRAP,

//265
		SUMMON_TELEPORT,
		CREATE_CROSSVR_TEAM,
		GET_KING_REWARD,		  //获得国王连任奖励
		MOBACTIVE_START,
		MOBACTIVE_FINISH,  //互动物品玩家点击主动完成
		
//270
		FASHION_COLORANT_COMBINE, 	// Youshuang add
		FASHION_ADD_COLOR, 		// Youshuang add
		ENTER_FACTION_BASE,		//进入帮派基地
		QILIN_INVITE,			  //麒麟邀请
		QILIN_INVITE_REPLY,		  //麒麟邀请回复

//275
		QILIN_CANCEL,			  //麒麟取消
		QILIN_DISCONNECT,		  //麒麟下马
		QILIN_RECONNECT,		  //麒麟上马
		GET_ACHIEVEMENT_AWARD,	// Youshuang add
		ADD_FAC_BUILDING,		//创建基地建筑	
//280
		UPGRADE_FAC_BUILDING,		//升级基地建筑
		REMOVE_FAC_BUILDING,		//拆除基地建筑
		PET_CHANGE_SHAPE,		//宠物改变形象
		PET_REFINE_ATTR,		//宠物改变属性
		ACTIVE_EMOTE_ACTION_INVITE,

//285		
		ACTIVE_EMOTE_ACTION_INVITE_REPLY,
		ACTIVE_EMOTE_ACTION_CANCEL,
		GET_RAID_TRANSFORM_TASK,
		GET_FAC_BASE_INFO,
		GET_CS_6V6_AWARD,

//290
		GET_FACBASE_MALL_INFO,		// Youshuang add
		SHOP_FROM_FACBASE_MALL,		// Youshuang add
		CONTRIBUTE_FACBASE_CASH,	// Youshuang add
		GET_FACBASE_CASH_ITEMS_INFO,	// Youshuang add
		BUY_FACBASE_CASH_ITEM,		// Youshuang add

//295
		BID_ON_FACBASE,			// Youshuang add
		POST_FAC_BASE_MSG,		// 基地留言板
		EXCHANGE_CS_6V6_AWARD,
		PUPPET_FORM_CHANGE,
		FAC_BASE_TRANSFER,		//基地内传送

//300
		EXCHANGE_CS_6V6_MONEY,
		WITHDRAW_FACBASE_AUCTION,	//领取基地拍卖物品
		WITHDRAW_FACBASE_COUPON,	//领取拍卖失败退回的金券
		GET_FACBASE_AUCTION,		//客户端获取基地拍卖列表 gdeliveryd转发过来的消息
		LEAVE_FACTION_BASE,		//离开帮派基地

//305
		OBJECT_CHARGE_TO,		
		STOP_TRAVEL_AROUND,
		GET_SUMMON_PETPROP,
		TASK_FLY_TO_AREA,		// Youshuang add
		TALISMAN_ENCHANT_CONFIRM,	// Youshuang add

//310
		GET_SEEK_AWARD,
		SEEKER_BUY_SKILL,
		HIDE_AND_SEEK_RAID_APPLY,
		ARRANGE_POCKET,				// Youshuang add
		MOVE_MAX_POCKET_ITEM_TO_INVENTORY,	// Youshuang add

//315
		NEWYEAR_AWARD,				// Youshuang add
		CAPTURE_RAID_SUBMIT_FLAG,
		CAPTURE_RAID_APPLY,
		USE_TITLE,				// Youshuang add
		
//1000		
		GM_COMMAND_START = 1000,
		GMCMD_MOVE_TO_PLAYER,		//201
		GMCMD_RECALL_PLAYER,		//202
		GMCMD_OFFLINE,			//203
		GMCMD_TOGGLE_INVISIBLE,		//204
		GMCMD_TOGGLE_INVINCIBLE,	//205
		GMCMD_DROP_GENERATOR,		//206
		GMCMD_ACTIVE_SPAWNER,		//207
		GMCMD_GENERATE_MOB,             //208

		GMCMD_PLAYER_INC_EXP,		//209
		GMCMD_RESURRECT,		//210
		GMCMD_ENDUE_ITEM,		//211	
		GMCMD_ENDUE_SELL_ITEM,		//212
		GMCMD_REMOVE_ITEM,		//213
		GMCMD_ENDUE_MONEY,		//214
		GMCMD_ENABLE_DEBUG_CMD,		
		GMCMD_RESET_PROP,		
		GMCMD_MOVETOMAP,
		GM_COMMAND_END,

	};//C2S

	namespace CMD
	{
		using namespace INFO;
		struct player_move
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//命令序号
		};
		struct player_logout
		{
			cmd_header header;
		};
		struct select_target
		{	
			cmd_header header;
			int id;
		};
		struct normal_attack
		{
			cmd_header header;
			char force_attack;		//强制攻击位 0x01 普通强制攻击 0x02 攻击本帮派 0x04攻击白名 0x08 攻击红名 0x10 攻击家族
							//技能和物品的的定义与此相同
		};

		struct pickup_matter
		{
			cmd_header header;
			int mid;
			int type;
		};
		
		struct resurrect
		{
			cmd_header header;
		};
		struct player_stop_move
		{
			cmd_header header;
			A3DVECTOR pos;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;		//命令序号
			unsigned short use_time;
		};

		struct get_item_info
		{
			cmd_header header;
			unsigned char where;
			unsigned char index;
		};

		struct get_inventory
		{
			cmd_header header;
			unsigned char where;
		};

		struct get_inventory_detail
		{
			cmd_header header;
			unsigned char where;
		};
		
		struct exchange_inventory_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_inventory_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct drop_inventory_item
		{
			cmd_header header;
			unsigned char index;
			unsigned short amount;
		};

		struct drop_equipment_item
		{
			cmd_header header;
			unsigned char index;
		};

		struct exchange_equip_item
		{
			cmd_header header;
			unsigned char idx1;
			unsigned char idx2;
		};

		struct equip_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_eq;
		};

		struct move_item_to_equipment
		{
			cmd_header header;
			unsigned char idx_inv;  //src
			unsigned char idx_eq;	 //dest
		};

		struct player_goto
		{
			cmd_header header;
			A3DVECTOR pos;
		};

		struct drop_money
		{
			cmd_header header;
			size_t amount; 
		};

		struct self_get_property
		{
			cmd_header header;
		};

		struct learn_skill 
		{
			cmd_header header;
			size_t skill_id;
		};

		struct get_extprop_base
		{
			cmd_header header;
		};

		struct get_extprop_move
		{
			cmd_header header;
		};

		struct get_extprop_attack
		{
			cmd_header header;
		};

		struct get_extprop_defense
		{
			cmd_header header;
		};

		struct team_invite
		{
			cmd_header header;
			int id;		//想谁发起邀请 
		};

		struct team_agree_invite
		{
			cmd_header header;
			int id;		//谁进行的邀请
			int team_seq;
		};

		struct team_reject_invite
		{
			cmd_header header;
			int id;		//谁进行的邀请
		};

		struct team_leave_party
		{
			cmd_header header;
		};

		struct team_kick_member
		{
			cmd_header header;
			int id;
		};
		
		struct team_get_teammate_pos
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct get_others_equipment
		{
			cmd_header header;
			unsigned short size;
			int idlist[];
		};


		struct set_pickup_flag
		{
			cmd_header header;
			short pickup_flag;
		};


		struct service_hello
		{
			cmd_header header;
			int id;
		};

		struct service_get_content
		{
			cmd_header header;
			int service_type;
		};

		struct service_serve
		{
			cmd_header header;
			int service_type;
			size_t len;
			char content[];
		};

		struct logout
		{
			cmd_header header;
			int logout_type;
			int offline_agent;
		};

		struct get_own_wealth
		{
			cmd_header header;
			char detail_inv;
			char detail_equip;
			char detail_task;
		};

		struct get_all_data
		{
			cmd_header header;
			char detail_inv;
			char detail_equip;
			char detail_task;
		//	char detail_fashion;
		};

		struct use_item
		{
			cmd_header header;
			unsigned char where;
			unsigned char count;
			unsigned short index;
			int  item_id;
		};


		struct cast_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			A3DVECTOR skillpos;
			int  targets[];
		};

		/**
		 * @brief 查询阵法信息
		 */
		struct query_circleofdoom_info
		{
			cmd_header header;
			int sponsor_id;     //阵法发起者的id
		};

		/**
		 * @brief 获取对应的VIP奖励，带上award_item_id是检查客户端数据版本是否与服务器一致
		 */
		struct obtain_vip_award
		{
			cmd_header header;
			int award_id;				//奖励id
			int award_item_id;			//奖励所对应的物品的id
		};

		/**
		 * @brief 获取在线倒计时奖励，尝试获取及获取都是用这个结构
		 */
		struct obtain_online_award
		{
			cmd_header header;
			int		award_id;				//奖励id
			int		award_index;			//奖励的第几轮倒计时
			int     small_giftbag_id;		//本轮小礼包id
		};

		/**
		 * @brief 90级前免费洗点洗天书
		 */
		struct reset_skill_prop
		{
			cmd_header header;
			unsigned char opcode;		//1表示洗技能点，2表示洗天书
		};

		/**
		 * @brief 准备摆摊
		 */
		struct test_personal_market
		{
			cmd_header header;
			int index;    //如果没有使用道具摆摊，index设为-1
			int item_id;  //如果没有使用道具，item_id设为-1或0都可以
		};

		/**
		 * @brief 加入副本，这里没有走NPC服务，现在用于帮战副本，因为帮众不需要报名
		 */
		struct join_raid_room
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			int room_id;
			char raid_faction; //阵营：0 无阵营 1 攻 2 守 3 观察者
		};

		/**
		 * @brief player首次退出原因，用来记log
		 */
		struct player_first_exit_reason
		{
			cmd_header header;
			unsigned char reason;
		};

		struct cancel_action
		{
			cmd_header header;
		};

		struct recharge_equipped_flysword
		{	
			cmd_header header;
			unsigned char element_index; 
			int count;
		};
		
		struct recharge_flysword
		{	
			cmd_header header;
			unsigned char element_index; 
			unsigned char flysword_index;
			int count;
			int flysword_id;
		};

		struct use_item_with_target
		{
			cmd_header header;
			unsigned char where;
			unsigned char force_attack; //只对攻击性物品有效
			unsigned short index;
			int  item_id;
		};

		struct sit_down
		{
			cmd_header header;
		};

		struct stand_up
		{
			cmd_header header;
		};
		
		struct emote_action 
		{
			cmd_header header;
			unsigned short action;
		};

		struct task_notify
		{
			cmd_header header;
			unsigned int size;
			char buf[0];
		};

		struct assist_select
		{
			cmd_header header;
			int partner;
		};

		struct continue_action
		{
			cmd_header header;
		};

		struct get_item_info_list
		{
			cmd_header header;
			char  where;
			unsigned char  count;
			unsigned char  item_list[];
		};

		struct gather_material
		{
			cmd_header header;
			int mid;
			short tool_where;
			short tool_index;
			int tool_type;
			int task_id;
		};

		struct get_trashbox_info
		{
			cmd_header header;
			char detail;
		};

		struct exchange_trashbox_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_trashbox_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct exchange_trashbox_inventory
		{
			cmd_header header;
			unsigned char idx_tra;
			unsigned char idx_inv;
		};

		struct move_trashbox_item_to_inventory
		{
			cmd_header header;
			unsigned char idx_tra;
			unsigned char idx_inv;
			unsigned short amount;
		};

		struct move_inventory_item_to_trashbox
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_tra;
			unsigned short amount;
		};
		
		struct excnahge_trashbox_money
		{
			cmd_header header;
			unsigned int inv_money;
			unsigned int trashbox_money;
		};

		struct tricks_action 
		{
			cmd_header header;
			unsigned char action;
		};

		struct set_adv_data
		{
			cmd_header header;
			int data1;
			int data2;
		};

		struct clr_adv_data
		{
			cmd_header header;
		};

		struct team_lfg_request
		{
			cmd_header header;
			int id;
		};

		struct team_lfg_reply
		{
			cmd_header header;
			int id;
			bool result;
		};

		struct query_player_info_1
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct query_npc_info_1
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct session_emote_action
		{
			cmd_header header;
			//限制为256之内
			unsigned char action;
		};

		struct concurrent_emote_request
		{
			cmd_header header;
			unsigned short action;
			int target;
		};

		struct concurrent_emote_reply
		{
			cmd_header header;
			unsigned short result;
			unsigned short action;
			int target;
		};

		struct team_change_leader
		{
			cmd_header header;
			int new_leader;
		};

		struct dead_move 
		{
			cmd_header header;
			float y;
			unsigned short use_time;	//使用的时间 单位是ms
							//使用的时间对于逻辑服务器来说，只是一个参考值
							//同时用于检测用户的指令是否正确，无论如何，
							//用户的移动都会在固定的0.5秒钟以后进行
			unsigned short speed;
			unsigned char  move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;
		};
		
		struct dead_stop_move 
		{
			cmd_header header;
			float y;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;
		};

		struct enter_sanctuary
		{
			cmd_header header;
		};

		struct open_personal_market
		{
			cmd_header header;
			int index;    //如果没有使用摆摊道具index为-1，使用了index为item在包裹中的位置
			int item_id;  //如果没有使用摆摊道具item_id为-1，使用了则为道具的id
			unsigned short count;
			char name[28];
			struct entry_t
			{
				int type;
				size_t index;
				size_t count;
				size_t price;
			} list[];
		};

		struct cancel_personal_market
		{
			cmd_header header;
		};

		struct query_personal_market_name
		{
			cmd_header header;
			unsigned short count;
			int list[];
		};

		struct complete_travel
		{
			cmd_header header;
		};

		struct cast_instant_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			A3DVECTOR skillpos;
			int  targets[];
		};
		
		struct cast_charge_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			int charge_target;
			A3DVECTOR charge_pos;
			unsigned char target_count;
			int  targets[];
		};

		struct destroy_item
		{
			cmd_header header;
			unsigned char where;		//在哪个包裹栏，0 标准，2 任务，1 装备
			unsigned char index;		//索引
			int type;			//是哪种物品
		};

		struct enable_pvp_state
		{
			cmd_header header;
		};

		struct disable_pvp_state
		{
			cmd_header header;
		};

		struct switch_fashion_mode
		{
			cmd_header header;
		};

		struct region_transport
		{
			cmd_header header;
			int region_index;
			int target_tag;
		};

		struct cast_pos_skill
		{
			cmd_header header;
			int skill_id;
			A3DVECTOR pos;
		};

		struct active_rush_mode
		{
			cmd_header header;
			int is_active;
		};

		struct query_double_exp_info
		{
			cmd_header header;
		};

		struct duel_request
		{
			cmd_header header;
			int target;
		};

		struct duel_reply
		{
			cmd_header header;
			int who;
			int param; // 0 同意  1 不同意
		};

		struct bind_player_request {
			cmd_header header;
			int who;
		};

		struct bind_player_invite
		{
			cmd_header header;
			int who;
		};

		struct bind_player_request_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct bind_player_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct query_other_equip_detail
		{
			cmd_header header;
			int target;
		};

		struct produce_item
		{
			cmd_header header;
			int recipe_id;
		};

		struct mall_shopping
		{
			cmd_header header;
			unsigned int count;
			struct __entry
			{
				short goods_id;
				short goods_index; 
				short goods_slot;
			}list[];
			//.....
		};

		struct  select_title
		{
			cmd_header header;
			short title;
		};

		struct debug_delivery_cmd
		{
			cmd_header header;
			short  type;
			char buf[];
		};
		
		struct lottery_cashing 
		{
			cmd_header header;
			int item_index;
		};

		struct check_recorder
		{
			cmd_header header;
			int item_index;
		};

		struct mall_shopping_2
		{
			cmd_header header;
			int goods_id;
			unsigned short goods_index; 
			unsigned short goods_slot;
			unsigned short count;
		};

		struct use_item_with_arg
		{
			cmd_header header;
			unsigned char where;
			unsigned char count;
			unsigned short index;
			int  item_id;
			char arg[];
		};

		struct auto_bot_begin
		{
			cmd_header header;
			unsigned char active;
			unsigned char need_question;
		};

		struct auto_bot_continue
		{
			cmd_header header;
		};

		struct get_battle_score
		{
			cmd_header header;
		};

		struct get_battle_info
		{
			cmd_header header;
		};

		struct equip_pet_bedge
		{
			cmd_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};

		struct equip_pet_equip
		{
			cmd_header header;
			unsigned char inv_index;
			unsigned char pet_index;
		};

		struct set_pet_status
		{
			cmd_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char main_status; // 0 战斗 1 采集 2 制造 3 休息
			unsigned char sub_status; // 只有main_status为 1时有用 ,0 种植 1 伐木 2 狩猎 3 钓鱼 4 采矿 5 考古
		};

		struct set_pet_rank
		{
			cmd_header header;
			unsigned char pet_index;
			int pet_tid;
			unsigned char rank;
		};

		struct summon_pet
		{
			cmd_header header;
			unsigned char pet_index;
		};

		struct recall_pet
		{
			cmd_header header;
			unsigned char pet_index;
		};

		struct pet_ctrl_cmd
		{
			cmd_header header;
			unsigned char pet_index;
			int target;
			int pet_cmd;
			char buf[];
		};
		
		struct combine_pet
		{
			cmd_header header;
			unsigned char pet_index;
			unsigned char combine_type;	//0 通灵 1 御宝
		};

		struct uncombine_pet
		{
			cmd_header header;
			unsigned char pet_index;
		};

		struct move_pet_bedge
		{
			cmd_header header;
			unsigned char src_index;
			unsigned char dst_index;
		};

		struct set_fashion_mask
		{
			cmd_header header;
			int fashion_mask;
		};

		struct join_instance
		{
			cmd_header header;
			int battle_id;
		};

		struct bind_ride_invite
		{
			cmd_header header;
			int who;
		};

		struct bind_ride_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct bind_ride_kick
		{
			cmd_header header;
			char pos;
		};

		struct post_sns_message
		{
			cmd_header header;
			char message_type;  // 0 --marriage  1--friend 2--mentor 3--faction
			unsigned char occupation;
			char gender; 
			int level;
			int faction_level;
			unsigned int message_len;
			char message[];
			
		};

		struct apply_sns_message
		{
			cmd_header header;
			char message_type;
			int message_id;
			unsigned int message_len;
			char message[];
		};

		struct vote_sns_message
		{
			cmd_header header;
			char vote_type; // 0 --against  1--support
			char message_type;
			int message_id;
		};

		struct response_sns_message
		{
			cmd_header header;
			char message_type;
			int message_id;
			short dst_index;  //responsed message's id
			unsigned int message_len;
			char message[];
		};

		struct exchange_pocket_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_pocket_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned short amount;
		};

		struct exchange_inventory_pocket_item
		{
			cmd_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
		};

		struct move_pocket_item_to_inventory
		{
			cmd_header header;
			unsigned char idx_poc;
			unsigned char idx_inv;
			unsigned short amount;
		};

		struct move_inventory_item_to_pocket
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_poc;
			unsigned short amount;
			
		};

		struct move_all_pocket_item_to_inventory
		{
			cmd_header header;
		};

		struct move_max_pocket_item_to_inventory
		{
			cmd_header header;
		};

		struct cast_item_skill
		{
			cmd_header header;
			int skill_id;
			char skill_type; //技能类型: 0--普通技能  1--瞬发技能
			int item_id;
			short item_index;
			unsigned char force_attack;
			unsigned char target_count;
			int targets[];
		};

		struct update_fashion_hotkey
		{
			cmd_header header;
			int count;
			struct key_combine
			{
				int index;
				int id_head;
				int id_cloth;
				int id_shoe;
			}key[];
		};

		struct exchange_fashion_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct exchange_inventory_fashion_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_fas;
		};

		struct exchange_equipment_fashion_item
		{
			cmd_header header;
			unsigned char idx_equ;
			unsigned char idx_fas;
		};
		
		/**
		 * @brief 坐骑飞剑包裹里交换物品
		 */
		struct exchange_mount_wing_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		/**
		 * @brief 普通包裹 <-> 坐骑飞剑包裹 
		 */
		struct exchange_inventory_mountwing_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_mw;
		};

		/**
		 * @brief 装备栏 <-> 坐骑飞剑包裹
		 */
		struct exchange_equipment_mountwing_item
		{
			cmd_header header;
			unsigned char idx_equ;
			unsigned char idx_mw;
		};

		/**
		 * @brief 隐藏vip等级信息
		 */
		struct hide_vip_level
		{
			cmd_header header;
			char is_hide;	//0表示显示vip等级信息，1表示隐藏vip等级信息
		};
		
		struct exchange_hotkey_equipment_fashion
		{
			cmd_header header;
			unsigned char idx_key;
			unsigned char idx_head;		//0xff代表那个部位不需要替换
		       	unsigned char idx_cloth;
			unsigned char idx_shoe;	
		};
		
		struct bonusmall_shopping
		{
			cmd_header header;
			int goods_id;
			unsigned short goods_index; 
			unsigned short goods_slot;
			unsigned short count;
		};

		struct query_others_achievement
		{
			cmd_header header;
			int target;
		};

		struct update_pvp_mask
		{
			cmd_header header;
			char pvp_mask;
		};
		
		struct start_transform 
		{
			cmd_header header;
		};

		struct stop_transform 
		{
			cmd_header header;
		};
		
		struct cast_transform_skill
		{
			cmd_header header;
			int skill_id;
			char skill_type; //技能类型: 0--普通技能  1--瞬发技能
			unsigned char force_attack;
			unsigned char target_count;
			A3DVECTOR pos;
			int targets[];
		};

		struct enter_carrier  
		{
			cmd_header header;
			int carrier_id; 		//想要乘坐的交通工具id
			A3DVECTOR rpos;			//相对坐标
			unsigned char rdir;		//相对方向
		};

		struct leave_carrier 
		{
			cmd_header header;
			int carrier_id; 		//想要离开的交通工具id
			A3DVECTOR pos;			//绝对坐标和方向
			unsigned char dir;		//绝对方向
		};
		
		struct move_on_carrier	
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//命令序号
		};

		struct stop_move_on_carrier
		{
			cmd_header header;
			A3DVECTOR rpos;
			unsigned short speed;
			unsigned char rdir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;		//命令序号
			unsigned short use_time;
		};

		struct exchange_hometown_money
		{
			cmd_header header;
			int amount;
		};
		
		struct get_server_timestamp 
		{
			cmd_header header;
		};

		struct territory_leave
		{
			cmd_header header;
		};

		struct update_combine_skill
		{
			cmd_header header;
			int skill_id;
			int element_count;
			int element_id[];
		};

		struct unique_bid_request
		{
			cmd_header header;
			int bid_money_lowerbound;
			int bid_money_upperbound;
		};

		struct unique_bid_get
		{
			cmd_header header;
		};	

		struct get_clone_equipment
		{
			cmd_header header;
		        int master_id;	
			int clone_id;
		};

		struct task_fly_pos
		{
			cmd_header header;
			int task_id;
			int npc_id;
		};
		
		struct zonemall_shopping
		{
			cmd_header header;
			int goods_id;
			unsigned short goods_index; 
			unsigned short goods_slot;
			unsigned short count;
		};
		
		struct activity_fly_pos
		{
			cmd_header header;
			int id;
		};

		struct deity_levelup
		{
			cmd_header header;
		};

		struct raid_leave
		{
			cmd_header header;
		};

		struct cancel_pulling
		{
			cmd_header header;
		};

		struct get_raid_info
		{
			cmd_header header;
		};

		struct query_be_spirit_dragged
		{
			cmd_header header;
			int id_be_spirit_dragged;
		};

		struct query_be_pulled
		{
			cmd_header header;
			int id_be_pulled;
		};

		struct get_raid_count
		{
			cmd_header header;
			int map_id;
		};
		
		struct five_anni_request
		{
			cmd_header header;
			char request; //0,获取5周年数据; 1,恭喜；2,领奖
		};

		struct upgrade_treasure_region
		{
			cmd_header header;
			int item_id;
			int item_index;
			int region_id;
			int region_index;
		};	

		struct unlock_treasure_region
		{
			cmd_header header;
			int item_id;
			int item_index;
			int region_id;
			int region_index;
		};

		struct dig_treasure_region
		{
			cmd_header header;
			int region_id;
			int region_index;
		};

		struct start_random_tower_monster
		{
			cmd_header header;
			char client_idx;
		};
		
		struct adopt_littlepet
		{
			cmd_header header;
		};

		struct feed_littlepet
		{
			cmd_header header;
			int item_id;
			int item_count;
		};

		struct get_littlepet_award
		{
			cmd_header header;
		};

		struct rune_identify
		{
			cmd_header header;
			int rune_index;			//元魂在包裹里面的位置
			int assist_index;		//辅助道具
		};

		struct rune_combine
		{
			cmd_header header;
			int rune_index;			//元魂 (装备栏)
			int assist_index;		//辅助元魂 (包裹栏)
		};

		// Youshuang
		struct talisman_enchant_confirm
		{
			cmd_header header;
			size_t index1;
			char confirm;
		};
		
		struct fashion_colorant_combine
		{
			cmd_header header;
			int colorant_index1;
			int colorant_index2;
			int colorant_index3;
		};

		struct fashion_add_color
		{
			cmd_header header;
			int colorant_index;
			int where;
			int fashion_item_index;
		};

		struct get_achieve_award
		{
			cmd_header header;
			unsigned short achieve_id;
			unsigned int award_id;
		};

		struct get_facbase_mall_info
		{
			cmd_header header;
		};

		struct shop_from_facbase_mall
		{
			cmd_header header;
			int item_index;
			int item_id;
			int cnt;
		};

		struct contribute_facbase_cash
		{
			cmd_header header;
			int cnt;
		};

		struct get_facbase_cash_items_info
		{
			cmd_header header;
		};


		struct buy_facbase_cash_item
		{
			cmd_header header;
			int item_index;
		};
		
		struct bid_on_facbase
		{
			cmd_header header;
			int item_index;
			int item_id;
			int coupon;
			int name_len;
			char player_name[];
		};

		struct task_fly_to_area
		{
			cmd_header header;
			int task_id;
		};

		struct newyear_award
		{
			cmd_header header;
			char award_type;
		};
		
		struct  use_title
		{
			cmd_header header;
			short title;
		};
		// end
		
		struct rune_refine
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
			int assist_index;		//辅助道具
		};

		struct rune_reset
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
			int assist_index;		//辅助道具
		};

		struct rune_decompose
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
		};

		struct rune_levelup
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
		};

		struct rune_open_slot
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
		};

		struct rune_change_slot
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
			int assist_index;
		};

		struct rune_erase_slot
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
			int stone_index;
		};

		struct tower_reward
		{
			cmd_header header;
			int raid_template_id;		
			size_t level;
			int type;	//终身，每日
		};
	
		struct rune_install_slot
		{
			cmd_header header;
			int rune_index;			//元魂在装备栏里面的位置
			int slot_index;			//安装的slot位置
			int stone_index;		//安装的符文index
			int stone_id;			//符文id
		};

		struct rune_refine_action
		{
			cmd_header header;
			bool accept_result;
			int rune_index;			//元魂在装备栏里面的位置
		};

		struct get_task_award
		{
			cmd_header header;
			int type;	//1-补填身份信息奖励  2-补填账号以及邮箱信息奖励 
		};

		struct pk_1st_guess
		{
			cmd_header header;
			int area;
			int cnt;
			struct bet_item
			{
				int index;
				int cnt;
			}items[];
		};

		struct pk_top3_guess
		{
			cmd_header header;
			bool cancel;
			int area;
			int cnt;
			struct bet_item
			{
				int index;
				int cnt;
			}items[];
		};

		struct pk_1st_guess_reward
		{
			cmd_header header;
		};
		
		struct pk_1st_guess_result_reward
		{
			cmd_header header;
		};

		struct pk_top3_guess_reward
		{
			cmd_header header;
		};

		struct pk_top3_guess_result_reward
		{
			cmd_header header;
			char type; //0, 全中; 1, 中2个; 2, 中1个
		};

		struct get_player_bet_data
		{
			cmd_header header;
		};

		struct start_special_move
		{
			cmd_header header;
			A3DVECTOR velocity;		//当前速度
			A3DVECTOR acceleration; 	//当前加速度
			A3DVECTOR cur_pos;
			char collision_state;		//0: 没有碰撞   1:发生碰撞 
			int timestamp;
		};

		struct special_move
		{
			cmd_header header;
			A3DVECTOR velocity;		//当前速度
			A3DVECTOR acceleration; 	//当前加速度
			A3DVECTOR cur_pos;		//当前位置信息
			unsigned short stamp;		//发送消息时间戳
			char collision_state;		//0: 没有碰撞   1:发生碰撞 
			int timestamp;
		};

		struct stop_special_move
		{
			cmd_header header;
			unsigned char dir;			//玩家当前的朝向
			A3DVECTOR cur_pos;		//当前位置
			unsigned short stamp;		//发送消息时间戳
		};

		struct collision_raid_apply
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			bool is_team;
			char is_cross;	//是否报名跨服6vs6n
		};

		struct change_wing_color
		{
			cmd_header header;
			unsigned char wing_color;
		};

		struct astrology_identify
		{
			cmd_header header;
			int item_index;
			int item_id;
		};

		struct astrology_upgrade
		{
			cmd_header header;
			int item_index;
			int item_id;
			int stone_index;
			int stone_id;
		};

		struct astrology_destroy
		{
			cmd_header header;
			int item_index;
			int item_id;
		};

		struct talisman_refineskill_result
		{
			cmd_header header;
			int talisman1_index;
			int talisman1_id;
			char result;
		};

		struct get_collision_award
		{
			cmd_header header;
			bool daily_award;		//是否领取每日奖励
			int award_index;		//奖励物品对应的index
		};

		struct cancel_ice_crust
		{
			cmd_header header;
		};

		struct remedy_metempsychosis_level
		{
			cmd_header header;
			int item_index;
		};
		
		struct merge_potion
		{
			cmd_header header;
			int bottle_id;
			int bottle_index;
			int potion_id;
			int potion_index;
		};

		struct kingdom_leave
		{
			cmd_header header;
		};

		struct get_collision_player_pos
		{
			cmd_header header;
			int roleid;
		};

		struct take_liveness_award
		{
			cmd_header header;
			int grade;  // 0, 1, 2, 3表示档次
		};

		struct raid_level_award
		{
			cmd_header header;
			char level; //-1表示final，0-9表示关卡
		};

		struct get_raid_level_result
		{
			cmd_header header;
		};

		struct open_trashbox
		{
			cmd_header header;
			unsigned int passwd_size;
			char passwd[];
		};

		struct delivery_gift_bag
		{
			cmd_header header;
			int index;
			int item_id;
		};

		struct get_cash_gift_award
		{
			cmd_header header;
		};

		struct gen_prop_add_item
		{
			cmd_header header;
			int material_id;
			short material_idx;
			short material_cnt;
		};

		struct rebuild_prop_add_item
		{
			cmd_header header;
			int itemId;
			short itemIdx;
			int forgeId;	//辅助道具ID
			short forgeIdx;	//辅助道具Idx
		};

		struct get_propadd
		{
			cmd_header header;
		};

		struct buy_kingdom_item
		{
			cmd_header header;
			char type;		//1-国王  2-个人  
			int index;
		};

		struct king_try_call_guard 
		{
			cmd_header header;
		};

		struct get_touch_award
		{
			cmd_header header;
			char type;		//1-金瓜子 2-银瓜子
			int count;
		};

		struct bath_invite
		{
			cmd_header header;
			int who;
		};

		struct bath_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct flow_battle_leave
		{
			cmd_header header;
		};

		struct deliver_kingdom_task
		{
			cmd_header header;
			int task_type;	//1-白色 2-绿色 3-蓝色 4-橙色
		};

		struct receive_kingdom_task
		{
			cmd_header header;
			int task_type;	//1-白色 2-绿色 3-蓝色 4-橙色
		};

		struct kingdom_fly_bath_pos
		{
			cmd_header header;
		};

		struct fuwen_compose
		{
			cmd_header header;
			bool consume_extra_item;	//是否消耗额外道具增加合成次数
			int extra_item_index;		//消耗的额外道具index
		};

		struct fuwen_upgrade
		{
			cmd_header header;
			int main_fuwen_index;		//主符文
			int main_fuwen_where;		//主符文所在的包裹
			int assist_count;
			int assist_fuwen_index[];		//辅助符文
		};

		struct fuwen_install
		{
			cmd_header header;
			int src_index;			//符文在包裹里面的位置
			int dst_index;			//要安装到符文包裹里面的目标位置 (0开始)
		};

		struct fuwen_uninstall
		{
			cmd_header header;
			int fuwen_index;		//符文在符文包裹里面的位置
			int assist_index;		//道具在普通包裹里面的位置
			int inv_index;			//符文拆除后放在包裹什么位置
		};

		struct resize_inv
		{
			cmd_header header;
			char type;				//0: inv; 1: trashbox
			int  new_size;
			int  cnt;
			struct cost_item
			{
				int item_idx;
				int item_id;
				int item_cnt;
			}items[];
		};

		struct register_flow_battle
		{
			cmd_header header;
		};

		struct arrange_inventory
		{
			cmd_header header;
			char pageind; //0->整理全部
		};

		struct arrange_trashbox
		{
			cmd_header header;
			char pageind; //0->整理全部
		};

		struct arrange_pocket
		{
			cmd_header header;
		};

		struct get_web_order
		{
			cmd_header header;
			int64_t order_id;
		};

		struct customize_rune
		{
			cmd_header header;
			int type;
			int count;	//数量
			int prop[];
		};

		struct get_cross_server_battle_info
		{
			cmd_header header;
			int map_id;
		};

		struct control_trap
		{
			cmd_header header;
			int id;
			int tid;
		};

		struct summon_teleport
		{
			cmd_header header;
			int npc_id;
		};

		struct create_crossvr_team
		{
			cmd_header header;
			int name_len;
			char team_name[];
		};

		struct get_king_reward
		{
			cmd_header header;
		};

		struct mobactive_start
		{
			cmd_header header;
			int npc_id;
			int tool_type;
			int task_id;
		};

		struct mobactive_finish 
		{
			cmd_header header;
		};

		struct qilin_invite
		{
			cmd_header header;
			int who;
		};

		struct qilin_invite_reply
		{
			cmd_header header;
			int who;
			int param;
		};

		struct qilin_cancel
		{
			cmd_header header;
		};

		struct qilin_up
		{
			cmd_header header;
		};

		struct qilin_down
		{
			cmd_header header;
		};

		struct enter_faction_base
		{
			cmd_header header;
		};

		struct add_fac_building
		{
			cmd_header header;
			int fid;	//帮派 id
			int field_index; //地块索引 从 1 开始编号
			int building_tid; //建筑的模板 id
		};
		struct upgrade_fac_building
		{
			cmd_header header;
			int fid;
			int field_index; //地块索引 从 1 开始编号
			int building_tid; //建筑的模板 id
			int cur_level; //当前等级
		};
		struct remove_fac_building
		{
			cmd_header header;
			int fid;
			int field_index; //地块索引 从 1 开始编号
			int building_tid; //建筑的模板 id
		};
		struct post_fac_base_msg
		{
			cmd_header header;
			int fid;
			size_t msg_len;
			char msg[];
		};
		struct pet_change_shape
		{
			cmd_header header;
			int pet_index;
			int shape_id; //1-7
		};

		struct pet_refine_attr
		{
			cmd_header header;
			int pet_index;
			int attr_type; // 0 - 14
			int assist_id;
			int assist_count; //1 10 100
		};

		struct active_emote_action_invite
		{
			cmd_header header;
			int who;
			int type;
		};

		struct active_emote_action_invite_reply
		{
			cmd_header header;
			int who;
			int type;
			int param;
		};

		struct active_emote_action_cancel
		{
			cmd_header header;
		};

		struct get_raid_transform_task
		{
			cmd_header header;
		};


		struct get_fac_base_info
		{
			cmd_header header;
		};

		struct get_cs_6v6_award
		{
			cmd_header header;
			int award_type;	//0-每周分档奖励 1-每周兑换币奖励 2-战队赛季奖励
			int award_level; //奖励的档位
		};

		struct exchange_cs_6v6_award
		{
			cmd_header header;
			int index;
			int item_id;
			int currency_id;
		};

		struct exchange_cs_6v6_money
		{
			cmd_header header;
			int type;	//0: 金兑换银  1:金兑换铜  2:银兑换铜
			int count;	//参加兑换的数量
		};

		struct puppet_form_change
		{
			cmd_header header;
		};

		struct fac_base_transfer
		{
			cmd_header header;
			int region_index;
		};

		struct withdraw_facbase_auction
		{
			cmd_header header;
			int item_index;
		};

		struct withdraw_facbase_coupon
		{
			cmd_header header;
		};

		struct get_facbase_auction
		{
			cmd_header header;
		};

		struct leave_faction_base
		{
			cmd_header header;
		};

		struct object_charge_to
		{
			cmd_header header;
			int type;
			int target_id;
			A3DVECTOR pos;
		};

		struct stop_travel_around
		{
			cmd_header header;
		};

		struct get_summon_petprop 
		{
			cmd_header header;
		};

		struct get_seek_award
		{
			cmd_header header;
		};

		struct seeker_buy_skill
		{
			cmd_header header;
		};

		struct hide_and_seek_raid_apply
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			bool is_team;
		};

		struct capture_raid_submit_flag
		{
			cmd_header header;
		};

		struct capture_raid_apply
		{
			cmd_header header;
			int map_id;
			int raid_template_id;
			bool is_team;
		};

/*------------------------------内部GM 命令------------------------------------*/		
		struct  gmcmd_move_to_player
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_recall_player
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_player_inc_exp
		{
			cmd_header header;
			int exp;
			int sp;
		};

		struct gmcmd_endue_item
		{
			cmd_header header;
			int item_id;
			size_t count;
		};

		struct gmcmd_endue_sell_item
		{
			cmd_header header;
			int item_id;
			size_t count;
		};

		struct gmcmd_remove_item
		{
			cmd_header header;
			int item_id;
			size_t count;
		};

		struct gmcmd_endue_money
		{
			cmd_header header;
			int money;
		};


		struct gmcmd_offline
		{
			cmd_header header;
		};

		struct gmcmd_resurrect
		{
			cmd_header header;
		};

		struct gmcmd_enable_debug_cmd
		{
			cmd_header header;
		};

		struct gmcmd_drop_generator
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_active_spawner
		{
			cmd_header header;
			bool is_active;
			int sp_id;
		};

		struct gmcmd_generate_mob
		{
			cmd_header header;
			int mob_id;
			int vis_id;
			short count;
			short life;
			size_t name_len;
			char name[];
		};

		struct gmcmd_movetomap
		{
			cmd_header header;
			int world_tag;
			float x;
			float y;
			float z;	
		};
	}
}


#pragma pack()
#endif

