#ifndef __ONLINEGAME_GS_FACTION_H__
#define __ONLINEGAME_GS_FACTION_H__

//����Ķ�������elements.data�������һ��

enum
{
	FACTION_WHITE		= 0x00002,		//��ɫ��
	FACTION_PARIAH		= 0x00004,		//��ɫ��
	FACTION_BLUE		= 0x00008,		//��ɫ��
	FACTION_BATTLEOFFENSE	= 0x00010,		//��ս����
	FACTION_BATTLEDEFENCE	= 0x00020,		//��ս�ط�
	FACTION_OFFENSE_FRIEND	= 0x00040,		//�����ѷ�
	FACTION_DEFENCE_FRIEND	= 0x00080,		//�ط��ѷ�

	FACTION_DYNAMIC_MASK 	= 0x000FE
	
};
#endif
