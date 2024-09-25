#ifndef __ONLINEGAME_GS_OBJECT_H__
#define __ONLINEGAME_GS_OBJECT_H__

#include <spinlock.h>
#include <common/types.h>
#include <common/packetwrapper.h>
#include <ASSERT.h>

#include "config.h"


class world;
class gobject_imp;
struct slice;
struct gobject
{
	enum 
	{
		MSG_MASK_PLAYER_MOVE  	= 0x0001,
//		MSG_MASK_NPC_MOVE	= 0x0002,
//		MSG_MASK_MATTER_DROP	= 0x0004,
//		MSG_MASK_PLAYER_DEATH	= 0x0008,
//		MSG_MASK_ACTIVE		= 0x0010,
		MSG_MASK_ATTACK		= 0x0020,
		MSG_MASK_ENCHANT	= 0x0040,
		MSG_MASK_CRY_FOR_HELP	= 0x0080,
	};

	int	spinlock;	//������
#ifdef _DEBUG
	int 	cur_msg;	//���һ��ִ�е���Ϣ��
#endif
	unsigned int crc;	//���ܵ�crcУ��ֵ
	bool 	b_chk_state;	//��ʾ�ö����Ƿ�ʹ���еļ����
	bool 	b_zombie;	//�Ƿ�ʬ״̬
	bool	b_disconnect;	//�Ƿ��Ѿ����ߣ�ֻ��playerʹ��
	unsigned char dir;	//���������ķ���
	int	msg_mask;	//������Ϣ�ı�־
	gobject * pPrev;	//ǰһ��ָ��
	gobject * pNext;	//��һ��ָ��
	slice 	* pPiece;	//�����Ŀ�ָ��
	int	collision_id;	//��ײ��־λ
	int	tag;		//������������ID
	int	w_serial;	//�����������������к�
	world   * plane;	//��������������ָ��
	A3DVECTOR pos;		//����������λ��
	XID 	ID;		//id,�����û���ʹ��uid,����������Ʒ��NPC��ʹ�� 2λ��־ | 14λ world_index | 16λ����
	gobject_imp * imp;	//ʵ�ʵľ���ʵ��
	float	body_size;	//����Ĵ�С�߶� 
public:
	inline void Lock() {mutex_spinlock(&spinlock);}
	inline void Unlock() {mutex_spinunlock(&spinlock);}
	inline int  TryLock() { return mutex_spinset(&spinlock);}
	inline bool IsEmpty() { return !b_chk_state;}
	inline bool IsActived() { return b_chk_state;}
	inline bool IsZombie() { return b_zombie;}
	inline void SetActive() { b_chk_state = true;}
	inline void ClrActive() { b_chk_state = false;}
	inline void Clear()
	{
		crc = 0;
		b_chk_state = false; 
		b_zombie = false;
		dir = 0;
		msg_mask = 0;
		ID.id = -1;
		ID.type = -1;
		imp = NULL;
		body_size = 0.f;
		b_disconnect = false;
		collision_id = 0;
	}

	inline void SetWorld(int world_tag, int world_serial)
	{
		ASSERT(spinlock);
		tag		= world_tag;
		w_serial 	= world_serial;
	}
};

struct object_base_info
{
	short cls;			//ְҵ
	short gender;			//�Ա�
	int faction;			//��ϵ
	int level;			//����
	int hp;				//hp
	int max_hp;			//mp
	int mp;				//���� 

	static bool CheckCls(int c, int64_t mask1, int64_t mask2)
	{
		int cc = c & 0x7F;
		return (cc <= 0x3F) ? (((int64_t)1 << cc) & mask1) : (((int64_t)1 << (cc - 0x40)) & mask2);
	}
};


struct gactive_object : public gobject
{
	object_base_info base_info;
	unsigned int object_state;	//�����ʾ�����״̬
	unsigned char shape_form;	//��������
	unsigned char emote_form;	//��������
	unsigned char effect_count;
	unsigned char npc_reborn_flag;	//��NPC���ԣ���������ʱ��Ҫ��������
	int64_t extend_state;
	int 	invisible_rate;		//��������
	int 	anti_invisible_rate;	//����������

	int extend_dir_state;			//����������Ч��, ���32��
	short extend_dir_state_dirs[32];	//����������Ч���ķ������32��

	enum 
	{
		MALE = 0,
		FEMALE = 1,
	};

	template<typename WRAPPER>
	WRAPPER & Export(WRAPPER & wrapper)
	{
		wrapper << object_state << shape_form << effect_count << extend_state << invisible_rate << anti_invisible_rate << extend_dir_state;
		wrapper.push_back(extend_dir_state_dirs, sizeof(extend_dir_state_dirs));
		return wrapper;
	}

	template<typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		wrapper >> object_state >> shape_form >> effect_count >> extend_state << invisible_rate << anti_invisible_rate;
		wrapper.pop_back(extend_dir_state_dirs, sizeof(extend_dir_state_dirs));
		return wrapper;
	}

	void Clear()
	{
		object_state = 0;
		shape_form = 0;
		emote_form = 0;//�ƶ���ʱ���ô��̣���Ϊ��ʱ��������Ӧ�ı���
		effect_count = 0;
		extend_state = 0;;
		base_info.faction = 0;
		invisible_rate = 0;
		anti_invisible_rate = 0;
		extend_dir_state = 0;
		memset(extend_dir_state_dirs, 0, sizeof(extend_dir_state_dirs));
		gobject::Clear();
	}
	enum		//�����ö�����Ƿŵ�object_state�ϵ�
	{	
	//���ǿ��Խ�playerר�����ݷŵ���NPCר������ƽ�е�λ����
		STATE_SHAPE		= 0x00000001,   //�Ƿ����״̬
		STATE_EMOTE		= 0x00000002,   //�Ƿ�����������
		STATE_INVADER 		= 0x00000004,   //�Ƿ����
		STATE_FLY 		= 0x00000008,   //�Ƿ����
		STATE_FAMILY		= 0x00000010,   //�Ƿ����
		STATE_SITDOWN		= 0x00000020,   //�Ƿ�����
		STATE_EXTEND_PROPERTY	= 0x00000040,   //�Ƿ�����չ״̬
		STATE_ZOMBIE		= 0x00000080,	//�Ƿ�ʬ��

		STATE_TEAM		= 0x00000100,   //�Ƿ��Ա
		STATE_TEAMLEADER	= 0x00000200,   //�Ƿ�ӳ�
		STATE_ADV_MODE		= 0x00000400,   //�Ƿ��й������
		STATE_MAFIA		= 0x00000800,   //�Ƿ���ɳ�Ա
		STATE_MARKET		= 0x00001000,	//�Ƿ����ڰ�̯
		STATE_FASHION_MODE	= 0x00002000,	//�Ƿ�ʱװģʽ
		STATE_GAMEMASTER	= 0x00004000,	//���������GM����״̬
		STATE_PVP_NOPROTECTED	= 0x00008000,	//�Ƿ��ٽ���PVP����

		STATE_EFFECT		= 0x00010000,	//�Ƿ�������Ч��
		STATE_IN_PVP_COMBAT	= 0x00020000,	//�Ƿ�����PVP��
		STATE_IN_DUEL_MODE	= 0x00040000,	//�Ƿ����ھ�����
		STATE_MOUNT		= 0x00080000,	//���������
		STATE_IN_BIND		= 0x00100000,	//�ͱ��˰���һ��
		STATE_BATTLE_OFFENSE	= 0x00200000,	//ս��������
		STATE_BATTLE_DEFENCE	= 0x00400000,	//ս�����ط�
		STATE_TITLE		= 0x00800000,	//���г�ν

		STATE_SPOUSE		= 0x01000000,	//������ż
		STATE_PET_COMBINE1	= 0x02000000,	//����ͨ��
		STATE_PET_COMBINE2	= 0x04000000,	//��������
		STATE_SECT_MASTER_ID	= 0x08000000,	//����ʦ��
		STATE_IN_KILLINGFIELD	= 0x10000000,	//�Ƿ���ɱ¾ս������
		STATE_IN_VIP		= 0x20000000,	//����VIP״̬
		STATE_BATTLE_VISITOR	= 0x40000000,	//ս����ս��
		STATE_TRANSFORM		= 0x80000000,	//�Ƿ��ڱ���״̬

		STATE_STATE_CORPSE	= 0x00000008,	//NPC�Ƿ����ʬ������������ZOMBIE��һ������ʬ�������Ҳ����ZOMBIE
		STATE_NPC_ADDON1	= 0x00000100,	//NPC��������λ1 
		STATE_NPC_ADDON2	= 0x00000200,	//NPC��������λ2 
		STATE_NPC_ADDON3	= 0x00000400,	//NPC��������λ3 
		STATE_NPC_ADDON4	= 0x00000800,	//NPC��������λ4 
		STATE_NPC_ALLADDON	= 0x00000F00,	//NPC��������λ 
		STATE_NPC_PET		= 0x00001000,	//NPC��һ��PET���������PET������ID
		STATE_NPC_NAME		= 0x00002000,	//NPC�ж��ص����֣��������һ�ֽ�char����������
		STATE_NPC_SUMMON	= 0x00004000,	//NPC�ٻ���,��������˵�ID
		STATE_NPC_OWNER		= 0x00008000,	//NPC�й���
		STATE_NPC_CLONE		= 0x00010000,	//NPC�Ƿ���
		STATE_NPC_DIM		= 0x00020000,	//NPC����״̬
		STATE_NPC_FROZEN	= 0x00040000,	//NPC����״̬
		STATE_DIR_EXT_STATE 	= 0x00080000,	//NPC���������չ״̬
		STATE_NPC_INVISIBLE	= 0x00100000,	//NPC����
		STATE_NPC_TELEPORT1	= 0x00200000,	//������NPC(ף��)
		STATE_NPC_TELEPORT2	= 0x00400000,	//������NPC(����)
		STATE_PHASE		= 0x00800000,	//��λ
	};
	
	inline bool IsFemale() const
	{
		return base_info.gender == FEMALE;
	}

	inline short GetClass() const 
	{
		return base_info.cls;
	}

	inline void SetClassGender(short cls, bool is_female)
	{
		base_info.cls = cls;
		base_info.gender = is_female?FEMALE:MALE;
	}

	inline void SetObjectState(int state)
	{
		object_state |= state;
	}

	inline void ClrObjectState(int state)
	{
		object_state &= ~state;
	}

	inline bool CheckObjectState(int state) const
	{
		return object_state & state;
	}

	inline bool IsDuelMode() const
	{
		return CheckObjectState(STATE_IN_DUEL_MODE);
	}

	inline bool IsMountMode() const
	{
		return CheckObjectState(STATE_MOUNT);
	}

	inline bool IsFlyMode() const
	{
		return CheckObjectState(STATE_FLY);
	}

	inline void SetBattleOffense() 
	{
		ClrObjectState(STATE_BATTLE_DEFENCE);
		SetObjectState(STATE_BATTLE_OFFENSE);
	}

	inline void SetBattleDefence()
	{
		ClrObjectState(STATE_BATTLE_OFFENSE);
		SetObjectState(STATE_BATTLE_DEFENCE);
	}

	inline void ClrBattleMode()
	{
		ClrObjectState(STATE_BATTLE_OFFENSE | STATE_BATTLE_DEFENCE);
	}

	inline bool IsBattleOffense() const
	{
		return CheckObjectState(STATE_BATTLE_OFFENSE);
	}

	inline bool IsBattleDefence() const
	{
		return CheckObjectState(STATE_BATTLE_DEFENCE);
	}

	inline bool IsBattleVisitor() const
	{
		return CheckObjectState(STATE_BATTLE_VISITOR);
	}

	inline bool IsVIPState() const
	{
		return CheckObjectState(STATE_IN_VIP);
	}
	inline bool IsTransformMode() const
	{
		return CheckObjectState(STATE_TRANSFORM);
	}


};

struct gowner
{
	enum _owner_type
	{
		OWNER_NONE,		//��
		OWNER_SELF,		//�Լ�
		OWNER_MASTER,		//ʦ��
		OWNER_APPRENTICE,	//ͽ��
		OWNER_SPOUSE,		//����
		OWNER_TEAM,		//����
		OWNER_TEAM_OR_SELF,	//������Լ�
		OWNER_TEAM_AND_SELF,	//������Լ�
		OWNER_ANY,		//�ֵ���������ˢ�ֵ��ˣ�����û������Ĺ����߼�
		OWNER_FAMILY,		//���� (����)
		OWNER_MAFIA,		//���� (����)
	};

	gowner()
	{
		_owner_type = OWNER_NONE;
		_owner_id1 = 0;	//roleid
		_owner_id2 = 0;  //team id, master id, family id, mafia id
	}

	void Clear()
	{
		_owner_type = OWNER_NONE;
		_owner_id1 = 0;	//roleid
		_owner_id2 = 0;  //team id, master id, family id, mafia id
	}

	char GetOwnerType()
	{
		return _owner_type;
	}

	void SetOwnerType(char type)
	{
		_owner_type = type;
	}

	void SetOwnerID(int id1, int id2)
	{
		_owner_id1 = id1;
		_owner_id2 = id2;
	}

	bool IsOwner(int roleid, int team_id, int master_id)
	{
		switch(_owner_type)
		{
			case OWNER_NONE:
				return true;

			case OWNER_SELF:
			case OWNER_MASTER:
			case OWNER_SPOUSE:
			{
				return _owner_id1 == roleid; 
			}
			break;

			case OWNER_APPRENTICE:
			{
				return _owner_id2 == master_id;
			}
			break;

			case OWNER_TEAM:
			{
				return _owner_id2 == team_id;
			}
			break;

			case OWNER_TEAM_OR_SELF:
			{
				return (_owner_id1 == roleid || _owner_id2 == team_id) ;
			}
			break;

			case OWNER_TEAM_AND_SELF:
		      	{
				return (_owner_id1 == roleid && _owner_id2 == team_id);
			}	
			break;

			case OWNER_ANY:
			{
				return true;
			}
			break;
		}
		return false;
	}

	int GetNameID()
	{
		switch(_owner_type)
		{
			case OWNER_SELF:
				return _owner_id1;
			case OWNER_MASTER:
				return _owner_id1;
			case OWNER_APPRENTICE:
				return _owner_id2;
			case OWNER_SPOUSE:
				return _owner_id1;
			case OWNER_TEAM:
			case OWNER_TEAM_AND_SELF:
				return _owner_id2;
			case OWNER_TEAM_OR_SELF:
				return (_owner_id2 !=0 ) ?  _owner_id2 : _owner_id1;
			case OWNER_ANY:
				return _owner_id1;
			default:
				return 0;

		}
		return 0;
	}

	char _owner_type;
	int _owner_id1;
	int _owner_id2;
};

struct gnpc: public gactive_object
{
	size_t spawn_index;
	int native_state;
	int tid;		//ʵ�ʵ�tid
	int vis_tid;		//�ɼ���tid
	int monster_faction;	//�����С��
	int cruise_timer;	//�й�ʱ�ļ�����
	int idle_timer;
	int idle_timer_count;	//������idle״̬��ʱ
	int master_id;		//����ID��ֻ�д���PET״̬��NPC�Ż��д�״̬
	unsigned char pet_star; //������Ǽ���Ϣ
	unsigned char pet_shape;//�����ģ��
	unsigned char pet_face; //��������
	unsigned char teleport_count; //�����ͻ��ص�����(�����ж��Ƿ�ɴ���)
	int npc_idle_hearbeat;	//npcidleʱ���������� ����ֵ��0 ����֮
	short name_size;	//NPC���Զ�һ���ֳ��ȣ�ֻ�����������ֱ�־��npc��״̬����Ч 
	char npc_name[20];	//NPC���Զ������֣�ֻ�����������ֱ�־��npc��״̬����Ч
	unsigned short phase_id;
	gowner owner;

	enum
	{
		TYPE_NORMAL ,
		TYPE_NATIVE ,
		TYPE_EXTERN_NATIVE,
		TYPE_FREE,
	};
	inline gnpc* get_next() { return (gnpc*)pNext;}
	inline gnpc* get_prev() { return (gnpc*)pPrev;}
	void Clear()
	{
		tid = 0;
		vis_tid = 0;
		master_id = 0;
		pet_star = 0;
		name_size = 0;
		native_state = TYPE_NORMAL;
		npc_idle_hearbeat = 0;
		phase_id = 0;
		teleport_count = 0;
		owner.Clear();
		gactive_object::Clear();
	}
	bool IsNative()
	{
		return native_state == TYPE_NATIVE;
	}
	
	void SetOwnerType(char type)
	{
		owner.SetOwnerType(type);
	}
	
	void SetOwnerID(int owner_id1, int owner_id2)
	{
		owner.SetOwnerID(owner_id1, owner_id2);
	}
	
	bool IsNPCOwner(int roleid, int team_id, int master_id)
	{
		return owner.IsOwner(roleid, team_id, master_id);
	}
	
	template<typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		gactive_object::Import(wrapper); 
		wrapper >> ID >> pos >> msg_mask >> tid >> vis_tid ;
		wrapper.pop_back(&base_info,sizeof(base_info));
		wrapper >> monster_faction >> body_size >> master_id >> pet_star >> pet_shape >> pet_face >> teleport_count >> name_size;
		return wrapper.pop_back(npc_name,sizeof(npc_name));
	}
	
	template<typename WRAPPER>
	WRAPPER & Export(WRAPPER & wrapper)
	{
		gactive_object::Export(wrapper); 
		wrapper << ID << pos << msg_mask << tid ;
		wrapper.push_back(&base_info,sizeof(base_info));
		wrapper << monster_faction << body_size << master_id << pet_star << pet_shape << pet_face << teleport_count << name_size;
		return wrapper.push_back(npc_name,sizeof(npc_name));
	}
};

struct gmatter : public gobject
{
	unsigned char dir1;
	unsigned char rad;
	unsigned char matter_state;	
	unsigned char battle_flag;				//0 ���� 1 ����  2 �ط�
	unsigned short phase_id;
	int combine_state;  // Youshuang add
	gowner owner;

	enum 
	{
		BATTLE_FLAG_NEUTRAL,
		BATTLE_FLAG_ATTACKER,
		BATTLE_FLAG_DEFENDER
	};
	enum
	{
		STATE_MASK_NORMAL_OBJECT	= 0x00,		//��ͨ��Ʒ ���� 
		STATE_MASK_DYN_OBJECT 		= 0x01,		//��ʾ����ƷΪ��̬��Ʒ����IDӦ�Ӷ�̬ID��Ѱ��
		STATE_MASK_BATTLE_FLAG		= 0x02,		//��ʾ����ƷΪս��������
		STATE_MASK_OWNER_MATTER		= 0x04,
		STATE_MASK_PHASE		= 0x10,		//��λ
		STATE_MASK_COMBINE_MINE		= 0x20,		// combine mine

		BATTLE_FLAG_WHITE		= 0,
		BATTLE_FLAG_RED			= 0,
		BATTLE_FLAG_BLUE		= 0,
	};
	int matter_type;
	int spawn_index;		//ֻ�Կ������
	int name_id;			//���ֹ���ID

	inline gmatter* get_next() { return (gmatter*)pNext;}
	inline gmatter* get_prev() { return (gmatter*)pPrev;}

	inline bool IsAttackerFlag()
	{
		return battle_flag == BATTLE_FLAG_ATTACKER;
	}

	inline bool IsDefenderFlag()
	{
		return battle_flag == BATTLE_FLAG_DEFENDER;
	}

	inline bool IsNeutralFlag()
	{
		return battle_flag == BATTLE_FLAG_NEUTRAL;
	}
	void SetDirUp(unsigned char d, unsigned char d1, unsigned char r)
	{
		dir = d;
		dir1 = d1;
		rad = r;
	}
	inline void Clear()
	{
		gobject::Clear();
		dir1 = rad = matter_state = battle_flag = 0;
		owner.Clear();
	}
	
	void SetOwnerType(char type)
	{
		owner.SetOwnerType(type);
	}
	
	void SetOwnerID(int owner_id1, int owner_id2)
	{
		owner.SetOwnerID(owner_id1, owner_id2);
	}
	
	bool IsMineOwner(int roleid, int master_id, int team_id)
	{
		return owner.IsOwner(roleid, master_id, team_id);
	}
};

#endif

