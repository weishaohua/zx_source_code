#include "duel_filter.h"
#include "clstab.h"
#include "actobject.h"
#include "playertemplate.h"
#include <glog.h>
#include "sfilterdef.h"

DEFINE_SUBSTANCE(pvp_duel_filter,filter,CLS_FILTER_PVP_DUEL)

void 
pvp_duel_filter::OnAttach()
{
	_parent.DuelStart(_target);
	_parent.SendClientDuelStart(_target);
}

void 
pvp_duel_filter::OnDetach()
{
	if(!_is_release)
	{
		_is_release = true;
		_parent.DuelStop();
		_parent.SendClientDuelStop(_target);
		_parent.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);
	}
}

void 
pvp_duel_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	if(msg.ainfo.attacker == _target)
	{
		if(_mode)
		{
			//���������޵�
			msg.target_faction = 0;
			msg.force_attack  = 0;
		}
		else
		{
			//��δ����� ������ainfo �м����Ƿ���������
			msg.attacker_mode |= attack_msg::PVP_DUEL;
		}
	}
}       

void 
pvp_duel_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
	if(msg.ainfo.attacker == _target)
	{
		if(_mode)
		{
			if(!msg.helpful)
			{
				//���������޵�
				msg.target_faction = 0;
				msg.force_attack  = 0;
			}
		}
		else
		{
			if(msg.helpful)
			{
				msg.target_faction = 0xFFFFFFFF;
			}
			else
			{
				//��δ�����
				msg.attacker_mode |= attack_msg::PVP_DUEL;
			}
		}
	}
}

void
pvp_duel_filter::Heartbeat(int tick)
{
	if(!_mode) return;
	_mode --;
	if(_mode <= 0)
	{
	 	_is_deleted = true;
	}
}


void 
pvp_duel_filter::OnModify(int ctrlname, void * ctrlval, size_t ctrlen)
{
	if(ctrlname == FMID_DUEL_END)
	{
		_mode = 5;
		OnDetach();
	}
}

