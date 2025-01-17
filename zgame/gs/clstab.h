#ifndef __ONLINEGAME_GS_CLS_TAB_H__
#define __ONLINEGAME_GS_CLS_TAB_H__

enum
{
	CLS_PLAYER_IMP,					//0
	CLS_PLAYER_IMP_LESS_10,				//1
	CLS_PLAYER_CONTROLLER,				//2
	CLS_PLAYER_CONTROLLER_LESS_10,			//3
	CLS_PLAYER_DISPATCHER,				//4
	CLS_NPC_IMP,					//5
	CLS_NPC_CONTROLLER,				//6
	
	CLS_NPC_DISPATCHER,				//7
	CLS_MATTER_IMP,					//8
	CLS_MATTER_DISPATCHER,				//9
	CLS_MATTER_CONTROLLER,				//10
	CLS_MATTER_ITEM_BASE_IMP,			//11
	CLS_MATTER_ITEM_DISPATCHER,			//12
	CLS_GM_DISPATCHER,				//13
	CLS_PVP_PLAYER_IMP,				//14
	CLS_CULT_PVP_PLAYER_IMP,			//15  混沌地图
	CLS_RACE_PVP_PLAYER_IMP,			//16
	CLS_CULT2_PVP_PLAYER_IMP,			//17  仙佛魔地图, 怪物杀死会损失经验
	CLS_ZONE_PLAYER_IMP,				//18  跨服玩家
	CLS_PET_IMP,
	CLS_PET_DISPATCHER,
	CLS_BATTLEGROUND_PLAYER_IMP,
	CLS_SUMMON_IMP,
	CLS_SUMMON_DISPATCHER,
	CLS_CARRIER_NPC_IMP,

	
	CLS_MATTER_ITEM_IMP,
	CLS_MATTER_ITEM_CONTROLLER,
	CLS_MATTER_MONEY_IMP,
	CLS_MATTER_MINE_IMP,
	CLS_MATTER_DYN_IMP,
	CLS_NPC_AI_DEFAULT,	
	CLS_NPC_AI_POLICY_BASE,
	CLS_NPC_AI_POLICY_MASTER,
	CLS_NPC_AI_POLICY_MINOR,
	CLS_NPC_AI_POLICY_BOSS,
	CLS_NPC_AI_TASK,
	CLS_NPC_AI_MELEE_TASK,
	CLS_NPC_AI_RANGE_TASK,
	CLS_NPC_AI_MAGIC_TASK,
	CLS_NPC_AI_MAGIC_MELEE_TASK,
	CLS_NPC_AI_FOLLOW_MASTER_TASK,
	CLS_NPC_AI_PET_FOLLOW_MASTER_TASK,
	CLS_NPC_AI_FOLLOW_TARGET_TASK,
	CLS_NPC_AI_RUNAWAY_TASK,
	CLS_NPC_AI_SILIENT_RUNAWAY_TASK,
	CLS_NPC_AI_SILIENT_TASK,
	CLS_NPC_AI_SKILL_TASK,
	CLS_NPC_AI_SKILL_TASK_2,
	CLS_NPC_AI_REST_TASK,
	CLS_NPC_AI_REGENERATION_TASK,
	CLS_NPC_AI_PATROL_TASK,
	CLS_NPC_AI_POLICY_PATROL,
	CLS_NPC_AI_FIX_MELEE_TASK,
	CLS_NPC_AI_RETURNHOME_TASK,
	CLS_NPC_AI_FIX_MAGIC_TASK,
	CLS_NPC_AI_POLICY_PET,
	CLS_NPC_AI_POLICY_SUMMON,
	CLS_NPC_AI_SUMMON_FOLLOW_MASTER_TASK,
	CLS_NPC_AI_CLONE_FOLLOW_MASTER_TASK,
	CLS_NPC_AI_PET_SKILL_TASK,

	CLS_NPC_AGGRO_POLICY,
	CLS_NPC_MINOR_AGGRO_POLICY,
	CLS_NPC_MOBS_AGGRO_POLICY,
	CLS_NPC_BOSS_AGGRO_POLICY,
	CLS_SERVICE_NPC_IMP,
	CLS_NPC_AI_POLICY_GUARD,
	CLS_NPC_AI_POLICY_SERVICE,
	CLS_NPC_AI_POLICY_TURRET,
	CLS_PET_AGGRO_POLICY,
	CLS_TURRET_AGGRO_POLICY,
	CLS_SUMMON_AGGRO_POLICY,
	CLS_PROTECTED_NPC_IMP,

	CLS_NPC_GUARD_AGENT,
	CLS_NPC_PATROL_AGENT,
	CLS_NPC_BASE_PATROL_AGENT,
	CLS_MOBACTIVE_NPC_IMP,
	CLS_NPC_AI_POLICY_MOB_ACTIVE,
	CLS_NPC_AI_MOB_ACTIVE_PATROL_TASK,

	CLS_SESSION_BASE = 100,
	CLS_SESSION_EMPTY,
	CLS_SESSION_MOVE,
	CLS_SESSION_STOP_MOVE,
	CLS_SESSION_NORMAL_ATTACK,
	CLS_SESSION_NPC_ZOMBIE,
	CLS_SESSION_NPC_ATTACK,
	CLS_SESSION_NPC_RANGE_ATTACK,
	CLS_SESSION_NPC_KEEP_OUT,
	CLS_SESSION_NPC_DELAY,
	CLS_SESSION_NPC_FLEE,
	CLS_SESSION_NPC_SILENT_FLEE,
	CLS_SESSION_NPC_FOLLOW_TARGET,
	CLS_SESSION_NPC_EMPTY,
	CLS_SESSION_NPC_CRUISE,
	CLS_SESSION_SKILL,
	CLS_SESSION_PRODUCE,
	CLS_SESSION_CANCEL_ACTION,
	CLS_SESSION_USE_ITEM,
	CLS_SESSION_USE_ITEM_WITH_TARGET,
	CLS_SESSION_PLAYER_SIT_DOWN,
	CLS_SESSION_NPC_SKILL,
	CLS_SESSION_GATHER,
	CLS_SESSION_USE_TRASHBOX,
	CLS_SESSION_NPC_FOLLOW_MASTER,
	CLS_SESSION_EMOTE,
	CLS_SESSION_NPC_REGENERATION,
	CLS_SESSION_GATHER_PREPARE,
	CLS_SESSION_RESURRECT,
	CLS_SESSION_RESURRECT_BY_ITEM,
	CLS_SESSION_RESURRECT_IN_TOWN,
	CLS_SESSION_NPC_PATROL,
	CLS_SESSION_NPC_REGEN,
	CLS_SESSION_ENTER_SANCTUARY,
	CLS_SESSION_SAY_HELLO,
	CLS_SESSION_INSTANT_SKILL,
	CLS_SESSION_COSMETIC,
	CLS_SESSION_REGION_TRANSPORT,
	CLS_SESSION_RESURRECT_PROTECT,
	CLS_SESSION_POS_SKILL,
	CLS_SESSION_SUMMON_PET,
	CLS_SESSION_RECALL_PET,
	CLS_SESSION_COMBINE_PET,
	CLS_SESSION_UNCOMBINE_PET,
	CLS_SESSION_P_START_MOVE,
	CLS_SESSION_P_MOVE,
	CLS_SESSION_P_STOP_MOVE,
	CLS_SESSION_FREE_PET,
	CLS_SESSION_ADOPT_PET,
	CLS_SESSION_REFINE_PET,
	CLS_SESSION_FOLLOW_TARGET,
	CLS_SESSION_REFINE_PET2,
	CLS_SESSION_SPIRIT_SKILL,
	CLS_SESSION_ACTIVE_EMOTE,



	CLS_ITEM = 200,
	CLS_ITEM_EQUIP,
	CLS_ITEM_PET_EGG,
	CLS_ITEM_RF_STONE,
	CLS_ITEM_PR_STONE,
	CLS_ITEM_SPEC_PR_STONE,
	CLS_ITEM_SK_STONE,
	CLS_ITEM_COLORANT,

	CLS_FILTER = 400,			//由于filter要存盘，这个值不要改 
	CLS_FILTER_HEALING,
	CLS_FILTER_MANA_GEN,
	CLS_FILTER_MANA_LIFE_GEN,
	CLS_FILTER_SKILL_INTERRUPT,
	CLS_FILTER_INVINCIBLE,
	CLS_FILTER_INVINCIBLE_SPEC_ID,
	CLS_FILTER_PVP_LIMIT,
	CLS_FILTER_SITDOWN,
	CLS_FILTER_ONLINE_AGENT,
	CLS_FILTER_GATHER_INTERRUPT,
	CLS_FILTER_NPC_PASSIVE,
	CLS_FILTER_EFFECT,
	CLS_FILTER_BANISH_INVINCIBLE,
	CLS_FILTER_CHECK_INSTANCE_KEY,
	CLS_FILTER_PVP_DUEL,
	CLS_FILTER_MOUNT,
	CLS_FILTER_CHECK_BATTLEFIELD_KEY,
	CLS_FILTER_TOWER_BUILD,
	CLS_FILTER_PET_COMBINE1,
	CLS_FILTER_PET_COMBINE2,
	CLS_FILTER_BATTLEGROUND_INVINCIBLE,
	CLS_FILTER_LOGON_INVINCIBLE,
	CLS_FILTER_INVISIBLE,
	CLS_FILTER_AZONE,
	CLS_FILTER_DEITY,
	CLS_FILTER_COD_SPONSOR,
	CLS_FILTER_COD_MEMBER,
	CLS_FILTER_BATTLEFLAG,
	CLS_FILTER_KINGDOM_BATTLEPREPARE,
	CLS_FILTER_KINGDOM_EXP,
	CLS_FILTER_KINGDOM_BATH,
	CLS_FILTER_TELEPORT1,
	CLS_FILTER_TELEPORT2,
	CLS_FILTER_COD_COOLDOWN,
	CLS_FILTER_ACTIVE_EMOTE,

	CLS_USER_DEFINE = 0x400,
	CLS_SKILL_FILTER_BASE = 4096,
	CLS_SKILL_FILTER_END = 8192,

	CLS_BG_PLAYER_IMP = 10000,   		// battleground player base imp
	CLS_CB_PLAYER_IMP = 10001,              // common battleground player imp
	CLS_CB_MATTER_FLAG_IMP = 10002,         // common battleground flag imp
	CLS_KF_PLAYER_IMP = 10003,		// killing field player imp
	CLS_KF_MATTER_FLAG_IMP = 10004, 	// killing filed matter imp
	CLS_AR_PLAYER_IMP = 10005,		// arena player imp
	CLS_AR_MATTER_FLAG_IMP = 10006, 	// arena matter imp
	CLS_CS_PLAYER_IMP = 10007,		// cross server battleground player imp
	CLS_CS_MATTER_FLAG_IMP = 10008, 	// cross server battleground matter imp
	CLS_IN_PLAYER_IMP = 10009,		// instance battleground player imp
	CLS_IN_MATTER_FLAG_IMP = 10010, 	// instance battleground matter imp
	CLS_CS_MELEE_PLAYER_IMP = 10011,	// instance battleground player imp
	CLS_CS_MELEE_MATTER_FLAG_IMP = 10012, 	// instance battleground matter imp
	CLS_CH_PLAYER_IMP      = 10013,     //challenge_space battleground player imp
	CLS_CH_MATTER_FLAG_IMP = 10014,     //challenge_space battleground matter imp
	CLS_CS_TEAM_PLAYER_IMP = 10015,     //cross_server team battleground player imp
	CLS_CS_TEAM_MATTER_FLAG_IMP = 10016,//cross_server team battleground matter imp
	CLS_CS_FLOW_PLAYER_IMP = 10017,	//跨服流水席战场玩家实现

	CLS_BF_PLAYER_IMP = 20001,		// battlefield player imp
	CLS_TR_PLAYER_IMP = 30001,		// territory player imp
	CLS_RAID_PLAYER_IMP =30002,		// Raid player imp
	CLS_TOWER_RAID_PLAYER_IMP = 30003, 	//Tower raid player imp
	CLS_COLLISION_RAID_PLAYER_IMP=30004,
	CLS_MAFIA_BATTLE_RAID_PLAYER_IMP=30005,
	CLS_KINGDOM_PLAYER_IMP=30006,
	CLS_KINGDOM2_PLAYER_IMP=30007,
	CLS_LEVEL_RAID_PLAYER_IMP=30008,
	CLS_KINGDOM_BATH_PLAYER_IMP=30009,
	CLS_FACBASE_PLAYER_IMP=30010,
	CLS_TRANSFORM_RAID_PLAYER_IMP=30011,
	CLS_CR_RAID_PLAYER_IMP=30012,  // cr raid player imp, Youshuang add
	CLS_MATTER_COMBINE_MINE_IMP=30013, // combine mine imp, Youshuang add
	CLS_STEP_RAID_PLAYER_IMP=30014,
	CLS_COLLISION2_RAID_PLAYER_IMP=30015,
	CLS_SEEK_RAID_PLAYER_IMP=30016,
	CLS_CAPTURE_RAID_PLAYER_IMP=30017,
};
#endif
