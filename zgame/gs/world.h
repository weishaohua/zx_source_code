#ifndef __ONLINEGAME_GS_WORLD_H__
#define __ONLINEGAME_GS_WORLD_H__

#include <map>
#include <vector>
#include <hashtab.h>
#include <amemory.h>
#include <timer.h>
#include <threadpool.h>
#include <common/types.h>
#include <set>
#include <glog.h>

#include "gimp.h"
#include "grid.h"
#include "msgqueue.h"
#include "terrain.h"
#include "template/itemdataman.h"
#include "worldmanager.h"
#include "npcgenerator.h"
#include "gmatrix.h"

extern abase::timer	g_timer;
class MsgDispatcher;
class GSvrPool;
class CNPCGenMan;
/*
 *	NPC和物品的ID转换函数
 */
inline int ID2WIDX(int id) { return 0;}
inline int ID2IDX(int id) { return id & 0x0FFFFFFF; }
inline int MKOBJID(int worldid,int idx)
{
	int tmp = (idx & 0x0FFFFFFF);
	return tmp;
}

inline void MAKE_ID(XID & id, int u_id)
{
	id.id = u_id;
	if((u_id & 0x80000000) == 0)
	{
		id.type = GM_TYPE_PLAYER;
	}
	else if((u_id & 0x40000000) == 0)
	{
		id.type = GM_TYPE_NPC;
	}
	else if((u_id & 0x20000000) == 0)
	{
		id.type = GM_TYPE_MATTER;
	}
	else if((u_id & 0x10000000) == 0) //分身
	{
		id.type = GM_TYPE_NPC;
	}
}

//通过attack message 专用ID转换， 避免的宠物的判定 现在这个判定就可以使用
inline bool IS_HUMANSIDE(const XID & id) { return id.type == GM_TYPE_PLAYER;}

class gclonenpc;

template<typename T> inline int MERGE_ID(int oldid);
template<> inline int MERGE_ID<gnpc>(int oldid) { return oldid | 0x80000000;}
template<> inline int MERGE_ID<gclonenpc>(int oldid) { return oldid | 0xE0000000;}
template<> inline int MERGE_ID<gmatter>(int oldid) { return oldid | 0xC0000000;}
template<> inline int MERGE_ID<gplayer>(int oldid) { return oldid;}

class  world_data_ctrl
{
//这个类用于世界的数据控制，会添加一系列的虚函数
	public:
	virtual ~world_data_ctrl() {}
	virtual world_data_ctrl * Clone() = 0;
	virtual void Reset() = 0;
	virtual void Tick(world * pPlane) = 0;
	virtual void BattleFactionSay(int faction ,const void * buf, size_t size) = 0;
	virtual void BattleSay(const void * buf, size_t size) = 0;
};

class world
{
	typedef abase::hashtab<int,int,abase::_hash_function,abase::fast_alloc<> >	query_map;//用户的查询表

	grid 	w_grid;
	int 	w_index;	//世界区域的索引
	int     w_tag;      //世界Tag
	query_map w_player_map;	//玩家的查找表
//	query_map w_npc_map;	//在本地的外部npc查询表
	int	w_pmap_lock;	//player 的userid -> index id的map锁
	int	w_nmap_lock;	//npc 外部npc-->本地连接的查询表
	int 	w_message_counter[GM_MSG_MAX];	//消息数目的计数表
	npc_generator 	w_npc_gen; //怪物生成管理器
public:
	struct off_node_t{
		int idx_off;
		int x_off;
		int z_off;
		off_node_t(grid & grid,int offset_x,int offset_y):x_off(offset_x),z_off(offset_y)
		{
			idx_off = offset_y*grid.reg_column + offset_x;
		}
		bool operator==(const off_node_t & rhs) const 
		{
			return rhs.idx_off == idx_off;
		}
	};
	abase::vector<off_node_t,abase::fast_alloc<> > w_off_list;
	int	w_near_vision;		//暂时没有使用，近点的索引
	int	w_far_vision;		//最远的距离所涵盖的范围
	int	w_true_vision;		//完全可视的范围,暂时没有使用
	int	w_plane_index;		//在世界中的位面索引
	int	w_player_count;		//本世界中玩家的数目
	float	w_vision;		//视野范围，和far_vision对应的距离
	world_manager * w_world_man;
	int 	w_obsolete;		//由manager使用这个变量
	instance_hash_key w_ins_key;	//副本使用的instance_key ， 由manager使用这个变量
	int	w_activestate;		//激活状态 0:未激活 1:激活 2:冷却  由manager来控制
	int	w_index_in_man;		//在管理器中的索引，为MsgQueue2和manager所使用
	int	w_create_timestamp;	//创建的时间戳，由manager使用 
	int	w_destroy_timestamp;	//删除的时间戳，由manager使用 不是所有的副本这个都有效的
	int	w_ins_kick;		//是否踢出instance key不符合的玩家
	int	w_battle_result;	//给战场用的 战场结果
	int	w_offense_goal;		//战场攻方目标
	int	w_offense_cur_score;	//战场攻方得分
	int	w_defence_goal;		//战场守方目标
	int	w_defence_cur_score;	//战场守方得分
	int 	w_end_timestamp; 	//战场结束时间，到此时间时，所有玩家都将被自动踢出 
					//只有batle_result有效后,此值才会被使用

public:
//初始化函数
	world();
	~world();
	bool 	Init(int world_index, int world_tag);
	void 	InitManager(world_manager * man) { w_world_man = man;}
	bool 	InitNPCGenerator(CNPCGenMan & npcgen);
	bool	TriggerSpawn(int condition);
	bool 	ClearSpawn(int condition);
	void 	InitTimerTick();
	bool 	CreateGrid(int row,int column,float step,float startX,float startY);
	int	BuildSliceMask(float near,float far);			//创建距离查询所需要的mask

	void 	DuplicateWorld(world * dest) const;	//复制世界，除了NPC生成器 .....
	
	inline world_manager * GetWorldManager() { return w_world_man;}
	inline int GetTag() { return w_tag; }
	inline int GetWorldIndex() { return w_index; }
	//分配一个NPC数据，返回一个上了锁了NPC结构
	inline gnpc 	*AllocNPC() 
	{ 
		gnpc *pNPC = gmatrix::AllocNPC(); 
		if(pNPC) pNPC->plane = this;
		return pNPC;
	}
	inline void 	FreeNPC(gnpc* pNPC) 
	{ 
		ASSERT(pNPC->plane == this);
		pNPC->plane = NULL;
		return gmatrix::FreeNPC(pNPC); 
	}

	inline bool CheckPlayerDropCondition()
	{
		return gmatrix::CheckPlayerDropCondition();
	}

	//分配一个Matter数据，返回一个上了锁的Matter结构
	inline gmatter *AllocMatter() 
	{ 
		gmatter * pMatter = gmatrix::AllocMatter(); 
		 if(pMatter) pMatter->plane = this;
		return pMatter;
	}
	inline void 	FreeMatter(gmatter *pMatter) 
	{
		ASSERT(pMatter->plane == this);
		pMatter->plane = NULL;
		return gmatrix::FreeMatter(pMatter); 
	}

	//	增加/删除玩家和对象的函数
	//分配一个玩家数据，并返回一个锁定的玩家结构
	inline gplayer *AllocPlayer() 
	{ 
		gplayer * pPlayer = gmatrix::AllocPlayer(); 
		if(pPlayer) 
		{
			pPlayer->plane = this;
			interlocked_increment(&w_player_count);
		}
		return pPlayer;
	}

	inline void 	FreePlayer(gplayer * pPlayer)
	{
		if(w_world_man)
		{
			w_world_man->PlayerLeaveThisWorld(w_plane_index,pPlayer->ID.id);
		}
		interlocked_decrement(&w_player_count);
		ASSERT(pPlayer->plane == this);
		pPlayer->plane = NULL;
		return gmatrix::FreePlayer(pPlayer);
	}

	inline int GetPlayerInWorld() 
	{
		return w_player_count; 
	}

	inline void InsertPlayerToMan(gplayer *pPlayer) 
	{ 
		gmatrix::InsertPlayerToMan(pPlayer);
	}
	inline void RemovePlayerToMan(gplayer *pPlayer) 
	{ 	
		gmatrix::RemovePlayerToMan(pPlayer);
	}

	int InsertPlayer(gplayer *);		//根据位置，插入一个对象到世界中，返回插入的区域索引号
	int InsertNPC(gnpc*);			//根据位置，插入一个对象到世界中，返回插入的区域索引号
	int InsertMatter(gmatter *);		//根据位置，插入一个对象到世界中，返回插入的区域索引号
	
	void RemovePlayer(gplayer *pPlayer); 	//从世界中移出一个对象，不free
	void RemoveNPC(gnpc *pNPC);		//从世界中移出一个对象，不free
	void RemoveMatter(gmatter *pMatter);	//从世界中移出一个对象，不free

	//从管理器中移出NPC，用于不在场景中的npc
	inline void RemoveNPCFromMan(gnpc * pNPC)
	{
		gmatrix::RemoveNPCFromMan(pNPC);
	}

	inline void RemoveMatterFromMan(gmatter * pMatter)
	{
		gmatrix::RemoveMatterFromMan(pMatter);
	}

	void Release();

public:
//	取得属性的inline函数
	inline gmatter * GetMatterByIndex(size_t index) const  { return gmatrix::GetMatterByIndex(index);}
	inline gplayer*  GetPlayerByIndex(size_t index) const   {return gmatrix::GetPlayerByIndex(index);}
	inline gnpc* 	 GetNPCByIndex(size_t index) const   { return gmatrix::GetNPCByIndex(index);}
	inline size_t GetPlayerIndex(gplayer *pPlayer)  const  { return gmatrix::GetPlayerIndex(pPlayer);}
	inline size_t GetMatterIndex(gmatter *pMatter)  const  { return gmatrix::GetMatterIndex(pMatter);}
	inline size_t GetNPCIndex(gnpc *pNPC)  const  { return gmatrix::GetNPCIndex(pNPC);}
	inline grid&	 GetGrid() { return w_grid;}
	inline const rect & GetLocalWorld() { return w_grid.local_region;}
	inline bool PosInWorld(const A3DVECTOR & pos)
	{
		return w_grid.IsLocal(pos.x,pos.z);
	}

	inline bool MapPlayer(int uid,int index) { 
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.put(uid,index);
	}
	
	inline int UnmapPlayer(int uid) {
		spin_autolock alock(w_pmap_lock);
		return w_player_map.erase(uid);
	}

	inline int FindPlayerIndex(int uid)
	{
		return gmatrix::FindPlayer(uid);
	}
	
	inline gplayer *  GetPlayerByID(int uid) {
		int index = gmatrix::FindPlayer(uid);
		if(index < 0) return NULL;
		gplayer * pPlayer = GetPlayerByIndex(index);
		if(pPlayer->plane == this) 
			return pPlayer;
		else 
			return NULL;
	}

	inline int GetPlayerCount()
	{
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.size();
	}

	//
	enum
	{
		QUERY_OBJECT_STATE_ACTIVE = 0x01,
		QUERY_OBJECT_STATE_ZOMBIE = 0x02,
		QUERY_OBJECT_STATE_DISCONNECT = 0x04,
	};
	struct object_info
	{
		int state;
		int tag;
		A3DVECTOR pos;
		float body_size;
		short cls;
		short gender;
		int faction;
		int level;
		int hp;
		int mp;
		int tid;	//npc or matter
		int invisible_rate;
		int anti_invisible_rate;
	};

	bool QueryObject(const XID & id,object_info & info,bool other_plane = false);	//查询一个其他对象的状态
	
	void GetColumnPlayers(std::vector<XID>& player_list, const A3DVECTOR& src, const A3DVECTOR& target, int self, float fRadius, float height);


public:
	void RunTick();		//由manager控制调用或者内部自动调用
	void ResetWorld();	//重置世界，只有副本才会调用这个（这个操作会重生所有的怪物等）
	void DumpMessageCount();

	void SetWorldCtrl(world_data_ctrl * ctrl);
	world_data_ctrl * w_ctrl;
private:
	friend class MsgQueue;

	int HandlePlaneMessage(const MSG & msg);
public:


	/*
	 *	广播消息，按照距离将包转发给周围的所有对�
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastMessage(const MSG & message,float fRadius,int mask); 		

	/*
	 *	广播盒形消息，按照距离将包转发给周围的所有对象
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastLocalBoxMessage(const MSG & message,const rect & rt);

	/*
	 *	广播球形消息，按照距离将包转发给周围的所有对象
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象(已经被取消)
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	广播柱形消息，按照距离将包转发给周围的所有对象,该对象必须在柱中
	 *	柱的起始坐标在message中，终点是target
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	广播椎形消息，在椎中的对象会收到这个消息
	 *	圆锥的圆心即为消息的发出点
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t max_count,std::vector<exclude_target>& targets_exclude);


	/*
	 *	同BroadcastMessage，唯一的区别是只在本地做相应的转发操作
	 *	BroadcastMessage的本地发送是通过调用本函数完成的
	 */
	int BroadcastLocalMessage(const MSG & message,float fRadius,int mask);

	/*
	 *	广播球形消息，按照距离将包转发给周围的所有对象
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastLocalSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	广播柱形消息，按照距离将包转发给周围的所有对象,该对象必须在柱中
	 *	柱的起始坐标在message中，终点是target
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastLocalCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	广播椎形消息，在椎中的对象会收到这个消息
	 *	圆锥的圆心即为消息的发出点
	 *	msg.target 的类型决定了收到者，msg.target的ID必须为-1
	 *	mask用于过滤消息接收对象
	 *	在座这个广播时，会自动判断是否要转发到其他的服务器上
	 *	msg.source不会收到这个广播消息
	 */
	int BroadcastLocalTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t max_count,std::vector<exclude_target>& targets_exclude);


	int DispatchPlaneMessage(const MSG & message);
	
	/*
	 *	检查一个坐标加值后是否仍然在正确范围内,内部使用的函数.
	 */
	inline static bool check_index(const grid * g,int x,int z, const world::off_node_t &node)
	{
		int nx = x + node.x_off;
		if(nx < 0 || nx >= g->reg_column) return false;
		int nz = z + node.z_off;
		if(nz < 0 || nz >= g->reg_row) return false;
		return true;
	}

	int GetRegionPlayerCount(int id, A3DVECTOR & pos, float & radius, std::vector<exclude_target>& targets_excludes);
	int GetRegionPlayer(abase::vector<gobject*,abase::fast_alloc<> > &list, int id, const A3DVECTOR & target, float & fRadius, std::vector<exclude_target>& targets_exclude);

	void BattleFactionSay(int faction, const void * msg, size_t size);
	void BattleSay(const void * msg, size_t size);


public:
//模板函数接口
	/*
	 *	当一个对象在两个格子间移动,判断该对象离开了哪些格子的视野,会调用相应的enter和leave函数对象
	 */
	template <typename ENTER,typename LEAVE>
	inline void MoveBetweenSlice(slice * pPiece, slice * pNewPiece,ENTER enter,LEAVE leave)
	{
		int i;
		grid * pGrid = &GetGrid();
		int ox,oy,nx,ny;
		pGrid->GetSlicePos(pPiece,ox,oy);
		pGrid->GetSlicePos(pNewPiece,nx,ny);
		float vision = w_vision + pGrid->slice_step - 1e-3;
		float dis = pNewPiece->Distance(pPiece);
		if(dis > vision)
		{
			//本格的无法看见 所以要进行离开本格的操作,后面的循环并没有判断本格
			leave(pPiece);
			enter(pNewPiece);
			if(dis > vision*2)
			{
				for(i = 0; i < w_far_vision; i ++)
				{
					const world::off_node_t &node = w_off_list[i];
					slice * pTmpPiece = pPiece + node.idx_off;
					leave(pTmpPiece);
				}

				for(i = 0; i < w_far_vision; i ++)
				{
					const world::off_node_t &node = w_off_list[i];
					slice * pTmpPiece = pNewPiece + node.idx_off;
					enter(pTmpPiece);
				}
				return ;
			}
		}

		for(i = 0; i < w_far_vision; i ++)
		{
			const world::off_node_t &node = w_off_list[i];
			if(check_index(pGrid,ox,oy,node)) 
			{
				slice * pTmpPiece = pPiece + node.idx_off;
				if(pTmpPiece->Distance(pNewPiece) > vision && pTmpPiece->IsInWorld())
				{
					leave(pTmpPiece);
					//离开了这个slice
				}
			}

			if(check_index(pGrid,nx,ny,node))
			{
				slice * pTmpPiece = pNewPiece + node.idx_off;
				if(pTmpPiece->Distance(pPiece) > vision && pTmpPiece->IsInWorld())
				{
					enter(pTmpPiece);
				}
			}
		}
	}

	/*
	 *	扫描附近所有的小格子,按照预定的范围来扫描,这里不进行格子是否在本服务器的判断 
	 */
	template <typename FUNC >
	inline void ForEachSlice(slice * pStart, FUNC func,int vlevel = 0)
	{
		int total = vlevel?w_near_vision:w_far_vision;
		int slice_x,slice_z;
		GetGrid().GetSlicePos(pStart,slice_x,slice_z);
		for(int i = 0; i <total; i ++)
		{
			off_node_t &node = w_off_list[i]; 
			int nx = slice_x + node.x_off; 
			int nz = slice_z + node.z_off; 
			if(nx < 0 || nz < 0 || nx >= GetGrid().reg_column || nz >= GetGrid().reg_row) continue;
			slice * pNewPiece = pStart+ node.idx_off;
			func(i,pNewPiece);
		}
	}

	/*
	 * 按照位置和范围扫描附近所有的小格,并且依次调用相应的处理函数对象
	 * 这里会首先判断格子是否在当前服务器中,否则不会发送到func函数中
	 */
	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos, float fRadius, FUNC func)
	{
		grid * pGrid = &GetGrid();
		float fx = pos.x - pGrid->grid_region.left;
		float fz = pos.z - pGrid->grid_region.top;
		float inv_step = pGrid->inv_step;
		int ofx1 = (int)((fx - fRadius) * inv_step);
		int ofx2 = (int)((fx + fRadius) * inv_step);
		int ofz1 = (int)((fz - fRadius) * inv_step);
		int ofz2 = (int)((fz + fRadius) * inv_step);
		if(ofx1 < 0) ofx1 = 0;
		if(ofx2 >= pGrid->reg_column) ofx2 = pGrid->reg_column -1;
		if(ofz1 < 0) ofz1 = 0;
		if(ofz2 >= pGrid->reg_row) ofz2 = pGrid->reg_row - 1;

		slice * pPiece = pGrid->GetSlice(ofx1,ofz1);
		for(int i = ofz1;i <= ofz2; i ++,pPiece += pGrid->reg_column)
		{
			slice * pStart = pPiece;
			for(int j = ofx1; j <= ofx2; j ++, pStart++)
			{
				if(pStart->IsInWorld()) func(pStart,pos);
			}
		}
	}

	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos, const rect & rt, FUNC func)
	{
		grid * pGrid = &GetGrid();
		
		float inv_step = pGrid->inv_step;
		int ofx1 = (int)((rt.left   - pGrid->grid_region.left) * inv_step);
		int ofx2 = (int)((rt.right  - pGrid->grid_region.left) * inv_step);
		int ofz1 = (int)((rt.top    - pGrid->grid_region.top ) * inv_step);
		int ofz2 = (int)((rt.bottom - pGrid->grid_region.top ) * inv_step);
		if(ofx1 < 0) ofx1 = 0;
		if(ofx2 >= pGrid->reg_column) ofx2 = pGrid->reg_column -1;
		if(ofz1 < 0) ofz1 = 0;
		if(ofz2 >= pGrid->reg_row) ofz2 = pGrid->reg_row - 1;

		slice * pPiece = pGrid->GetSlice(ofx1,ofz1);
		for(int i = ofz1;i <= ofz2; i ++,pPiece += pGrid->reg_column)
		{
			slice * pStart = pPiece;
			for(int j = ofx1; j <= ofx2; j ++, pStart++)
			{
				if(pStart->IsInWorld()) func(pStart,pos);
			}
		}
	}

	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos1, const A3DVECTOR & pos2, FUNC func)
	{
		rect rt(pos1,pos2);
		ForEachSlice(pos1,rt,func);
	}

	template <int foo>
		inline static void InspirePieceNPC(slice * pPiece,int tick)
		{
			int timestamp = pPiece->idle_timestamp;
			if(tick - timestamp < 40)
			{
				return;
			}
			pPiece->Lock();
			if(tick - pPiece->idle_timestamp < 40)	//由piece决定每两秒设置一次
			{
				pPiece->Unlock();
				return;
			}
			pPiece->idle_timestamp = tick;
			gnpc * pNPC = (gnpc*)(pPiece->npc_list);
			while(pNPC)
			{
				pNPC->idle_timer = NPC_IDLE_TIMER;
				pNPC = (gnpc*)(pNPC->pNext);
			}
			pPiece->Unlock();
		}
	/*
	 *	扫描附近所有的小格子,按照预定的范围来扫描,这里不进行格子是否在本服务器的判断 
	 */
	template <int foo>
	inline void InspireNPC(slice * pStart, int vlevel = 0)
	{
		int total = vlevel?w_near_vision:w_far_vision;
		int slice_x,slice_z;
		GetGrid().GetSlicePos(pStart,slice_x,slice_z);
		int tick = g_timer.get_tick();
		InspirePieceNPC<0>(pStart,tick);

		for(int i = 0; i <total; i ++)
		{
			off_node_t &node = w_off_list[i]; 
			int nx = slice_x + node.x_off; 
			int nz = slice_z + node.z_off; 
			if(nx < 0 || nz < 0 || nx >= GetGrid().reg_column || nz >= GetGrid().reg_row) continue;
			slice * pNewPiece = pStart+ node.idx_off;
			InspirePieceNPC<0>(pNewPiece,tick);
		}
	}

private:
	void CheckGSvrPoolUpdate();						//检查当前在线的游戏服务器列表
	void ConnectGSvr(int index, const char * ipaddr, const char * unixaddr);	//连接另外一台服务器 

	/*
	 * 这个类是定期刷新游戏服务器的类,它将作为线程池的一个任务来进行
	 * 生成它的地方是相关的定时器函数
	 */

	int _message_handle_count;				//记录当前message处理的嵌套 

private:
	//消息发送器的函数，这两个函数只有消息发送器连接上和断开时调用
};

template <typename WRAPPER>
inline int WrapObject(WRAPPER & wrapper,controller * ctrl, gobject_imp * imp, dispatcher * runner)
{
	ctrl->SaveInstance(wrapper);
	imp->SaveInstance(wrapper);
	runner->SaveInstance(wrapper);
	return 0;
}

template <typename WRAPPER,typename OBJECT>
inline int RestoreObject(WRAPPER & wrapper,OBJECT *obj,world * pPlane)
{
	controller * ctrl =NULL;
	gobject_imp * imp = NULL;
	dispatcher * runner = NULL;

	ctrl = substance::DynamicCast<controller>(substance::LoadInstance(wrapper));
	if(ctrl) imp = substance::DynamicCast<gobject_imp>(substance::LoadInstance(wrapper));
	if(imp) runner = substance::DynamicCast<dispatcher>(substance::LoadInstance(wrapper));
	if(!ctrl || !runner ||!imp) 
	{
		delete imp;
		delete ctrl;
		return -1;
	}
	obj->imp = imp;
	imp->_runner = runner;
	imp->_commander = ctrl;
	imp->Init(pPlane,obj);
	ctrl->Init(imp);
	runner->init(imp);
	//以后还需要调用ReInit
	return 0;
}


#endif

