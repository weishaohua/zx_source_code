#ifndef __ONLINEGAME_GS_BUFF_CREATURE_H__
#define __ONLINEGAME_GS_BUFF_CREATURE_H__

#include <stdlib.h>
#include <timer.h>
#include <arandomgen.h>
#include <vector.h>
#include <amemory.h>

#include <common/types.h>
#include "config.h"
#include "property.h"

class gactive_imp;
class active_buff			//������BUFF��Ҳ����DEBUFF ͬʱ��ά����ѡ����Ϣ
{
	bool _icon_state_flag;		//
	bool _gfx_state_flag;		//Ч��״̬�Ƿ�Ӧ��ˢ��
	unsigned char _subscibe_timer;
	abase::vector<buff_t_s, abase::fast_alloc<> >  _icon_state; 		//ͼ���б�
	abase::vector<buff_t, abase::fast_alloc<> > _self_icon_state; 	//buff�б�����б��ͼ���б�˳����Ҫ����һ��
	abase::vector<int, abase::fast_alloc<> > _gfx_state_list; 		//����ɼ�״̬�����ü����б�
	abase::vector<link_sid, abase::fast_alloc<> > _subscibe_list;		 //�����б�
	void UpdateVisibleState(gactive_imp * pImp); 			//����GFX״̬��Ϣ����ʱ����
	void UpdateSubscibeList(gactive_imp * pImp);			//���¶���״̬�� ��ʱ����
	void SendIconStateToOther(gactive_imp * pImp,int user_id,int cs_index, int cs_sid);
	void SendSelectInfoToPlayer(gactive_imp *pImp, int user_id, int cs_index, int cs_sid);
public:
	active_buff();

	inline void Update(gactive_imp * pImp)
	{
		UpdateVisibleState(pImp);
		UpdateSubscibeList(pImp);
		_icon_state_flag = false;
	}

	bool BuffExist(){ return !_icon_state.empty();}

	void InsertSubscibe(gactive_imp * pImp,const XID & target, const link_sid & ld);
	void RemoveSubscibe(const XID & target);
	void ClearSubscibeList(gactive_imp * pImp);
	void ExchangeSubscibeTo(gactive_imp * pImp, const XID & target, bool exchange, int latency);
	void ExchangeSubscibeFrom(gactive_imp *pImp, const void * buf, size_t size);

	void Swap(active_buff & rhs)
	{
		abase::swap(_icon_state_flag,rhs._icon_state_flag);
		abase::swap(_gfx_state_flag,rhs._gfx_state_flag);
		abase::swap(_subscibe_timer,rhs._subscibe_timer);
		_icon_state.swap(rhs._icon_state);
		_self_icon_state.swap(rhs._self_icon_state);
		_gfx_state_list.swap(rhs._gfx_state_list);
		_subscibe_list.swap(rhs._subscibe_list);
	}

public:
//����gfxЧ��������ͼ��Ч��
	inline void IncVisibleState(unsigned short state)
	{
		//���Ӽ���������ˢ�±�־ ע����������˳�������е�
		_gfx_state_flag =  ((++_gfx_state_list[state]) == 1) || _gfx_state_flag;
	}
	
	inline void DecVisibleState(unsigned short state)
	{
		_gfx_state_flag = ((--_gfx_state_list[state]) == 0) || _gfx_state_flag;
	}
	inline void ClearVisibleState(unsigned short state)
	{
		_gfx_state_flag = _gfx_state_flag || (_gfx_state_list[state] != 0);
		_gfx_state_list[state] = 0;
	}

	inline void UpdateBuff(short buff_id, short buff_level, int end_time, char overlay_cnt)
	{
		size_t count = _icon_state.size();
		for(size_t i = count ; i > 0; i--) 
		{
			if(_icon_state[i - 1].buff_level == buff_level && _icon_state[i - 1].buff_id == buff_id) 
			{
				if(_icon_state[i-1].overlay_cnt == overlay_cnt)
				{
					return;
				}
				else
				{
					_icon_state[i-1].overlay_cnt = overlay_cnt;
					_self_icon_state[i-1].overlay_cnt = overlay_cnt;
					_icon_state_flag = true;
					 return;
				}
			}
		}
		_icon_state.push_back(buff_t_s(buff_id,buff_level,overlay_cnt));
		_self_icon_state.push_back(buff_t(buff_id,buff_level,end_time,overlay_cnt));
		_icon_state_flag = true;
	}


	inline void RemoveBuff(short state, short buff_level)
	{
		size_t count = _icon_state.size();
		for(size_t i = count ; i > 0; i--) 
		{
			if(_icon_state[i-1].buff_id == state && _icon_state[i-1].buff_level == buff_level)
			{
				_icon_state.erase(_icon_state.begin() + (i-1));
				_self_icon_state.erase(_self_icon_state.begin() + (i-1));
				_icon_state_flag = true;
				return ;
			}
		}
	}

	inline void ClearBuff()
	{
		_icon_state_flag = false;
		_icon_state.clear();
		_self_icon_state.clear();
	}
	
	inline const  buff_t_s * GetBuffForElse(size_t & size) const
	{
		size = _icon_state.size();
		return _icon_state.begin();
	}

	inline  const buff_t * GetBuff(size_t & size) const
	{
		size = _self_icon_state.size() ;
		return _self_icon_state.begin();
	}
};

#endif

