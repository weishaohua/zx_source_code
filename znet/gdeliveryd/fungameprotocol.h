#ifndef __GNET_FUNGAME_PROTOCOL_H
#define __GNET_FUNGAME_PROTOCOL_H

namespace FUNGAME
{
	
#pragma pack(1)
namespace S2C
{
	struct fun_game_s2c_cmd_header 
	{
		unsigned char game_type;
		unsigned char cmd;
	};

	namespace INFO
	{
		struct player_base_info
		{
			int roleid;
			char direction;
			char state;
			char gender;
			int64_t score;
			int win_count;
			int lose_count;
			int draw_count;    //ƽ����
			int escape_count;  //���ܾ���
			char name_size;
			char name[20];
		};

		struct room_info
		{
			short roomid;
			char player_size;
			player_base_info players[];
		};

		struct player_state
		{
			int roleid;
			char direction;
			char state;
		};

		struct normal_card_info
		{
			char card_size;
			char cards[];
		};

		struct landlord_result_info
		{
			int roleid;	
			int score;
		};
		struct landlord_card_info
		{
			char direct;	
			char card_size;
		};
		struct landlord_reonline_info
		{
			room_info _room_info;
			int score;
			char lord_dir;  // ����λ��
			char desk_dir;  // ������������
			char next_dir;  // ��ǰ������ 
			landlord_card_info cards[2];// ���������������Ϣ
			std::vector<char> selfcards; // ������
			std::vector<char> deskcards; // ������
		};
	}; 

	enum  //CMD
	{
		GET_ROOM_LIST,		//��һ�ȡ�����б�
		NOTIFY_ROOM_INFO,	//֪ͨ������Ϣ
		PLAYER_INFO,		//֪ͨ�����Ϣ
		PLAYER_ENTER_ROOM, 	//��ҽ��뷿��
		PLAYER_LEAVE_ROOM, 	//����뿪����
		PLAYER_STATE_CHANGE,	//���״̬�仯֪ͨ 
		PLAYER_REENTER_INVITE,	//��Ҷ��ߺ� ������Ҽ�����Ϸ 

		ROOM_GAME_IS_READY, 	//��Ϸ�����ѹ��� �ȴ���ҿ�ʼ��
		ROOM_DISPATCH_NORMAL_CARD,  //��Ϸ��ʼ ����
		PLAYER_PICKUP_NORMAL_CARD, //����õ��� 
		PLAYER_INTURN_NORMAL_CARD, //�ֵ��ɸ���ҳ��� 
		PLAYER_THROW_NORMAL_CARD, //��ҳ���
		PLAYER_GIVEUP_NORMAL_CARD, //����������� 

		PLAYER_AUCTION,
		ROOM_GAME_END,
		LANDLORD_RESULT_BROADCAST, //�㲥��Ϸ���
		LANDLORD_REONLINE_DATASEND, //�����ߺ��͵�ǰ����

		PLAYER_ENTER_HALL,	//�������
		PLAYER_LEAVE_HALL,	//�뿪����
		NOTIFY_HALL_MEMBERS,	//���ش����е�����б�
	};

	namespace CMD
	{
		using namespace INFO;

		struct get_room_list 
		{
			fun_game_s2c_cmd_header header;
			char room_size;
			room_info data[];
		};

		struct notify_room_info
		{
			fun_game_s2c_cmd_header header;
			room_info data;
		};

		struct notify_hall_members
		{
			fun_game_s2c_cmd_header header;
			int member_num;
			player_base_info players[];
		};

		struct player_info
		{
			fun_game_s2c_cmd_header header;
			player_base_info data;
		};

		struct player_enter_hall
		{
			fun_game_s2c_cmd_header header;
			player_base_info data;
		};

		struct player_leave_hall
		{
			fun_game_s2c_cmd_header header;
			int roleid;
		};

		struct player_enter_room
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			player_base_info data;
		};

		struct player_leave_room
		{
			fun_game_s2c_cmd_header header;
			int roleid;
			short roomid;
		};

		struct player_state_change
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			player_state data;
		};

		struct player_reenter_invite
		{
			fun_game_s2c_cmd_header header;
			short roomid;
		};

		struct room_game_is_ready 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
		};

		struct room_dispatch_normal_card
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
			int next_roleid;
			normal_card_info cards;
		};

		struct player_inturn_normal_card 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
		};

		struct player_throw_normal_card 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
			int next_roleid; //��һ�������� 
			normal_card_info cards;
		};

		struct player_pickup_normal_card 
		{
			fun_game_s2c_cmd_header header;
			short roomid;
			int roleid;
			normal_card_info cards;
		};

		struct player_giveup_normal_card
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			int roleid;
			int next_roleid; //��һ�������� 
		};
		struct player_auction
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			int roleid;
			int next_roleid; //��һ�������� 
			char score; //��ֵ
		};
		struct room_game_end
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			int roleid;
			char reason; //����ԭ��
		};
		struct landlord_result_broadcast
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			char size;
			landlord_result_info results[];
		};
		struct landlord_reonline_datasend
		{
			fun_game_s2c_cmd_header	header;
			short roomid;
			landlord_reonline_info info;
		};
	};

}; // end namespace S2C

namespace C2S
{
	struct fun_game_c2s_cmd_header
	{
		unsigned char game_type;
		unsigned char cmd_type;
	};

	namespace INFO
	{
		struct normal_card_info
		{
			char card_size;
			char cards[];
		};
	}; 

	enum  //CMD
	{
		GET_ROOM_LIST,   	//��һ�ȡ�����б�
		PLAYER_ENTER_ROOM,	//��ҽ��뷿�� 
		PLAYER_LEAVE_ROOM, 	//����뿪����
		PLAYER_BEGIN_GAME,	//��ҿ�ʼ��Ϸ
		PLAYER_REENTER_ROOM,	//��Ҷ��ߺ� ���½��뷿��

		PLAYER_PICKUP_NORMAL_CARD, //����õ��� 
		PLAYER_THROW_NORMAL_CARD, //��ҳ���
		PLAYER_GIVEUP_NORMAL_CARD, //����������� 
		PLAYER_AUCTION,		//����

		PLAYER_ENTER_HALL,	//��ҽ������
		PLAYER_LEAVE_HALL,	//����뿪����
	};

	namespace CMD
	{
		using namespace INFO;

		struct player_enter_hall
		{
			fun_game_c2s_cmd_header header;
		};

		struct player_leave_hall
		{
			fun_game_c2s_cmd_header	header;
		};

		struct get_room_list 
		{
			fun_game_c2s_cmd_header	header;
			char index;
		};

		struct player_enter_room
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
			char direction;
		};

		struct player_begin_game
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_leave_room
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_reenter_room
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_throw_normal_card
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
			normal_card_info cards;
		};

		struct player_pickup_normal_card
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};

		struct player_giveup_normal_card
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
		};
		struct player_auction
		{
			fun_game_c2s_cmd_header	header;
			short roomid;
			int roleid;
			char score; //��ֵ
		};
	};


}; // end namespace C2S

#pragma pack()  
}; //end namespace FUNGAME 

#endif
