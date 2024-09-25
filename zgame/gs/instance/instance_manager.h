#ifndef __ONLINEGAME_GS_INSTANCE_MANAGER_H__
#define __ONLINEGAME_GS_INSTANCE_MANAGER_H__

#include "../world.h"
#include <string>

class instance_world_manager : public world_manager
{
protected:
	typedef abase::hashtab<int, instance_hash_key ,instance_hash_function, abase::fast_alloc<> > KEY_MAP;

	player_cid				_cid;
	world *					_plane_template;		//����ģ�壬���е����綼�����︴��(Ӧ�ò��ȴ���NPC)
	abase::vector<world*>  	_cur_planes;  			//��ǰ����������б�
	abase::vector<int>  	_planes_state; 			//��ǰ����������б��״̬�������Ƿ���Բ���ʹ�ã���
	int						_alive_plane_count;		//��ǰ���ڵ�������Ŀ
	int						_active_plane_count;	//��ǰ�����������Ŀ
	size_t					_max_active_index;		//������������������
	int						_planes_capacity;		//������Ŀ���ޣ� ����ؼӼ���ļ���ȴ��
	int						_pool_threshold_low;	//����ص���ֵ������������ֵ�����´β�ѯʱ�����������Ŀ
	int						_pool_threshold_high;	//����ص���ֵ������ص���Ŀ����������ֵ
	int						_idle_time;				//������ж�ûᱻ�ջ�

	KEY_MAP		 			_key_map;				//���������world��Ӧ��
	int						_key_lock;				//�������������
	size_t					_heartbeat_counter; 	//����������ʱ�� 
//	int						_world_free_count;		//���е�������Ŀ�����Ժ����Ҫɾ����

	std::string 			_restart_shell;			//�������������
	//query_map 			_pworld_map; 			//����������ڵ�������б�
	//int					_pworld_lock;			//�ϱ��Ӧ����
	int						_player_limit_per_instance;	//ÿ�������������������
	int						_heartbeat_lock;		//Ϊ�˱����ظ�heartbeat����
	int						_pool_lock;				//����ص�lock

	CNPCGenMan 		*_npcgen;						//����NPC�ֲ��ṹ

	void TimerTick();

	abase::vector<world *> _planes_pool;			//����أ�����Ķ��ǿ�������
	abase::vector<world *> _planes_cooldown;		//������ȴ�� Ҫ���ͷŵ����綼���ȷŵ�����
	abase::vector<world *> _planes_cooldown2;		//������ȴ�� �ǿ��Խ����ͷŵ� �ͷ����������ȴ�ؽ��н���

//	virtual bool InitNetClient(const char * gmconf);
	virtual void FinalInit(const char * servername) {}
	virtual void PreInit(const char * servername) {}

	virtual int OnMobDeath(world * pPlane, int faction,int tid,const A3DVECTOR& pos, int attacker_id){ return 0; }
protected:
	void RegroupCoolDownWorld(bool recycle_world = true);
	void FillWorldPool();
public:
	inline int GetPlayerWorldIdx(int uid)
	{
		int index = -1;
		{
			//int *pTmp;
			//mutex_spinlock(&_pworld_lock);
			//pTmp = _pworld_map.nGet(uid);
			//if(pTmp) index = * pTmp;
			//mutex_spinunlock(&_pworld_lock);
		}
		return index;
	}

	inline void SetPlayerWorldIdx(int uid, int svr)
	{
		//spin_autolock alock(_pworld_lock);
		//_pworld_map.find_or_insert(uid,svr) = svr;
	}

	inline void RemovePlayerWorldIdx(int uid)
	{
		//spin_autolock alock(_pworld_lock);
		//_pworld_map.erase(uid);
	}

	inline void RemovePlayerWorldIdx(int uid, int plane_index)
	{
		//spin_autolock alock(_pworld_lock);
		//query_map::iterator it = _pworld_map.find(uid);
		//if(it != _pworld_map.end())
		//{
		//	if(*(it.value()) == plane_index)
		//	{
		//		_pworld_map.erase(uid);
		//	}
		//}
	}

	inline int GetWorldByKey(const instance_hash_key & ikey )
	{
		int index = -1;
		{
			int *pTmp;
			mutex_spinlock(&_key_lock);
			pTmp = _key_map.nGet(ikey);
			if(pTmp) index = *pTmp; 
			mutex_spinunlock(&_key_lock);
		}
		return index;
	}

	virtual void TransformInstanceKey(const instance_key::key_essence & key, instance_hash_key & hkey)
	{
		hkey.key1 = key.key_level2.first;
		hkey.key2 = key.key_level2.second;
		if(hkey.key1 <= 0)
		{
			hkey.key1 = key.key_level1;
		}
	}

public:
	instance_world_manager():_key_map(300)//,_pworld_map(1024)
	{
		_heartbeat_counter = 0;
		//_pworld_lock = 0;
		_key_lock = 0;

		_plane_template = NULL;
		_alive_plane_count = 0;
		_active_plane_count = 0;
		_max_active_index = 0;
		_planes_capacity = 0;
		_pool_threshold_low = 0;
		_pool_threshold_high = 0;
		_heartbeat_lock = 0;
		_pool_lock = 0;
		_idle_time = 60*20;
	}

	int Init(const char * gmconf_file,const char * servername);
	virtual void Heartbeat();
	virtual int CheckPlayerSwitchRequest(const XID & who, const instance_key * key,const A3DVECTOR & pos, int ins_timer);
	world * AllocWorld(const instance_hash_key & ikey,int & world_index);
	world * AllocWorldWithoutLock(const instance_hash_key & ikey,int & world_index);
	void FreeWorld(world * pPlane, int ins_index);
	virtual world * GetWorldInSwitch(const instance_hash_key & ikey,int & world_index);
	
	virtual void SetIncomingPlayerPos(gplayer * pPlayer, const A3DVECTOR & origin_pos);
	
public:
	virtual void RestartProcess();
	virtual bool InitNetIO(const char * servername);
	virtual void GetPlayerCid(player_cid & cid);
	virtual bool CompareInsKey(const instance_key & key, const instance_hash_key & hkey);
	virtual world * GetWorldByIndex(size_t index);
	virtual size_t GetWorldCapacity();
	virtual int GetOnlineUserNumber();
	virtual void PlayerLeaveThisWorld(int plane_index, int useid);
	virtual void GetLogoutPos(gplayer_imp * pImp, int & world_tag ,A3DVECTOR & pos);
	virtual void SwitchServerCancel(int link_id,int user_id, int localsid);
	virtual void UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size, char flag);
	virtual bool IsUniqueWorld();
	virtual world * CreateWorldTemplate();
	virtual world_message_handler * CreateMessageHandler();

	virtual void HandleSwitchRequest(int lid,int uid, int sid,int source, const instance_key &key);
	virtual void HandleSwitchStart(int uid, int source_tag, const A3DVECTOR & pos, const instance_key & key);
};

class instance_world_message_handler : public world_message_handler
{
protected:
	instance_world_manager * _manager;
	virtual ~instance_world_message_handler(){}
	int PlayerComeIn(instance_world_manager *, world * pPlane,const MSG &msg);

	virtual void SetInstanceFilter(gplayer_imp * pImp,instance_key &  ikey);
	virtual void PlayerPreEnterServer(gplayer * pPlayer, gplayer_imp * pimp,instance_key &  ikey) {}
public:
	instance_world_message_handler(instance_world_manager * man):world_message_handler(man),_manager(man) {}
	virtual int HandleMessage(const MSG& msg);
	virtual int RecvExternMessage(int msg_tag, const MSG & msg);
};


#endif

