#ifndef __ONLINEGAME_GS_BF_WORLD_MANAGER_H__ 
#define __ONLINEGAME_GS_BF_WORLD_MANAGER_H__

#include "../global_manager.h"
#include "../world.h"
#include "../usermsg.h"

struct battle_field_info;
class bf_player_imp;
#define PLAYER_CONTRIBUTION_ONE_PAGE_COUNT 10

struct bf_param
{
	int battle_id;
	int world_tag;
	int attacker_mafia_id;
	int defender_mafia_id;
	int attacker_assistant_mafia_id;
	int end_timestamp;
};

enum
{
	CONSTRUCTION_RES_KEY_A = 0,
	CONSTRUCTION_RES_KEY_B = 1,
	//现在是两种建设资源
	CONSTRUCTION_RES_COUNT,
	
	CONTRIBUTION_NUM_PER_MATERIAL = 10,
};

enum
{
	//为城战中所有需要存盘的数据做套索引
	CITY_STRUCTURE_ATTR_TYPE_TOWER = 0,
	CITY_STRUCTURE_ATTR_TYPE_DOOR = 1,
	CITY_STRUCTURE_ATTR_TYPE_COUNT,

	CITY_STRUCTURE_KEY_MIN = 1,
	CITY_STRUCTURE_KEY_MAX = 10,

	CITY_STRUCTURE_DEATH_INIT_HP = 100,
	CITY_STRUCTURE_MAX_HP = 6000000,
};

enum
{
	ATTACK		= 0,	//攻击等级
	DEFENCE		= 1,	//防御等级
	RANGE		= 2,	//射程等级
	AI_POLICY	= 3,	//ai等级
	MAX_HP		= 4,	//hp		就是当前血量
	ATTR_COUNT,
};

//城市建设度
struct city_construction_res
{
	size_t num;
};

//城市内建筑属性
struct city_structure_attr
{
	size_t type;
	struct attr_t
	{
		size_t level;
	};
	abase::hash_map<int,attr_t> am;
};

struct player_contribution
{
	//玩家贡献度
	struct player_contribution_node
	{
		int player_id;
		size_t contribute[CONSTRUCTION_RES_COUNT];

		player_contribution_node():player_id(0)
		{
			memset(contribute,0,sizeof(contribute));
		}

		player_contribution_node(int id):player_id(id)
		{
			memset(contribute,0,sizeof(contribute));
		}

		bool operator==(int id) const
		{
			return player_id == id;
		}

		int GetPlayerID() const { return player_id; }

		size_t GetContributionNum(size_t index) const
		{
			if(index >= CONSTRUCTION_RES_COUNT) return 0;
			return contribute[index];
		}

		void SetContributionNum(size_t index,size_t num)
		{
			if(index >= CONSTRUCTION_RES_COUNT) return;
			contribute[index] = num;
		}

		void AddContributionNum(size_t index,size_t add_num)
		{
			if(index >= CONSTRUCTION_RES_COUNT) return;
			size_t temp_num = contribute[index];
			temp_num += add_num;
			if(temp_num < contribute[index])
			{
				//加越界了
				temp_num = 0xFFFFFFFF;
			}
			contribute[index] = temp_num;
		}

		uint64_t GetContributionSum() const
		{
			uint64_t sum = 0;
			for(size_t i = 0;i < CONSTRUCTION_RES_COUNT;++i)
			{
				uint64_t temp = sum;
				temp += contribute[i];
				if(temp < sum)
				{
					//加越界了,其实不可能
					temp = ~0;
				}
				sum = temp;
			}
			return sum;
		}
	};
	
	typedef abase::vector<player_contribution_node,abase::fast_alloc<> > PLAYER_CONTRIBUTION_VECTOR;
	PLAYER_CONTRIBUTION_VECTOR player_contri_vec;

	void clear()
	{
		player_contri_vec.clear();
	}

	size_t size() const
	{
		return player_contri_vec.size();
	}

	static bool __NodeCompare(const player_contribution_node& lhs,const player_contribution_node& rhs)
	{
		return lhs.GetContributionSum() < rhs.GetContributionSum();
	}

	static bool __ContriCompare(const player_contribution_node& lhs,uint64_t contri_sum)
	{
		return lhs.GetContributionSum() >= contri_sum;
	}

	inline PLAYER_CONTRIBUTION_VECTOR::iterator Find(int player_id) const
	{
		return (PLAYER_CONTRIBUTION_VECTOR::iterator) std::find(player_contri_vec.begin(),player_contri_vec.end(),player_id);
	}

	inline PLAYER_CONTRIBUTION_VECTOR::iterator Find(uint64_t contri_sum) const
	{
		return (PLAYER_CONTRIBUTION_VECTOR::iterator) std::lower_bound(player_contri_vec.begin(),player_contri_vec.end(),contri_sum,__ContriCompare);
	}

	bool AddContribution(int player_id,size_t index,size_t add_num);
};

//gs关心的城市信息
//使用更改存盘的策略
//保存所有打包数据
//log更改信息
struct gs_city_detail
{
	size_t stamp;//存盘戳
	typedef abase::hash_map<int,city_construction_res> CONTRUCTION_RES_MAP;
	typedef abase::hash_map<int,city_structure_attr> STRUCTURE_ATTR_MAP;
	CONTRUCTION_RES_MAP ccrm;		//city construction resource map
	STRUCTURE_ATTR_MAP csam;		//city structure attr map
	player_contribution contribution;
	int service_npc_born_timestamp;

	void clear()
	{
		ccrm.clear();
		csam.clear();
		contribution.clear();
		service_npc_born_timestamp = 0;
	}
};

struct battlefield_consturct_temp
{
	int tid;
	int attack_max_lev;		//攻击力等级上限
	struct
	{
		int attack_value;	//攻击力等级1-20的攻击力
		int attack_extra;	//攻击力等级1-20的附加伤害
	}attack_level[20];
	int attack_war_material;	//建设攻击力等级消耗城防物资种类
	float attack_co[3];		//建设攻击力等级消耗城防物资值系数a,b,c
	int defence_max_lev;		//防御力等级上限
	struct
	{
		int defence_value;	//防御力等级1-20的防御力
		int defence_extra;	//防御力等级1-20的减免附加伤害
	}defence_level[20];
	int defence_war_material;	//建设防御力等级消耗城防物资种类
	float defence_co[3];		//建设防御力等级消耗城防物资值系数a,b,c
	int range_max_lev;		//射程等级上限
	int range_values[5];		//射程等级1-5
	int range_war_material;		//建设射程等级消耗城防物资种类
	float range_co[3];		//建设射程等级消耗城防物资值系数a,b,c
	int strategy_max_lev;		//策略等级上限
	int strategy_id[5];		//策略等级1-5
	int strategy_war_material;	//建设策略等级消耗城防物资种类
	float strategy_co[3];		//建设策略等级消耗城防物资值系数a,b,c
	int init_hp;			//初始hp,只在第一次使用
	int hp_war_material;		//建设HP消耗城防物资种类
	int lvlup_hp;			//HP每次建设提高的点数
	int hp_material_num;		//每次建设HP所需城防物资值
};

class bf_world_manager_base : public global_world_manager
{
	typedef abase::hashtab<battlefield_consturct_temp,int,abase::_hash_function> MAP;
	MAP _bfc_map;

	bool __InsertTemplate(const battlefield_consturct_temp & bfct)
	{
		return _bfc_map.put(bfct.tid,bfct);
	}

	const battlefield_consturct_temp * __GetTemplate(int tid)
	{
		return _bfc_map.nGet(tid);
	}

	static bf_world_manager_base& __GetInstance()
	{
		static bf_world_manager_base __Singleton;
		return __Singleton;
	}

public:
	static bool Insert(const battlefield_consturct_temp & bfct)
	{
		bool rst = __GetInstance().__InsertTemplate(bfct);
		ASSERT(rst);
		return rst;
	}

	static const battlefield_consturct_temp* Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman& dataman);

	typedef std::map<int,int> ConstuctKeyTidMap;
	ConstuctKeyTidMap _key_tid_map;
	struct structure_info
	{
		int tid;
		unsigned char status;
		A3DVECTOR pos;
		int npc_tid;
		float hp_factor;
	};
	typedef std::map<int,structure_info> StructureInfoKeyMap;
	StructureInfoKeyMap _key_map;

private:
	bool Save(raw_wrapper& ar);
	bool Load(raw_wrapper& ar);
public:
	enum CREATE_BATTLEFIELD_RESULT
	{
		CBFR_INVALID_WORLD_TAG		= -1001,
		CBFR_NOT_BATTLE_SERVER		= -1002,
		CBFR_INVALID_STATUS		= -1003,
		CBFR_INVALID_BATTLE_ID		= -1004,
		CBFR_GET_DB_DATA_ERROR		= -1005,
		CBFR_GET_DB_TIME_OUT		= -1006,
		CBFR_LAST_TIME_TOO_SHORT	= -1007,
	};
	
	enum ROLE_IN_WAR
	{
		ROLE_IN_WAR_NULL		= 0,	//无
		ROLE_IN_WAR_1			= 1,	//中心建筑
		ROLE_IN_WAR_2			= 2,	//炮塔
		ROLE_IN_WAR_3			= 3,	//箭塔
		ROLE_IN_WAR_TURRET		= 4,	//投石车
		ROLE_IN_WAR_5			= 5,	//传送点
		ROLE_IN_WAR_REVIVE_POS		= 6,	//复活点
		ROLE_IN_WAR_7			= 7,	//服务NPC
		ROLE_IN_WAR_8			= 8,	//占领标志物
		ROLE_IN_WAR_ENTRY_POS		= 9,	//进入点
		ROLE_IN_WAR_KEY_BUILDING	= 10,	//标志性建筑(做胜利条件)
		ROLE_IN_WAR_NORMAL_BUILDING	= 11,	//普通建筑
	};

	enum
	{
		//踢出时间,注意 战场关闭的时间需要比最大踢出时间大一些
		DEFAULT_KICKOUT_TIME_MIN =		5,
		DEFAULT_KICKOUT_TIME_MAX =		10,
		DEFAULT_KICKOUT_CLOSE_TIME =		20,

		DEFAULT_PREPARE_TIME =			600,		//开始前准备时间
		DEFAULT_MAX_LAST_TIME =			3600 * 6,	//持续时间

		BATTLEFIELD_BROADCAST_ID =		30,		//城战喊话id

		//保证battle的tag是连续的区间
		BATTLE_WORLD_TAG_BEGIN =		201,		//战场地图号起始
		//控制器的问题
		CTRL_CONDISION_ID_BATTLE_MIN =		70000,		//最小战场控制器id
		//...
		CTRL_CONDISION_ID_BATTLE_MAX =		70500,		//最大战场控制器id,要2000个才够用,晕死
		CTRL_CONDISION_ID_BATTLE_OFFSET = CTRL_CONDISION_ID_BATTLE_MAX - CTRL_CONDISION_ID_BATTLE_MIN + 1,
		//使用这个基值来计算出实际在战场中使用的id
		CTRL_ID_BATTLE_BASE =			90000,		//基值

		MAX_ENTRY_POS_COUNT =			1,		//最多进入点
		MAX_AUTO_TASK_ID =			5,		//最多任务
		MAX_RES_COUNT =				8,		//最多资源

		DEFAULT_RETRY_COUNT =			100,
		BATTLE_PREPARE2_CTRL_ID	=		70468,
		CONSTURCT_PREPARE2_CTRL_ID =		70467,
		SERVICE_NPC_CTRL_ID =			70499,
	};
	
	enum BATTLE_RESULT
	{
		BR_NONE			= -1,	//无结果
		BR_DRAW			= 0,	//平手
		BR_ATTACKER_WIN		= 1,	//攻方获胜
		BR_DEFENDER_WIN		= 2,	//守方获胜
	};

	enum
	{
		BATTLE_PVP = 0,
		BATTLE_PVE = 1,
	};

	enum PLAY_TYPE
	{
		PT_KILL_NPC		= 0,
		PT_GAIN_RESOURCE	= 1,
		PT_DEFENCE		= 2,
	};

	enum BATTLE_FACTION
	{
		BF_NONE			= 0,	//无阵营
		BF_ATTACKER		= 1,	//攻击方
		BF_DEFENDER		= 2,	//防守方
		BF_ATTACKER_ASSISTANT	= 3,
	};

	enum BATTLE_STATUS
	{
		BS_READY		= 0,	//服务器准备完毕等待delivery命令
		BS_PREPARING		= 1,	//战斗准备状态
		BS_RUNNING		= 2,	//战斗开始状态
		BS_CLOSING		= 3,	//战斗关闭,清理怪物等操作进行中
	};

	enum BATTLE_INDEX
	{
		BI_ATTACKER		= 0,
		BI_DEFENDER		= 1,
		BI_ATTACKER_ASSISTANT	= 2,
		BI_MAX			= 3,
	};

	struct player_battle_info
	{
		player_battle_info():level(0),battle_faction(BF_NONE),kill(0),death(0)
		{
		}
		player_battle_info(int l,int bf,int k,int d):
			level(l),battle_faction(bf),kill(k),death(d)
		{
		}
		int level;
		int battle_faction;
		int kill;
		int death;
	};

protected:
	int _battle_id;			//战场id,内部唯一标识
	
	int _active_ctrl_id_prepare;	//战场准备开启的控制器id
	int _active_ctrl_id_prepare2;	//战场准备开启的控制器id
	int _active_ctrl_id_start;	//战场开始时开启的开启的控制器id
	int _active_ctrl_id_end;	//战场结束时开启的控制器id

	typedef std::map<int,player_battle_info> PlayerInfoMap;
	typedef std::map<int,player_battle_info>::iterator PlayerInfoIt;

	A3DVECTOR invalid_first_area_min;		//全局非法区域1
	A3DVECTOR invalid_first_area_max;
	A3DVECTOR invalid_second_area_min;		//全局非法区域2
	A3DVECTOR invalid_second_area_max;
	//单方数据
	struct competitor_data
	{
		struct revive_pos_t
		{
			A3DVECTOR pos;
			bool active;
		};
		typedef std::map<int,std::vector<revive_pos_t> > CtrlReviveMap;
		typedef std::map<int,std::vector<revive_pos_t> >::iterator CtrlReviveMapIt;
		
		PlayerInfoMap player_info_map;
		cs_user_map player_list;
		int max_player_limit;				//最大人数限制
		int cur_player_count;				//当前人数
		abase::vector<A3DVECTOR> town_list;		//回城点
		abase::vector<A3DVECTOR> entry_list;		//进入点
		CtrlReviveMap revive_map;			//复活点
		int win_res_point;				//资源类剧本胜利点数：int
		struct
		{
			int res_tid;				//资源类剧本资源,矿物ID
			int res_inc_speed;			//资源8积攒速�?点/10秒
		}res_info[MAX_RES_COUNT];

		int reward_win_tid;				//胜利奖励物品id
		int reward_lose_tid;				//失败奖励物品id
		int reward_draw_tid;				//平局奖励物品id
		int mafia_id;					//帮派id
		std::set<int>	_key_npc_list;
		A3DVECTOR prepare_valid_area_min;		//准备时合法区域
		A3DVECTOR prepare_valid_area_max;
	};
	competitor_data _competitor[BI_MAX];
	cs_user_map  _all_list;

	int _kickout_time_min;
	int _kickout_time_max;
	int _kickout_close_time;

	int _cur_timestamp;		//流程使用时间戳
	int _end_timestamp;		//战争结束时间戳
	int _heartbeat_counter;		//心跳计时
	int _battle_info_seq;		//发送信息序列
	int _status;			//战争状态
	int _battle_result;
	int _battle_lock;

	static int _city_detail_lock;
	static gs_city_detail _city_detail;	//城市信息
	int _get_counter;		//读取重试次数
	int _put_counter;		//写入重试次数

	std::map<int,bool> _service_ctrl_id_map;

	//加锁顺序 _battle_lock _city_detail_lock
public:
	bf_world_manager_base();
	virtual ~bf_world_manager_base();

        virtual void AdjustNPCAttrInBattlefield(gnpc_imp* pImp,ai_param& aip,int ai_cid);
	int* GetBattleLock() { return &_battle_lock; }

	//注册回城点
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction);
	//注册复活点
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id);
	//注册进入点
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	//初始化
	virtual int Init(const char* gmconf_file,const char* servername, int tag, int index);
	//Deliliver连接
	virtual void OnDeliveryConnected(std::vector<battle_field_info>& info);
	//战场开启
	virtual int CreateBattleField(const bf_param& param,bool force);
	//战场重置
	virtual void Reset();
	//注册建筑
	virtual void RecordBattleBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building = false ) {}
	//注册旗子
	virtual void RecordFlag(const A3DVECTOR& pos,int faction,int tid,bool is_key = false);
	//注册矿
	virtual void RecordMine(const A3DVECTOR& pos,int faction,int tid,bool is_key = false);
	//战场阵营喊话
	virtual void BattleFactionSay( int faction ,const void* buf, size_t size, int channel = 0, const void *aux_data = 0, size_t dsize =0, int char_emote =0, int self_id =0);

	//战场喊话
	virtual void BattleSay(const void* buf, size_t size );
	//战场聊天
	virtual void BattleChat(int faction,const void* buf, size_t size,int channel,int self_id);

	bool SendBattleFieldConstructionInfo(bf_player_imp* pImp);
	bool SendBattleFieldContributionInfo(bf_player_imp* pImp,int page);
	virtual bool QueryBattlefieldConstructInfo(bf_player_imp* pImp,int type,int page);
	virtual bool BattlefieldContribute(bf_player_imp* pImp,int res_type);
	virtual bool BattlefieldConstruct(bf_player_imp* pImp,int key,int type,int cur_level);
	void SendBattlefieldInfo(bf_player_imp* pImp);

public:
	virtual void OnActiveSpawn(int id, bool active);
	virtual void Heartbeat();
	virtual void PostPlayerLogin(gplayer* pPlayer);
	virtual int OnPlayerLogin(const GDB::base_info* pInfo,const GDB::vecdata* data,bool is_gm);
	virtual int OnMobDeath(world * pPlane,int faction,int tid,const A3DVECTOR& pos, int attacker_id) { return 0; }
	virtual void PlayerEnter(gplayer* pPlayer,int battle_faction);
	virtual void PlayerLeave(gplayer* pPlayer,int battle_faction);

	//获得回城点
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	//获得复活点
	//获取某方复生点,攻,守,争夺,索引
	virtual bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag);
	//获得随机进入点
	void GetRandomEntryPos(A3DVECTOR& pos,int battle_faction);

	void ChangeBattleStatus(int status); 
	virtual void OnPrepare();
	virtual void OnRun();
	virtual void OnClose();
	virtual void OnReady();
public:
	//战场map返回true
	inline bool IsBattleWorld() { return false; }
	inline bool IsBattleFieldWorld() { return true; }
	inline int GetBattleID() { return _battle_id; }//init后就不变了,不用加锁
	inline int GetBattleResult() { return _battle_result; }
	inline int GetBattleStatus() { return _status; }
	inline bool IsBattleStart() { return (BS_PREPARING == _status || BS_RUNNING == _status); }
        inline bool IsBattlePreparing() { return BS_PREPARING == _status; }
	inline int GetBattleEndTimestamp() { return _end_timestamp; }
	inline int GetKickoutTimeMin() { return _kickout_time_min; }
	inline int GetKickoutTimeMax() { return _kickout_time_max; }
	inline int GetKickoutCloseTime() { return _kickout_close_time; } 

	inline int GetMafiaID(int battle_index)
	{
		if(battle_index < BI_ATTACKER || battle_index >= BI_MAX) return 0;
		return _competitor[battle_index].mafia_id;
	}

	bool CheckInvalidPos(const A3DVECTOR& pos)
	{
		if((pos.x - invalid_first_area_min.x) * (pos.x - invalid_first_area_max.x) < 0 &&
			(pos.y - invalid_first_area_min.y) * (pos.y - invalid_first_area_max.y) < 0 &&
			(pos.z - invalid_first_area_min.z) * (pos.z - invalid_first_area_max.z) < 0)
		{
			return true;
		}
		if((pos.x - invalid_second_area_min.x) * (pos.x - invalid_second_area_max.x) < 0 &&
			(pos.y - invalid_second_area_min.y) * (pos.y - invalid_second_area_max.y) < 0 &&
			(pos.z - invalid_second_area_min.z) * (pos.z - invalid_second_area_max.z) < 0)
		{
			return true;
		}
		return false;
	}

	bool CheckValidPreparePos(int battle_faction,const A3DVECTOR pos)
	{       
		A3DVECTOR pos1,pos2;                            
		if(BF_ATTACKER == battle_faction)               
		{
			pos1 = _competitor[BI_ATTACKER].prepare_valid_area_min;
			pos2 = _competitor[BI_ATTACKER].prepare_valid_area_max;
		}
		else if(BF_DEFENDER == battle_faction)
		{
			pos1 = _competitor[BI_DEFENDER].prepare_valid_area_min;
			pos2 = _competitor[BI_DEFENDER].prepare_valid_area_max;
		}
		else if(BF_ATTACKER_ASSISTANT == battle_faction)               
		{
			pos1 = _competitor[BI_ATTACKER].prepare_valid_area_min;
			pos2 = _competitor[BI_ATTACKER].prepare_valid_area_max;
		}
		if((pos.x - pos1.x) * (pos.x - pos2.x) > 0 ||
				(pos.y - pos1.y) * (pos.y - pos2.y) > 0 ||
				(pos.z - pos1.z) * (pos.z - pos2.z) > 0)
		{
			return false;
		}
		return true;            
	}

	inline int GetGetCounter() { return _get_counter; }
	inline int GetPutCounter() { return _put_counter; }

public:
	virtual bool IsUseForBattle() { return false; }
	bool IsBattleInfoRefresh(int& info_seq);

public:
	int TranslateCtrlID(int which);
	virtual bool CheckServiceCtrlID(int ctrl_id);
	void GetAutoTaskID(int battle_faction,int* pTaskID);
	void KillAllMonsters();
	
protected:
	void SendBattleInfo() { ++_battle_info_seq; }
	void SendBattleEnd(int result)
	{
		packet_wrapper h1(64);
		using namespace S2C;
		CMD::Make<CMD::battlefield_end>::From(h1,result);
		if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
	}
	void DumpWorldMsg();
	bool BufToGSCityDetail(const void* buf,size_t size);
	size_t CalcCityDetailBufSize();
	void GSCityDetailToBuf(void* buf,size_t& size);

public:
	void GetCityDetail();
	void PutCityDetail();
	virtual void OnGetCityDetail(int ret_code,int stamp,const void* buf = NULL,size_t size = 0);
	virtual void OnPutCityDetail(int ret_code);

private:
	bool AddPlayer(int battle_index)
	{
		if(_competitor[battle_index].cur_player_count >= _competitor[battle_index].max_player_limit) return false;
		int p = interlocked_increment(&_competitor[battle_index].cur_player_count);
		if(p > _competitor[battle_index].max_player_limit)
		{
			interlocked_decrement(&_competitor[battle_index].cur_player_count);
			return false;
		}
		return true;
	}

	void DelPlayer(int battle_index)
	{
		interlocked_decrement(&_competitor[battle_index].cur_player_count);
	}

	inline void AddMapNode(cs_user_map& map,gplayer* pPlayer)
	{
		int link_index = pPlayer->cs_index;
		std::pair<int,int> val(pPlayer->ID.id,pPlayer->cs_sid);
		if(link_index >= 0 && val.first >= 0)
		{
			map[link_index].push_back(val);
		}
	}

	inline void DelMapNode(cs_user_map& map,gplayer* pPlayer)
	{
		int link_index = pPlayer->cs_index;
		std::pair<int,int> val( pPlayer->ID.id, pPlayer->cs_sid);
		if(link_index >= 0 && val.first >= 0)
		{
			cs_user_list& list = map[link_index];
			int id =  pPlayer->ID.id;
			size_t i = 0;
			for(;i < list.size();++i)
			{
				if(id == list[i].first)
				{
					list.erase(list.begin() + i);
					--i;
				}
			}
		}
	}

public:
	static global_world_manager* new_bf_world_manager(const char* gmconf_file,const char* servername);
};

//城战类型,杀关键怪物的
//胜利条件:杀死
class bf_world_manager_battle : public bf_world_manager_base
{
	
public:
	bf_world_manager_battle();
	virtual ~bf_world_manager_battle();

	virtual int Init(const char* gmconf_file,const char* servername);
	virtual void Reset();

	virtual void RecordBattleBuilding(const A3DVECTOR& pos,int faction,int tid,bool is_key_building);
	virtual int OnMobDeath(world * pPlane,int faction,int tid,const A3DVECTOR& pos, int attacker_id);
	
public:
	virtual bool IsUseForBattle() { return true; }
	virtual void OnClose();
	virtual void OnBuyArcher(gplayer_imp* pImp);
        virtual void OnBattleNPCNotifyHP(int faction,int tid,int cur_hp,int max_hp);
};

//城战类型,杀关键怪物的
//胜利条件:杀死
class bf_world_manager_construction : public bf_world_manager_base
{
	bool _service_npc_borned;
	
public:
	bf_world_manager_construction();
	virtual ~bf_world_manager_construction();

	virtual int Init(const char* gmconf_file,const char* servername);
	virtual int CreateBattleField(const bf_param& param,bool force);
	virtual void Reset();
	virtual void OnDeliveryConnected(std::vector<battle_field_info>& info);

public:
	virtual void Heartbeat();
	virtual void OnPrepare();
	virtual void OnClose();
};

#endif

