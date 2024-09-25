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
 *	NPC����Ʒ��IDת������
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
	else if((u_id & 0x10000000) == 0) //����
	{
		id.type = GM_TYPE_NPC;
	}
}

//ͨ��attack message ר��IDת���� ����ĳ�����ж� ��������ж��Ϳ���ʹ��
inline bool IS_HUMANSIDE(const XID & id) { return id.type == GM_TYPE_PLAYER;}

class gclonenpc;

template<typename T> inline int MERGE_ID(int oldid);
template<> inline int MERGE_ID<gnpc>(int oldid) { return oldid | 0x80000000;}
template<> inline int MERGE_ID<gclonenpc>(int oldid) { return oldid | 0xE0000000;}
template<> inline int MERGE_ID<gmatter>(int oldid) { return oldid | 0xC0000000;}
template<> inline int MERGE_ID<gplayer>(int oldid) { return oldid;}

class  world_data_ctrl
{
//�����������������ݿ��ƣ������һϵ�е��麯��
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
	typedef abase::hashtab<int,int,abase::_hash_function,abase::fast_alloc<> >	query_map;//�û��Ĳ�ѯ��

	grid 	w_grid;
	int 	w_index;	//�������������
	int     w_tag;      //����Tag
	query_map w_player_map;	//��ҵĲ��ұ�
//	query_map w_npc_map;	//�ڱ��ص��ⲿnpc��ѯ��
	int	w_pmap_lock;	//player ��userid -> index id��map��
	int	w_nmap_lock;	//npc �ⲿnpc-->�������ӵĲ�ѯ��
	int 	w_message_counter[GM_MSG_MAX];	//��Ϣ��Ŀ�ļ�����
	npc_generator 	w_npc_gen; //�������ɹ�����
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
	int	w_near_vision;		//��ʱû��ʹ�ã����������
	int	w_far_vision;		//��Զ�ľ��������ǵķ�Χ
	int	w_true_vision;		//��ȫ���ӵķ�Χ,��ʱû��ʹ��
	int	w_plane_index;		//�������е�λ������
	int	w_player_count;		//����������ҵ���Ŀ
	float	w_vision;		//��Ұ��Χ����far_vision��Ӧ�ľ���
	world_manager * w_world_man;
	int 	w_obsolete;		//��managerʹ���������
	instance_hash_key w_ins_key;	//����ʹ�õ�instance_key �� ��managerʹ���������
	int	w_activestate;		//����״̬ 0:δ���� 1:���� 2:��ȴ  ��manager������
	int	w_index_in_man;		//�ڹ������е�������ΪMsgQueue2��manager��ʹ��
	int	w_create_timestamp;	//������ʱ�������managerʹ�� 
	int	w_destroy_timestamp;	//ɾ����ʱ�������managerʹ�� �������еĸ����������Ч��
	int	w_ins_kick;		//�Ƿ��߳�instance key�����ϵ����
	int	w_battle_result;	//��ս���õ� ս�����
	int	w_offense_goal;		//ս������Ŀ��
	int	w_offense_cur_score;	//ս�������÷�
	int	w_defence_goal;		//ս���ط�Ŀ��
	int	w_defence_cur_score;	//ս���ط��÷�
	int 	w_end_timestamp; 	//ս������ʱ�䣬����ʱ��ʱ��������Ҷ������Զ��߳� 
					//ֻ��batle_result��Ч��,��ֵ�Żᱻʹ��

public:
//��ʼ������
	world();
	~world();
	bool 	Init(int world_index, int world_tag);
	void 	InitManager(world_manager * man) { w_world_man = man;}
	bool 	InitNPCGenerator(CNPCGenMan & npcgen);
	bool	TriggerSpawn(int condition);
	bool 	ClearSpawn(int condition);
	void 	InitTimerTick();
	bool 	CreateGrid(int row,int column,float step,float startX,float startY);
	int	BuildSliceMask(float near,float far);			//���������ѯ����Ҫ��mask

	void 	DuplicateWorld(world * dest) const;	//�������磬����NPC������ .....
	
	inline world_manager * GetWorldManager() { return w_world_man;}
	inline int GetTag() { return w_tag; }
	inline int GetWorldIndex() { return w_index; }
	//����һ��NPC���ݣ�����һ����������NPC�ṹ
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

	//����һ��Matter���ݣ�����һ����������Matter�ṹ
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

	//	����/ɾ����ҺͶ���ĺ���
	//����һ��������ݣ�������һ����������ҽṹ
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

	int InsertPlayer(gplayer *);		//����λ�ã�����һ�����������У����ز��������������
	int InsertNPC(gnpc*);			//����λ�ã�����һ�����������У����ز��������������
	int InsertMatter(gmatter *);		//����λ�ã�����һ�����������У����ز��������������
	
	void RemovePlayer(gplayer *pPlayer); 	//���������Ƴ�һ�����󣬲�free
	void RemoveNPC(gnpc *pNPC);		//���������Ƴ�һ�����󣬲�free
	void RemoveMatter(gmatter *pMatter);	//���������Ƴ�һ�����󣬲�free

	//�ӹ��������Ƴ�NPC�����ڲ��ڳ����е�npc
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
//	ȡ�����Ե�inline����
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

	bool QueryObject(const XID & id,object_info & info,bool other_plane = false);	//��ѯһ�����������״̬
	
	void GetColumnPlayers(std::vector<XID>& player_list, const A3DVECTOR& src, const A3DVECTOR& target, int self, float fRadius, float height);


public:
	void RunTick();		//��manager���Ƶ��û����ڲ��Զ�����
	void ResetWorld();	//�������磬ֻ�и����Ż�������������������������еĹ���ȣ�
	void DumpMessageCount();

	void SetWorldCtrl(world_data_ctrl * ctrl);
	world_data_ctrl * w_ctrl;
private:
	friend class MsgQueue;

	int HandlePlaneMessage(const MSG & msg);
public:


	/*
	 *	�㲥��Ϣ�����վ��뽫��ת������Χ�����ж��
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastMessage(const MSG & message,float fRadius,int mask); 		

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalBoxMessage(const MSG & message,const rect & rt);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���(�Ѿ���ȡ��)
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���,�ö������������
	 *	������ʼ������message�У��յ���target
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	�㲥׵����Ϣ����׵�еĶ�����յ������Ϣ
	 *	Բ׶��Բ�ļ�Ϊ��Ϣ�ķ�����
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t max_count,std::vector<exclude_target>& targets_exclude);


	/*
	 *	ͬBroadcastMessage��Ψһ��������ֻ�ڱ�������Ӧ��ת������
	 *	BroadcastMessage�ı��ط�����ͨ�����ñ�������ɵ�
	 */
	int BroadcastLocalMessage(const MSG & message,float fRadius,int mask);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	�㲥������Ϣ�����վ��뽫��ת������Χ�����ж���,�ö������������
	 *	������ʼ������message�У��յ���target
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	�㲥׵����Ϣ����׵�еĶ�����յ������Ϣ
	 *	Բ׶��Բ�ļ�Ϊ��Ϣ�ķ�����
	 *	msg.target �����;������յ��ߣ�msg.target��ID����Ϊ-1
	 *	mask���ڹ�����Ϣ���ն���
	 *	��������㲥ʱ�����Զ��ж��Ƿ�Ҫת���������ķ�������
	 *	msg.source�����յ�����㲥��Ϣ
	 */
	int BroadcastLocalTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t max_count,std::vector<exclude_target>& targets_exclude);


	int DispatchPlaneMessage(const MSG & message);
	
	/*
	 *	���һ�������ֵ���Ƿ���Ȼ����ȷ��Χ��,�ڲ�ʹ�õĺ���.
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
//ģ�庯���ӿ�
	/*
	 *	��һ���������������Ӽ��ƶ�,�жϸö����뿪����Щ���ӵ���Ұ,�������Ӧ��enter��leave��������
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
			//������޷����� ����Ҫ�����뿪����Ĳ���,�����ѭ����û���жϱ���
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
					//�뿪�����slice
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
	 *	ɨ�踽�����е�С����,����Ԥ���ķ�Χ��ɨ��,���ﲻ���и����Ƿ��ڱ����������ж� 
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
	 * ����λ�úͷ�Χɨ�踽�����е�С��,�������ε�����Ӧ�Ĵ���������
	 * ����������жϸ����Ƿ��ڵ�ǰ��������,���򲻻ᷢ�͵�func������
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
			if(tick - pPiece->idle_timestamp < 40)	//��piece����ÿ��������һ��
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
	 *	ɨ�踽�����е�С����,����Ԥ���ķ�Χ��ɨ��,���ﲻ���и����Ƿ��ڱ����������ж� 
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
	void CheckGSvrPoolUpdate();						//��鵱ǰ���ߵ���Ϸ�������б�
	void ConnectGSvr(int index, const char * ipaddr, const char * unixaddr);	//��������һ̨������ 

	/*
	 * ������Ƕ���ˢ����Ϸ����������,������Ϊ�̳߳ص�һ������������
	 * �������ĵط�����صĶ�ʱ������
	 */

	int _message_handle_count;				//��¼��ǰmessage�����Ƕ�� 

private:
	//��Ϣ�������ĺ���������������ֻ����Ϣ�����������ϺͶϿ�ʱ����
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
	//�Ժ���Ҫ����ReInit
	return 0;
}


#endif

