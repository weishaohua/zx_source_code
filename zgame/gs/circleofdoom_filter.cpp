#include "circleofdoom_filter.h"
#include "clstab.h"
#include "player_imp.h"
#include "skill_filter.h"
#include "actsession.h"
#include "cooldowncfg.h"


DEFINE_SUBSTANCE(cod_sponsor_filter, filter, CLS_FILTER_COD_SPONSOR)
DEFINE_SUBSTANCE(cod_member_filter, filter, CLS_FILTER_COD_MEMBER)
DEFINE_SUBSTANCE(cod_cooldown_filter, filter, CLS_FILTER_COD_COOLDOWN)


//---------------------------------------------------------------------------------------------------------------------
//circle_of_doom sponsor
//
cod_sponsor_filter::cod_sponsor_filter(gactive_imp * imp, float radius, int faction, int self_faction_id, bool is_flymode, int filter_id, int skill_id)
: filter(object_interface(imp), MASK)
{
	_filter_id = filter_id;
	_faction = faction;
	_radius  = radius;
    _self_faction_id = self_faction_id;	
	_is_flymode = is_flymode;
	_team_member_count = 0;
	_skill_session_id = -1;
	_skill_id = skill_id;
}


cod_sponsor_filter::~cod_sponsor_filter()
{
	_filter_id = -1;
	_faction = -1;
	_radius  = -1;
	_self_faction_id = -1;
	_team_member_count = 0;
	_skill_session_id = -1;
}


void 
cod_sponsor_filter::Heartbeat(int tick)
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	if( pImp->IsCircleOfDoomPrepare() )
	{
		BroadcastCODPrepareMassge();

		//׼���׶ζ����Ա�����仯���ϼ��ܣ�����Ϊ�˷�ֹ����
		if(pImp->IsInTeam() && CheckTeamChange())
		{
			pImp->SendTo<0>(GM_MSG_CANCEL_BE_CYCLE, pImp->GetParent()->ID, _skill_session_id);
			pImp->_runner->error_message(S2C::ERR_CIRCLE_CANCEL_TEAM_CHANGE);
			
			//ɾ��filter
			_is_deleted = true;
		}
	}

	//��ӵ����
	if( object_interface::TEAM_CIRCLE_OF_DOOM == _faction )
	{
		if( !pImp->IsInTeam() )
		{
			pImp->CircleOfDoomClearMember();
		}
	}

	//����ģʽ�仯�����󣬵�->�ɡ���->��
	if( pImp->GetParent()->IsFlyMode() != _is_flymode )
	{
		pImp->CircleOfDoomClearMember();
	}

	//����������
	if( pImp->GetParent()->IsInvisible() )
	{
		pImp->CircleOfDoomClearMember();
	}

	//���Ƿ���
	if( !pImp->IsInCircleOfDoom() )
	{
		_is_deleted = true;	
	}

	//��̯���󷨽���
	if( pImp->IsMarketState() )
	{
		pImp->SendTo<0>(GM_MSG_CANCEL_BE_CYCLE, pImp->GetParent()->ID, -1);
		_is_deleted = true;
	}
}

void 
cod_sponsor_filter::OnAttach()
{
	BroadcastCODPrepareMassge();

	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	if( pImp->IsInTeam() )
	{
		_team_member_count = pImp->GetMemberList(_team_list);
		_skill_session_id = pImp->GetCurSessionID();
	}
}

void 
cod_sponsor_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->SetCircleOfDoomStop();
	_skill_session_id = -1;
}

bool 
cod_sponsor_filter::CheckTeamChange()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	XID list[TEAM_MEMBER_CAPACITY];

	int count = pImp->GetMemberList(list);
	if(count != _team_member_count) return true;

	for(int i = 0; i < count; i++)
	{
		bool is_change = true;
		for(int j = 0; j < _team_member_count; j++)
		{
			if(list[i].id == _team_list[j].id)
			{
				is_change = false;
			}
		}

		if(is_change) return true;
	}

	return false;
}

int
cod_sponsor_filter::BroadcastCODPrepareMassge()
{
	abase::vector<gobject*,abase::fast_alloc<> > list;
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	std::vector<exclude_target> empty;
	pImp->_plane->GetRegionPlayer(list, _parent.GetSelfID().id, _parent.GetPos(), _radius, empty);

	//team_id���ܱ䣬����ת�ƶӳ�
	if(object_interface::TEAM_CIRCLE_OF_DOOM == _faction)
	{
		_self_faction_id = pImp->GetTeamID();
	}

	prepare_param param;
	param.faction    = _faction;
	param.faction_id = _self_faction_id;
	param.radius     = _radius;
	param.is_flymode = _is_flymode;
	param.skill_id = _skill_id;

	abase::vector<gobject*,abase::fast_alloc<> >::iterator iter;
	iter = list.begin();
	for( iter = list.begin(); iter != list.end(); iter++ )
	{
		gplayer * pPlayer = (gplayer *)(*iter);
		if(NULL == pPlayer)
			continue;

		switch(_faction)
		{
			case object_interface::TEAM_CIRCLE_OF_DOOM:
				{
					if( pImp->IsInTeam() && (_self_faction_id == pPlayer->team_id) ) 
					{
						MSG msg;
						BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_PREPARE, (*iter)->ID, _parent.GetSelfID(), 
								_parent.GetPos(), 0, &param, sizeof(param) );
						gmatrix::SendMessage(msg);
					}
				}
				break;

			case object_interface::MAFIA_CIRCLE_OF_DOOM:
				{
					if( pImp->OI_GetMafiaID() > 0 && (_self_faction_id == pPlayer->id_mafia) )
					{
						MSG msg;
						BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_PREPARE, (*iter)->ID, _parent.GetSelfID(), 
								_parent.GetPos(), 0, &param, sizeof(param) );
						gmatrix::SendMessage(msg);
					}
				}
				break;

			case object_interface::FAMILY_CIRCLE_OF_DOOM:
				{
					if( pImp->OI_GetFamilyID() > 0 && (_self_faction_id == pPlayer->id_family) )
					{
						MSG msg;
						BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_PREPARE, (*iter)->ID, _parent.GetSelfID(), 
								_parent.GetPos(), 0, &param, sizeof(param) );
						gmatrix::SendMessage(msg);
					}
				}
				break;

			default:
				{
					__PRINTF("��circleofdoom_filter faction����!\n");
				}
				break;
		}
	}
	
	return list.size();
}



//----------------------------------------------------------------------------------------------------------------------
//circle_of_doom member
//
cod_member_filter::cod_member_filter(gactive_imp * imp, const XID & sponsor, const A3DVECTOR &pos, cod_param &param, int filter_id)
	: filter(object_interface(imp), MASK)
{
	_filter_id = filter_id;
	_sponsor_xid.type = sponsor.type;
	_sponsor_xid.id   = sponsor.id;
	_sponsor_pos = pos;

	_radius      = param.radius;
	_is_flymode  = param.is_flymode;
	_sponsor_faction    = param.faction;
}

cod_member_filter::~cod_member_filter()
{
	_filter_id = -1;
	_sponsor_xid.type = -1;
	_sponsor_xid.id   = -1;
}


void 
cod_member_filter::Heartbeat(int tick)
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();

	//����������顢ת�ƶӳ������,��prepare�׶�
	if( object_interface::TEAM_CIRCLE_OF_DOOM == _sponsor_faction )
	{
		if( pImp->IsInTeam() )
		{
			if( !pImp->IsMember(_sponsor_xid) )
			{
				LeaveCircleOfDoom();
				return ;
			}
		}
		else
		{
			LeaveCircleOfDoom();
			return ;
		}
	}

	//����ģʽ�仯�����󣬵�->�ɡ���->��
	//����������
	if( pImp->GetParent()->IsFlyMode() != _is_flymode || pImp->GetParent()->IsInvisible() )
	{
		LeaveCircleOfDoom();
		return;
	}

	//�������۵ľ���
	CheckDistance();

	//���Ƿ���
	if( !pImp->IsInCircleOfDoom() )
	{
		_is_deleted = true;	
	}

	//��̯���󷨽���
	if( pImp->IsMarketState() )
	{
		_is_deleted = true;	
	}
}


void 
cod_member_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	MSG msg;
	BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_ENTER, _sponsor_xid, pImp->_parent->ID, pImp->_parent->pos);
	gmatrix::SendMessage(msg);

	CheckDistance();
}

void 
cod_member_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->LeaveCircleOfDoom();
	MSG msg;
	BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_LEAVE, _sponsor_xid, pImp->_parent->ID, pImp->_parent->pos);
	gmatrix::SendMessage(msg);
}

void
cod_member_filter::CheckDistance()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	if( pImp->IsCircleOfDoomPrepare() )
	{
		float squared_radius = (_radius * _radius); 
		if(pImp->_parent->pos.squared_distance(_sponsor_pos) > squared_radius)
		{
			LeaveCircleOfDoom();
		}
	}
	else if(_parent.IsInvisible())
	{
		LeaveCircleOfDoom();
	}
	else
	{
		A3DVECTOR tmp_pos;
		float body_size;

		if( 1 == _parent.QueryObject(_sponsor_xid, tmp_pos, body_size) )
		{
			float squared_radius = (_radius * _radius); 
			if(pImp->_parent->pos.squared_distance(tmp_pos) > squared_radius)
			{
				LeaveCircleOfDoom();
			}
		}
		else
		{
			LeaveCircleOfDoom();
		}
	}
}


void 
cod_member_filter::LeaveCircleOfDoom()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->LeaveCircleOfDoom();
	MSG msg;
	BuildMessage(msg, GM_MSG_CIRCLE_OF_DOOM_LEAVE, _sponsor_xid, pImp->_parent->ID, pImp->_parent->pos);
	gmatrix::SendMessage(msg);
	_is_deleted = true;
}


cod_cooldown_filter::cod_cooldown_filter(gactive_imp * imp, int period, int filter_id) 
: timeout_filter(object_interface(imp), period, MASK)
{
	_filter_id = filter_id;
	_buff_id = GNET::HSTATE_COD_COOLDOWN; 
}


void 
cod_cooldown_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->SetCoolDown(COOLDOWN_INDEX_CIRCLE_OF_DOOM, 60000);

	for(size_t i = 0; i < 6; ++i)
	{
		_parent.EnhanceResistanceTenaciy(i, 50);
	}
	_parent.UpdateMagicData();
	
}

void 
cod_cooldown_filter::OnDetach()
{
	for(size_t i = 0; i < 6; ++i)
	{
		_parent.ImpairResistanceTenaciy(i, 50);
	}
	_parent.UpdateMagicData();
}

