#ifndef __GNET_GAMEMASTER_H
#define __GNET_GAMEMASTER_H

#include <map>

namespace GNET
{

class GameMaster
{
public:
	enum
	{
		PRV_TOGGLE_NAMEID	= 0,	//�л����������ID
		PRV_HIDE_BEGOD		= 1,	//����������޵�״̬
		PRV_ONLINE_ORNOT	= 2,	//�л��Ƿ�����
		PRV_CHAT_ORNOT		= 3,	//�л��Ƿ��������
		PRV_MOVETO_ROLE		= 4,	//�ƶ���ָ����ɫ���
		PRV_FETCH_ROLE		= 5,	//��ָ����ɫ�ٻ���GM���
		PRV_MOVE_ASWILL		= 6,	//�ƶ���ָ��λ��
		PRV_MOVETO_NPC		= 7,	//�ƶ���ָ��NPCλ��
		PRV_MOVETO_MAP		= 8,	//�ƶ���ָ����ͼ��������
		PRV_ENHANCE_SPEED	= 9,	//�ƶ�����
		PRV_FOLLOW		= 10,	//�������
		PRV_LISTUSER		= 11,	//��ȡ��������б�
		PRV_FORCE_OFFLINE	= 100,	//ǿ��������ߣ�����ֹ��һ��ʱ������
		PRV_FORBID_TALK		= 101,	//����
		PRV_FORBID_TRADE	= 102,	//��ֹ��Ҽ䡢�����NPC���ף������һ�����
		PRV_FORBID_SELL		= 103,	//����
		PRV_BROADCAST		= 104,	//ϵͳ�㲥
		PRV_SHUTDOWN_GAMESERVER	= 105,	//�ر���Ϸ������
		PRV_SUMMON_MONSTER	= 200,	//�ٻ�����
		PRV_DISPEL_SUMMON	= 201,	//��ɢ���ٻ�����
		PRV_PRETEND		= 202,	//αװ
		PRV_GMMASTER		= 203,	//GM����Ա
	};

	enum
	{
		STATUS_SILENT      = 0x01,
	};

	int userid;
	unsigned int linksid;
	unsigned int localsid;
	int status;
	ByteVector privileges;
	GameMaster(int _uid,unsigned int _link,unsigned int _local, ByteVector& _rights) : userid(_uid), linksid(_link), 
		localsid(_local), status(0), privileges(_rights)
	{
	}
	GameMaster(const GameMaster& r) : userid(r.userid), linksid(r.linksid), localsid(r.localsid), status(r.status),
		privileges(r.privileges)	
	{
	}
	~GameMaster() {}

	bool IsAuthorized(unsigned char right)
	{
		for(ByteVector::iterator it=privileges.begin();it!=privileges.end();++it)
			if(*it==right)
				return true;
		return false;
	}
};

class PrivateChat;
class MasterContainer
{
public:
	typedef std::map<int/*roleid*/,GameMaster> MasterMap;
	MasterMap  masters;

	static MasterContainer & Instance() { static MasterContainer instance; return instance; }

	GameMaster* Find( int roleid )
	{
		MasterMap::iterator it = masters.find(roleid);
		if(it!=masters.end())
			return &(it->second);
		return NULL;
	}

	void Insert(int userid, int roleid, int linksid, int localsid, ByteVector& privileges);
	void Erase(int roleid);
	bool Authenticate(int roleid, unsigned int sid, unsigned char right);
	void Broadcast(PrivateChat& data, int &roleid);
	bool SetSilent(int roleid, bool set);
	
};

};

#endif

