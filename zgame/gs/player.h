#ifndef __ONLINEGAME_GS_PLAYER_H__
#define __ONLINEGAME_GS_PLAYER_H__

#include <time.h>
#include "object.h"
#include <common/packetwrapper.h>
#include "config.h"

struct gplayer  : public gactive_object
{
	enum{
		EMPTY 		= 0,
		WAITING_LOGIN 	= 1,
		WAITING_ENTER	= 2,
		LOGIN_OK	= 3,	//��½������״̬
		WAITING_LOGOUT	= 4,
		DISCONNECTED 	= 5,
		WAITING_SWITCH  = 6,
	};
	enum
	{
		TITLE_FREEMAN    = 0,
		TITLE_SYSTEM     = 1,
		TITLE_MASTER     = 2, //����
		TITLE_VICEMASTER = 3, //������
		TITLE_CAPTAIN    = 4, //����
		TITLE_HEADER     = 5, //����
		TITLE_MEMBER     = 6,
	};
	inline gplayer * get_next() { return (gplayer*)pNext;}
	inline gplayer * get_prev() { return (gplayer*)pPrev;}
	int	cs_index;		//���ӷ���������
	int 	cs_sid;			//���ӷ�����session ID
	int	login_state;		//��ǰ��¼״̬����δ����impʱʹ��
	int 	id_mafia;		//����id
	int 	id_family;		//����id
	int 	id_zone;		//������id
	int	extra_state;		//��չobject�е�object_state
	int	extra_state2;		//��չobject�е�object_state
	short	effect_list[MAX_PLAYER_EFFECT_COUNT];		//�����ǵ���Ŀ��gactive_object��������
	unsigned char rank_mafia;	//�����ڽ�ɫ( 2-���� 3-������ 4-���� 5-����)
	unsigned char market_id;	//��ǰ̯��
	unsigned char invisible;	//�Ƿ�����
	unsigned char rank;		//����, ����ս������Ч
	unsigned char vip_type;		//VIP������, Ĭ����0
	unsigned char rdir;		//������������Է���
	unsigned char circle_title;	//Ȧ��ְλ
	unsigned char vipaward_level; //vip�����ȼ�
	char	is_hide_vip_level;		//�Ƿ�����vip�ȼ�
	int		market_item_id;			//ʹ�õ��߰�̯ʱ���ͻ���ʹ����������ͬ������
	char	sect_flag;
	char	cultivation;		//�컯 ��ħ������
	char 	mount_type;		//��˵�����
	int	sect_id;
	int	circle_id;		//Ȧ��id
	int 	adv_data1;
	int 	adv_data2;
	char 	pk_level;
	char	level;			//��Ҽ��𼶱�
	char 	dt_level;		//��ҷ���ȼ�
	char 	bind_type;		//������ 1���� 2 ���� 
	char	mount_level;		//�����ļ���
	char	level2;			//ת������
	char 	bind_pos;		//���˰�ʱ���position
	unsigned char wing_color;	//�ɽ���ɫ
	bool bind_disconnect;		//��״̬���ڶϿ���״̬
	int	bind_target;		//�󶨵��� ��˭
	int 	mount_id;		//�������ʲô����
	int	title_id;		//���г�ν 
	int	reputation;		//����
	int	spouse_id;		//��ż
	int	team_id;		//����id
	int	fashion_mask;		//ʱװ״̬
	int 	transform_id;		//����ģ��ID
	int	carrier_id;		//��ͨ����ID
	A3DVECTOR rpos;			//�������������λ��
	A3DVECTOR before_mobactive_pos;     //����ǰλ��

	char darklight_state;	//�ͻ�����ʾ����ԯ�ⰵ״̬0�� �⣻1����
	char darklight_spirits[MAX_XUANYUAN_SPIRITS];	//��ԯ����
	char mirror_image_cnt;	//��������

	char start_fly_type;	//0: �ɽ�; 1:����

	char weapon_effect_level;

	short flow_battle_ckill;
	char puppet_id;		//����id
	int 	mobactive_id;		//������Ʒid 
	char mobactive_pos; 
	char mobactive_state;

	char active_emote_first_player;
	int active_emote_id;
	int active_emote_type;
	int travel_vehicle_id;
	
	bool	is_waitting_login() { return login_state == WAITING_LOGIN;}
	void Clear()
	{
		cs_sid = -1;
		cs_index = -1;
		login_state =  EMPTY;
		id_mafia = 0;
		rank_mafia = 0;
		id_family = 0;
		id_zone = 0;
		extra_state = 0;
		extra_state2 = 0;
		market_id = 0;
		ID.id = 0;
		base_info.cls = -1;
		base_info.gender = 0;
		invisible = 0;
		adv_data1 = adv_data2 = 0;
		bind_type  =0;
		level = 0;
		dt_level = 0;
		mount_level = 0;
		mount_type = 0;
		bind_target = 0;
		bind_disconnect = 0;
		bind_pos = 0;
		mount_id = 0;
		title_id = 0;
		reputation = 0;
		spouse_id = 0;
		team_id = 0;
		cultivation = 0;
		fashion_mask = 0;
		rank = 0;
		vip_type = 0;
		transform_id = 0;
		carrier_id = 0;
		rdir = 0;
		circle_id = 0;
		sect_id = 0;
		circle_title = 0;
		vipaward_level = 0;
		is_hide_vip_level = 0;
		market_item_id = -1;
		darklight_state = 0;
		wing_color = 0;
		memset(darklight_spirits, 0, sizeof(darklight_spirits));
		mirror_image_cnt = 0;
		start_fly_type = -1;
		weapon_effect_level = 0;
		flow_battle_ckill = 0;
		puppet_id = 0;
		mobactive_id = 0;
		mobactive_pos = 0;
		mobactive_state = 0;
		active_emote_id = 0;
		active_emote_first_player = 0;
		active_emote_type = -1;
		travel_vehicle_id = 0;
		gactive_object::Clear();
	}

	template <typename WRAPPER>
	WRAPPER &Export(WRAPPER & wrapper)
	{
		gactive_object::Export(wrapper);
		wrapper << ID << pos << msg_mask << crc << 
		id_mafia << rank_mafia << id_family << id_zone << market_id << invisible;
		wrapper.push_back(&base_info,sizeof(base_info));
		wrapper<< body_size << 
		cs_sid << cs_index << login_state << 
		adv_data1 << adv_data2 << mount_type << sect_id << circle_id << level << dt_level << 
		bind_type << mount_level << bind_pos << bind_disconnect <<
		bind_target << mount_id << title_id <<reputation << spouse_id<< team_id <<  
		rank << vip_type << transform_id << carrier_id <<  extra_state << extra_state2 << rdir << rpos << before_mobactive_pos <<
		circle_title << vipaward_level << is_hide_vip_level << darklight_state << mirror_image_cnt 
		<< market_item_id << wing_color << start_fly_type << weapon_effect_level << flow_battle_ckill << puppet_id << 
		mobactive_id << mobactive_pos << mobactive_state << active_emote_first_player << active_emote_id << active_emote_type << travel_vehicle_id;
		wrapper.push_back(effect_list,sizeof(effect_list));
		return wrapper.push_back(darklight_spirits,sizeof(darklight_spirits));
	}

	template <typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		gactive_object::Import(wrapper);
		wrapper >> ID >> pos >> msg_mask >> crc >> 
		id_mafia >> rank_mafia >> id_family >> id_zone >> market_id >> invisible ;
		wrapper.pop_back(&base_info,sizeof(base_info));
		wrapper >> body_size >>
		cs_sid >> cs_index >> login_state >> 
		adv_data1 >> adv_data2 >> mount_type >> sect_id >> circle_id >> level >> dt_level >>
		bind_type >> mount_level >> bind_pos >> bind_disconnect >>
		bind_target >> mount_id >> title_id >> reputation >> spouse_id >> team_id >>  
		rank >> vip_type >> transform_id >> carrier_id >>  extra_state >> extra_state2 >>  rdir >> rpos >> before_mobactive_pos >>
	       	circle_title >> vipaward_level >> is_hide_vip_level >> darklight_state >> mirror_image_cnt 
		>> market_item_id >> wing_color >> start_fly_type >> weapon_effect_level >> flow_battle_ckill >>puppet_id >>  
		mobactive_id >> mobactive_pos >> mobactive_state >> active_emote_first_player >>  active_emote_id >> active_emote_type >> travel_vehicle_id;
		wrapper.pop_back(effect_list,sizeof(effect_list));
		return wrapper.pop_back(darklight_spirits,sizeof(darklight_spirits));
	}

	//��Щ״̬����������player����
	enum
	{
		STATE_CARRIER		= 0x00000001,	//������ͨ����
		STATE_ANONYMOUS		= 0x00000002,	//����״̬
		STATE_INVISIBLE		= 0x00000004,	//Ӱ��״̬
		STATE_DIM		= 0x00000008,	//����״̬
		STATE_CLONE		= 0x00000010,	//����״̬
		STATE_CIRCLE		= 0x00000020,	//Ȧ��״̬
		STATE_CROSSZONE		= 0x00000040,	//���״̬
		STATE_IGNITE		= 0x00000080,	//ȼ��״̬
		STATE_FROZEN		= 0x00000100,	//����״̬
		STATE_FURY		= 0x00000200,	//��ʥ֮ŭ״̬
		STATE_DARK		= 0x00000400,	//�ڰ�״̬
		STATE_COLDINJURE	= 0x00000800,	//����״̬
		STATE_CIRCLEOFDOOM  = 0x00001000,   //��״̬��ֻ�����󷨷��������ϲ���
		STATE_TURNBUFF		= 0x00002000,	//��ת����
		STATE_TURNDEBUFF	= 0x00004000,	//��ת����
		STATE_BLOODTHIRSTY	= 0x00008000,	//��Ѫ״̬
		STATE_BESPIRITDRAGGED= 0x00010000,	//��ǣ��״̬
		STATE_BEPULLED		= 0x00020000,	//������״̬
		STATE_VIPAWARD		= 0x00040000,   //VIP����״̬
		STATE_IN_COLLISION_RAID = 0x00080000,	//����ײս��
		STATE_XY_DARKLIGHT_STATE = 0x00100000, //��ԯ�Ĺⰵ״̬
		STATE_XY_SPIRITS	= 0x00200000,	//��ԯ����
		STATE_MIRROR_IMAGE	= 0x00400000,	//����
		STATE_WING_COLOR	= 0x00800000,	//�ɽ���ɫ
		STATE_DIR_EXT_STATE = 0x01000000,	//���������չ״̬
		STATE_EXTRA_EQUIP_EFFECT = 0x02000000,	//װ�������Ч
		STATE_FLOW_BATTLE	= 0x04000000,	//��ˮϯս��״̬
		STATE_PUPPET		= 0x08000000,	//������̬	
		STATE_QILIN		= 0x10000000,	//������̬
		STATE_MOBACTIVE 	= 0x20000000,   //������Ʒ״̬
		STATE_ACTIVE_EMOTE      = 0x40000000,   //��������
		STATE_TRAVEL		= 0x80000000,   //���ɷ���
	};

	enum        
	{   
		STATE_SEEKRAID_DIE = 0x00000001,
		STATE_CAPTURE_RAID_FLAG = 0x00000002,
	};  

	//��ײս����ص�STATE
	enum
	{
		STATE_COLLISION_ASHILL	= 0x00000001,	//������ɽ
		STATE_COLLISION_NOMOVE	= 0x00000002,	//����
		STATE_COLLISION_CHAOS	= 0x00000004,	//����
		STATE_COLLISION_VOID    = 0x00000008,	//����

	};
	enum
	{
		DLTYPE_LIGHT = 0,
		DLTYPE_DARK,
	};
	
	enum
	{
		SPIRIT_TYPE_NONE = 0,
		SPIRIT_TYPE_DARK,
		SPIRIT_TYPE_LIGHT,
		SPIRIT_TYPE_DARKLIGHT,
		SPIRIT_TYPE_NUM,
	};

	inline void SetExtraState(int state)
	{
		extra_state |= state;
	}

	inline void ClrExtraState(int state)
	{
		extra_state &= ~state;
	}

	inline bool CheckExtraState(int state) const
	{
		return extra_state & state;
	}

        inline void SetExtraState2(int state)
        {   
                extra_state2 |= state;
        }   

        inline void ClrExtraState2(int state)
        {   
                extra_state2 &= ~state;
        }   

        inline bool CheckExtraState2(int state) const
        {   
                return extra_state2 & state;
        }   
	
	inline bool IsCarrierMode() const
	{
		return CheckExtraState(STATE_CARRIER);
	}

	inline bool IsAnonymousMode() const
	{
		return CheckExtraState(STATE_ANONYMOUS);
	}

	inline bool IsInvisible() const
	{
		return CheckExtraState(STATE_INVISIBLE);
	}

	inline bool IsCloneMode() const
	{
		return CheckExtraState(STATE_CLONE);
	}

	inline bool IsIgniteState() const
	{
		return CheckExtraState(STATE_IGNITE);
	}

	inline bool IsFrozenState() const
	{
		return CheckExtraState(STATE_FROZEN);
	}
	
	inline bool IsColdInjureState() const
	{
		return CheckExtraState(STATE_COLDINJURE);
	}

	inline bool IsFuryState() const
	{
		return CheckExtraState(STATE_FURY);
	}

	inline bool IsTurnBuffState() const
	{
		return CheckExtraState(STATE_TURNBUFF);
	}

	inline bool IsTurnDebuffState() const
	{
		return CheckExtraState(STATE_TURNDEBUFF);
	}

	inline bool IsBloodThirstyState() const
	{
		return CheckExtraState(STATE_BLOODTHIRSTY);
	}

	inline bool IsBePulledState() const
	{
		return CheckExtraState(STATE_BEPULLED);
	}

	inline bool IsBeSpiritDraggedState() const
	{
		return CheckExtraState(STATE_BESPIRITDRAGGED);
	}

	inline bool IsXYDarkLightState() const
	{
		return CheckExtraState(STATE_XY_DARKLIGHT_STATE);
	}
	
	inline bool IsXYSpiritsState() const
	{
		return CheckExtraState(STATE_XY_SPIRITS);
	}
	
	inline bool IsMirrorImageState() const
	{
		return CheckExtraState(STATE_MIRROR_IMAGE);
	}

	inline bool IsFlowBattleState() const
	{
		return CheckExtraState(STATE_FLOW_BATTLE);
	}

	inline bool IsPuppetState() const
	{
		return CheckExtraState(STATE_PUPPET);
	}

	inline bool IsQilinState() const
	{
		return CheckExtraState(STATE_QILIN);
	}

	inline bool IsMobAactiveState() const
	{
		return CheckExtraState(STATE_MOBACTIVE);
	}



public:
	
};

inline bool make_link_sid(gplayer * dest, link_sid & id)
{
	id.cs_id = dest->cs_index;
	id.cs_sid = dest->cs_sid;
	id.user_id = dest->ID.id;
	return id.cs_id >= 0;
}

//�����������������key���ڽ��븱������ʱ��Ҫ����key
struct  instance_key
{
	struct key_essence
	{
		int key_level1;			//һ��key�� ��Щ������Ҫ��� ��׼key���Լ���ID
		struct 
		{
			int first;
			int second;
		}key_level2;			//����key����Щ������Ҫ���  ��׼key���Լ��Ķ���ID��seq
		int key_level3;			//����key����Щ������Ҫ���  ��׼key���Լ��İ���ID
		int key_level4;			//�ļ�key����Щ������Ҫ���������Ϊר�и���׼���ģ������ս��
	}; 
	
	key_essence essence;
	key_essence target;
};

template <typename WRAPPER >
WRAPPER & operator << (WRAPPER & wrapper ,const instance_key & rhs)
{
	return wrapper.push_back(&rhs, sizeof(rhs));
}

template <typename WRAPPER >
WRAPPER & operator >>  (WRAPPER & wrapper ,instance_key & rhs)
{
	return wrapper.pop_back(&rhs, sizeof(rhs));
}

struct instance_hash_key
{
	int key1;
	int key2;
	bool operator ==(const instance_hash_key & rhs) const
	{
		return key1 == rhs.key1 && key2 == rhs.key2;
	}
};

struct instance_hash_function
{
	inline long operator()(const instance_hash_key & key) const
	{
		return key.key1 ^ key.key2;
	}
};

struct player_var_data
{
	enum 
	{
		BASIC_VAR_DATA_SIZE = 16,
		CUR_VERSION	= 1,
	};
	int version;
	int mafia_death;	//��ս��ɱ�����ۼ�
	char dead_flag;		//�Ƿ�����״̬(3��)
	bool resurrect_state;	//����״̬(�Ƿ��ԭ�ظ���)
	float resurrect_exp_reduce; //����״̬ʱ��ʧ��exp����ԭ�ظ�������Ч)
	instance_hash_key ins_key;  //�ϴ��˳�ʱ���ڵĸ���hash key
	int last_instance_timestamp;//���һ�ν��븱����ʱ���
	int last_instance_tag;	    //���һ�ν��븱����ID
	A3DVECTOR last_instance_pos;//���һ�ν��븱��������
	int dir;		    //�������ʱ�ķ���

	template <typename PLAYER_IMP>
	void MakeData(gplayer * pPlayer, PLAYER_IMP * pImp)
	{
		version = CUR_VERSION;
		mafia_death = pImp->GetMafiaDuelDeaths();
		dead_flag =  pImp->GetDeadFlag();
		float hp_recover = 0.1f;	//���ֵ�Ͳ�������
		resurrect_state = pImp->GetResurrectState(resurrect_exp_reduce, hp_recover);
		ins_key = pImp->_plane->w_ins_key;
		pImp->GetLastInstancePos( last_instance_tag, last_instance_pos,last_instance_timestamp);
		dir = pPlayer->dir;

	}

	template <typename PLAYER_IMP>
	static void SetData(gplayer * pPlayer, PLAYER_IMP * pImp, const void * buf, size_t size)
	{
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case CUR_VERSION:
				{
					pImp->SetDeadFlag(pVar->dead_flag);
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce);
					pPlayer->dir = pVar->dir;
					pImp->SetMafiaDuelDeaths(pVar->mafia_death);
				}
				break;
		}
	}

	template <typename INSTANCE_KEY>
	static void GetInstanceKey(const void * buf, size_t size,INSTANCE_KEY & key)
	{
		memset(&key,0,sizeof(key));
		if(size < sizeof(int)) 
		{
			key.key1 = 0;
			key.key2 = time(NULL);
			return;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case CUR_VERSION:
				{
					key = pVar->ins_key;
				}
				return ;
		}
		key.key1 = 0;
		key.key2 = time(NULL);
		return;
	}

	template <int >
	static void GetLastInstance(const void * buf, size_t size,int &last_ins_tag, A3DVECTOR & pos,int & create_ts)
	{
		if(size < sizeof(int)) 
		{
			last_ins_tag = -1;
			create_ts = -1;
			return;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case CUR_VERSION:
				{
					last_ins_tag = pVar->last_instance_tag;
					pos = pVar->last_instance_pos;
					create_ts = pVar->last_instance_timestamp;
				}
				return ;
		}
		last_ins_tag = -1;
		create_ts = -1;
		return;
	}

	enum 
	{
		ALIVE 	= 0,
		KILLED_BY_PLAYER = 1,
		KILLED_BY_NPC = 2,
	};

};
enum
{
	PLAYER_MONSTER_TYPE = 256
};

#endif

