#ifndef __ONLINEGAME_GS_PLAYERTEAM_H__
#define __ONLINEGAME_GS_PLAYERTEAM_H__

#include <common/types.h>
#include <common/base_wrapper.h>
#include <glog.h>
#include "config.h"
#include "staticmap.h"
#include "property.h"
#include "gmatrix.h"

class gplayer_imp;
class world;

class player_team  // :: public abase::ASmallObject
{
public:
	struct invite_t
	{
		long timestamp;
		XID id;
	};
	typedef abase::static_map<int , invite_t, TEAM_MEMBER_CAPACITY - 1> INVITE_MAP;

	struct team_entry{
		int time_out;			//ĳ��״̬�ĳ�ʱʱ��
		int self_seq;			//�������ţ�ÿ���һ�ζ��飬��һ��һ,��ֵ������ϵͳʱ��)
		int pickup_flag;		//��ʼ��������ʱ��ȡ��Ʒ������
		long last_invite_time;		//���һ�α������ʱ���
		int cls_count;			//������ְҵ��Ŀ
		int change_leader_timeout;	//����leader��timeout
		int cur_max_level;		//��ǰ�����е���󼶱�
		int cur_wallow_level;		//��ǰ���������Եȼ�
		int min_timer_counter;		//������С�����counter
		INVITE_MAP invite_map;		//������˵�����
		INVITE_MAP apply_map;		//����������
		gplayer_imp * imp;

	};

	struct agree_invite_entry
	{
		short cls;
		short gender;
		int cs_index;
		int cs_sid;
		team_mutable_prop data;
	};

	struct member_entry
	{
		XID id;
		team_mutable_prop data;
		short cls;			//ְҵ
		short gender;			//�Ա�
		int cs_index;			//��Ա��cs index
		int cs_sid;			//��Ա��sid
		A3DVECTOR pos;			//��Ա��λ��
		int timeout;			//��Ա��ʱ���
		bool is_changed;		//��һ���Ƿ����仯
	};

	struct team_t
	{
	private:
		static int team_gt_id_lock;	
		static int team_gt_id_seed;	// ÿ��10����ӣ�ÿ��10*3600*24*7����int�㹻��junbo 2012-10-22

	public:
		static int GenerateTeamId() 
		{
			spin_autolock keeper(team_gt_id_lock);
			team_gt_id_seed += 1;
			return team_gt_id_seed;
		}
		enum
		{
			TEAM_NO_CHANGE,
			TEAM_PART_CHANGED,
			TEAM_ALL_CHANGED,
		};
		
		XID 		leader;		//�ӳ�
		int 		team_seq;	//�������
		int			team_gt_id;	//����Ψһid, gt�ã� ֮ǰ�߼��Զӳ�id��Ϊ����id������������������team_id����ʹ��team_gt_id��junbo 2012-10-22
		size_t		member_count;
		int 		change_flag;
		int 		pickup_flag;		//��ʼ��������ʱ��ȡ��Ʒ������
		
		member_entry 	member_list[TEAM_MEMBER_CAPACITY]; 
	};

	struct roll_list
	{
		size_t tail;
		XID  list[TEAM_MEMBER_CAPACITY]; 
	};

	class team_control
	{
	protected:
		void ChangeState(player_team * pTeam, int state)
		{
			pTeam->_team_state = state;
		}
		
		team_entry & GetData(player_team * pTeam) { return pTeam->_data;}
		team_t & GetTeam(player_team * pTeam) { return pTeam->_team;}
		
		template <int foo>
		static inline void SendMessage(world *pPlane,const MSG & msg)
		{
			player_team::SendMessage<0>(pPlane,msg);
		}
		
	private:
		friend class player_team;

//����		
		virtual void OnHeartbeat(player_team * pTeam) = 0;

//�ͻ��˲���
		virtual void CliInviteOther(player_team * pTeam,const XID & member)  = 0;
		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)  = 0;
		virtual void CliRejectInvite(player_team * pTeam, const XID & leader) = 0;
		virtual bool CliLeaveParty(player_team * pTeam) = 0;
		virtual void CliKickMember(player_team * pTeam,const XID & member) = 0;
		virtual void CliAgreeApply(player_team *, int id, bool result) = 0;
		virtual void CliChangeLeader(player_team *, const XID & new_leader) = 0;

//��Ϣ
		virtual void MsgInvite(player_team *pTeam, const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid) = 0;
		virtual void MsgAgreeInvite(player_team *,const XID & member,const A3DVECTOR &pos,const agree_invite_entry&,int seq) = 0;
		virtual void MsgRejectInvite(player_team *,const XID & member) = 0;
		virtual void MsgJoinTeam(player_team *,const XID & leader,const member_entry*, size_t count,int pickup_flag,const void * ins_key , size_t ik_size, int team_gt_id) = 0;
		virtual void MsgJoinTeamFailed(player_team *pTeam, const XID & leader) = 0;
		virtual void MsgMemberUpdateData(player_team *, const XID & member,const A3DVECTOR &,const team_mutable_prop &) = 0;
		virtual void MsgLeaderUpdateData(player_team *, const XID & leader,const member_entry *,size_t) = 0;
		virtual void MsgMemberLeaveRequest(player_team * pTeam, const XID & member) = 0;
		virtual void MsgNotifyMemberLeave(player_team*,const XID& leader,const XID& member,int type) = 0;
		virtual void MsgNewMember(player_team*,const XID& leader,const member_entry * list, size_t count) =0;
		virtual void MsgLeaderCancelParty(player_team *,const XID & leader,int seq) = 0;
		virtual void MsgApplyParty(player_team*, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid) = 0;
		virtual void MsgApplyPartyReply(player_team*, const XID & leader, int seq) = 0;
		virtual void MsgChangeToLeader(player_team*, const XID & leader) = 0;
		virtual void MsgLeaderChanged(player_team*, const XID & new_leader) = 0;
		virtual bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount) =0;
		virtual void LostConnection(player_team * pTeam) = 0;
		virtual void Logout(player_team * pTeam) = 0;

	};



	enum
	{
		TEAM_NORMAL,
		TEAM_WAIT,
		TEAM_MEMBER,
		TEAM_LEADER,
		TEAM_STATE_NUM,
	};

protected:
	static team_control * _team_ctrl[TEAM_STATE_NUM];

	int 		_team_state;		//��ӵ�״̬
	team_entry 	_data;	
	team_t		_team;
	roll_list	_item_roll;
	friend class team_control;
	friend class team_control_normal;
	friend class team_control_wait;
	friend class team_control_member;
	friend class team_control_leader;

	member_entry * FindMember(const XID & id)
	{
		for(size_t i = 0; i < _team.member_count; i ++)
		{
			if(_team.member_list[i].id == id) return _team.member_list + i;
		}
		return false;
	}

	template <int foo>
	inline static void SendMessage(world *pPlane,const MSG & msg)
	{
		gmatrix::SendMessage(msg);
	}
	

protected:

	void CalcRaceCount();
	void CalcMaxLevel();
	void CalcExactMaxLevel();

	bool BecomeLeader(const XID & first_member, const agree_invite_entry & prop,const A3DVECTOR &pos);
	bool FromMemberToLeader(const XID & leader);
	bool ChangeLeader(const XID & new_leader);
	bool FromLeaderToMember(const XID & new_leader);
	bool BecomeMember(const XID & leader, const member_entry * list, size_t count);
	void BecomeNormal();
	bool LeaderAddMember(const XID & member,const agree_invite_entry  &prop,const A3DVECTOR &pos);

	member_entry * UpdateMemberData(const XID & id, const A3DVECTOR &pos, const team_mutable_prop & data)
	{
		if(!IsInTeam()) return NULL;
		member_entry * pEntry = FindMember(id);
		if(pEntry)
		{
			if(pEntry->data != data)
			{
				pEntry->data = data;
				pEntry->is_changed = true;
				_team.change_flag = team_t::TEAM_PART_CHANGED;
			}
			//λ������ǿ�Ƹ���
			pEntry->pos = pos;
		}
		return pEntry;
	}

	bool LeaderUpdateMembers(const XID & source,const member_entry * list , size_t count)
	{
		if(source == _team.leader)
		{
			if(count > TEAM_MEMBER_CAPACITY)
			{
				return false;
			}
			for(size_t i = 0; i < count ; i++)
			{
				if(_team.member_count >= i
					|| _team.member_list[i].data != list[i].data
					|| !(_team.member_list[i].id == list[i].id))
				{
					//ֻ�е����ݸı���޸����ݣ�����Ϊ�˼��ٴ���ռ��
					_team.member_list[i] = list[i];
					_team.member_list[i].is_changed = true;
				}
				else
				{
					//ʲô�����޸� ����Ȼԭ��is_changedҲ����Ϊtrue
					//ֻ����λ����Ϣ
					_team.member_list[i].pos = list[i].pos;
				}
			}
			_team.member_count = count;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			CalcRaceCount();
			CalcMaxLevel();
			return true;
		}
		else
		{
			return false;
		}
	}

	void LeaderRemoveMember(member_entry * pEntry);
	void MemberRemoveMember(member_entry * pEntry);

	void SendSelfDataToMember(const team_mutable_prop & data);
	void SendTeamDataToClient();

	void SendGroupMessage(const MSG & msg)
	{
		size_t count = _team.member_count;
		size_t index = 0;
		XID  list[TEAM_MEMBER_CAPACITY];
		for(size_t i = 0;i < count; i ++)
		{
			if(_team.member_list[i].id == msg.source) continue;
			list[index++] = _team.member_list[i].id;
		}
		if(index)  gmatrix::SendMessage(list, list + index, msg);
	}

	void SendAllMessage(const MSG & msg, int ex_id)
	{
		size_t count = _team.member_count;
		size_t index = 0;
		XID  list[TEAM_MEMBER_CAPACITY];
		for(size_t i = 0;i < count; i ++)
		{
			if(i == (size_t) ex_id) continue;
			list[index++] = _team.member_list[i].id;
		}
		if(index)  gmatrix::SendMessage(list, list + index, msg);
	}

	void SendAllMessage(const MSG & msg)
	{
		size_t count = _team.member_count;
		XID  list[TEAM_MEMBER_CAPACITY];
		for(size_t i = 0;i < count; i ++)
		{
			list[i] = _team.member_list[i].id;
		}
		gmatrix::SendMessage(list, list + count, msg);
	}

	void SendMemberMessage(int idx, MSG & msg)
	{
		ASSERT((size_t)idx <_team.member_count);
		msg.target = _team.member_list[idx].id;
		gmatrix::SendMessage(msg);
	}
	
	
public:
	int GetNearMemberCount(int tag, const A3DVECTOR & pos);
	void SendMessage(const MSG & msg, float range, bool norangelimit, int max_count);		//������Ϣ������
	void SendAllMessage(const MSG & msg, float range, bool norangelimit, int max_count);	//������Ϣ������ �����Լ�
	void SendGroupData(const void * buf, size_t size);

	player_team()
	{
		memset(&_team,0,sizeof(_team));
		memset(&_data,0,sizeof(_data));
		memset(&_item_roll,0,sizeof(_item_roll));
		_team_state = TEAM_NORMAL;
		_data.time_out = 0;
		_data.last_invite_time = 0;
	}

	void Init(gplayer_imp * pPlayer);
	
	bool Save(archive & ar);
	bool Load(archive & ar);
	void Swap(player_team & rhs);
	
	const member_entry &GetMember(size_t index)  const
	{ 
		ASSERT(index < _team.member_count);
		return _team.member_list[index];
	}

	int GetMemberNum() const 
	{ 
		if(_team_state != TEAM_MEMBER && _team_state != TEAM_LEADER) return 0;
		return _team.member_count;
	}
	bool IsInTeam() const { return _team_state == TEAM_MEMBER || _team_state == TEAM_LEADER; } 
	bool IsLeader() const { return _team_state == TEAM_LEADER; } 
	const XID & GetLeader() const  {ASSERT(IsInTeam()); return _team.leader;}
	bool IsMember(const XID & member) const
	{
		ASSERT(IsInTeam());
		for(size_t i = 0; i < _team.member_count; i ++)
		{
			if(_team.member_list[i].id == member)
			{
				return true;
			}
		}
		return false;
	}

	int GetMemberList(XID * list) 
	{ 
		if(_team_state != TEAM_MEMBER && _team_state != TEAM_LEADER) return 0;
		for(size_t i = 0; i < _team.member_count; i ++)
		{
			list[i] = _team.member_list[i].id;
		}
		return _team.member_count;
	}

	int GetMember(const XID & member, A3DVECTOR & pos, int & level,int & tag)
	{
		if(!IsInTeam()) return -1;
		for(int i = 0; i < (int)_team.member_count; i ++)
		{
			if(_team.member_list[i].id == member)
			{
				pos = _team.member_list[i].pos;
				level = _team.member_list[i].data.level;
				tag = _team.member_list[i].data.world_tag;
				return i;
			}
		}
		return -1;
	}

	bool GetMemberPos(const XID & member,A3DVECTOR &pos,int & tag)
	{
		ASSERT(IsInTeam());
		for(size_t i = 0; i < _team.member_count; i ++)
		{
			if(_team.member_list[i].id == member)
			{
				pos = _team.member_list[i].pos;
				tag = _team.member_list[i].data.world_tag;
				return true;
			}
		}
		return false;
	}

	bool DispatchTeamMoney(int tag, const A3DVECTOR & pos, int money);
	
	void GetTeamID(int & id, int & seq)
	{
		if(IsInTeam())
		{
			id = _team.leader.id;
			seq = _team.team_seq;
		}
	}

	int GetEffLevel()
	{
		if(IsInTeam())
		{
			return _data.cur_max_level;
		}
		return 0;
	}

	int GetWallowLevel();
	int GetTeamSeq()
	{
		ASSERT(IsInTeam());
		return _team.team_seq;
	}

	int GetTeamID()
	{
		if(IsInTeam()) 
			return _team.leader.id;
		else
			return -1;
	}
	
	void OnHeartbeat()
	{
		_team_ctrl[_team_state]->OnHeartbeat(this);
	}

	
	
//��ҵĲ����ͽ�����Ϣ

	//�ͻ���Ҫ�󷢳�����				�ͻ���
	bool CliInviteOther(const XID & member)
	{
		if(!(_team_state == TEAM_LEADER) && !(_team_state == TEAM_NORMAL))
		{	
			return false;
		}

		_team_ctrl[_team_state]->CliInviteOther(this,member);
		return true;
	}

	//�ͻ��˵���ͬ�����				�ͻ���
	bool CliAgreeInvite(const XID & leader,int seq)	
	{
		if(_team_state != TEAM_NORMAL)
		{
			return false;
		}

		_team_ctrl[_team_state]->CliAgreeInvite(this, leader,seq);
		return true;
	}

	//�ͻ��˵���˾ܾ�����				�ͻ���
	void CliRejectInvite(const XID & leader)
	{
		if(_team_state == TEAM_NORMAL)
		{
			_team_ctrl[_team_state]->CliRejectInvite(this,leader);
		}
	}

	//�ͻ��˷������뿪����				�ͻ���
	bool CliLeaveParty()
	{
		if(!(_team_state == TEAM_LEADER) && !(_team_state == TEAM_MEMBER))
		{
			return false;
		}

		return _team_ctrl[_team_state]->CliLeaveParty(this);
	}

	//�ͻ���Ҫ���߳�һ���û�			�ͻ���
	bool CliKickMember(const XID & member)	
	{
		if(_team_state != TEAM_LEADER) return false;
		_team_ctrl[TEAM_LEADER]->CliKickMember(this,member);
		return false;
	}

	//����һ��һ����Ա���ö�Աͬ�� ���룩	��Ϣ
	void MsgAgreeInvite(const XID & member,const A3DVECTOR &pos,const agree_invite_entry& prop,int seq)
	{
		_team_ctrl[_team_state]->MsgAgreeInvite(this,member,pos,prop,seq);
	}

	//�����뷽�����˾ܾ��������Ϣ			��Ϣ
	void MsgRejectInvite(const XID & member)
	{
		_team_ctrl[_team_state]->MsgRejectInvite(this,member);
	}

	//leader������invite��Ϣ			��Ϣ
	bool MsgInvite(const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid)
	{
		if(_team_state == TEAM_NORMAL)
		{
			_team_ctrl[TEAM_NORMAL]->MsgInvite(this,leader,seq,pickup_flag, familyid, mafiaid, level, sectid, referid);
			return true;
		}
		else
		{
			return false;
		}
	}

	//leader������ͬ��������Ϣ  ��Ϣ
	void MsgJoinTeam(const XID & leader,const member_entry *list,size_t count, int pickup_flag,const void * ins_key , size_t ik_size, int team_gt_id)
	{
		_team_ctrl[_team_state]->MsgJoinTeam(this,leader,list,count,pickup_flag,ins_key,ik_size, team_gt_id);
	}
	
	//��Ա�������޷��������Ϣ			��Ϣ
	void MsgJoinTeamFailed(const XID & member)
	{
		if(_team_state == TEAM_LEADER)
		{
			_team_ctrl[_team_state]->MsgJoinTeamFailed(this,member);
		}
	}

	//��Ա�����˸����������ݵ���Ϣ			��Ϣ
	void MsgMemberUpdateData(const XID & member , const A3DVECTOR &pos, const team_mutable_prop & data)
	{
		if(IsInTeam())
		{
			_team_ctrl[_team_state]->MsgMemberUpdateData(this,member,pos,data);
		}
	}

	//leader�����˸������г�Ա���ݵ���Ϣ		��Ϣ
	void MsgLeaderUpdateData(const XID & leader, const member_entry * list, size_t count)
	{
		if(_team_state == TEAM_MEMBER)
		{
			_team_ctrl[_team_state]->MsgLeaderUpdateData(this,leader,list,count);
		}
	}

	//leader�յ��˳�ԱҪ���뿪����Ϣ		��Ϣ
	void MsgMemberLeaveRequest(const XID & member)
	{
		if(_team_state == TEAM_LEADER)
		{
			_team_ctrl[TEAM_LEADER]->MsgMemberLeaveRequest(this, member);
		}

	}

	//leader �߳�������Ա				��Ϣ 
	void MsgLeaderKickMember(const XID & leader, const XID & member)
	{
		if(_team_state == TEAM_MEMBER)
		{
			_team_ctrl[TEAM_MEMBER]->MsgNotifyMemberLeave(this,leader,member,1);
		}
	}

	//leader  ֪ͨ��֪������Ա���뿪��Ϣ
	void MsgNotifyMemberLeave(const XID & leader,const XID & member,int type = 0)
	{
		if(_team_state == TEAM_MEMBER)
		{
			_team_ctrl[TEAM_MEMBER]->MsgNotifyMemberLeave(this,leader,member,type);
		}
	}


	//leader ֪ͨ��������Ա�������
	void MsgNewMember(const XID & leader, const member_entry * list, size_t count)
	{
		if(_team_state == TEAM_MEMBER)
		{
			_team_ctrl[TEAM_MEMBER]->MsgNewMember(this,leader,list,count);
		}
	}

	//leader������ȡ����ӵ���Ϣ
	void MsgLeaderCancelParty(const XID & leader,int seq)
	{
		if(_team_state == TEAM_MEMBER)
		{
			_team_ctrl[TEAM_MEMBER]->MsgLeaderCancelParty(this,leader,seq);
		}
	}

	//��ҽ������߲���
	void PlayerLostConnection()
	{
		return _team_ctrl[_team_state]->LostConnection(this);
	}

	void PlayerLogout()
	{
		return _team_ctrl[_team_state]->Logout(this);
	}

	//��������������������Ϣ
	void ApplyParty(const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
	{
		return _team_ctrl[_team_state]->MsgApplyParty(this,who, faction, familyid, mafiaid, level, sectid, referid);
	}

	void AgreeApply(int id, bool result)
	{
		return _team_ctrl[_team_state]->CliAgreeApply(this,id,result);
	
	}
	void ApplyPartyReply(const XID & leader,int seq)
	{
		_team_ctrl[_team_state]->MsgApplyPartyReply(this,leader,seq);
	}

	void CliChangeLeader(const XID & new_leader)
	{
		_team_ctrl[_team_state]->CliChangeLeader(this,new_leader);
	}

	void ChangeToLeader(const XID & leader)
	{
		_team_ctrl[_team_state]->MsgChangeToLeader(this,leader);
	}

	void LeaderChanged(const XID & newleader)
	{
		_team_ctrl[_team_state]->MsgLeaderChanged(this,newleader);
	}

	void DispatchExp(const A3DVECTOR &pos,int * list ,size_t count, int64_t exp,int level,int total_level, int max_level ,int min_level, int npcid,float r, bool no_exp_punish);
	bool PickupTeamMoney(int world_tag, const A3DVECTOR & pos,int amount) 
	{
		if(!IsLeader() || amount <=0) return false;
		return _team_ctrl[TEAM_LEADER]->PickupTeamMoney(this,world_tag,  pos,amount);
	}

	bool IsRandomPickup()
	{
		ASSERT(IsInTeam());
		return _team.pickup_flag == 0;
	}
	
	void SetPickupFlag(int pickup_flag)
	{
		_data.pickup_flag = pickup_flag;
	}

	void AddRollMember(const XID & who);
	void DelRollMember(const XID & who);
	void ClearRollMember();
	XID RollItem(const A3DVECTOR & pos,int tag);

	void NotifyTeamPickup(const A3DVECTOR & pos, int type, int count);
	void TeamChat(char channel, char emote_id, const void * buf, size_t len,int srcid, const void * aux_data, size_t dsize);
	void SendTeamMessage(MSG & msg, float range, bool exclude_self);
};

// ---------------  gt������Ҫ��¼���еĶ��飬2012-10-29 sunjunbo ----------------------

struct team_cache_entry
{
	int team_size;
	int leader_id;
	int members[TEAM_MEMBER_CAPACITY];

	team_cache_entry() : team_size(0), leader_id(0) {}
	team_cache_entry(const player_team::team_t& team) 
	{
		team_size = (int)team.member_count; 	
		leader_id = team.leader.id;
		for (size_t i = 0; i < team.member_count; i++) {
			members[i] = team.member_list[i].id.id;
		}
	}
};

class team_cache
{
	// ���еĶ���
	int lock;
	std::map<int, team_cache_entry> teams;

public:
	static team_cache instance;

	team_cache() : lock(0) {}

	// �����и��¶���
   	void UpdateTeam(int team_id, const team_cache_entry& entry);
	// ������ɾ������
   	void RemoveTeam(int team_id);
	// ͨ��GT���ж���
   	void SyncToGT();

	// ƴ��һ����ͬgs�䲻�ظ��Ķ���id
	inline static int64_t MakeGTTeamId(const int& team_id) 
	{
		return (((int64_t)gmatrix::GetServerIndex()) << 32) + team_id;
	}
};

void OnGTConnected(); 
// ----------------------------------------------------------------------------------------


#endif

