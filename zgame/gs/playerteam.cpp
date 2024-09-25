#include "world.h"
#include "playerteam.h"
#include "player_imp.h"
#include <common/protocol.h>
#include <arandomgen.h>
#include <map>
#include <algorithm>
#include "playertemplate.h"
#include "task/taskman.h"
#include <gsp_if.h>
#include "global_drop.h"
#include "usermsg.h"

int player_team::team_t::team_gt_id_seed = 0; 
int player_team::team_t::team_gt_id_lock = 0; 
team_cache team_cache::instance;

void OnGTConnected() 
{
	team_cache::instance.SyncToGT();	
}

void team_cache::UpdateTeam(int team_id, const team_cache_entry& entry)
{
	team_cache_entry old;
	old.team_size = -1;
	{ // ������������
		spin_autolock keeper(lock);

		std::map<int, team_cache_entry>::iterator it = teams.find(team_id);
		if (it != teams.end()) {
			old = it->second;
			teams.erase(it);
		}
		teams.insert(std::make_pair(team_id, entry));
	}

	int64_t	gt_team_id = MakeGTTeamId(team_id);  
	// ֪ͨgt
	if (old.team_size <= 0) { // �´���ͨ��
		GMSV::gt_team_info info;
		info.team_id = gt_team_id;
		info.captain = entry.leader_id;	
		copy(entry.members, entry.members + entry.team_size, back_inserter(info.members));
		GMSV::SendGTTeamCreate(info);	
	} else { // ����ͨ��
		if (old.leader_id != entry.leader_id) { // �ӳ��仯
			std::vector<int> members;
			members.push_back(entry.leader_id);
			GMSV::SendGTTeamMemberUpdate(gt_team_id, members, (char)2); // 2: team_cap		
		}	
		
		std::set<int> old_members;
		std::set<int> cur_members;
		copy(old.members, old.members + old.team_size, inserter(old_members, old_members.begin()));
		copy(entry.members, entry.members + entry.team_size, inserter(cur_members, cur_members.begin()));
		std::vector<int> added_members;
		std::vector<int> leaved_members;
		set_difference(old_members.begin(), old_members.end(), cur_members.begin(), cur_members.end(), back_inserter(leaved_members));
		set_difference(cur_members.begin(), cur_members.end(), old_members.begin(), old_members.end(), back_inserter(added_members));
		if (!added_members.empty()) {
			GMSV::SendGTTeamMemberUpdate(gt_team_id, added_members, 0); // 0: team_add
		}
		if (!leaved_members.empty()) {
			GMSV::SendGTTeamMemberUpdate(gt_team_id, leaved_members, 1); // 1: team_del
		}
	}
}
void team_cache::RemoveTeam(int team_id)
{
	int removed_size = 0;
	{ // ������������
		spin_autolock keeper(lock);

		removed_size = teams.erase(team_id);
	}
	
	if (removed_size)
	{
		GMSV::SendGTTeamDismiss(MakeGTTeamId(team_id));	
	}
}

void team_cache::SyncToGT()
{
	std::vector<GMSV::gt_team_info> gt_infos;
	{
		spin_autolock keeper(lock);
		GMSV::gt_team_info gt_info;
		for (std::map<int, team_cache_entry>::iterator it = teams.begin(); it != teams.end(); ++it) {
			gt_info.members.clear();
			gt_info.team_id = MakeGTTeamId(it->first);
			gt_info.captain = it->second.leader_id;	
			copy(it->second.members, it->second.members + it->second.team_size, back_inserter(gt_info.members));
			gt_infos.push_back(gt_info);
		}
	}	

	GMSV::SendGTSyncTeams(gt_infos);
}

class std_team_control : public player_team::team_control
{
	private:
		//�ͻ��˲���
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			return ;
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			return ;
		}

		virtual void CliRejectInvite(player_team * pTeam, const XID & member)
		{
			ASSERT(false);
		}

		virtual bool CliLeaveParty(player_team * pTeam) 
		{
			return false;
		}

		virtual void MsgJoinTeam(player_team *pTeam, const XID& leader,const player_team::member_entry*,size_t ,int pickup_flag,const void * ins_key , size_t ik_size, int team_gt_id)
		{
			//���Ͳ��ܼ���������Ϣ
			gplayer_imp * pImp = GetData(pTeam).imp;
			pImp->SendTo<0>(GM_MSG_JOIN_TEAM_FAILED,leader,0);
		}

		virtual void MsgJoinTeamFailed(player_team *pTeam, const XID & leader)
		{
			return;
		}

		virtual void MsgMemberUpdateData(player_team *,const XID & member, const A3DVECTOR & pos,const team_mutable_prop & data)
		{
			ASSERT(false);
		}

		virtual void MsgLeaderUpdateData(player_team *,const XID & leader,const player_team::member_entry * list,size_t count)
		{
			ASSERT(false);
		}

		virtual void MsgInvite(player_team *pTeam, const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			ASSERT(false);
		}

		virtual void CliKickMember(player_team *pTeam, const XID & member)
		{
			ASSERT(false);
		}

		virtual void MsgMemberLeaveRequest(player_team * pTeam, const XID & member)
		{
			ASSERT(false);
		}

		void MsgNotifyMemberLeave(player_team * pTeam, const XID & leader,const XID & member,int type) 
		{
			ASSERT(false);
		}

		virtual void MsgNewMember(player_team*,const XID& leader,const player_team::member_entry * list, size_t count)
		{
			ASSERT(false);
		}
		
		virtual void MsgLeaderCancelParty(player_team *,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		virtual bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount)
		{
			ASSERT(false);
			return false;
		}
		
		virtual void LostConnection(player_team * pTeam)
		{
			// do nothing
		}
		
		virtual void Logout(player_team * pTeam)
		{
			// do nothing
		}

		virtual void CliAgreeApply(player_team *, int id, bool result)
		{
			//do nothing
		}

		virtual void MsgApplyPartyReply(player_team*, const XID & leader, int seq)
		{
			//do nothing
		}
		
		virtual void CliChangeLeader(player_team *, const XID & new_leader)
		{
			//do nothing 
		}

		virtual void MsgChangeToLeader(player_team*, const XID & leader)
		{
			//do nothing
		}
		
		virtual void MsgLeaderChanged(player_team*, const XID & new_leader)
		{
			//do nothing
		}

};

class team_control_normal : public  std_team_control
{
	private:
		virtual void OnHeartbeat(player_team * pTeam) 
		{
			//���������ʱ������
			long t = g_timer.get_systime();
			gplayer_imp * pImp = GetData(pTeam).imp;
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			player_team::INVITE_MAP::iterator it = map.end();
			for(;it != map.begin(); )
			{
				--it;
				if(it->second.timestamp < t)
				{
					pImp->_runner->team_invite_timeout(it->second.id.id);
					map.erase(it);
				}
			}
		}

		//�ͻ��˲���
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.size() >= player_team::INVITE_MAP::CAPACITY)
			{
				//������ �������ڲ��������Ҫ��
				pImp->_runner->error_message(S2C::ERR_TEAM_CANNOT_INVITE);
				return ;
			}

			if(map.find(member.id) != map.end())
			{
				//������ �������ڲ��������Ҫ��
				pImp->_runner->error_message(S2C::ERR_TEAM_ALREADY_INVITE);
				return ;
			}

			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = member;
			map[member.id] = it;

			//������Ϣ ������Ҽ������
			msg_team_invite team_invite;
			int pickup_flag = GetData(pTeam).pickup_flag;
			team_invite.pickup_flag = pickup_flag;
			team_invite.family_id = pImp->GetParent()->id_family; 
			team_invite.mafia_id = pImp->GetParent()->id_mafia;
			team_invite.zone_id = pImp->GetParent()->id_zone;

			// Youshuang add
			team_invite.level = ( pImp->GetRebornCount() == 0 ) ? pImp->GetParent()->level : 0;
			team_invite.sectid = pImp->GetParent()->sect_id;
			team_invite.referid = pImp->GetReferID();
			// end

			pImp->SendTo2<0>(GM_MSG_TEAM_INVITE,member,GetData(pTeam).self_seq, pImp->GetFaction(),  &team_invite, sizeof(team_invite));
		}

		void MsgApplyPartyReply(player_team* pTeam, const XID & leader, int seq)
		{
			player_team::team_entry & data = GetData(pTeam);
			data.last_invite_time  = g_timer.get_systime() + 10;
			CliAgreeInvite(pTeam,leader,seq);
		}
		
		//ֻ��normal��������������������
		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			player_team::team_entry & data = GetData(pTeam);
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(data.last_invite_time < g_timer.get_systime())
			{
				//���볬ʱ��
				pImp->_runner->error_message(S2C::ERR_TEAM_INVITE_TIMEOUT);
				return;
			}

			//����ȴ�״̬
			data.invite_map.clear();
			ChangeState(pTeam,player_team::TEAM_WAIT);
			data.time_out = TEAM_WAIT_TIMEOUT;
			GetTeam(pTeam).leader = leader;
			GetTeam(pTeam).team_seq = seq;

			player_team::agree_invite_entry prop;
			gplayer * pPlayer = (gplayer*)pImp->_parent;
			prop.data.Init(pImp);
			prop.cs_index = pPlayer->cs_index;
			prop.cs_sid = pPlayer->cs_sid;
			prop.cls = pPlayer->base_info.cls;
			prop.gender = pPlayer->base_info.gender;

			//������Ϣ��leader
			pImp->SendTo<0>(GM_MSG_TEAM_AGREE_INVITE,leader,seq,&prop,sizeof(prop));

		}

		virtual void CliRejectInvite(player_team * pTeam, const XID & leader)
		{
			player_team::team_entry & data = GetData(pTeam);
			if(data.last_invite_time < g_timer.get_systime())
			{
				//���볬ʱ�ˣ�ֱ�ӷ���
				return;
			}

			//���ﲻ����������Ǻ�ʱ�������Ϊͬʱ�����ж������

			//������Ϣ��leader
			gplayer_imp * pImp = data.imp;
			pImp->SendTo<0>(GM_MSG_TEAM_REJECT_INVITE,leader,0);
		}

		//��Ϣ
		void MsgAgreeInvite(player_team * pTeam,const XID & member,const A3DVECTOR & pos,const player_team::agree_invite_entry& prop,int seq)
		{
			//�ܵ�ĳ����ҷ�����ͬ��������Ϣ
			//��������Ƿ���invite�б���
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if(data.invite_map.end() == it || seq != data.self_seq)
			{
				//δ�ҵ����ʵ� ������ʱ
				return;
			}
			data.invite_map.erase(it);

			//��ɶ���, �������ģʽ
			//����Ա���뵽������
			data.min_timer_counter = 0;
			pTeam->BecomeLeader(member,prop,pos);
			data.time_out = TEAM_LEADER_UPDATE_INTERVAL;
			pTeam->_team.team_seq = data.self_seq;
			pTeam->_team.pickup_flag = data.pickup_flag;


			//������Ϣ���ö�Ա
			gplayer_imp * pImp = data.imp;
			pImp->_runner->join_team(pTeam->_team.leader,data.pickup_flag);
			int count = pTeam->_team.member_count;
			int pickup_flag = pTeam->_team.pickup_flag;
			size_t buf_size = sizeof(player_team::member_entry) * count + pImp->_cur_ins_key_list.size()*(sizeof(int)*3);
			char * buf = (char*)malloc(buf_size);
			size_t buf_header = sizeof(player_team::member_entry) * count;
			memcpy(buf,pTeam->_team.member_list,buf_header);
			bool bRst = pImp->GetInstanceKeyBuf(buf + buf_header, buf_size - buf_header);
			ASSERT(bRst);
			pImp->SendTo2<0>(GM_MSG_JOIN_TEAM,member,(count & 0x7FFF) |(pickup_flag << 16),
					pTeam->_team.team_gt_id, // add by sunjunbo 2012.10.29  param2��ʾ�����Ψһid
					buf,buf_size);
			free(buf);

			//���͸������ݸ��ö�Ա ע����ܻ��������Ϣ֮ǰ��.....
			
			//����������صĵ���
			PlayerTaskInterface  task_if(pImp);
			OnTeamSetup(&task_if);

			//ˢ����ӱ�־�����ڷ���
//			pImp->SendTeamVisibleStateToOther(member.id,prop.cs_index,prop.cs_sid); 

			pImp->NotifyMasterInfo();

			//�ӳ����»�Ծ�ȣ��������
			pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_TEAM);

			//֪ͨGT
			team_cache::instance.UpdateTeam(pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
		}

		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//�ܵ�ĳ����ҷ����˲�ͬ��������Ϣ
			//��������Ƿ���invite�б���
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if(data.invite_map.end() == it)
			{
				//û���ҵ�,ֱ�ӷ���
				return;
			}
			data.invite_map.erase(it);

			//���;ܾ���Ϣ
			data.imp->_runner->player_reject_invite(member);
		}

		void MsgInvite(player_team *pTeam, const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//�յ���һ���˷�����������Ϣ
			player_team::team_entry & data = GetData(pTeam);

			data.last_invite_time = g_timer.get_systime() + TEAM_INVITE_TIMEOUT2;
			data.imp->_runner->leader_invite(leader,seq,pickup_flag, familyid, mafiaid, level, sectid, referid);
		}
		
		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//do nothing
		}
};

class team_control_wait : public  std_team_control
{
	private:
		virtual void OnHeartbeat(player_team * pTeam) 
		{
			if(--(GetData(pTeam).time_out) <= 0)
			{
				//��ʱ��,�ص�normal״̬
				ChangeState(pTeam,player_team::TEAM_NORMAL);
				pTeam->_data.imp->_runner->error_message(S2C::ERR_TEAM_JOIN_FAILED);
			}
		}

		//�ͻ��˲���
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			ASSERT(false);
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		void CliRejectInvite(player_team * pTeam, const XID & member)
		{
			ASSERT(false);
		}

		//��Ϣ

		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//������,�Լ���Ҫ�����Ķ�����
			return ;
		}


		void MsgJoinTeam(player_team *pTeam, const XID & leader, const player_team::member_entry * list,size_t count, int pickup_flag,const void * ins_key , size_t ik_size, int team_gt_id)
		{
			player_team::team_t & team =  GetTeam(pTeam);
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(!(team.leader == leader))
			{
				//��ƥ��Ķӳ�
				//���ش�����Ϣ
				pImp->SendTo<0>(GM_MSG_JOIN_TEAM_FAILED,leader,0);
				return ;
			}

			//����������Ĳ���,���ڲ���֯�����б�,��Ϊ������û������
			team.member_count = 0;
			team.pickup_flag = pickup_flag;

			GetData(pTeam).min_timer_counter = 0;
			
			//�����Ա״̬ ,��������ṹ
			pTeam->BecomeMember(leader,list,count);
			pTeam->_team.team_gt_id = team_gt_id;
			pTeam->_data.time_out = TEAM_LEADER_TIMEOUT;
			__PRINTF("enter team: timeout%d\n",pTeam->_data.time_out);

			//�����Ͷӳ�һ����instance key
			pImp->SetInstanceKeyBuf(ins_key,ik_size);

			//֪ͨplayer�������״̬
			pImp->_runner->join_team(leader,pickup_flag);

			//�������е���ӿɼ����ݸ�����
			pImp->SendTeamDataToMembers();

			//��Ա���»�Ծ�ȣ��������
			pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_TEAM);

			pImp->NotifyMasterInfo();

		}

		void MsgAgreeInvite(player_team * pTeam, const XID & member,const A3DVECTOR &,const player_team::agree_invite_entry & prop,int seq)
		{
			//ʲô�����������Ե�
			//��Ϊֻ��leader����
		}

		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//do nothing
		}
};

class team_control_member : public  std_team_control
{
	private:
		virtual void OnHeartbeat(player_team * pTeam) 
		{
			gplayer_imp * pImp = GetData(pTeam).imp;
			//������ leader��ʱ�䲻����notify�Ļ�,���г�ʱ����
			//�����ʱ���Կ�����30������
			if((--GetData(pTeam).time_out) <= 0)
			{
				__PRINTF("��Ա���ֶӳ���ʱ���Զ��뿪����\n");
				//��ʱ�ˣ�ִ���뿪�����߼�
				MsgNotifyMemberLeave(pTeam, pTeam->_team.leader,pImp->_parent->ID,0);
				return ;
			}
			if((GetData(pTeam).min_timer_counter++) > 30)
			{
				//ÿ30�������󼶱�
				pTeam->CalcExactMaxLevel();
				GetData(pTeam).min_timer_counter = 0;

			}
		
			//���Ͷ������ݸ��ͻ���
			pTeam->SendTeamDataToClient();

			//������Ҫ�Ƿ�Ҫ��������
			team_mutable_prop data(pImp);
			if(pImp->GetRefreshState())
			{
				pTeam->SendSelfDataToMember(data);
			}
			else if(pTeam->_data.time_out & 0x01)	//ÿ���뷢��һ������
			{
				player_team::team_t & team =  GetTeam(pTeam);
				pImp->SendTo<0>(GM_MSG_MEMBER_NOTIFY_DATA,team.leader,0,&data,sizeof(data));
			}
		}


		//�ͻ��˲���
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			ASSERT(false);
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		void CliRejectInvite(player_team * pTeam, const XID & leader)
		{
			ASSERT(false);
		}

		bool CliLeaveParty(player_team * pTeam)
		{
			gplayer_imp * pImp = GetData(pTeam).imp;
			player_team::team_t & team =  GetTeam(pTeam);
			//�����뿪������Ϣ �ӳ�ͬ����뿪����
			pImp->SendTo<0>(GM_MSG_LEAVE_PARTY_REQUEST,team.leader,0);
			return true;
		}

		//��Ϣ
		void MsgAgreeInvite(player_team * pTeam, const XID & member,const A3DVECTOR &,const player_team::agree_invite_entry & prop,int seq)
		{
			//��״̬������˲���,������ʱ
			return;
		}

		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//������
			return ;
		}

		//��Ա�����Լ�������
		void MsgMemberUpdateData(player_team * pTeam,const XID & member, const A3DVECTOR &pos,const team_mutable_prop & data)
		{
			pTeam->UpdateMemberData(member,pos,data);
		}

		//�ӳ����´�ҵ�����
		void MsgLeaderUpdateData(player_team *pTeam,const XID & leader,const player_team::member_entry * list,size_t count)
		{
			pTeam->LeaderUpdateMembers(leader,list,count);
			//���¶ӳ���ʱ���
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;

			GetData(pTeam).imp->NotifyMasterInfo();
		}


		//�յ�leader���������˼�����Ϣ
		void MsgNewMember(player_team* pTeam,const XID& leader,const player_team::member_entry * list, size_t count)
		{
			ASSERT(count >= 2);
			if(leader != pTeam->_team.leader)  return ;
			pTeam->LeaderUpdateMembers(leader,list,count);
			const player_team::member_entry & ent = list[count-1];
			pTeam->_data.imp->_runner->new_member(ent.id);
			//���¶ӳ���ʱ���
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;
//			pTeam->_data.imp->SendTeamVisibleStateToOther(ent.id.id,ent.cs_index,ent.cs_sid);

                        gplayer* player = (gplayer*)pTeam->_data.imp->_parent;
			if(player->IsInvisible()) pTeam->_data.imp->SendAppearToTeamMember(ent.id.id,ent.cs_index,ent.cs_sid);
			return ;
		}

		void MsgNotifyMemberLeave(player_team * pTeam, const XID & leader,const XID & member,int type) 
		{
			//�յ�leader�������߳�ĳmember����Ϣ
			if(leader != pTeam->_team.leader)  return ;
			gplayer_imp * pImp = pTeam->_data.imp;
			if(member == pImp->_parent->ID)
			{

				//��������ϵͳ�Ľӿ�
				PlayerTaskInterface task_if(pImp);
				OnTeamMemberLeave(&task_if);
			
				//���Լ�,ֱ���뿪����
				pImp->_runner->leave_party(leader,type);
				pTeam->BecomeNormal();

				//����Ҫ�ؽ����еĸ���key
				//��Ϊ�����key��Ȼ����

				return ;
			}
			
			player_team::member_entry * pEntry = pTeam->FindMember(member);
			if(pEntry)
			{
				//ɾ���ö�Ա
				pTeam->MemberRemoveMember(pEntry);
				pImp->_runner->member_leave(leader,member,type);
			}
			pImp->NotifyMasterInfo();
		}

		//�յ�leader��������ȡ����ӵ���Ϣ
		virtual void MsgLeaderCancelParty(player_team *pTeam,const XID & leader,int seq)
		{
			if(leader != pTeam->_team.leader || seq != pTeam->_team.team_seq)  return;
			
			//��������ϵͳ�Ľӿ�
			PlayerTaskInterface task_if(pTeam->_data.imp);
			OnTeamMemberLeave(&task_if);


			pTeam->BecomeNormal();
			ChangeState(pTeam,player_team::TEAM_NORMAL);
			//	pTeam->_data.imp->_runner->leader_cancel_party(leader);
			pTeam->_data.imp->_runner->leave_party(leader,2);
			pTeam->_data.imp->NotifyMasterInfo();

		}

		virtual void LostConnection(player_team * pTeam)
		{
			CliLeaveParty(pTeam);
		}

		virtual void Logout(player_team * pTeam)
		{
			LostConnection(pTeam);
		}

		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//ת��������뵽�ӳ�����
			const XID & leader = pTeam->GetLeader();
			MSG msg;
			msg_apply_party msg_apply;
			msg_apply.faction = faction;
			msg_apply.family_id = familyid;
			msg_apply.mafia_id = mafiaid;
			// Youshuang add
			msg_apply.level = level;
			msg_apply.sectid = sectid;
			msg_apply.referid = referid;
			// end
			BuildMessage(msg, GM_MSG_TEAM_APPLY_PARTY, leader, who,A3DVECTOR(0,0,0), 0,  &msg_apply, sizeof(msg_apply)); 
			SendMessage<0>(pTeam->_data.imp->_plane,msg);
		}

		void MsgChangeToLeader(player_team* pTeam, const XID & leader)
		{
			if(leader != GetTeam(pTeam).leader)  return ;
			//��Ϣ�ķ����ߵ�ȷ��leader
			//����״̬���߼���ת�������Լ����leader��������ת����Ϣ�����еĶ��� 
			//���ҵ���player�Ķӳ�ת���߼�
			if(pTeam->FromMemberToLeader(leader))
			{
				MSG msg;
				BuildMessage(msg, GM_MSG_TEAM_LEADER_CHANGED, XID(-1,-1),GetData(pTeam).imp->_parent->ID,A3DVECTOR(0,0,0));
				//���Ǹ������еĶ�Ա�б�
				pTeam->SendGroupMessage(msg);
				
				GetData(pTeam).time_out = 1;	//һ���Ӻ󼴽������е����ݸ���
				//���ͻ��˷�����Ϣ
				gplayer_imp * pImp = pTeam->_data.imp;
				pImp->_runner->change_team_leader(leader,pImp->_parent->ID);
			}
		}

		void MsgLeaderChanged(player_team* pTeam, const XID & new_leader)
		{
			if(!pTeam->IsMember(new_leader))
			{
				//���˲��ڶ�����
				return ;
			}
			
			//��ʱ��㲥һ����Ϣ֪ͨ��Χ����Ʒ�޸������Ȩ��
			//���߿������30������ɱ���Ĺ���Ƿ�õ�����Ӿ��飩�� ������ٽ�����Ʒ������Ϣ�㲥
			//��������Ʒ������Ȩ��������team/seq��Ϊ�ж���׼
			//���ڲ�������������ˣ�����Ҳ������Ҫ����
			
			XID old_leader = pTeam->_team.leader;
			pTeam->ChangeLeader(new_leader);
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;

			//���ͻ��˷�����Ϣ
			gplayer_imp * pImp = pTeam->_data.imp;
			pImp->_runner->change_team_leader(old_leader,new_leader);
		}

		bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount)
		{
			return pTeam->DispatchTeamMoney(tag, pos, amount);
		}

};

class team_control_leader: public std_team_control
{
	private:

		virtual void OnHeartbeat(player_team * pTeam) 
		{
			long ts = g_timer.get_systime();
			gplayer_imp * pImp = GetData(pTeam).imp;
			XID self = pImp->_parent->ID;

			//���������ʱ������
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			player_team::INVITE_MAP::iterator it = map.end();
			for(;it != map.begin(); )
			{
				--it;
				if(it->second.timestamp < ts)
				{
					pImp->_runner->team_invite_timeout(it->second.id.id);
					map.erase(it);
				}
			}

			//���������ʱ������
			player_team::INVITE_MAP & apply_map = GetData(pTeam).apply_map;
			it = apply_map.end();
			for(;it != apply_map.begin(); )
			{
				--it;
				if(it->second.timestamp < ts)
				{
					apply_map.erase(it);
				}
			}
			
			//�����ʱ�ĸı�ӳ�����
			if(GetData(pTeam).change_leader_timeout)
			{
				if( (-- GetData(pTeam).change_leader_timeout) <= 0)
				{
					pImp->RebuildInstanceKey();
					__leave_party(pTeam);
					return;
				}
			}

			//������ member ��ʱ�䲻����notify�Ļ�,���г�ʱ����
			//�����ʱ���Կ�����10������(���߸��ӳ�)
			if(GetData(pTeam).time_out & 0x01)
			{
				MSG msg;
				MSG * pMsg = NULL;
				for(size_t i = 0 ; i < pTeam->_team.member_count; )
				{
					player_team::member_entry & member = pTeam->_team.member_list[i];
					if((member.timeout -= 2 ) <= 0 && member.id != self)
					{	
						//��ʱ,ɾ��֮
						if(!pMsg)
						{
							BuildMessage(msg,GM_MSG_MEMBER_LEAVE,XID(-1,-1),
									self,pImp->_parent->pos,member.id.id);
							pMsg = &msg;
						}
						SendGroupMessage(pTeam,*pMsg);
						pImp->_runner->member_leave(self,member.id,0);
						__PRINTF("�ӳ����ֶ�Ա%d��ʱ,���¶����б�\n",member.id.id);
						
						int member_id = member.id.id;
						//ȥ����Ա		
						pTeam->LeaderRemoveMember(&member);
						if(pTeam->_team.member_count > 1)
						{
							//��������������һ������������__leave_party�߼�
							//֪ͨ����ӿ�
							PlayerTaskInterface task_if(pImp);
							OnTeamDismissMember(&task_if,member_id);
						}


					}
					else
					{
						i ++;
					}
				}
			}

			//���ֻʣ���Լ������˳�����
			if(pTeam->_team.member_count <= 1)
			{
				__leave_party(pTeam);
				return;
			}

			//���ŷ��Ͷ������ݸ��ͻ���
			pTeam->SendTeamDataToClient();

			//ÿ30�������󼶱�
			if((GetData(pTeam).min_timer_counter++) > 30)
			{
				pTeam->CalcExactMaxLevel();
				GetData(pTeam).min_timer_counter = 0;

			}

			//�����Լ���λ�� 
			pTeam->_team.member_list[0].pos = pImp->_parent->pos;

			//�Ƿ�Ҫ��������
			if((--GetData(pTeam).time_out) <= 0)
			{
				//��������member���ݵ������������
				GetData(pTeam).time_out = TEAM_LEADER_UPDATE_INTERVAL;

				//���Ͷ�����Ϣ�����ж�Ա
				int count = pTeam->_team.member_count;
				MSG msg;
				BuildMessage(msg,GM_MSG_LEADER_UPDATE_MEMBER,XID(-1,-1),
						self,pImp->_parent->pos,
						count,pTeam->_team.member_list,
						sizeof(player_team::member_entry)*count);
				SendGroupMessage(pTeam,msg);
				pTeam->CalcMaxLevel();

				//����ȫ�����±�־��ʹ���Լ�Ҳ�����յ���������
				pTeam->_team.change_flag = player_team::team_t::TEAM_ALL_CHANGED;
			}
			else
			if(pImp->GetRefreshState())
			{
				team_mutable_prop data(pImp);
				//�����Լ���Ѫֵ�Ȳ���
				pTeam->_team.member_list[0].pos = pImp->_parent->pos;
				pTeam->_team.member_list[0].data = data;
				pTeam->SendSelfDataToMember(data);
			}
		}

		//�ͻ��˲���
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.size() + GetTeam(pTeam).member_count >= TEAM_MEMBER_CAPACITY)
			{
				//������ �������ڲ��������Ҫ��
				pImp->_runner->error_message(S2C::ERR_TEAM_CANNOT_INVITE);
				return ;
			}

			if(map.find(member.id) != map.end())
			{
				//���͹�������
				pImp->_runner->error_message(S2C::ERR_TEAM_ALREADY_INVITE);
				return ;
			}

			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = member;
			map[member.id] = it;

			//������Ϣ ������Ҽ������
			msg_team_invite team_invite;
			int pickup_flag = GetTeam(pTeam).pickup_flag;	//�ӵ�ǰ�Ķ�����ȡ��
			team_invite.pickup_flag = pickup_flag;
			team_invite.family_id = pImp->GetParent()->id_family; 
			team_invite.mafia_id = pImp->GetParent()->id_mafia;
			team_invite.zone_id = pImp->GetParent()->id_zone;

			// Youshuang add
			team_invite.level = ( pImp->GetRebornCount() == 0 ) ? pImp->GetParent()->level : 0;
			team_invite.sectid = pImp->GetParent()->sect_id;
			team_invite.referid = pImp->GetReferID();
			// end
			
			pImp->SendTo2<0>(GM_MSG_TEAM_INVITE,member,GetTeam(pTeam).team_seq, pImp->GetFaction(), &team_invite, sizeof(team_invite));
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		void CliRejectInvite(player_team * pTeam, const XID & leader)
		{
			ASSERT(false);
		}


		inline 	void __leave_party(player_team * pTeam)
		{
			//֪ͨ����ӿ� ,���������������ɢ֮ǰ��� ���������ǲ��Ǻ���?
			PlayerTaskInterface task_if(pTeam->_data.imp);
			OnTeamDisband(&task_if);

			if(GetData(pTeam).change_leader_timeout > 0)
			{
				//������ڵȴ����Ķӳ�����ȴ�ڣ����ؽ�����key
				pTeam->_data.imp->RebuildInstanceKey();
			}

			pTeam->_data.imp->_runner->leave_party(pTeam->_team.leader,0);
			//ת������״̬
			GetData(pTeam).self_seq ++;
			pTeam->BecomeNormal();

			//ͨ��GT�����ɢ
			team_cache::instance.RemoveTeam((int)pTeam->_team.team_gt_id);
		}

		bool CliLeaveParty(player_team * pTeam)
		{
			//����ͻ��˷������뿪���������
			//������Ҫ���ǽ�ɢ����
			gplayer_imp * pImp = GetData(pTeam).imp;
			//���������ɢ��Ϣ
			MSG msg;
			BuildMessage(msg,GM_MSG_LEADER_CANCEL_PARTY,XID(-1,-1),
					pImp->_parent->ID,pImp->_parent->pos,
					pTeam->_team.team_seq);
			SendGroupMessage(pTeam,msg);

			__leave_party(pTeam);
			return true;
		}

		void CliKickMember(player_team *pTeam, const XID & member)
		{
			player_team::member_entry * pEntry = pTeam->FindMember(member);
			if(pEntry)
			{
				//���ö�Ա�����߳�����Ϣ���뿪�ķ�ʽҲ����
				MSG msg;
				gplayer_imp *pImp = pTeam->_data.imp;
				XID self = pImp->_parent->ID;
				BuildMessage(msg,GM_MSG_LEADER_KICK_MEMBER,XID(-1,-1),
						self,pImp->_parent->pos,member.id);
				SendGroupMessage(pTeam,msg);

				//ɾ���ö�Ա
				pTeam->LeaderRemoveMember(pEntry);
				pImp->_runner->member_leave(self,member,1);

				//����ʣ���˼�����Ա 
				//�����һ������ô��ɢ����
				if(pTeam->_team.member_count <= 1)
				{
					__leave_party(pTeam);
					return ;
				}

				//֪ͨ����ӿ�
				PlayerTaskInterface task_if(pImp);
				OnTeamDismissMember(&task_if,member.id);
			}
		}

//��Ϣ
		void MsgAgreeInvite(player_team * pTeam, const XID & member,const A3DVECTOR & member_pos,const player_team::agree_invite_entry & prop,int seq)
		{
			//�ܵ�ĳ����ҷ�����ͬ��������Ϣ
			//��������Ƿ���invite�б���
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if( pTeam->_team.member_count >= TEAM_MEMBER_CAPACITY || data.invite_map.end() == it || seq != GetTeam(pTeam).team_seq)
			{
				//δ�ҵ����ʵ� ������ʱ
				return;
			}
			data.invite_map.erase(it);

			//����Ա���뵽������
			pTeam->LeaderAddMember(member,prop,member_pos);


			player_team::member_entry * list = pTeam->_team.member_list;
			int count = pTeam->_team.member_count;
			//���Ͷ�����Ϣ�����ж�Ա
			gplayer_imp * pImp = data.imp;
			const A3DVECTOR & pos = pImp->_parent->pos;
			XID self= pImp->_parent->ID;
			MSG msg;
			int pickup_flag = pTeam->_team.pickup_flag;

			size_t buf_size = sizeof(player_team::member_entry) * count + pImp->_cur_ins_key_list.size()*(sizeof(int)*3);
			char * buf = (char*)malloc(buf_size);
			size_t buf_header = sizeof(player_team::member_entry) * count;
			memcpy(buf,list,buf_header);
			bool bRst = pImp->GetInstanceKeyBuf(buf + buf_header, buf_size - buf_header);
			ASSERT(bRst);
			BuildMessage2(msg,GM_MSG_JOIN_TEAM,member,self,pos,(count & 0x7FFF) |(pickup_flag << 16),
					pTeam->_team.team_gt_id, // add by sunjunbo 2012.10.29  param2��ʾ�����Ψһid
					buf,buf_size);
			SendMessage<0>(pImp->_plane,msg);
			free(buf);

			msg.content_length = buf_header;
			msg.content = list;

			//���һ����Ա�϶����¶�Ա
			ASSERT(list[count-1].id == member);
			//������������Ա���������˽������Ϣ 
			msg.param = count;
			msg.message = GM_MSG_NEW_MEMBER;
			for(int i = 0; i < count - 1 ; i ++)
			{
				msg.target = list[i].id;
				SendMessage<0>(pImp->_plane,msg);
			}

			//�������ø���ʱ�䣨��Ϊ�Ѿ��������˷����˸�����Ϣ��
			data.time_out = TEAM_LEADER_UPDATE_INTERVAL;
			pTeam->_data.imp->_runner->new_member(list[count-1].id);

//			pImp->SendTeamVisibleStateToOther(member.id,prop.cs_index,prop.cs_sid);

			//֪ͨ����ϵͳ
			PlayerTaskInterface task_if(pImp);
			Task_OnTeamAddMember(&task_if,count - 1);

			//֪ͨGT
			team_cache::instance.UpdateTeam((int)pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
		}


		virtual void MsgJoinTeamFailed(player_team *pTeam, const XID & leader)
		{
			//��Ա�������ʧ��
			//���ö�Աɾ������֪ͨ������Ա
			return;
		}
		
		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//�ܵ�ĳ����ҷ����˲�ͬ��������Ϣ
			//��������Ƿ���invite�б���
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if(data.invite_map.end() == it)
			{
				//û���ҵ�,ֱ�ӷ���
				return;
			}
			data.invite_map.erase(it);

			//���;ܾ���Ϣ
			data.imp->_runner->player_reject_invite(member);
		}

		void SendGroupMessage(player_team *pTeam,MSG & msg)
		{
			player_team::member_entry * list = pTeam->_team.member_list;
			int count = pTeam->_team.member_count;
			for(int i = 0; i < count; i++)
			{
				if(list[i].id == msg.source) continue;

				msg.target = list[i].id;
				gmatrix::SendMessage(msg);
			}
		}


		void MsgMemberLeaveRequest(player_team * pTeam, const XID & member)
		{
			player_team::member_entry * pEntry = pTeam->FindMember(member);
			if(pEntry)
			{
				gplayer_imp *pImp = pTeam->_data.imp;
				//֪ͨ���ж�Ա�ö�Ա�뿪���뿪��Ҳʹ�������Ϣ���뿪����
				MSG msg;
				BuildMessage(msg,GM_MSG_MEMBER_LEAVE,XID(-1,-1),
						pImp->_parent->ID,pImp->_parent->pos,member.id);
				SendGroupMessage(pTeam,msg);

				pImp->_runner->member_leave(pImp->_parent->ID,member,0);

				// ɾ���ö�Ա
				pTeam->LeaderRemoveMember(pEntry);

				//����ʣ���˼�����Ա 
				//�����һ������ô��ɢ����
				if(pTeam->_team.member_count <= 1)
				{
					__leave_party(pTeam);
					return ;
				}

				//֪ͨ����ӿ�
				PlayerTaskInterface task_if(pImp);
				OnTeamDismissMember(&task_if,member.id);

				//֪ͨGT
				team_cache::instance.UpdateTeam((int)pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
			}
		}

		void MsgMemberUpdateData(player_team *pTeam, const XID & member,const A3DVECTOR & pos, const team_mutable_prop & prop)
		{
			//��Ա����������
			player_team::member_entry * pEntry = pTeam->UpdateMemberData(member,pos,prop);
			if(pEntry)
			{
				//���¶�Ա��ʱ���
				pEntry->timeout = TEAM_MEMBER_TIMEOUT;
			}
			else
			{
				gplayer_imp * pImp = pTeam->_data.imp;
				//����һ��������Ϣ
				pImp->SendTo<0>(GM_MSG_MEMBER_NOT_IN_TEAM,member,0);
			}
		}

		void MsgLeaderUpdateData(player_team *, const XID & leader, const player_team::member_entry *, size_t count)
		{
			ASSERT(false);
			return ;
		}

		bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount)
		{
			return pTeam->DispatchTeamMoney(tag, pos, amount);
		}

		void LostConnection(player_team * pTeam)
		{
			//������,��Ҫ���ӳ������ڶ�λ
			if(pTeam->_team.member_count >= 2)
			{
				XID new_leader = pTeam->_team.member_list[1].id;
				CliChangeLeader(pTeam, new_leader);
				pTeam->_data.imp->LazySendTo<0>(GM_MSG_LEAVE_PARTY_REQUEST,new_leader,0,37);
			}
			//��������key
			pTeam->_data.imp->RebuildInstanceKey();
		}

		void Logout(player_team * pTeam)
		{
			//�ǳ���������ҵ�instance_key,�϶������Լ���
			CliLeaveParty(pTeam);
		}
		
		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			if(pTeam->_team.member_count >= TEAM_MEMBER_CAPACITY) 
			{
				//�����Ƿ񷢳��޷����������
				return;
			}

			//�Լ���������
			player_team::INVITE_MAP & map = GetData(pTeam).apply_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.size() >= player_team::INVITE_MAP::CAPACITY) return;

			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = who;
			map[who.id] = it;

			pImp->_runner->send_party_apply(who.id, familyid, mafiaid, level, sectid, referid);
		}

		void CliAgreeApply(player_team *pTeam, int id, bool result)
		{
			//�Լ���������
			player_team::INVITE_MAP & map = GetData(pTeam).apply_map;
			player_team::INVITE_MAP & invite_map = GetData(pTeam).invite_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.find(id) == map.end())
			{
				//δ�ҵ������� 
				return;
			}

			if(!result) 
			{
				//���;ܾ����ݸ�������
				pImp->SendTo<0>(GM_MSG_ERROR_MESSAGE,XID(GM_TYPE_PLAYER,id),S2C::ERR_TEAM_REFUSE_APPLY);
				map.erase(id);
				return;
			}

			if(pTeam->_team.member_count >= TEAM_MEMBER_CAPACITY 
					|| invite_map.size() >= player_team::INVITE_MAP::CAPACITY)
			{
				//�����Ƿ񷢳��޷����������
				return;
			}

			//����ͬ���������Ϣ�����
			//�������б������һ��
			//������������
			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = XID(GM_TYPE_PLAYER,id);
			invite_map[id] = it;
			pImp->SendTo<0>(GM_MSG_TEAM_APPLY_REPLY,XID(GM_TYPE_PLAYER,id),pTeam->_team.team_seq);
		}

		virtual void CliChangeLeader(player_team * pTeam, const XID & new_leader)
		{
			//����Ŀ���Ƿ��Ա
			if(GetData(pTeam).change_leader_timeout) return;
			if(new_leader == pTeam->_team.leader) return;
			if(GetData(pTeam).change_leader_timeout) return;
			if(pTeam->IsMember(new_leader))
			{
				gplayer_imp * pImp = GetData(pTeam).imp;
				pImp->SendTo<0>(GM_MSG_TEAM_CHANGE_TO_LEADER,new_leader,0);
				pTeam->_data.change_leader_timeout = 25;
			}
		}

		void MsgLeaderChanged(player_team* pTeam, const XID & new_leader)
		{
			if(!pTeam->IsMember(new_leader))
			{
				//���˲��ڶ�����
				return ;
			}
			GetData(pTeam).change_leader_timeout = 0;
			pTeam->FromLeaderToMember(new_leader);
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;
			//��������ԭʼkey ���⸱������
			pTeam->_data.imp->RebuildInstanceKey();


			//���ͻ��˷�����Ϣ
			gplayer_imp * pImp = pTeam->_data.imp;
			pImp->_runner->change_team_leader(pImp->_parent->ID,new_leader);

			//֪ͨGT
			team_cache::instance.UpdateTeam((int)pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
		}
};

player_team::team_control * player_team::_team_ctrl[player_team::TEAM_STATE_NUM] =
{
	new team_control_normal,
	new team_control_wait,
	new team_control_member,
	new team_control_leader
};

void
player_team::Init(gplayer_imp * pPlayer)
{
	_data.imp = pPlayer;
	_data.self_seq = g_timer.get_systime();
}

void 
player_team::NotifyTeamPickup(const A3DVECTOR & pos, int type, int count)
{	
	//������ô��֪ͨ��....������������еĶ��ѵ�cs_index��sid�Ļ�������ֱ�ӷ������ݣ�Ҳδ������
	//����Ч�ʿ��ܱȽϸ�   �����������Ļ�Ҳ���Կ������ַ�ʽ
	//����������Ϣϵͳ�����У����Ҫ����cs_index��sid������Ҫ���ϴ�䶯

	MSG msg;
	BuildMessage(msg,GM_MSG_TEAM_PICKUP_NOTIFY,XID(-1,-1),_data.imp->_parent->ID,pos,type,&count,sizeof(count));
	SendGroupMessage(msg);
}
void            
player_team::TeamChat(char channel, char emote_id, const void * buf, size_t len, int srcid, const void * aux_data, size_t dsize )
{                       
	GMSV::chat_msg msg;
	int self = srcid;
	msg.speaker = self;
	msg.msg = buf;
	msg.size = len; 
	msg.data = aux_data;
	msg.dsize = dsize;
	msg.channel = channel;
	msg.emote_id = emote_id;
	size_t count = _team.member_count;
	for(size_t i = 0;i < count; i ++)
	{
		const member_entry & member = _team.member_list[i];
		GMSV::SendChatMsg(member.cs_index, member.id.id, member.cs_sid,msg);
	}               
}

void 
player_team::SendMessage(const MSG & msg, float range,bool norangelimit, int max_count)
{
	size_t count = _team.member_count;
	if(max_count > 0 && (size_t)max_count < count)
	{
		count = (int)max_count;
	}
	size_t index = 0;
	XID  list[TEAM_MEMBER_CAPACITY];
	ASSERT(msg.source == _data.imp->_parent->ID);
	range *= range;
	world *pPlane = _data.imp->_plane;
	for(size_t i = 0;i < count; i ++)
	{
		const XID & member =_team.member_list[i].id;
		if(!norangelimit)
		{
			world::object_info info;
			if(msg.source == member  || 
					!pPlane->QueryObject(member,info) || 
					info.pos.squared_distance(msg.pos) >= range)
			{
				continue;
			}
		}
		list[index++] = member;
	}
	if(index) gmatrix::SendMessage(list,list + index, msg);
}

void 
player_team::SendAllMessage(const MSG & msg, float range,bool norangelimit, int max_count)
{
	size_t count = _team.member_count;
	if(max_count > 0 && (size_t)max_count < count)
	{
		count = (int)max_count;
	}
	size_t index = 0;
	XID  list[TEAM_MEMBER_CAPACITY];
	ASSERT(msg.source == _data.imp->_parent->ID);
	range *= range;
	world *pPlane = _data.imp->_plane;
	for(size_t i = 0;i < count; i ++)
	{
		const XID & member =_team.member_list[i].id;
		if(!norangelimit)
		{
			world::object_info info;
			if(msg.source != member  &&
					(!pPlane->QueryObject(member,info) || 
					 info.pos.squared_distance(msg.pos) >= range))
			{
				continue;
			}
		}
		list[index++] = member;
	}
	if(index) gmatrix::SendMessage(list,list + index, msg);
}


bool player_team::Save(archive & ar)
{
	ar << _team_state << _data.time_out << _data.self_seq << _data.pickup_flag << _data.cur_max_level;
	ar << _data.invite_map.size();
	INVITE_MAP::iterator it = _data.invite_map.begin();
	long t = g_timer.get_systime();
	for(; it != _data.invite_map.end(); it ++)
	{
		ar << it->first << (int)(it->second.timestamp - t) << it->second.id;	
	}

	ar << _team.leader << _team.team_seq << _team.member_count << _team.change_flag << _team.pickup_flag;
	for(size_t i = 0; i < _team.member_count; i ++)
	{
		member_entry & ent = _team.member_list[i];
		ar << ent.id << ent.data << ent.cls << ent.gender << ent.cs_index << ent.cs_sid  << ent.pos << ent.timeout << ent.is_changed;
	}
	return true;
}

void 
player_team::Swap(player_team & rhs)
{
#define Set(var,cls) var = cls.var
	Set(_team_state , rhs);
	Set(_data.time_out,rhs);
	Set(_data.self_seq,rhs);
	Set(_data.pickup_flag,rhs);
	Set(_data.cur_max_level,rhs);
	Set(_data.cur_wallow_level,rhs);
	Set(_item_roll,rhs);
	INVITE_MAP::iterator it = rhs._data.invite_map.begin();
	_data.invite_map.clear();
	for(; it != rhs._data.invite_map.end(); it ++)
	{
		_data.invite_map[it->first] = it->second;
	}

	Set(_team.leader ,rhs);
	Set(_team.team_seq ,rhs);
	Set(_team.member_count ,rhs);
	Set(_team.change_flag ,rhs);
	Set(_team.pickup_flag ,rhs);

	for(size_t i = 0; i < _team.member_count; i ++)
	{
		_team.member_list[i] = rhs._team.member_list[i];
	}
#undef Set
}

bool player_team::Load(archive & ar)
{
	ar >> _team_state >> _data.time_out >> _data.self_seq >> _data.pickup_flag >> _data.cur_max_level;
	size_t size;
	ar >> size;
	_data.invite_map.clear();
	long t = g_timer.get_systime();
	for(size_t i = 0; i < size; i ++)
	{
		int id;
		invite_t inv;
		ar >> id >> (int&)(inv.timestamp) >> inv.id;
		inv.timestamp += t;
		_data.invite_map[id] = inv;
	}

	ar >> _team.leader >> _team.team_seq >> _team.member_count >> _team.change_flag >> _team.pickup_flag;
	for(size_t i = 0; i < _team.member_count; i ++)
	{
		member_entry & ent = _team.member_list[i];
		ar >> ent.id >> ent.data >> ent.cls >> ent.gender >> ent.cs_index >> ent.cs_sid  >> ent.pos >> ent.timeout >> ent.is_changed;
	}
	return true;
}

void 
player_team::DispatchExp(const A3DVECTOR &pos,int * list ,size_t count, int64_t exp,int level,int total_level, int max_level ,int min_level,int npcid,float r, bool no_exp_punish)
{
	ASSERT(IsLeader());

	//������߼�����гͷ�
	float exp_adj = 0;
	gplayer_imp * pImp = _data.imp;
	bool bReborn = pImp->GetRebornCount() > 0 ? true: false;
	player_template::GetExpPunishment(max_level - level,&exp_adj, bReborn, no_exp_punish);
	//�ṩ����ӳ�
	//�������20��û�мӳ�
	if(max_level - min_level < 30)
	{
		//����������ڱ����Ļ���ֵ�Ͻ��е��� 
		player_template::SetTeamBonus(count,_data.cls_count,&exp_adj);
	}
	exp = (int64_t)(exp * exp_adj + 0.5f);

	//���о������
	MSG msg;
	msg_grp_exp_t mexp;
	mexp.level = level;
	mexp.rand = r;
	BuildMessage(msg,GM_MSG_TEAM_EXPERIENCE,XID(-1,-1),_team.leader,pos,npcid,&mexp,sizeof(mexp));

	//total_level�����Ѿ������˲���С��20��������
	float factor = 1/(float)total_level;

	for(size_t i = 0;i < count; i ++)
	{
		int index = list[i];
		int level = _team.member_list[index].data.level;
		float factor2 = level * factor;
		mexp.exp = (int64_t)(exp * factor2 + 0.5f);
		msg.target = _team.member_list[index].id;
		SendMessage<0>(_data.imp->_plane,msg);
	}
}

void player_team::CalcRaceCount()
{
	_data.cls_count = 0;
	if(!IsInTeam()) return;
	size_t member_count = _team.member_count;
	int cls_mask = 0;
	int cls_mask1 = 0;
	for(size_t i = 0; i < member_count ; i++)
	{
		int cls = _team.member_list[i].cls & 0x7F;
		if(cls <= 0x3F)
		{
			int mask = (1 << cls);
			if((cls_mask & mask) == 0)
			{
				cls_mask |= mask;
				_data.cls_count ++;
			}
		}
		else
		{
			int mask = (1 << (cls - 0x40));
			if((cls_mask1 & mask) == 0)
			{
				cls_mask1 |= mask;
				_data.cls_count ++;
			}
		}
	}
}

int player_team::GetNearMemberCount(int tag, const A3DVECTOR & pos)
{
	int mlist_count = 0;
	//ȷ����������
	for(int i = 0; i < GetMemberNum(); i ++)
	{
		const player_team::member_entry &ent = GetMember(i);
		if(ent.pos.squared_distance(pos) > TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE || tag  !=ent.data.world_tag)
		{
			continue;
		}
		mlist_count ++;
	}
	return mlist_count;
}

void player_team::CalcMaxLevel()
{
	if(!IsInTeam()) return;
	CalcExactMaxLevel();
}

void 
player_team::CalcExactMaxLevel()
{
	int self = _data.imp->_parent->ID.id;
	A3DVECTOR pos = _data.imp->_parent->pos;
	size_t count = _team.member_count;
	world::object_info info;

	int tag = _data.imp->_plane->GetTag();

	int max_level = 0;
	int wallow_level = 0;
	for(size_t i = 0; i < count; i ++)
	{
		const member_entry & member = _team.member_list[i];
		if(self == member.id.id)
		{
			int level = _data.imp->_basic.level;
			if(level > max_level)  max_level = level;

			int wallow = _data.imp->GetWallowLevel();
			if(wallow > wallow_level) wallow_level = wallow;
			continue;
		}
		if(member.data.world_tag != tag)
		{
			continue;
		}
		if(member.pos.squared_distance(pos) >= TEAM_ITEM_DISTANCE*TEAM_ITEM_DISTANCE)
		{
			continue;
		}
		int level = member.data.level;
		if(level > max_level)  max_level = level;

		int wallow = member.data.wallow_level;
		if(wallow > wallow_level) wallow_level = wallow;
	}
	_data.cur_max_level = max_level;
	_data.cur_wallow_level = wallow_level;

}

void 
player_team::LeaderRemoveMember(member_entry * pEntry)
{
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	if(pPlayer->IsInvisible()) _data.imp->SendDisappearToTeamMember(pEntry->id.id,pEntry->cs_index,pEntry->cs_sid);

	DelRollMember(pEntry->id);
	size_t offset = (_team.member_list + _team.member_count) - pEntry;
	memmove(pEntry,pEntry + 1, offset * sizeof(member_entry));
	_team.member_count --;
	CalcRaceCount();
	CalcMaxLevel();
	_data.imp->NotifyMasterInfo();
}

void
player_team::MemberRemoveMember(member_entry * pEntry)
{
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	if(pPlayer->IsInvisible()) _data.imp->SendDisappearToTeamMember(pEntry->id.id,pEntry->cs_index,pEntry->cs_sid);
	size_t offset = (_team.member_list + _team.member_count) - pEntry;
	memmove(pEntry,pEntry + 1, offset * sizeof(member_entry));
	_team.member_count --;
}


void 
player_team::AddRollMember(const XID & who)
{
	size_t n = _item_roll.tail;
	if(n >= TEAM_MEMBER_CAPACITY) return;
	for(size_t i = 0; i < n; i ++)
	{
		if(_item_roll.list[i] == who) return;
	}
	_item_roll.list[n] = who;
	_item_roll.tail ++;
}

void 
player_team::DelRollMember(const XID & who)
{
	size_t n = _item_roll.tail;
	for(size_t i = 0; i < n; i ++)
	{
		if(_item_roll.list[i] == who)
		{
			for(size_t j = i; j < n - 1; j ++)
			{
				_item_roll.list[j] = _item_roll.list[j +1];
			}
			_item_roll.tail --;
			return ;
		}
	}
}

void player_team::ClearRollMember()
{
	_item_roll.tail = 0;
}

XID 
player_team::RollItem(const A3DVECTOR & pos, int tag)
{
	size_t n = _item_roll.tail;
	ASSERT(n > 0);
	XID self = _data.imp->_parent->ID;
	world * pPlane = _data.imp->_plane;
	world::object_info info;
	for(size_t i = 0; i < n ; i ++)
	{	
		XID who = _item_roll.list[i];
		if(self == who)
		{
			if(_data.imp->GetWorldTag() != tag) continue;
			if(_data.imp->_parent->pos.squared_distance(pos) >= TEAM_ITEM_DISTANCE*TEAM_ITEM_DISTANCE) continue;
		}
		else
		{
			if(!pPlane->QueryObject(who,info,true) || 
					info.pos.squared_distance(pos) >= TEAM_ITEM_DISTANCE*TEAM_ITEM_DISTANCE)
			{
				continue;
			}
			if(tag != info.tag) 
			{
				continue;
			}
			if(info.state & (world::QUERY_OBJECT_STATE_ZOMBIE|world::QUERY_OBJECT_STATE_DISCONNECT))
			{
				continue;
			}
		}

		//�ҵ��˵�һ�����ʵ� �����ƶ�������λ��
		for(size_t j = i; j < n - 1; j ++)
		{
			_item_roll.list[j] = _item_roll.list[j +1];
		}
		_item_roll.list[n - 1] =  who;
		return who;
	}
	return XID(0,0);
}

int
player_team::GetWallowLevel()
{
	if(IsInTeam())
	{
		return _data.cur_wallow_level;
	}
	return _data.imp->GetWallowLevel();

}

bool
player_team::DispatchTeamMoney(int tag, const A3DVECTOR & pos, int amount)
{
	if(amount <=0) return false;

	XID mlist[TEAM_MEMBER_CAPACITY];
	int mlist_count = 0;
	gplayer_imp * pImp = _data.imp;
	XID self = pImp->_parent->ID;

	//ȷ���ܷ�Ǯ���������
	for(int i = 0; i < GetMemberNum(); i ++)
	{
		const player_team::member_entry &ent = GetMember(i);
		if(ent.pos.squared_distance(pos) > TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE || tag  !=ent.data.world_tag)
		{
			continue;
		}
		mlist[mlist_count] = ent.id;
		mlist_count ++;
	}

	size_t count = mlist_count;
	if(count < 1) return false;

	//��ʼ�ַ���Ǯ
	size_t acount = (size_t)((double)amount / (double)count);
	if(acount > MONEY_CAPACITY_BASE) acount = MONEY_CAPACITY_BASE;

	amount -= acount * count;
	int ex_id = -1;
	if(amount > 0)
	{
		ex_id = abase::Rand(0,count -1);
	}
	
	MSG msg;
	BuildMessage(msg,GM_MSG_RECEIVE_MONEY,XID(-1,-1),pImp->_parent->ID,pos,acount);

	if(acount > 0)
	{
		//���ͽ�Ǯ���ݸ���ض�Ա
		for(size_t i =0; i < count; i ++)
		{
			msg.target = mlist[i];
			msg.param = acount;
			if(ex_id == (int)i)
			{
				msg.param += amount;
			}
			gmatrix::SendMessage(msg);
		}
	}
	else if(amount > 0 || ex_id >= 0)
	{
		//Ǯ���٣� ����������ݸ��ض���Ա
		msg.target = mlist[ex_id];
		msg.param = amount;
		gmatrix::SendMessage(msg);
	}
	return true;
}

bool player_team::BecomeLeader(const XID & first_member, const agree_invite_entry & prop,const A3DVECTOR &pos)
{
	if(_team_state != TEAM_NORMAL)
	{
		ASSERT(false);
		return false;
	}
	_team.leader = _data.imp->_parent->ID;
	_team.team_gt_id = team_t::GenerateTeamId(); // by sunjunbo 2012.10.30, �������Ψһid
	_team.member_count = 1;
	_team.member_list[0].data = team_mutable_prop(_data.imp);
	_team.member_list[0].id = _team.leader;
	_team.member_list[0].is_changed = false;
	_team.member_list[0].timeout  = 0;
	_team.member_list[0].pos = _data.imp->_parent->pos;
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	_team.member_list[0].cls = pPlayer->base_info.cls;
	_team.member_list[0].gender = pPlayer->base_info.gender;
	_team.member_list[0].cs_index = pPlayer->cs_index;
	_team.member_list[0].cs_sid  = pPlayer->cs_sid;

	ClearRollMember();
	AddRollMember(_team.leader);

	_data.change_leader_timeout = 0;
	_data.cur_max_level = _data.imp->_basic.level;
	_data.cur_wallow_level = _data.imp->GetWallowLevel();
	_team_state = TEAM_LEADER;
	LeaderAddMember(first_member,prop,pos);
	_team.change_flag = team_t::TEAM_ALL_CHANGED;
	pPlayer->object_state |= gactive_object::STATE_TEAM | gactive_object::STATE_TEAMLEADER;
	pPlayer->team_id = _team.leader.id;
	_data.imp->_runner->player_in_team(1);
	//ʹ���Լ��ĸ���key��Ϊ��Ӹ����б�
	_data.imp->_team_ins_key_list = _data.imp->_cur_ins_key_list;

	GLog::log(GLOG_INFO,"�û�%d�����˶���(%d,%d)", _team.leader.id, _team.leader.id,_team.team_seq);
	return true;
}

bool player_team::FromMemberToLeader(const XID & leader)
{
	if(_team_state != TEAM_MEMBER)
	{
		ASSERT(false);
		return false;
	}
	XID self = _data.imp->_parent->ID;

	_data.change_leader_timeout = 0;
	for(size_t i = 1; i < _team.member_count; i ++)
	{
		if(_team.member_list[i].id == self)
		{
			member_entry ent = _team.member_list[0];
			_team.member_list[0] = _team.member_list[i];
			ent.timeout = TEAM_MEMBER_TIMEOUT;
			_team.member_list[i] = ent;
			_team.leader = self;
			_team_state = TEAM_LEADER;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			CalcRaceCount();
			CalcMaxLevel();
			gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
			pPlayer->object_state |= gactive_object::STATE_TEAM | gactive_object::STATE_TEAMLEADER;
			pPlayer->team_id = _team.leader.id;
			//������б��Ǹ����б� , ������Щ�б�ͳ������ڱ������
			_data.imp->_cur_ins_key_list = _data.imp->_team_ins_key_list;
			_data.imp->_runner->player_in_team(1);

			GLog::log(GLOG_INFO,"�û�%d��Ϊ�ӳ�(%d,%d)", self.id, leader.id, _team.team_seq);


			//����roll list ���ǲ��������ʽ
			ClearRollMember();
			XID mlist[TEAM_MEMBER_CAPACITY];
			size_t mcount = _team.member_count;
			for(size_t i = 0; i < mcount; i ++)
			{
				mlist[i] = _team.member_list[i].id;
			}

			while(mcount != 0)
			{
				int index = abase::Rand(0,(int)(mcount - 1));
				AddRollMember(mlist[index]);
				abase::swap(mlist[index],mlist[mcount-1]);
				mcount --;
			}
			return true;
		}
	}
	return false;
}

bool player_team::ChangeLeader(const XID & new_leader)
{
	if(_team_state != TEAM_MEMBER)
	{
		ASSERT(false);
		return false;
	}
	bool bRst = false;
	for(size_t i = 1; i < _team.member_count; i ++)
	{
		if(_team.member_list[i].id == new_leader)
		{
			member_entry ent = _team.member_list[0];
			_team.member_list[0] = _team.member_list[i];
			_team.member_list[i] = ent;
			_team.leader = new_leader;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			gplayer * pPlayer = (gplayer *)(_data.imp->_parent);
			pPlayer->team_id = _team.leader.id;
			bRst = true;
		}
		_team.member_list[i].timeout =TEAM_MEMBER_TIMEOUT;
	}
	return bRst;
}

bool player_team::FromLeaderToMember(const XID & new_leader)
{
	if(_team_state != TEAM_LEADER)
	{
		ASSERT(false);
		return false;
	}
	for(size_t i = 1; i < _team.member_count; i ++)
	{
		if(_team.member_list[i].id == new_leader)
		{
			member_entry ent = _team.member_list[0];
			_team.member_list[0] = _team.member_list[i];
			_team.member_list[i] = ent;
			_team.leader = new_leader;
			_team_state = TEAM_MEMBER;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
			pPlayer->object_state &= ~(gactive_object::STATE_TEAM | gactive_object::STATE_TEAMLEADER);
			pPlayer->object_state |= gactive_object::STATE_TEAM;
			pPlayer->team_id = _team.leader.id;
			_data.imp->_runner->player_in_team(2);

			return true;
		}
	}
	return false;
}

bool player_team::BecomeMember(const XID & leader, const member_entry * list, size_t count)
{
	if(_team_state != TEAM_WAIT)
	{
		ASSERT(false);
		return false;
	}
	ASSERT(leader == _team.leader);
	_team.member_count = 0;
	_team_state = TEAM_MEMBER;
	_data.cur_max_level = _data.imp->_basic.level;
	_data.cur_wallow_level = _data.imp->GetWallowLevel();
	LeaderUpdateMembers(leader,list,count);

	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	pPlayer->object_state &= ~gactive_object::STATE_TEAMLEADER;
	pPlayer->object_state |= gactive_object::STATE_TEAM;
	pPlayer->team_id = _team.leader.id;
	_data.imp->_runner->player_in_team(2);
	GLog::log(GLOG_INFO,"�û�%d��Ϊ��Ա(%d,%d)",pPlayer->ID.id,leader.id,_team.team_seq);

	if(pPlayer->IsInvisible())_data.imp->SendAppearToTeam();
	return true;
}

void player_team::BecomeNormal()
{
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	GLog::log(GLOG_INFO,"�û�%d�������(%d,%d)",pPlayer->ID.id,_team.leader.id,_team.team_seq);
	if(pPlayer->IsInvisible())_data.imp->SendDisappearToTeam();

	_team_state = TEAM_NORMAL;
	_team.leader = XID(-1,-1);
	_team.member_count = 0;
	_team.team_seq = 0;
	_data.time_out = 0;
	_data.invite_map.clear();
	_data.apply_map.clear();
	pPlayer->object_state &= ~(gactive_object::STATE_TEAMLEADER | gactive_object::STATE_TEAM);
	pPlayer->team_id = 0;
	_data.imp->_runner->player_in_team(0);
}

bool player_team::LeaderAddMember(const XID & member,const agree_invite_entry  &prop,const A3DVECTOR &pos)
{
	if(_team_state != TEAM_LEADER) return false;
	if(_team.member_count < TEAM_MEMBER_CAPACITY)
	{
		gplayer* player = (gplayer*)_data.imp->_parent;
		if(player->IsInvisible()) _data.imp->SendAppearToTeamMember(member.id,prop.cs_index,prop.cs_sid);

		int index = _team.member_count;
		_team.member_list[index].id = member;
		_team.member_list[index].is_changed = true;
		_team.member_list[index].timeout =TEAM_MEMBER_TIMEOUT;
		_team.member_list[index].data = prop.data;
		_team.member_list[index].pos = pos ;
		_team.member_list[index].cls = prop.cls ;
		_team.member_list[index].gender = prop.gender ;
		_team.member_list[index].cs_index = prop.cs_index ;
		_team.member_list[index].cs_sid = prop.cs_sid ;
		_team.member_count ++;
		if(_data.cur_max_level < prop.data.level) _data.cur_max_level = prop.data.level;
		if(_data.cur_wallow_level < prop.data.wallow_level) _data.cur_wallow_level = prop.data.wallow_level;
		_team.change_flag = team_t::TEAM_PART_CHANGED;
		CalcRaceCount();
		CalcMaxLevel();
		AddRollMember(member);
		return true;
	}
	return false;
}

void player_team::SendSelfDataToMember(const team_mutable_prop & data)
{
	if(!IsInTeam()) return;
	MSG msg;
	BuildMessage(msg,GM_MSG_MEMBER_NOTIFY_DATA,XID(-1,-1),_data.imp->_parent->ID,_data.imp->_parent->pos,0,&data,sizeof(data));
	SendGroupMessage(msg);
}

void player_team::SendTeamDataToClient()
{
	switch(_team.change_flag)
	{
		case team_t::TEAM_NO_CHANGE:
			return;

		case team_t::TEAM_PART_CHANGED:
			{
				size_t member_count = _team.member_count;
				abase::vector<const member_entry *,abase::fast_alloc<> > list;
				list.reserve(member_count);
				for(size_t i = 0; i < member_count ; i++)
				{
					if(_team.member_list[i].is_changed)
					{
						list.push_back(_team.member_list + i);
						_team.member_list[i].is_changed = false;
					}
				}
				if(list.size())
				{
					_data.imp->SendTeamData(_team.leader,member_count,list.size(),list.begin());
				}
				_team.change_flag = team_t::TEAM_NO_CHANGE;
			}
			return;

		case team_t::TEAM_ALL_CHANGED:
			{
				size_t member_count = _team.member_count;
				if(member_count)
				{
					_data.imp->SendTeamData(_team.leader,member_count,_team.member_list);
				}

				for(size_t i = 0; i < member_count ; i++)
				{
					_team.member_list[i].is_changed = false;
				}
				_team.change_flag = team_t::TEAM_NO_CHANGE;
			}
			return;

		default:
			ASSERT(false);
			break;
	}

}

void player_team::SendGroupData(const void * buf, size_t size)
{
	size_t count = _team.member_count;
	int self = _data.imp->_parent->ID.id;
	for(size_t i = 0;i < count; i ++)
	{
		const member_entry &  ent = _team.member_list[i];
		if(self == ent.id.id) continue;
		send_ls_msg(ent.cs_index, ent.id.id, ent.cs_sid,buf,size);
	}
}

