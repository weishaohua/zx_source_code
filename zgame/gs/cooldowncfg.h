#ifndef __ONLINE_GAME_GS_COOLDOWN_CONFIG_H__
#define __ONLINE_GAME_GS_COOLDOWN_CONFIG_H__

//单位为毫秒
#define EMOTE_COOLDOWN_TIME		1500								//表示表情动作的冷却时间
#define FASHION_COOLDOWN_TIME		4000							//表示更换时装的冷却时间
#define DROPMONEY_COOLDOWN_TIME		1671							//表示丢钱操作的冷却时间
#define DROPITEM_COOLDOWN_TIME  	321								//表示丢物品操作的冷却时间
#define QUERY_OTHER_EQUIP_COOLDOWN_TIME	20000						//察看别人装备的冷却时间
#define FIREWORKS_COOLDOWN_TIME		800								//使用标准烟花的时间
#define SKILLTRIGGER_COOLDOWN_TIME	120000							//使用技能物品的冷却时间
#define COUPLE_JUMP_COOLDOWN_TIME	5000							//夫妻使用物品的冷却时间
#define LOTTERY_COOLDOWN_TIME		100								//彩票的冷却时间
#define RECORDER_COOLDOWN_TIME		20000							//摄像机的冷却时间
#define RECORDER_CHECK_COOLDOWN_TIME	20000						//检查录像检查的冷却时间
#define	GETBATTLEINFO_COOLDOWN_TIME	30000							//获取战场信息
#define QUERY_OTHER_ACHIEVMENT_COOLDOWN_TIME    10000   			//查看别人的成就的冷却时间 
#define CHANGE_DS_TIME			180000								//跨服请求的冷却时间

#define GET_MALL_PRICE_COOLDOWN_TIME		30000					//获取商城里哪些限时商品正在销售
#define GET_VIP_AWARD_INFO_COOLDOWN_TIME	15000					//获取VIP奖励信息冷却时间
#define RESET_SKILL_PROP_COOLDOWN_TIME		(24 * 3600 * 1000) 		//90级以下免费洗点洗天书冷却时间
#define ARRANGE_TRASHBOX_COOLDOWN_TIME          5000               	//整理仓库的冷却时间
#define ARRANGE_INVENTORY_COOLDOWN_TIME         5000               	//整理背包的冷却时间
#define ARRANGE_POCKET_COOLDOWN_TIME         	5000               	//整理pocket的冷却时间

enum 
{
	COOLDOWN_INDEX_NULL,			//0, 表示无冷却时间，通常用于初始化或占位。
	COOLDOWN_INDEX_EMOTE,			//1, 表情动作的冷却时间。
	COOLDOWN_INDEX_DROP_MONEY,		//2, 丢钱操作的冷却时间。
	COOLDOWN_INDEX_DROP_ITEM,		//3, 丢物品操作的冷却时间。
	COOLDOWN_INDEX_SWITCH_FASHION,	//4, 更换时装的冷却时间。

	COOLDOWN_INDEX_ITEM_TELEPORT,		//5, 物品传送的冷却时间。
	COOLDOWN_INDEX_QUERY_OTHER_EQUIP,	//6, 查询其他玩家装备的冷却时间。
	COOLDOWN_INDEX_FIREWORKS,			//7, 烟花使用的冷却时间。
	COOLDOWN_INDEX_FARCRY,				//8, 远程呼叫的冷却时间。
	COOLDOWN_INDEX_SKILL_TRIGGER,		//9, 技能触发的冷却时间。

	COOLDOWN_INDEX_DBL_EXP,			//10, 双倍经验的冷却时间。
	COOLDOWN_INDEX_SKILL_ITEM_1,	//11, 不同技能物品的冷却时间。
	COOLDOWN_INDEX_SKILL_ITEM_2,	//12, 不同技能物品的冷却时间。
	COOLDOWN_INDEX_SKILL_ITEM_3,	//13, 不同技能物品的冷却时间。
	COOLDOWN_INDEX_PK_RESET,		//14, PK（玩家对战）重置的冷却时间。

	COOLDOWN_INDEX_MOUNT,			//15, 坐骑使用的冷却时间。
	COOLDOWN_INDEX_COUPLE_JUMP,		//16, 坐骑使用的冷却时间。
	COOLDOWN_INDEX_LOTTERY,			//17, 抽奖的冷却时间。
	COOLDOWN_INDEX_RECORDER,		//18, 记录仪相关操作的冷却时间。
	COOLDOWN_INDEX_RECORDER_CHECK,	//19, 记录仪相关操作的冷却时间。

	COOLDOWN_INDEX_TEAM_RELATION,		//20, 团队关系操作的冷却时间。
	COOLDOWN_INDEX_CD_ERASER,			//21, 冷却时间擦除的冷却时间。
	COOLDOWN_INDEX_PET_COMBINE1,		//22, 宠物合成的冷却时间。
	COOLDOWN_INDEX_PET_COMBINE2,		//23, 宠物合成的冷却时间。
	COOLDOWN_INDEX_PICKUP,				//24, 拾取物品的冷却时间。

	COOLDOWN_INDEX_SET_FASHION_MASK,	//25, 设置时装面具的冷却时间。
	COOLDOWN_INDEX_GETBATTLEINFO,		//26, 获取战斗信息的冷却时间。
	COOLDOWN_INDEX_HEALING_POTION,		//27, 不同药水的冷却时间。
	COOLDOWN_INDEX_MANA_POTION,			//28, 不同药水的冷却时间。
	COOLDOWN_INDEX_REJU_POTION,			//29, 不同药水的冷却时间。

	COOLDOWN_INDEX_STEP_HEALING,		//30, 逐步治疗、法力和恢复的冷却时间。
	COOLDOWN_INDEX_STEP_MANA,			//31, 逐步治疗、法力和恢复的冷却时间。
	COOLDOWN_INDEX_STEP_REJU,			//32, 逐步治疗、法力和恢复的冷却时间。
	COOLDOWN_INDEX_QUERY_OTHER_ACHIEVMENT, //33, 查询其他玩家成就的冷却时间。
	COOLDOWN_INDEX_TRANSFORM,			//34, 变形的冷却时间。

	COOLDOWN_INDEX_NEWBIE_CHAT,		//35, 新手聊天的冷却时间。
	COOLDOWN_INDEX_CHANGE_DS,		//36, 更改DS的冷却时间。
	COOLDOWN_INDEX_DEITY_POTION,	//37, 神药的冷却时间。
	COOLDOWN_INDEX_STEP_DEITY,		//38, 神药的冷却时间。
	COOLDOWM_INDEX_GET_MALL_PRICE,	//39, 获取商城价格的冷却时间。

	COOLDOWM_INDEX_GET_VIP_AWARD_INFO,			//40, 获取VIP奖励信息的冷却时间。
	COOLDOWN_INDEX_RESET_SKILL_PROP_SKILL,		//41, 重置技能属性的冷却时间。
	COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT,		//42, 重置技能属性的冷却时间。
	COOLDOWN_INDEX_BATTLE_FLAG,					//43, 战斗标志的冷却时间。
	COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON,	//44, 玩家首次退出原因的冷
	
	COOLDOWN_INDEX_BOTTLE_HP,		//45, 生命药水的冷却时间。
	COOLDOWN_INDEX_BOTTLE_MP,		//46, 魔法药水的冷却时间。
	COOLDOWN_INDEX_BOTTLE_DEITY,	//47, 神力药水的冷却时间。
	COOLDOWN_INDEX_SKILL_ITEM_4,	//48, 第四种技能物品的冷却时间。
	COOLDOWN_INDEX_SKILL_ITEM_5, 	//49, 第五种技能物品的冷却时间。

	COOLDOWN_INDEX_BATH,				//50, 洗澡的冷却时间。
	COOLDOWN_INDEX_KING_CALL_GUARD,		//51, 国王召唤卫兵的冷却时间。
	COOLDOWN_INDEX_ARRANGE_INVENTORY,	//52, 整理背包的冷却时间。
	COOLDOWN_INDEX_ARRANGE_TRASHBOX,	//53, 整理垃圾箱的冷却时间。
	COOLDOWN_INDEX_TELEPORT1,			//54, 祝福传送的冷却时间。

	COOLDOWN_INDEX_TELEPORT2,			//55, 诅咒传送的冷却时间。
	COOLDOWN_INDEX_CHANGE_PUPPET_FORM,	//56, 傀儡姿态切换的冷却时间。
	COOLDOWN_INDEX_CIRCLE_OF_DOOM,		//57, 结阵的冷却时间。
	COOLDOWN_INDEX_ARRANGE_POCKET,		//58, 整理口袋的冷却时间。

	COOLDOWN_INDEX_PRODUCE_BEGIN 	= 600,	//生产技能开始的冷却时间索引。
	COOLDOWN_INDEX_PRODUCE_END 	= 800,		//生产技能结束的冷却时间索引。

	//组合技编辑
	COOLDOWN_INDEX_COMBINE_BEGIN	= 900,	//组合技能开始的冷却时间索引。
	COOLDOWN_INDEX_COMBINE_END	= 950,		//组合技能结束的冷却时间索引。

	COOLDOWN_SPEC_ERROR1 = 1023,			//特殊错误1的冷却时间。
	COOLDOWN_SKILL_START = 1024,			//技能开始的冷却时间索引。
	COOLDOWN_INDEX_CHEATER = 997,			//作弊者的冷却时间。
	COOLDOWN_INDEX_ANTI_CHEAT = 998,		//反作弊的冷却时间。
};

#endif

