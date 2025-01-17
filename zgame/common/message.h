#ifndef __ONLINEGAME_COMMON_MESSAGE_H__
#define __ONLINEGAME_COMMON_MESSAGE_H__

#include <stdlib.h>
#include <amemory.h>
#include <ASSERT.h>
#include "types.h"
struct MSG 
{
	int 	message;	//消息的类型
	struct XID target;	//收消息的目标，可能是服务器，玩家，物品，NPC等
	struct XID source;	//从哪里发过来的，可能的id和上面一样
	A3DVECTOR pos;		//消息发出时的位置，有的消息可能位置并无作用
	int	param;		//一个参数，如果这个参数够用，那么就使用这个参数
	int 	param2;		//第二参数，因为常常发现需要这个参数
	size_t 	content_length;	//消息的具体数据长度
	const void * content;	//消息的具体数据 网络上传播时这个字段无效
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
		buf = abase::fast_allocator::align_alloc(sizeof(MSG) + length);		//必须对齐，考虑多个msg
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
	GM_MSG_NULL,				//空消息
	GM_MSG_FORWARD_USERBC,			//转发的用户广播
	GM_MSG_FORWARD,				//转发的消息，内容应该作为一条新的消息内容来解释
	GM_MSG_FORWARD_BROADCAST,		//转发的消息广播消息,content是另外一条完整的消息
	GM_MSG_USER_GET_INFO,			//用户取得必要的数据

//5
	GM_MSG_IDENTIFICATION,			//服务器告知自己的身份,原的类型必须是server并且id是他的符号
	GM_MSG_SWITCH_GET,			//取得用户数据,服务器切换，取得用户数据 param是 tag,content是key
	GM_MSG_SWITCH_START,			//用户通知管理器自己开始转移
	GM_MSG_SWITCH_NPC,			//NPC切换服务器
	GM_MSG_USER_MOVE_OUTSIDE,		//用户在边界移动

//10	
	GM_MSG_USER_NPC_OUTSIDE,		//NPC在边界处移动，不同之处在于NPC不需要取得新看到区域的对象
	GM_MSG_ENTER_WORLD,			//给controller的，表示用户已经进入了世界
	GM_MSG_ATTACK,				//目标和源都必须是个体
	GM_MSG_SKILL,				//目标和源都必须是个体
	GM_MSG_PICKUP,				//拣起物品,目标一般是物品 content是msg_pickup_t param是是否轮流分配

//15
	GM_MSG_FORCE_PICKUP,			//强制捡起物品，不校验所书者ID和组队ID
	GM_MSG_PICKUP_MONEY,			//物品通知用户拣到钱 param是钱数  content是谁丢弃的
	GM_MSG_PICKUP_TEAM_ITEM,		//物品通知组队捡到物品  param 是该物品的world_tag
	GM_MSG_RECEIVE_MONEY,			//通知玩家得到组队时怪物掉落的金钱 需要考虑金钱加成
	GM_MSG_PICKUP_ITEM,			//物品通知用户拣到物品 param是 palyer_id | 0x80000000(如果组队）

//20
	GM_MSG_ERROR_MESSAGE,			//让player发送一个error message
	GM_MSG_NPC_SVR_UPDATE,			//NPC发生了服务器切换，这个消息只发给处于移走状态的原生NPC
	GM_MSG_EXT_NPC_DEAD,			//外部的NPC的死亡消息(真正删除)，这个消息只发给处于移走状态的原生NPC
	GM_MSG_EXT_NPC_HEARTBEAT,		//外部NPC的心跳，用于判断是否超时 
	GM_MSG_WATCHING_YOU,			//主动怪物激活的消息,由玩家或npc发出，后面是一个watching_t的结构

//25
//	AGGRO  message 
	GM_MSG_GEN_AGGRO,			//生成aggro，后面附加了一个aggro_info_t的结构
	GM_MSG_TRANSFER_AGGRO,			//aggro的传送 目前只传送第一位 content是一个XID,如果该XID的id为-1    则清空仇恨列表 param是该人仇恨值
	GM_MSG_AGGRO_ALARM,			//aggro警报，当受到攻击时会发送，后面附加了一个aggro_alarm_t未使用
	GM_MSG_AGGRO_WAKEUP,			//aggro警报，将休眠的怪物惊醒,后面附加了一个aggro_alarm_t未使用
	GM_MSG_AGGRO_TEST,			//aggro测试,只有当发送者在aggro列表中，才会引发新的aggro，后面附加了一个aggro_info_t未使用
	
//30
	GM_MSG_OBJ_SESSION_END,			//对象的session完成
	GM_MSG_OBJ_SESSION_REPEAT,		//表示session要继续执行 
	GM_MSG_OBJ_ZOMBIE_END,			//表示要结束僵尸状态
	GM_MSG_EXPERIENCE,			//得到经验值	content 是一个msg_exp_t
	GM_MSG_GROUP_EXPERIENCE,		//得到组队经验值 conennt 是多个msg_grp_exp_t , param 造成的总伤害
	
//35
	GM_MSG_TEAM_EXPERIENCE,			//得到组队经验值 conennt 是msg_exp_t 超过距离经验值会被忽略 param 是杀死的npcid 如刮�0则不是本队伍杀死的
	GM_MSG_QUERY_OBJ_INFO00,		//取得对象的info00 param是发送者的sid ,content是一个int代表cs_index
	GM_MSG_HEARTBEAT,			//发给自己的心跳消息  参数是这次Heartbeat的秒数
	GM_MSG_HATE_YOU,
	GM_MSG_TEAM_INVITE,			//请求某人加入队伍param是teamseq, content是一个int 表示pickup_flag param2是faction

//40	
	GM_MSG_TEAM_AGREE_INVITE,		//被邀请人同意加入队伍 content是一个int(表示职业)+ team_mutable_prop
	GM_MSG_TEAM_REJECT_INVITE,		//拒绝加入邀请
	GM_MSG_JOIN_TEAM,			//队长同意某人加入队伍 param高位是捡取方式 param低位是队员个数，content是member_entry的表 
	GM_MSG_JOIN_TEAM_FAILED,		//对象无法加入队伍，应该从队伍中去除
	GM_MSG_MEMBER_NOTIFY_DATA,		//组队成员通知其他人自己的基础信息 content 是一个team_mutable_prop

//45	
	GM_MSG_NEW_MEMBER,			//leader通知新成员加入，content是一个member_entry list param是数量
	GM_MSG_LEAVE_PARTY_REQUEST,
	GM_MSG_LEADER_CANCEL_PARTY,
	GM_MSG_MEMBER_NOT_IN_TEAM,
	GM_MSG_LEADER_KICK_MEMBER,

//50	
	GM_MSG_MEMBER_LEAVE,
	GM_MSG_LEADER_UPDATE_MEMBER,
	GM_MSG_GET_MEMBER_POS,			//要求队友发送位置 param是发送者的sid ,content是一个int代表cs_index
	GM_MSG_QUERY_PLAYER_EQUIPMENT,		//取得特定玩家的数据，要求平面距离在一定范围之内param是发送者的sid ,content是一个int代表cs_index
	GM_MSG_TEAM_PICKUP_NOTIFY,		//队友分配到物品通知， param 是 type, content 是count

//55	
	GM_MSG_TEAM_CHAT___,			//组队聊天 param 是channel, content 是内容 已经作废，现在直接发送
	GM_MSG_SERVICE_REQUEST,			//player要求服务的消息 param 是服务类型 content 是具体数据 �
	GM_MSG_SERVICE_DATA,			//服务的数据到达 param 是服务类型  content 是 具体数据
	GM_MSG_SERVICE_HELLO,			//player 向服务商问好  param 是 player自己的faction
	GM_MSG_SERVICE_GREETING,		//服务商进行回话 可能需要在里面返回服务列表$$$$(现在未做)

//60	
	GM_MSG_SERVICE_QUIERY_CONTENT,		//取得服务内容 	 param 是服务类型, content可看作pair<cs_index,sid>
	GM_MSG_EXTERN_OBJECT_APPEAR_N,		//content 是extern_object_manager::object_appear [作废]
	GM_MSG_EXTERN_OBJECT_DISAPPEAR_N,	//消失或者    [作废]
	GM_MSG_EXTERN_OBJECT_REFRESH_N,		//更新位置和血值，param中保存的是血值  [作废]
	GM_MSG_USER_APPEAR_OUTSIDE,		//用户在外面出现，要发送必要的数据给该玩家，content 里是sid,param是linkd id

//65
	GM_MSG_FORWARD_BROADCAST_SPHERE_N,	//转发的消息广播消息,content是另外一条完整的消息 [作废]
	GM_MSG_FORWARD_BROADCAST_CYLINDER_N,	//转发的消息广播消息,content是另外一条完整的消息 [作废]
	GM_MSG_PRODUCE_MONSTER_DROP,		//通知系统产生怪物掉落物品和金钱， 发送源是所属者，param是money， content 是 struct { int team_id; int team_seq;int npc_id;int item_count; int item[];}
	GM_MSG_ENCHANT,				//使用辅助魔法
	GM_MSG_ENCHANT_ZOMBIE,			//使用辅助魔法,专门给死人用的

//70
	GM_MSG_OBJ_SESSION_REP_FORCE,		//表示session要repeat ，后面即使有任务也要继续执行
	GM_MSG_NPC_BE_KILLED,			//消息发给杀死npc的玩家，param 表示被杀死npc的类型 content是NPC的级别
	GM_MSG_NPC_CRY_FOR_HELP,		//npc 进行求救操作
	GM_MSG_PLAYER_TASK_TRANSFER,		//任务在player之间进行传送和通讯的函数
	GM_MSG_PLAYER_BECOME_INVADER,		//成为粉名 msg.param 是增加的时间

//75
	GM_MSG_PLAYER_KILL_PLAYER,		//杀死玩家的标志 msg.param代表对方的PK值
	GM_MSG_FORWARD_CHAT_MSG,		//转发的用户聊天信息,param是rlevel,source是XID(-channel,self_id)
	GM_MSG_QUERY_SELECT_TARGET,		//取得队友选择的对象
	GM_MSG_NOTIFY_SELECT_TARGET,		//取得队友选择的对象
	GM_MSG_SUBSCIBE_TARGET,			//要求订阅一个对象

//80
	GM_MSG_UNSUBSCIBE_TARGET,		//要求订阅一个对象
	GM_MSG_SUBSCIBE_CONFIRM,		//确认订阅是否存在
	GM_MSG_MONSTER_MONEY,			//通知玩家收到怪物掉落的金钱	param是金钱 需要考虑金钱加成
	GM_MSG_MONSTER_GROUP_MONEY,		//通知玩家收到队伍金钱  param是金钱 
	GM_MSG_GATHER_REQUEST,			//请求收集原料，  param 是玩家的faction, content 分别是玩家级别、采集工具和任务ID

//85
	GM_MSG_GATHER_REPLY,			//通知可以进行采集  param 是采集需要的时间
	GM_MSG_GATHER_CANCEL,			//取消采集
	GM_MSG_GATHER,				//进行采集，要求取得物品
	GM_MSG_GATHER_RESULT,			//采集完成，param 内是采集到的物品id, content是数量 和可能附加的任务ID
	GM_MSG_EXTERN_HEAL,			//给某某对象加血的消息

//90
	GM_MSG_INSTANCE_SWITCH_GET,		//取得用户数据,服务器切换，取得用户数据 用于副本间的切换 param是key
	GM_MSG_INSTANCE_SWITCH_USER_DATA,	//用户数据,SWITCH_SWITCH_GET的回应
	GM_MSG_EXT_AGGRO_FORWARD,		//通知原生npc进行仇恨转发 param 是rage大小， content是产生仇恨的id
	GM_MSG_TEAM_APPLY_PARTY,		//申请进入队伍选项      param2是faction
	GM_MSG_TEAM_APPLY_REPLY,		//申请成功回复 其中的param是seq	

//95
	GM_MSG_QUERY_INFO_1,			//查询INFO1，可以发给玩家或者NPC,param的内容是cs_index,content是sid
	GM_MSG_CON_EMOTE_REQUEST_NULL,		//进行协同动作的请求 param 是 action      已经作废
	GM_MSG_CON_EMOTE_REPLY_NULL,		//进行协同动作的回应 param 是action和同意与否的两个字节的 已经作废
	GM_MSG_TEAM_CHANGE_TO_LEADER,		//通知别人要成为leader
	GM_MSG_TEAM_LEADER_CHANGED,		//通知队友队长的改变

//100
	GM_MSG_OBJ_ZOMBIE_SESSION_END,		//死亡后进行session的操作，其他定义和正常的session操作一样
	GM_MSG_QUERY_MARKET_NAME,		//取得摆摊的名字，param是发送者的sid ,content是一个int代表cs_index
	GM_MSG_HURT,				//对象产生伤害 content是attacker_info_1, param1是damage, param2组合了攻击模式和是否非法攻击
	GM_MSG_DEATH,				//强行让对象死亡
	GM_MSG_PLANE_SWITCH_REQUEST,		//请求开始传送，content是key，如果进行传送，则返回 SWITCH_REPLAY

//105
	GM_MSG_PLANE_SWITCH_REPLY,		//传送请求被确认，content是key
	GM_MSG_SCROLL_RESURRECT,		//卷轴复活  param表示复活者是否开启了pvp模式1表示开启了
	GM_MSG_LEAVE_COSMETIC_MODE,		//脱离整容状态
	GM_MSG_DBSAVE_ERROR,			//数据库保存错误
	GM_MSG_SPAWN_DISAPPEAR,			//通知NPC和物品消失 param是condition

//110
	GM_MSG_PET_CTRL_CMD,			//玩家发来的控制消息会用这个消息发给宠物
	GM_MSG_ENABLE_PVP_DURATION,		//激活PVP状态
	GM_MSG_PLAYER_KILLED_BY_NPC,		//玩家被NPC杀死后NPC的逻辑
	GM_MSG_PLAYER_DUEL_REQUEST,             //玩家发出要求duel的请求
	GM_MSG_PLAYER_DUEL_REPLY,               //玩家回应duel的请求，param是是否答应duel

//115
	GM_MSG_PLAYER_DUEL_PREPARE,      	//决斗准备开始 3秒倒计时后开始
	GM_MSG_PLAYER_DUEL_START,               //决斗开始 
	GM_MSG_PLAYER_DUEL_CANCEL,		//停止决斗
	GM_MSG_PLAYER_DUEL_STOP,		//决斗结束
	GM_MSG_DUEL_HURT,			//PVP对象产生伤害content 被忽略

//120
	GM_MSG_PLAYER_BIND_REQUEST,		//请求骑在别人身上
	GM_MSG_PLAYER_BIND_INVITE,		//邀请别人骑在自己身上
	GM_MSG_PLAYER_BIND_REQ_REPLY,		//请求骑的回应
	GM_MSG_PLAYER_BIND_INV_REPLY,		//邀请骑的回应
	GM_MSG_PLAYER_BIND_PREPARE,		//准备开始连接

//125
	GM_MSG_PLAYER_BIND_LINK,		//连接开始
	GM_MSG_PLAYER_BIND_STOP,		//停止连接
	GM_MSG_PLAYER_BIND_FOLLOW,		//要求玩家跟随
	GM_MSG_QUERY_EQUIP_DETAIL,		//param 为faction, content 为cs_index 和cs_sid
	GM_MSG_PLAYER_RECALL_PET,		//让玩家强制消除召唤状态

//130
	GM_MSG_CREATE_BATTLEGROUND,		//要求战场服务器创建一个战场的消息，主要用于测试
	GM_MSG_BECOME_TURRET_MASTER,		//成为攻城车的master,param是tid, content 是faction
	GM_MSG_REMOVE_ITEM,			//删除一个物品的消息，用于攻城车控制后的物品减少 param是tid
	GM_MSG_NPC_TRANSFORM,			//NPC变形效果，content里保存 中间状态，中间时间 中间标志 最后状态
	GM_MSG_NPC_TRANSFORM2,			//NPC变形效果2，param 是目标ID 如果本来就和目标ID一致了，那么就不变形了

//135
	GM_MSG_TURRET_NOTIFY_LEADER,		//攻城车通知leader自己存在，让其无法再次进行召唤
	GM_MSG_PET_RELOCATE_POS,		//宠物要求重新定位坐标
	GM_MSG_PET_CHANGE_POS,			//主人修改了宠物的坐标
	GM_MSG_PET_DISAPPEAR,			//数据不正确,或者其它情况,主人要求宠物消失
	GM_MSG_PET_NOTIFY_HP_VP,		//宠物通知主人,告知自己的血量 精力

//140
	GM_MSG_PET_NOTIFY_DEATH,		//宠物通知主人自己的死亡
	GM_MSG_MASTER_INFO,			//主人通知宠物自己的数据
	GM_MSG_PET_LEVEL_UP,			//主人通知宠物升级了 ,content是 level
	GM_MSG_PET_HONOR_MODIFY,		//主人通知宠物的忠诚度发生变化
	GM_MSG_MASTER_ASK_HELP,			//主人要求宠物帮助
	
//145
	GM_MSG_REPU_CHG_STEP_1,			//威望转换第1步 被杀者传递自己的当前威望
	GM_MSG_REPU_CHG_STEP_2,			//威望转换第2步 杀人者传回结果  被杀者修改自己的威望
	GM_MSG_REPU_CHG_STEP_3,			//威望转换第3步 被杀者减完威望后 通知杀人者修改威望
	GM_MSG_TEAM_MEMBER_LVLUP,		//队长收到，低等级队员升级时为给队长增加威望的协议 param为升级前级别
	GM_MSG_KILL_PLAYER_IN_BATTLEGROUND,	// 当玩家在战场杀死敌方玩家时，会收到死者发来这个消息

//150
	GM_MSG_MODIFIY_BATTLE_DEATH,		// 玩家需要修改自己的死亡数信息，发送给world
	GM_MSG_MODIFIY_BATTLE_KILL,		// 玩家需要修改自己的杀人数信息，发送给world
	GM_MSG_PET_SET_COOLDOWN,		//宠物技能冷却
	GM_MSG_PET_SET_AUTO_SKILL,		//宠物技能自动施放
	GM_MSG_FEED_PET,			//主人给宠物吃药

//155
	GM_MSG_PET_CAST_SKILL,			//宠物使用技能
	GM_MSG_PET_HONOR_LEVEL_CHANGED,		//宠物的亲密度级别变了
	GM_MSG_PET_HUNGER_LEVEL_CHANGED,	//宠物的饱食度级别变了
	GM_MSG_MASTER_START_ATTACK,			//主人攻击
	GM_MSG_PET_INFO_CHANGED,		//宠物信息变化

//160
	GM_MSG_NPC_DISAPPEAR,			//NPC消失
	GM_MSG_TASK_AWARD_TRANSFOR,
	GM_MSG_PLAYER_KILL_PET,			//杀死玩家宠物
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
	GM_MSG_KILL_MONSTER_IN_BATTLEGROUND,	//战场里面怪物被玩家杀死
	GM_MSG_PLAYER_CATCH_PET,		//玩家抓宠物

//175
	GM_MSG_NPC_BE_CATCHED,			//怪物被抓了
	GM_MSG_NPC_BE_CATCHED_CONFIRM,
	GM_MSG_SYNC_BATTLE_INFO,
	GM_MSG_QUERY_ACHIEVEMENT,		//查询其他玩家成就列表
	GM_MSG_BATTLE_INFO_CHANGE,		//战场信息变化

//180
	GM_MSG_PLAYER_KILL_SUMMON,		//杀死玩家召唤怪
	GM_MSG_SUMMON_NOTIFY_DEATH,		//宠物通知主人自己的死亡
	GM_MSG_SUMMON_RELOCATE_POS,
	GM_MSG_SUMMON_CHANGE_POS,
	GM_MSG_EXTERN_ADD_MANA,			//通知目标加蓝

//185
	GM_MSG_PLAYER_ENTER_CARRIER,		//玩家请求上船
	GM_MSG_PLAYER_LEAVE_CARRIER,		//玩家请求下船
	GM_MSG_ENTER_CARRIER,			//通知玩家上船
	GM_MSG_LEAVE_CARRIER,			//通知玩家下船
	GM_MSG_CARRIER_SYNC_POS,		//船移动时同步自己的位置和方向给船上的对象

//190	
	GM_MSG_BATTLE_NPC_DISAPPEAR,		//战场NPC消失,用于战场清理
	GM_MSG_SUMMON_HEARTBEAT,		//召唤兽给玩家发送心跳信息
	GM_MSG_NPC_BORN,			//怪物创建，用于策略 
	GM_MSG_MONSTER_TRANSFORM2,		//对怪物使用照妖镜,原型为怪
	GM_MSG_MINE_TRANSFORM2,			//对怪物使用照妖镜,原型为矿物

//195
	GM_MSG_USE_COMBO_SKILL,			//连续技
	GM_MSG_TRY_CLEAR_AGGRO,			//清楚怪物仇恨
	GM_MSG_DEC_HP,				//扣除对方的HP, 不会加仇恨
	GM_MSG_EXCHANGE_STATUS,			//交换hp,mp
	GM_MSG_EXCHANGE_POS,			//交换位置

//200
	GM_MSG_HEAL_CLONE,			//给分身加hp
	GM_MSG_MANA_CLONE,			//给分身加mp
	GM_MSG_EXCHANGE_SUBSCIBE,		//交换订阅列表给
	GM_MSG_MINE_DISAPPEAR,			//清除所有矿物
	GM_MSG_QUERY_CLONE_EQUIPMENT,		//查询分身主人的装备信息

//205
	GM_MSG_PROTECTED_NPC_NOTIFY,		//通知保护的NPC状态
	GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER,	//同步战场信息给玩家
	GM_MSG_MASTER_DUEL_STOP,		//通知宠物和召唤兽主人决斗结束
	GM_MSG_TRANSFER_ATTACK,			//转移攻击消息
	GM_MSG_ROUND_START_IN_BATTLE,   //战场中一小局的开始

//210
	GM_MSG_ROUND_END_IN_BATTLE,   //战场中一小局的结束
	GM_MSG_ATTACK_FEED_BACK,	  //向攻击者发送攻击结果反馈, param为mask，param2为被攻击者等级(FEEDBACK_KILL),  或伤害数量(FEEDBACK_DAMAGE)
	GM_MSG_CIRCLE_OF_DOOM_PREPARE,  //阵法技能使用，开始收集人数
	GM_MSG_CIRCLE_OF_DOOM_STARTUP,  //阵法结阵成功，发消息给阵内的成员
	GM_MSG_CIRCLE_OF_DOOM_STOP,     //阵法终止、结束

//215
	GM_MSG_CIRCLE_OF_DOOM_ENTER,    //成员同意进入阵法
	GM_MSG_CIRCLE_OF_DOOM_LEAVE,    //成员离开阵法
	GM_MSG_CIRCLE_OF_DOOM_QUERY,    //查询阵法信息	
	GM_MSG_REMOVE_PERMIT_CYCLE_AREA,//清除发送消息者所施加的移动限制区域
	GM_MSG_CANCEL_BE_PULLED,		//解除被拉扯状态

//220
	GM_MSG_CANCEL_BE_CYCLE,         //解除阵法效果
	GM_MSG_QUERY_BE_SPIRIT_DRAGGED,	//查询被牵引信息
	GM_MSG_QUERY_BE_PULLED,			//查询被拉扯信息
	GM_MSG_TASK_SHARE_NPC_BE_KILLED,//消息发给杀死npc的玩家，param 表示被杀死npc的类型 content是NPC的级别
	GM_MSG_TASK_CHECK_STATE,	//摆摊或者交易结束后检查是否有被卡住的任务

//225
	GM_MSG_SUMMON_CAST_SKILL,		//召唤兽使用技能
	GM_MSG_SPIRIT_SESSION_END,		//轩辕灵使用的session结束
	GM_MSG_SPIRIT_SESSION_REPEAT,	//轩辕灵使用的session重复
	GM_MSG_SPIRIT_SESSION_REP_FORCE,//轩辕灵使用的表示session要repeat ，后面即使有任务也要继续执行
	GM_MSG_TALISMAN_SKILL,			//技能命中后通知施法者释放法宝技能

//230	
	GM_MSG_KINGDOM_BATTLE_HALF,	//国王战半场结束
	GM_MSG_KINGDOM_UPDATE_KEY_NPC,	//国王战辅助战场杀死NPC
	GM_MSG_REQUEST_BUFFAREA_BUFF,	//申请BUFF区域BUFF
	GM_MSG_KINGDOM_BATTLE_END,
	GM_MSG_LEVEL_RAID_INFO_CHANGE,	//level类副本中副本信息发生改变

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
	GM_MSG_GET_RAID_TRANSFORM_TASK, //副本变身
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

//GM所采用的消息	
	GM_MSG_GM_GETPOS=600,			//取得指定玩家的坐标 param 是 cs_index, content 是sid
	GM_MSG_GM_MQUERY_MOVE_POS,		//GM要求查询坐标 用于下一步跳转到玩家处 
	GM_MSG_GM_MQUERY_MOVE_POS_RPY,		//GM要求查询坐标的回应,用于GM的跳转命令 content是当前的instance key
	GM_MSG_GM_RECALL,			//GM要求进行跳转
	GM_MSG_GM_CHANGE_EXP,			//GM增加exp 和sp , param 是 exp , content 是sp
	GM_MSG_GM_ENDUE_ITEM,			//GM给与了若干物品 ，param 是item id, content 是数目 
	GM_MSG_GM_ENDUE_SELL_ITEM,		//GM给与了商店里卖的物品，其他同上
	GM_MSG_GM_REMOVE_ITEM,			//GM要求删除某些物品，param 是item id, content 是数目
	GM_MSG_GM_ENDUE_MONEY,			//GM增加或者减少金钱
	GM_MSG_GM_RESURRECT,			//GM要求复活
	GM_MSG_GM_OFFLINE,			//GM要求下线 
	GM_MSG_GM_DEBUG_COMMAND,		//GM要求下线 
	GM_MSG_GM_RESET_PP,			//GM进行洗点操作

	GM_MSG_MAX,

};

struct msg_usermove_t	//用户移动并且跨越边界的消息
{
	int cs_index;
	int cs_sid;
	int user_id;
	A3DVECTOR newpos;	//消息里面有oldpos
	size_t leave_data_size;	//离开发送的消息大小（该消息附加在后面)
	size_t enter_data_size;	//离开发送的消息大小（该消息附加在后面)
};

struct msg_aggro_info_t
{
	XID source;		//谁生成了这些仇恨
	int aggro;		//仇恨的大小
	int aggro_type;		//仇恨的类型
	int faction;		//对方所属的派系
	int level;		//对方的级别
};

struct msg_watching_t
{
	int faction;		//源的派系
	int invisible_rate;	//隐身能力
	int family_id;		//家族id
	int mafia_id;		//帮派id
	int zone_id;		//服务器id
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
	XID attacker;	//攻击者
	int rage;	
	int faction;	//发送者的派系
	int target_faction;	//目标的接受求救类型
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
	int eliminate_tool;	//消耗工具的ID
};

struct gather_result
{
	int amount;
	int task_id;
	int eliminate_tool;		//如果删除物品则附加此ID
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
	bool bpKilled;		//是否正常杀死，如是，则应进行PK值和其他方面的修正
	bool noDrop;		//是否不掉落死亡(帮战规则才会如此) 
	int mafia_id;		//被杀死者的帮派 用于帮战时
	int pkvalue;		//被杀死者的PKVALUE
	
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
	unsigned char reason;	//0 - 主动请要  1 - 被踢
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

