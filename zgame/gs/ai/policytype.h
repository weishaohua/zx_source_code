#ifndef _AI_POLICY_TYPE_H_
#define _AI_POLICY_TYPE_H_


#define OP_TEXT_LENGTH 256

//----------------------------------------------------------------------------------------
//Condition
//----------------------------------------------------------------------------------------

struct C_TIME_COME
{
	unsigned int uID;
};

struct C_PATH_END_POINT
{
	unsigned int uID;
};

struct C_HP_LESS
{
	float fPercent;
};


struct C_RANDOM
{
	float fProbability;
};

struct C_ENMITY_REACH
{
	int nPlayer;		//����
	int nPlayerLevel;	//��ʾ�˵ļ���
};

struct C_DISTANCE_REACH
{
	float fDistance;   //��������
};

struct C_ATTACK_BY_SKILL
{
	unsigned int uID;
};

struct C_VAR
{
	int iID;
};

struct C_CONSTANT
{
	int iValue;
};

struct C_RANK_LEVEL
{
	int iValue;
};


//----------------------------------------------------------------------------------------
//Operation
//----------------------------------------------------------------------------------------

struct O_ATTACK_TYPE
{
	unsigned int uType;
};

struct O_USE_SKILL
{
	unsigned int uSkill; //����
	unsigned int uLevel; //����
};

struct O_USE_RANGE_SKILL
{
	unsigned int uSkill; //����
	unsigned int uLevel; //����
	float        fRange; //��Χ��1-512��
};

struct O_TRIGGER_TASK
{
	unsigned int uTaskID;
};

struct O_CHANGE_PATH_VERSION8
{
	unsigned int uPathID;
};

struct O_CHANGE_PATH
{
	unsigned int uPathID;
	int			 iType; //��ֵΪ0���յ�ֹͣ����1��ԭ·���أ���2����βѭ����
};

struct O_SNEER_MONSTER
{
	float fRange;
};

struct O_TALK_TEXT
{
	unsigned int uSize;
	unsigned short* szData;
};

struct O_RUN_TRIGGER
{
	unsigned int uID;
};

struct O_STOP_TRIGGER
{
	unsigned int uID;
};

struct O_ACTIVE_TRIGGER
{
	unsigned int uID;
};

struct O_CREATE_TIMER
{
	unsigned int uID;
	unsigned int uPeriod;
	unsigned int uCounter;
};

struct O_SUMMON
{
	unsigned int uMounsterID;		//����ID
	unsigned int uBodyMounsterID;	//�������Σ�Ϊ0ʱʹ�����������
	unsigned int uMounsterNum;		//����������1-999��
	unsigned int uLife;				//��������ʱ�䣨0-MAX����0��ʾ�����Զ���ʧ��(s)
	unsigned short szName[16];		//�������֣����Ϊ�վ���ʵ����ID���������
	float    fRange;				//�������ɾ��뱾�巶Χ�뾶(m)
	bool     bFollow;				//�����Ƿ���汾�壬���Ϊ�������汾����ʧҲ����Ϊ��
	bool     bDispear;				//�Ƿ���汾����ʧ
};

struct O_KILL_TIMER
{
	unsigned int uID;
};

struct O_ACTIVE_CONTROLLER
{
	unsigned int uID;
	bool         bStop;//�Ƿ���ֹͣ����
};

struct O_SET_GLOBAL
{
	int  iID;
	int  iValue;
};

struct O_REVISE_GLOBAL
{
	int  iID;
	int  iValue;
};

struct O_ASSIGN_GLOBAL
{
	int  iIDSrc;
	int  iIDDst;
};

struct O_SUMMON_MINERAL
{
	unsigned int uMineralID;		//����ID
	unsigned int uMineralNum;		//����������1-999��
	unsigned int uHP;				//����������0-MAX��
	float    fRange;				//����౾�����(0-50m)
	bool     bBind;				    //�Ƿ��Ŀ��
};

struct O_DROP_ITEM
{
	unsigned int uItemID;			//����������Ʒid
	unsigned int uItemNum;			//����������Ʒ����
	unsigned int uExpireDate;		//����������Ʒ����Ч�ڣ�����Ʒ���ɶѵ�ʱ��Ч��Ĭ��Ϊ0û��ʱ�����ƣ�
};

struct O_CHANGE_HATE
{
	int			iHateValue;			//�ı�ĳ��ֵ Added 2012-08-21.
};

struct O_START_EVENT
{
	int iId;
};

struct O_STOP_EVENT
{
	int iId;
};

//----------------------------------------------------------------------------------------
//Target Occupation
//----------------------------------------------------------------------------------------
struct T_OCCUPATION
{
	unsigned int uBit;
};

#endif
