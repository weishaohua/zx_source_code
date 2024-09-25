#ifndef __NETGAME_GS_TEMPLATE_LOADER_H__
#define __NETGAME_GS_TEMPLATE_LOADER_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>
#include <vector>

#include "property.h"
#include "playertemplate.h"

class gplayer_imp;
struct pet_data;
struct mob_active_statement;

struct npc_template
{
	int tid;

	unsigned int faction;			
	unsigned int enemy_faction;		
	unsigned int monster_faction;		//�����С��ϵ���������
	unsigned int id_strategy;
	int role_in_war;			//��ս�еĽ�ɫ
	int immune_type;
	float body_size;
	float sight_range;
	unsigned int 	aggressive_mode;
	unsigned int	monster_faction_ask_help;
	unsigned int	monster_faction_can_help;
	float aggro_range;
	int aggro_time;
	int trigger_policy;
	unsigned int after_death;
	int patrol_mode;
	char drop_no_protected;
	char is_boss;
	char kill_exp;				//ɱ�������ʧexp
	char kill_drop;				//ɱ�������ʧ��Ʒ
	char collision_in_server;		//������ײ
	char is_skill_drop_adjust;			//�Ƿ��漼�ܵ�������������
	char no_exp_punish;	
	char can_catch;				//0����������׽, 1-8������׽�Ѷ� 
	char player_cannot_attack;		//��ҹ����Ƿ���Ч: (0:��Ч, 1:��Ч) 
	bool no_idle;				//�����Ƿ�����idleģʽ��Ĭ���ǻ����
	bool isfly;                           // Youshuang add
	bool show_damage;			// Youshuang add
	unsigned int task_share;
	unsigned int item_drop_share;
	unsigned int buff_area_id;
	struct
	{
		int as_count;
		int bs_count;
		int cs_count;
		struct 
		{
			int id;
			int level;
		} attack_skills[8];
		
		struct 
		{
			int id;
			int level;
		} bless_skills[8];

		struct
		{
			int id;
			int level;
		} curse_skills[8];

	}skills;
	
	player_template::prop_data base_lvl_data;
	player_template::prop_data lvl_up_data;

	int base_exp;
	float lvl_up_exp;

	basic_prop bp;
	q_extend_prop ep;
	q_item_prop ip;
	int dmg_reduce;
	int spec_damage;
	int war_role_config;

	//�����ǹ���npc������
	struct npc_statement
	{
		enum 
		{
			NPC_TYPE_NORMAL,
			NPC_TYPE_GUARD,
			NPC_TYPE_CARRIER,
		};

		int refresh_time;
		int attack_rule;
		int faction;		//���ܲ���Ҫ 
		float tax_rate;
		int life;		//���ܲ���Ҫ
		int npc_type;		//npc ����
		int need_domain;	//��Ҫ���ɵ��̲ſ��Խ��д���
		int id_territory;

		//���ַ���Ĵ���
		
		int  service_sell_num;
		struct goods_t
		{
			int goods;
			float discount;
		};
		goods_t service_sell_goods[8*48];

		bool service_purchase;
		bool service_heal;
		int  service_transmit_target_num;
		struct  __st_ent
		{
			A3DVECTOR       target_pos;
			int             target_tag;
			int             require_level;
			size_t          fee;
		}transmit_entry[16];

		int service_task_in_num;
		int service_task_in_list[256];

		int service_task_out_num;
		int service_task_out_list[257];

		int service_task_matter_num;
		int service_task_matter_list[32];

		bool service_install;
		bool service_uninstall;

		bool service_storage;
		int service_waypoint_id;	//����� <=0��Ϊ��Ч
		int service_double_exp;		//�Ƿ���˫���������
		int service_hatch_pet;		//��������
		int service_recover_pet;	//��ԭ���ﵰ

		int service_faction;		//�Ƿ�ɴ�����ɷ���
		int service_mail;		//�Ƿ����ʼ�����
		int service_auction;		//�Ƿ�����������
		int service_cash_trade;		//�㿨����
		struct
		{
			int has_service;
			int fee_per_unit;
		}service_reset_pkvalue;

		int service_equip_bind;
		struct 
		{
			int money_need;
			int item_need;
		} service_bind_prop;

		int service_destroy_bind;
		struct 
		{
			int money_need;
			int item_need;
		} service_destroy_bind_prop;

		int service_undestroy_bind;
		struct 
		{
			int money_need;
			int item_need;
		} service_undestroy_bind_prop;

		int service_reset_prop_count;   //����ϴ�����Ŀ��Ŀ
		struct __reset_prop
		{
			int object_need;
			int type;
		}reset_prop[6];

		int service_storage_mafia;
		int service_talisman;
		int service_blood_enchant;
		int service_spirit_addon;
		int service_spirit_remove;
		int service_spirit_charge;
		int service_spirit_decompose;
		int service_arena_challenge;

		int battle_field_challenge_service;	//��ս����,���ø�����
		int battle_field_construct_service;	//��ս����
		int battle_field_employ_service;	//��ս��Ӷ
		struct __employ_t
		{
			int price;
			int war_material_id;
			int war_material_count;
			int item_wanted;
			int remove_one_item;
			int controller_id[20];
		}employ_service_data;

		//�������
		int pet_service_adopt;
		int pet_service_free;
		int pet_service_combine;
		int pet_service_rename;

		int renew_mount_service;		//ѱ���������
		int lock_item_service;			//�����ͽ�������

		int service_restore_broken;
		int service_petequip_refine;
		int service_change_style;
		int service_magic_refine;
		int service_magic_restore;
	
		//����ս	
		int service_territory_challenge;
		int service_territory_enter;
		int service_territory_reward;

		int service_charge_telestation;
		int service_repair_damage;
		int service_equipment_upgrade;

		//���
		int service_crossservice_in;
		int service_crossservice_out;
		//�������ս��
		int service_crossservice_battle_sign_up;
		int service_crossservice_battle_out;

		int service_item_trade[4];

		int service_npc_produce_id;		//npc�����ϳɷ���
		
		float carrier_mins[3];
		float carrier_maxs[3];

		int service_consign;
		int	consign_margin;

		//Add by houjun 2010-03-11, ��ʯ��Ƕ��ط���
		int service_identify_gem_slots;
		int service_rebuild_gem_slots;
		int service_customize_gem_slots;
		int service_embed_gems;
		int service_remove_gems;
		int service_upgrade_gem_level;
		int service_upgrade_gem_quality;
		int service_extract_gem;
		int service_smelt_gem;
		//Add end.

		int service_change_name;
		int service_change_faction_name;

		int service_talisman_holylevelup;
		int service_talisman_embedskill;
		int service_talisman_refineskill;


		//�����̵�
		int reputation_shop_id;
		
		int service_equipment_upgrade2;
		int service_equipment_slot;
		int service_install_astrology;
		int service_uninstall_astrology;
		int service_astrology_identify;

		int service_kingdom_enter;

		int service_produce_jinfashen;
		int service_pet_reborn;


		int id_transcription;
		// ---------  �ϹŴ��� sunjunbo 2012-8-20 ----------
		// ����͵����
		int id_open_ui_trans;		// ���͵�id
		// ʹ�ô��͵����
		int id_use_ui_trans;  	    // ģ��id
		int use_ui_trans_id[256];	// ���͵㼯��
	};

	npc_statement * npc_data;
	mob_active_statement * mob_active_data;
	
	inline static void copy_skill(int &t_count, void *skill_list, int skill, int level)
	{
		int index = t_count;
		ASSERT(index < 8);
		int * list = (int*)skill_list;
		list += index * 2;
		*list = skill;
		*(list + 1) = level;
		t_count ++;
	}

};

struct mine_template
{
	int tid;
	int role_in_war;
	int std_amount;			//��׼���� 
	int bonus_amount;		//���Ӻ������
	float bonus_prop;		//ʹ�ø��������ĸ���
	int time_min;			//�ɼ�ʱ������
	int time_max;			//�ɼ�ʱ������
	unsigned int produce_kinds;
	unsigned int id_produce[16];
	float id_produce_prop[16];
	int need_equipment;
	int level;
	int exp;
	int task_in;
	int task_out;
	bool no_interrupted;
	bool gather_no_disappear;
	bool eliminate_tool;
	int   ask_help_faction;
	float ask_help_range;
	int   ask_help_aggro;
	struct 
	{
		int id_monster;
		int num;
		float radius;
		int remain_time;
	} monster_list[4];

	struct 
	{
		bool actived_all;
		bool deactived_all;
		unsigned char active_count;
		unsigned char deactive_count;
		int active_cond[4];
		int deactive_cond[4];
	}ctrl_info;

	// Youshuang add for combined mine
	std::vector<int> combine_tid;
	int result_tid;
	int gather_count;
	bool IsCombineMine(){ return ( combine_tid.size() > 0 ); }
	// end
};

struct recipe_template
{
	int id;
	int level;
	struct 
	{
		float probability;
		int target_id;
		size_t target_count_low;
		size_t target_count_high;
	}products[3];
	size_t use_time;
	size_t fee;
	int material_num;
	int material_total_count;
	struct __material
	{
		int item_id;
		size_t count;
	} material_list[6];
	int recipe_class; //���� 0 Ϊ��ͨ 1 Ϊ���﹤���䷽
	int required[7]; //���� 0-5 ������ 6 �ǹ��������
	int cool_type;
	int cool_time;
};

class itemdataman;
class npc_stubs_manager
{
	typedef abase::hashtab<npc_template, int ,abase::_hash_function> MAP;
	MAP _nt_map;
	bool __InsertTemplate(const npc_template & nt)
	{
		return _nt_map.put(nt.tid, nt);
	}

	npc_template * __GetTemplate(int tid)
	{
		return _nt_map.nGet(tid);
	}
	static npc_stubs_manager & __GetInstance()
	{
		static npc_stubs_manager __Singleton;
		return __Singleton;
	}
	npc_stubs_manager():_nt_map(1024) {}
public:
	static bool Insert(const npc_template & nt)
	{
		bool rst = __GetInstance().__InsertTemplate(nt);
		ASSERT(rst);
		return rst;
	}

	static npc_template * Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

class mine_stubs_manager
{
	typedef abase::hashtab<mine_template, int ,abase::_hash_function> MAP;
	MAP _mt_map;
	bool __InsertTemplate(const mine_template & mt)
	{
		return _mt_map.put(mt.tid, mt);
	}

	mine_template * __GetTemplate(int tid)
	{
		return _mt_map.nGet(tid);
	}
	static mine_stubs_manager & __GetInstance()
	{
		static mine_stubs_manager __Singleton;
		return __Singleton;
	}
	mine_stubs_manager():_mt_map(1024) {}
public:
	static bool Insert(const mine_template & mt)
	{
		bool rst = __GetInstance().__InsertTemplate(mt);
		ASSERT(rst);
		return rst;
	}

	static mine_template * Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman & dataman);
};


class recipe_manager
{
	typedef abase::hashtab<recipe_template, int ,abase::_hash_function> MAP;
	MAP _rt_map;
	bool __InsertTemplate(const recipe_template & rt)
	{
		recipe_template tmp= rt;
		tmp.material_total_count = 0;
		for(int i = 0; i < rt.material_num; i ++)
		{
			tmp.material_total_count += rt.material_list[i].count;
		}
		if(tmp.material_total_count > 0) 
			return _rt_map.put(rt.id, tmp);
		else 
			return false;
	}

	recipe_template * __GetTemplate(int id)
	{
		return _rt_map.nGet(id);
	}

	static recipe_manager & __GetInstance()
	{
		static recipe_manager __Singleton;
		return __Singleton;
	}

	recipe_manager():_rt_map(1024){}

public:
	static bool Insert(const recipe_template & rt)
	{
		return __GetInstance().__InsertTemplate(rt);
	}

	static recipe_template * GetRecipe(int id)
	{
		return __GetInstance().__GetTemplate(id);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

struct item_trade 
{
	int id;
	int item_num;

	struct  //������Ʒ      
	{
		int id;
		int count;
	}item_require[2];

	struct  //�������� 
	{
		int id;
		int count;
	}rep_require[2];

	struct  //��������
	{
		int id;
		int count;
	}special_require;

};

struct item_trade_template
{
	int id;
	item_trade item_trade_goods[48*4];
};

class item_trade_manager
{
	typedef abase::hashtab<item_trade_template, int ,abase::_hash_function> MAP;
	MAP _it_map;
	bool __InsertTemplate(const item_trade_template & it)
	{
		return _it_map.put(it.id, it);
	}

	item_trade_template * __GetTemplate(int id)
	{
		return _it_map.nGet(id);
	}

	static item_trade_manager & __GetInstance()
	{
		static item_trade_manager __Singleton;
		return __Singleton;
	}

	item_trade_manager():_it_map(128){}

public:
	static bool Insert(const item_trade_template & rt)
	{
		return __GetInstance().__InsertTemplate(rt);
	}

	static item_trade_template * GetItemTradeTemplate(int id)
	{
		return __GetInstance().__GetTemplate(id);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

struct reputation_shop_goods_t
{
	unsigned int	id_goods;

	struct
	{
		unsigned int	repu_id_req;
		unsigned int	repu_req_value;
		unsigned int	repu_id_consume;
		unsigned int	repu_consume_value;
	} repu_required_pre[3];
};

struct reputation_shop_template
{
	int id;
	reputation_shop_goods_t reputation_shop_sell_goods[4*48];
};

class reputation_shop_manager
{
	typedef abase::hashtab<reputation_shop_template, int ,abase::_hash_function> MAP;
	MAP _rs_map;
	bool __InsertTemplate(const reputation_shop_template & it)
	{
		return _rs_map.put(it.id, it);
	}

	reputation_shop_template * __GetTemplate(int id)
	{
		return _rs_map.nGet(id);
	}

	static reputation_shop_manager & __GetInstance()
	{
		static reputation_shop_manager __Singleton;
		return __Singleton;
	}

	reputation_shop_manager():_rs_map(8){}

public:
	static bool Insert(const reputation_shop_template & rt)
	{
		return __GetInstance().__InsertTemplate(rt);
	}

	static reputation_shop_template * GetReputationShopTemplate(int id)
	{
		return __GetInstance().__GetTemplate(id);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

struct npc_produce_template
{
	int id;
	int npc_produce_list[500];
};

class npc_produce_manager
{
	typedef abase::hashtab<npc_produce_template, int ,abase::_hash_function> MAP;
	MAP _np_map;
	bool __InsertTemplate(const npc_produce_template & it)
	{
		return _np_map.put(it.id, it);
	}

	npc_produce_template * __GetTemplate(int id)
	{
		return _np_map.nGet(id);
	}

	static npc_produce_manager & __GetInstance()
	{
		static npc_produce_manager __Singleton;
		return __Singleton;
	}

	npc_produce_manager():_np_map(8){}

public:
	static bool Insert(const npc_produce_template & np)
	{
		return __GetInstance().__InsertTemplate(np);
	}

	static npc_produce_template * GetNPCProduceTemplate(int id)
	{
		return __GetInstance().__GetTemplate(id);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

#endif

