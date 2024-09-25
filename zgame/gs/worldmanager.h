#ifndef __ONLINEGAME_GS_WORLD_MANANGER_H__
#define __ONLINEGAME_GS_WORLD_MANANGER_H__

#include <string>
#include <hashtab.h>
#include <spinlock.h>
#include <common/types.h>
#include <db_if.h>
#include "terrain.h"
#include "template/itemdataman.h"
#include "template/pathman.h"
#include "template/city_region.h"
#include "config.h"
#include "object.h"
#include "player.h"
#include "aiman.h"
#include "pathfinding/pathfinding.h"
#include "gmatrix.h"
#include "traceman.h"

struct gnpc_imp;
struct gmatter;
struct gplayer;
struct bg_param;
struct bf_param;
struct battle_field_info;
struct instance_field_info;
struct territory_field_info;
struct kingdom_field_info;
struct ai_param;
struct raid_param;
struct flow_battle_info;

class gplayer_imp;
class gnpc;
class buff_area;
#include "objmanager.h"

struct world_pos
{
	int tag;
	A3DVECTOR pos;
};

struct world_limit
{
	bool nothrow;
	bool allowroot;
	bool savepoint;
	bool nomount;
	bool gmfree;
	bool noduel;
	bool nobind;
	bool nofly;
	bool nocouplejump;
	bool notrade;
	bool nomarket;
	bool noshop;
	bool can_use_battleground_potion;
	bool can_use_battleground_skill_mater;
	bool faction_team;
	bool nodrug;
	bool nopetsummon;
	bool nopetcombine;
	bool nopetfood;
	bool no_longjump_in; //��������LongJump����ʽ������ͼ
	bool no_flytask; 
	bool need_deity;	//��Ҫ������ܽ����ͼ
};

namespace GDB
{
	struct base_info;
	struct vecdata;
};

struct MSG;
struct gplayer;
class world;
class gplayer_imp;
class world_manager;
class world_message_handler
{
protected:
	world_manager * _manager;
public:
	world_message_handler(world_manager *man):_manager(man) {}
	virtual ~world_message_handler(){}
	virtual int HandleMessage(const MSG& msg) = 0;
};

class world_manager
{
protected:
	path_manager* _pathman;
	city_region* _region;
	path_finding::PathfindMap* _movemap;
	trace_manager _trace_man;
	buff_area*	_buffarea;

	typedef abase::hash_map<int, int, abase::_hash_function, abase::fast_alloc<> > BuffAreaStatusMap;
	int status_map_lock;
	BuffAreaStatusMap statusMap;

	int _world_tag;	//�����tag�� ��ʾ���ĸ�λ�棬����������������͸���
	int _world_index;
	world_limit _world_limit;

	int _region_file_tag;		//�����ļ��ı�ǩ
	int _precinct_file_tag;		//�س��ļ��ı�ǩ
	world_pos _save_point;		//���ܴ��ڵĴ��̵�
	float _max_mob_sight_range;	//������������Ұ
	float _max_visible_range;	//��ҵĿɼ���Ұ
	world_message_handler * _message_handler;

protected:
	abase::hash_map<int, A3DVECTOR> w_service_npc_list;
	abase::hash_map<int, A3DVECTOR> w_normal_mobs_list;
public:
	enum INIT_RES
	{
		UNINITED = 0,
		INIT_SUCCESS,
		INIT_FAIL,
	};
	struct player_cid
	{
		int cid[3];
		player_cid()
		{
			cid[0] = cid[1] = cid[2] = -1;
		}
		bool Init(const char * str);
	};

protected:
	class SpawnActive : public ONET::Thread::Runnable
	{
		int _ctrl_id;
		bool _active;
		bool _local;
		int _world_tag;
	public:
		SpawnActive(int id, bool bVal, bool local, int world_tag):_ctrl_id(id),_active(bVal),_local(local),_world_tag(world_tag)
		{}

		virtual void Run()
		{
			if(_local && _world_tag > 0)
			{
				gmatrix::TriggerSpawn(_ctrl_id, _active, _world_tag);
			}
			else
			{
				gmatrix::TriggerSpawn(_ctrl_id, _active);
			}
			delete this;
		}
	};

	virtual void OnActiveSpawn(int id,bool active, bool local, int world_tag)
	{
		ONET::Thread::Pool::AddTask(new SpawnActive(id, active, local, world_tag));
	}

	virtual void OnActiveSpawn(int id,bool active)
	{
		OnActiveSpawn(id, active, false, -1);
	}

public:
	static void ActiveSpawn(world_manager * pManager , int id, bool active)
	{
		if(!pManager) return;
		pManager->ActiveCtrlIDLog(id, active);
		int id2 = pManager->TranslateCtrlID(id);
		if(id2 <= 0) return ;
		pManager->OnActiveSpawn(id2,active);
	}

public:
	CTerrain & GetTerrain() { return *(_movemap->GetTerrain());}
	path_manager* GetPathMan() { return _pathman;}
	path_finding::PathfindMap* GetMoveMap() { return _movemap;}
	trace_manager& GetTraceMan() { return _trace_man;}
	city_region*  GetCityRegion() { return _region; }
	buff_area* GetBuffArea() { return _buffarea; }
	bool NoBuffArea() const { return _buffarea->NoBuffArea(); }

	int GetWorldIndex() { return _world_index;}
	void SetWorldIndex(int world_index);
	int GetWorldTag() { return _world_tag;}
	const world_limit & GetWorldLimit() { return _world_limit;}

	int GetRegionTag(){ return _region_file_tag;};
	int GetPrecinctTag(){ return _precinct_file_tag;};
	bool InitWorldLimit(const char * servername);
	const world_pos & GetSavePoint() { return _save_point;}
	float GetMaxMobSightRange() { return _max_mob_sight_range;}
	void SetWorldTag(int tag ) {_world_tag = tag;}

	bool IsBuffAreaActive(int areaTid, int& npcTid);
	void ActiveBuffArea(int areaTid, int npcid);
	void DeactiveBuffArea(int areaTid);

protected:
	int InitBase(const char * section);
	bool InitTerrain(std::string base_path, std::string section,const rect & rt);
	bool InitMoveMap(const char* ground, const char* water, const char* air);
	bool InitRegionData(const char * precinct_path,const char * region_path);

	//�ͷ�worldmanager��Դ, ������, Add by Houjun 2011-7-13
	bool ReleasePathMan();
	bool ReleaseMoveMap();
	bool ReleaseRegionData();
	bool ReleaseTraceMan();
	//Add end.
	
	bool ReleaseBuffArea();

public:
	bool GetServiceNPCPos(int id, A3DVECTOR & pos)
	{
		abase::hash_map<int, A3DVECTOR>::iterator it = w_service_npc_list.find(id);
		if(it == w_service_npc_list.end())
		{
			return false;
		}
		else
		{
			pos = it->second;
			return true;
		}
	}

	bool GetMobNPCPos(int id, A3DVECTOR & pos)
	{
		abase::hash_map<int, A3DVECTOR>::iterator it = w_normal_mobs_list.find(id);
		if(it == w_normal_mobs_list.end())
		{
			return false;
		}
		else
		{
			pos = it->second;
			return true;
		}
	}
	
	world_manager():_pathman(0), _region(0), _movemap(0), _buffarea(0), _max_mob_sight_range(15.f),_max_visible_range(DEFAULT_GRID_SIGHT_RANGE),_message_handler(0)
	{
		status_map_lock = 0;
		_save_point.tag = -1;
		_save_point.pos = A3DVECTOR(0.f,0.f,0.f);
	}

	virtual ~world_manager() 
	{
		if(_message_handler) delete _message_handler;
	}

	int PlaneSwitch(gplayer_imp * pImp,const A3DVECTOR & pos,int tag,const instance_key & key, size_t fee);

	//Add by Houjun 2011-07-13, �������ͷź�������������Դ��Ҫ�������ͷ�
	virtual void Release();
	
public:	
	virtual void Heartbeat();
	virtual bool InitNetIO(const char * servername) = 0;
	virtual void GetPlayerCid(player_cid & cid) = 0;
	virtual bool CompareInsKey(const instance_key & key, const instance_hash_key & hkey) { return true;}
	virtual bool CheckKeyInvalid(const instance_key & key,const instance_hash_key &cur_key) {return true;}
	virtual bool IsUniqueWorld() = 0;
public:
//�������
	virtual world * GetWorldByIndex(size_t index) = 0;
	virtual size_t GetWorldCapacity() = 0;
	virtual int GetOnlineUserNumber() { return 0;}
	//virtual void HandleSwitchRequest(int lid,int uid, int sid,int source, const instance_key & key) = 0;
	virtual void HandleSwitchStart(int uid, int source_tag, const A3DVECTOR & pos, const instance_key & key) = 0;
	virtual void PlayerLeaveThisWorld(int plane_index, int useid){}
	virtual void GetLogoutPos(gplayer_imp * pImp, int & world_tag ,A3DVECTOR & pos) = 0;
	virtual void SwitchServerCancel(int link_id,int user_id, int localsid) = 0;
	virtual void FactionLogin(const instance_hash_key &hkey,const void * buf, size_t size) {}
	virtual bool GetTownPosition(gplayer_imp *pImp, const A3DVECTOR &opos, A3DVECTOR &pos, int & tag);
	virtual void RecordBattleBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building = false ) {}
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction) {}
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id = 0) {}
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction) {}
	virtual void RecordMob(int type, int tid, const A3DVECTOR &pos,int faction,int cnt); //type: 0��־�Խ��� 1:npc,2:mob
	virtual void OnDeliveryConnected( std::vector<battle_field_info>& info ) {}
	virtual void OnDeliveryConnected( std::vector<instance_field_info>& info ) {}
	virtual void OnDeliveryConnected( std::vector<territory_field_info>& info ) {}
	virtual void OnDeliveryConnected( std::vector<kingdom_field_info>& info ) {}
	virtual void OnDeliveryConnected( std::vector<flow_battle_info>& info ) {}
	virtual int OnMobDeath(world * pPlane, int faction, int tid, const A3DVECTOR& pos, int attacker_id) = 0;
	virtual void OnMobReborn( int tid ){} // Youshuang add
	virtual world * GetWorldLogin(int id, const GDB::base_info * pInfo, const GDB::vecdata * data);
	virtual int OnPlayerLogin(const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm ) {return 0;}
	virtual bool CreateBattleGround(const bg_param &) { return false;} //�յ�Ҫ�󴴽�ս����Э��
	virtual int CreateBattleField(const bf_param &,bool) { return false;} //�յ�Ҫ�󴴽���ս��Э��
	virtual bool CreateBattleTerritory(int battle_id, int tag_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp){return false;}
	virtual bool CreateKingdom(char fieldtype, int tag_id, int defender, std::vector<int> attacker_list) { return false;}
	virtual void StopKingdom(char fieldtype, int tag_id){}
	virtual void PostPlayerLogin( gplayer* pPlayer ) { }
	// ս������
	virtual int GetBattleID() { return 0; }
	virtual bool IsFlowBattle() { return false; }
	virtual void FlowBattlePlayerLeave(int role_id, char faction) {}
	// ת��ctrlID
	virtual int TranslateCtrlID( int which );
	virtual void ActiveCtrlIDLog(int which, bool active){}
	virtual bool CheckServiceCtrlID(int ctrl_id) { return false; }

	virtual void BattleFactionSay( int faction ,const void* buf, size_t size, int channel = 0, const void *aux_data = 0, size_t dsize =0, int char_emote =0, int self_id =0){}
	virtual void BattleSay( const void* buf, size_t size ){ }

	virtual void GetAllDataWorldManager(gplayer_imp * pImp){}

public:
	void SwitchPlayerTo(int uid, int desttag, const A3DVECTOR & pos, const instance_key & ins_key);
	virtual int SwitchPlayerFrom(world_manager *from, gplayer * pPlayer, const A3DVECTOR & pos, const instance_key & ins_key); 

	inline int HandleWorldMessage(const MSG & message)
	{
		return _message_handler->HandleMessage(message);
	}
	virtual bool IsBattleWorld() { return false; }
	virtual bool IsBattleFieldWorld() { return false; }
	virtual bool IsRaidWorld() { return false; }
	virtual bool IsFacBase() { return false; }
	virtual bool IsKingdomWorld() { return false; }
	virtual bool IsGlobalWorld(){return !IsBattleWorld() && !IsBattleFieldWorld() && !IsRaidWorld() && !IsKingdomWorld() && !IsFacBase(); }
	virtual int  GetRaidID() { return -1; }
	virtual int  GetRaidType() { return -1; }
	virtual bool IsLimitSkillInWorld(int skill_id ) { return false; }
	virtual bool CanCastSkillInWorld(int skill_id, int who) { return true;}
	virtual void AdjustNPCAttrInBattlefield(gnpc_imp* pImp,ai_param& aip,int ai_cid) {}
	virtual void RecordBattleNPC(gnpc_imp * pImp){}
	virtual void OnBuyArcher(gplayer_imp* pImp) {}
	virtual void OnBattleNPCNotifyHP(int faction,int tid,int cur_hp,int max_hp) {}
	virtual void OnCastSkill(gactive_imp* pImp, int skill_id) {}
	virtual bool PlayerBuySeekerSkill(gplayer_imp* pImp) { return false; }
	
	virtual bool GetCommonData(int key, int & value, int data_type = 0);
	virtual void SetCommonData(int key, int value, int data_type = 0);
	virtual bool ModifyCommonData(int key, int offset, int data_type = 0); 


	//Raid�������, Add by Houjun 2011-07-13
	virtual bool IsIdle() const { return false; } 
	virtual bool CanEnterWorld() const { return true; }
	//Add end.
	virtual bool IsFree() { return false; } 
	//liuyue-facbase �麯�� Ĭ����� clienttag ���� _world_tag Ŀǰֻ�и����Ͱ��ɻ������ֶ�ʵ����ͼ clienttag ���� worldtag ��һ��
	virtual int GetClientTag() { return _world_tag; } //�ͻ���Ҫʹ�õ� tag
	virtual bool SaveCtrlSpecial() { return false; } //�Ƿ�Ϊ��ͬʵ���������������״̬ �����Ͱ��ɻ��ط��� true

	virtual INIT_RES OnInitMoveMap() { return UNINITED; } //�����Ƿ��Լ���ʼ�� MoveMap
	virtual bool OnReleaseMoveMap() { return false; }

	virtual INIT_RES OnInitCityRegion() { return UNINITED; } //�����Ƿ���Լ���ʼ�� CityRegoin
	virtual bool OnReleaseCityRegion() { return false; }

	virtual INIT_RES OnInitBuffArea() { return UNINITED; } //�����Ƿ���Լ���ʼ�� BuffArea
	virtual bool OnReleaseBuffArea() { return false; }

	virtual INIT_RES OnInitPathMan() { return UNINITED; }
	virtual bool OnReleasePathMan() { return false; }

	virtual bool OnTraceManAttach() { return true; }
	virtual void OnDisconnect() {} //�� gdeliveryd �Ͽ�����ʱ�Ĵ���
	virtual void OnFacBasePropChange(int roleid, int fid, int type, int delta) {}
	virtual void OnFacBaseBuildingProgress(int fid, int task_id, int value) {}
	virtual void HandleFBaseDeliverCMD(int roleid, const void * buf, size_t size) {}
	virtual void HandleFBaseClientCMD(int roleid, const void * buf, size_t size, int linkid, int localsid) {}
	virtual void FacBaseTaskPutAuction(int roleid, int name_len, char * playername, int itemid) {}
	virtual void OnFacBaseStop(int fid) {}
	virtual void DebugSetHider(int player_id, int ishide) {}
	virtual void DebugSetSeekRaidScore(int player_id, int score) {}
};

#endif

