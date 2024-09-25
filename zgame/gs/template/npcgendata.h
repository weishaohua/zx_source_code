/*
 * FILE: NPCGenMan.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef _NPCGENMAN_H_
#define _NPCGENMAN_H_

#include "vector.h"
#include "hashtab.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

/*	Version history
ver 13: - new struct NPCGENFILECTRL13, add attaching area
			- struct CONTROLLER update
13->14: ��λ���� NPCGENFILEAREA14,NPCGENFILERESAREA14,NPCGENFILEDYNOBJ14
*/
#define AIGENFILE_VERSION		14

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

//	Data file header. AIGENFILE_VERSION < 6
struct NPCGENFILEHEADER
{
	int		iNumAIGen;		//	Number of NPC generator
	int		iNumResArea;	//	Number of resource area
};

//	Data file header. AIGENFILE_VERSION >= 6 && < 7
struct NPCGENFILEHEADER6
{
	int		iNumAIGen;		//	Number of NPC generator
	int		iNumResArea;	//	Number of resource area
	int		iNumDynObj;		//	Number of dynamic objects
};

//	Data file header. AIGENFILE_VERSION >= 7
struct NPCGENFILEHEADER7
{
	int		iNumAIGen;		//	Number of NPC generator
	int		iNumResArea;	//	Number of resource area
	int		iNumDynObj;		//	Number of dynamic objects
	int		iNumNPCCtrl;	//	Number of NPC controller
};

//	Data file header. AIGENFILE_VERSION < 7
struct NPCGENFILEAREA
{
	int		iType;			//	0, on terrain; 1, box area
	int		iNumGen;		//	Number of NPC generator in this area
	float	vPos[3];
	float	vDir[3];
	float	vExts[3];
	int		iNPCType;		//	NPC type
	int		iGroupType;		//	Monster group type

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����
};

//	Data file header. AIGENFILE_VERSION >= 7 && < 12
struct NPCGENFILEAREA7
{
	int		iType;			//	0, on terrain; 1, box area
	int		iNumGen;		//	Number of NPC generator in this area
	float	vPos[3];
	float	vDir[3];
	float	vExts[3];
	int		iNPCType;		//	NPC type
	int		iGroupType;		//	Monster group type

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����

	int		idCtrl;			//	Controller id
	int		iLifeTime;		//	Monster's life time
	int		iMaxNum;		//	Maximum monster number
};

//	Data file header. AIGENFILE_VERSION >= 12
struct NPCGENFILEAREA12
{
	int		iType;			//	0, on terrain; 1, box area
	int		iNumGen;		//	Number of NPC generator in this area
	float	vPos[3];
	float	vDir[3];
	float	vExts[3];
	int		iNPCType;		//	NPC type
	int		iGroupType;		//	Monster group type

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����

	int		idCtrl;			//	Controller id
	int		iLifeTime;		//	Monster's life time
	int		iMaxNum;		//	Maximum monster number

	//	ver >= 12
	int 	dwExportID;		//	Export ID
	int		iAttachNum;		//	Attaching area number. -1 means this area is a attaching area itself.
};

//	Data file header. AIGENFILE_VERSION >= 14
struct NPCGENFILEAREA14
{
	int		iType;			//	0, on terrain; 1, box area
	int		iNumGen;		//	Number of NPC generator in this area
	float	vPos[3];
	float	vDir[3];
	float	vExts[3];
	int		iNPCType;		//	NPC type
	int		iGroupType;		//	Monster group type

	bool	bInitGen;		//	�����Ƿ���ʼʱ����
	// bool	bAutoRevive;	//	��������Ƿ��Զ�����
	char	cReviveType;	//  �����������ͣ�0-������ 1-�Զ����� 2-����������
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����

	int		idCtrl;			//	Controller id
	int		iLifeTime;		//	Monster's life time
	int		iMaxNum;		//	Maximum monster number

	int 	dwExportID;		//	Export ID
	int		iAttachNum;		//	Attaching area number. -1 means this area is a attaching area itself.

	// ver >=14
	int		iPhase;
};

// NPC
struct NPCGENFILEAIGEN
{
	size_t	dwID;			//	NPC ID
	size_t	dwNum;			//	NPC number
	size_t	dwRefresh;		//	Refresh time
	size_t	dwDiedTimes;	//	Time of death before new property generatored
	size_t	dwAggressive;	//	Aggressive monster
	float	fOffsetWater;	//	Offset to water face
	float	fOffsetTrn;		//	Offset to terrain surface
	
	size_t	dwFaction;		//	�����Լ������
	size_t	dwFacHelper;	//	����Ѱ����������
	size_t	dwFacAccept;	//	������Ӧ���������
	bool	bNeedHelp;		//	�Ƿ�Ҫ���
	bool	bDefFaction;	//	�ù���Ĭ�����
	bool	bDefFacHelper;	//	�ù���Ĭ�ϵİ�����
	bool	bDefFacAccept;	//	������ӦĬ�ϵ�������

	int		iPathID;		//	·��ID, ���������·���ƶ�
	int		iLoopType;		//	ѭ����ʽ 0 ��ʾ��ѭ����1��ʾ��ԭ·���أ�ѭ������2��ʾ�����յ��ֵ���ʼ�����ѭ��
	int		iSpeedFlag;		//	1, run; 0, walk
	int		iDeadTime;		//	Disappear time after died (s)
};

//	Resources area. AIGENFILE_VERSION < 6
struct NPCGENFILERESAREA
{
	float	vPos[3];
	float	fExtX;
	float	fExtZ;
	int		iNumRes;		//	Resource number

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����
};

//	Resources area. AIGENFILE_VERSION >= 6
struct NPCGENFILERESAREA6
{
	float	vPos[3];
	float	fExtX;
	float	fExtZ;
	int		iNumRes;		//	Resource number

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����

	unsigned char dir[2];
	unsigned char rad;
};

//	Resources area. AIGENFILE_VERSION >= 7 && < 12
struct NPCGENFILERESAREA7
{
	float	vPos[3];
	float	fExtX;
	float	fExtZ;
	int		iNumRes;		//	Resource number

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����
	
	unsigned char dir[2];
	unsigned char rad;

	int		idCtrl;			//	Controller id
	int		iMaxNum;		//	Maximum resource number
};

//	Resources area. AIGENFILE_VERSION >= 12
struct NPCGENFILERESAREA12
{
	float	vPos[3];
	float	fExtX;
	float	fExtZ;
	int		iNumRes;		//	Resource number

	bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����

	unsigned char dir[2];
	unsigned char rad;

	int		idCtrl;			//	Controller id
	int		iMaxNum;		//	Maximum resource number

	//	ver >= 12
	int		dwExportID;		//	Export ID
	int		iAttachNum;		//	Attaching area number. -1 means this area is a attaching area itself.
};

//	Resources area. AIGENFILE_VERSION >= 14
struct NPCGENFILERESAREA14
{
	float	vPos[3];
	float	fExtX;
	float	fExtZ;
	int		iNumRes;		//	Resource number

    bool	bInitGen;		//	�����Ƿ���ʼʱ����
	bool	bAutoRevive;	//	��������Ƿ��Զ�����
	bool	bValidOnce;		//	�����Ƿ�ͬʱ��Чһ��
	size_t	dwGenID;		//	�����ƶ����
	
	unsigned char dir[2];
	unsigned char rad;

	int		idCtrl;			//	Controller id
	int		iMaxNum;		//	Maximum resource number

	//	ver >= 12
	int		dwExportID;		//	Export ID
	int		iAttachNum;		//	Attaching area number. -1 means this area is a attaching area itself.

	//  ver >= 14
	int		iPhase;			// ��λ
};

struct NPCGENFILERES
{
	int		iResType;		//	Resource type
	int		idTemplate;		//	ID in template database
	size_t	dwRefreshTime;	//	Refresh time
	size_t	dwNumber;		//	Resource number
	float	fHeiOff;		//	Height offset
};

//	Dynamic object
struct NPCGENFILEDYNOBJ
{
	size_t	dwDynObjID;		//	Dynamic object ID
	float	vPos[3];
	unsigned char dir[2];
	unsigned char rad;
};

//	Dynamic object
struct NPCGENFILEDYNOBJ9
{
	size_t	dwDynObjID;		//	Dynamic object ID
	float	vPos[3];
	unsigned char dir[2];
	unsigned char rad;

	unsigned char scale;
};

//	Dynamic object
struct NPCGENFILEDYNOBJ10
{
	size_t	dwDynObjID;		//	Dynamic object ID
	float	vPos[3];
	unsigned char dir[2];
	unsigned char rad;

	unsigned char scale;

	unsigned int idController;	//	Controller ID
};

//	Dynamic object
struct NPCGENFILEDYNOBJ14
{
	size_t	dwDynObjID;		//	Dynamic object ID
	float	vPos[3];
	unsigned char dir[2];
	unsigned char rad;

	unsigned char scale;

	unsigned int idController;	//	Controller ID
	int		iPhase;				//  ��λ
};



//	Controller time
struct NPCCTRLTIME
{
	int iYear;		//	�������� [1900-2100] -1 ��������
	int iMouth;		//	����ȡֵ����[0,11]  -1 ��������
	int iWeek;		//	����ȡֵ����[0,6]  0 ���������� -1 ��������
	int iDay;		//	����ȡֵ����[1,31]  -1 ��������
	int iHours;		//	����ȡֵ����[0,23]  -1 ��������
	int iMinutes;	//	����ȡֵ����[0,59]
};

//	NPC controller object
struct NPCGENFILECTRL
{
	size_t	id;
	int		iControllerID;		// 	��������ID�����ID�ɲ߻�����������ǰ��������IDһ�£����ڶԿ���������һЩ����
	char	szName[128];
	bool	bActived;			//	�Ƿ��ʼʱ�ͼ���
	int		iWaitTime;			//	�����ȴ�����ʱ��ſ�ʼ���ɶ���
	int		iStopTime;			//	�����ȴ�����ʱ����Զ�ֹͣ���� int  �����������Ч
	
	bool	bActiveTimeInvalid;
	bool	bStopTimeInvalid;

	NPCCTRLTIME ActiveTime;		//	�Ƿ���ڶ�ʱ�����ʱ��,�Լ�ʱ��ֵ�� ����ṹ 
	NPCCTRLTIME StopTime;		//	�Ƿ���ڶ�ʱֹͣ��ʱ��,�Լ�ʱ��ֵ   ����ṹ
};

//	NPC controller object. AIGENFILE_VERSION >= 8 && AIGENFILE_VERSION < 11
struct NPCGENFILECTRL8
{
	size_t	id;
	int		iControllerID;		// 	��������ID�����ID�ɲ߻�����������ǰ��������IDһ�£����ڶԿ���������һЩ����
	char	szName[128];
	bool	bActived;			//	�Ƿ��ʼʱ�ͼ���
	int		iWaitTime;			//	�����ȴ�����ʱ��ſ�ʼ���ɶ���
	int		iStopTime;			//	�����ȴ�����ʱ����Զ�ֹͣ���� int  �����������Ч
	
	bool	bActiveTimeInvalid;
	bool	bStopTimeInvalid;

	NPCCTRLTIME ActiveTime;		//	�Ƿ���ڶ�ʱ�����ʱ��,�Լ�ʱ��ֵ�� ����ṹ 
	NPCCTRLTIME StopTime;		//	�Ƿ���ڶ�ʱֹͣ��ʱ��,�Լ�ʱ��ֵ   ����ṹ

	int		iActiveTimeRange;	//	�����ʱ�䷶Χֵ,��λ����
};

//	NPC controller object. AIGENFILE_VERSION >= 11 && AIGENFILE_VERSION < 13
struct NPCGENFILECTRL11
{
	size_t	id;
	int		iControllerID;		// 	��������ID�����ID�ɲ߻�����������ǰ��������IDһ�£����ڶԿ���������һЩ����
	char	szName[128];
	bool	bActived;			//	�Ƿ��ʼʱ�ͼ���
	int		iWaitTime;			//	�����ȴ�����ʱ��ſ�ʼ���ɶ���
	int		iStopTime;			//	�����ȴ�����ʱ����Զ�ֹͣ���� int  �����������Ч
	
	bool	bActiveTimeInvalid;
	bool	bStopTimeInvalid;

	NPCCTRLTIME ActiveTime;		//	�Ƿ���ڶ�ʱ�����ʱ��,�Լ�ʱ��ֵ�� ����ṹ 
	NPCCTRLTIME StopTime;		//	�Ƿ���ڶ�ʱֹͣ��ʱ��,�Լ�ʱ��ֵ   ����ṹ

	int		iActiveTimeRange;	//	�����ʱ�䷶Χֵ,��λ����
	bool	bRepeatActived;		//	�Ƿ���ظ�����
};

//	NPC controller object. AIGENFILE_VERSION >= 13
struct NPCGENFILECTRL13
{
	size_t	id;
	int		iControllerID;		// 	��������ID�����ID�ɲ߻�����������ǰ��������IDһ�£����ڶԿ���������һЩ����
	char	szName[128];
	bool	bActived;			//	�Ƿ��ʼʱ�ͼ���
	int		iWaitTime;			//	�����ȴ�����ʱ��ſ�ʼ���ɶ���
	int		iStopTime;			//	�����ȴ�����ʱ����Զ�ֹͣ���� int  �����������Ч
	
	bool	bActiveTimeInvalid;
	bool	bStopTimeInvalid;

	NPCCTRLTIME ActiveTime;		//	�Ƿ���ڶ�ʱ�����ʱ��,�Լ�ʱ��ֵ�� ����ṹ 
	NPCCTRLTIME StopTime;		//	�Ƿ���ڶ�ʱֹͣ��ʱ��,�Լ�ʱ��ֵ   ����ṹ

	int		iActiveTimeRange;	//	�����ʱ�䷶Χֵ,��λ����
	bool	bRepeatActived;		//	�Ƿ���ظ�����

	int		  nTimeSegNum;			// ʱ��θ���
	abase::vector<NPCCTRLTIME>	aTimeSeg;	// ���ʱ��Σ�size��СΪnTimeSegNum*2, ������Ԫ��Ϊ��ʼʱ�䣬ż����Ϊ����ʱ��

	int GetTimeSegNum() const { return (nTimeSegNum & 0xffff); }
	int GetTimeSegLogic() const	{ return (nTimeSegNum/0xffff); }	// 0:����  1:����
};

#pragma pack()

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CNPCGenMan
//	
///////////////////////////////////////////////////////////////////////////

class CNPCGenMan
{
public:		//	Types

	//	Area type
	enum
	{
		AREA_ONTERRAIN = 0,
		AREA_BOX,
	};

	//	NPC type
	enum
	{
		NPC_MONSTER = 0,
		NPC_SERVER,
		NPC_INTERACTION,
	};
	
	struct AREA
	{
		int		iType;			//	0, on terrain; 1, box area
		int		iNumGen;		//	Number of NPC generator in this area
		float	vPos[3];
		float	vDir[3];
		float	vExts[3];
		int		iFirstGen;		//	First generator's index
		int		iNPCType;		//	NPC type
		int		iGroupType;		//	Monster group type
		char	cRevive;		//	�����������ͣ�0-������ 1-�Զ�����(����) 2-����������(������Ʒ)
		int		idCtrl;			//	Controller id
		int		iLifeTime;		//	Resource's life time
		int		iMaxNum;		//	Maximum resource number
		int		iPhase;			//  ��λ

		abase::vector<int>	aAttachAreas;	//	Attaching areaes
	};

	struct NPCGEN
	{
		int		iArea;			//	Index of area this NPC generator belongs to
		size_t	dwID;			//	NPC ID
		size_t	dwNum;			//	NPC number
		size_t	dwRefresh;		//	Refresh time
		size_t	dwDiedTimes;	//	Time of death before new property generatored
		size_t  dwAggressive;	//	Aggressive monster
		float	fOffsetWater;	//	Offset to water face
		float	fOffsetTrn;		//	Offset to terrain surface
		size_t	dwFaction;		//	�����Լ������
		size_t	dwFacHelper;	//	����Ѱ����������
		size_t	dwFacAccept;	//	������Ӧ���������
		bool	bNeedHelp;		//	�Ƿ�Ҫ���
		bool	bDefFaction;	//	�ù���Ĭ�����
		bool	bDefFacHelper;	//	�ù���Ĭ�ϵİ�����
		bool	bDefFacAccept;	//	������ӦĬ�ϵ�������
		int		iPathID;		//	·��ID, ���������·���ƶ�
		int		iLoopType;		//	ѭ����ʽ 0 ��ʾ��ѭ����1��ʾ��ԭ·���أ�ѭ������2��ʾ�����յ��ֵ���ʼ�����ѭ��
		int		iSpeedFlag;		//	1, run; 0, walk
		int		iDeadTime;		//	Disappear time after died (s)
	};

	//	Resources area
	struct RESAREA
	{
		float	vPos[3];
		float	fExtX;
		float	fExtZ;
		int		iFirstRes;		//	First resource index
		int		iResNum;		//	Resources number
		bool	bAutoRevive;	//	��������Ƿ��Զ�����
		int		idCtrl;			//	Controller id
		int		iMaxNum;		//	Maximum resource number
		int 	iPhase;			//  ��λ
		
		unsigned char dir[2];
		unsigned char rad;

		abase::vector<int>	aAttachAreas;	//	Attaching areaes
	};

	struct RES
	{
		int		iResType;		//	Resource type
		int		idTemplate;		//	ID in template database
		size_t	dwRefreshTime;	//	Refresh time
		size_t	dwNumber;		//	Resource number
		float	fHeiOff;		//	Height offset
	};
	
	//	Dynamic object
	struct DYNOBJ
	{
		size_t	dwDynObjID;		//	Dynamic object ID
		float	vPos[3];
		unsigned char dir[2];
		unsigned char rad;
		unsigned int idCtrl;	//	Controller ID
		unsigned char scale;
		int		iPhase;
	};
	
	//	Controller object data
	struct CONTROLLER
	{
		size_t	id;
		int		iControllerID;		// 	��������ID�����ID�ɲ߻�����������ǰ��������IDһ�£����ڶԿ���������һЩ����
		char	szName[128];
		bool	bActived;			//	�Ƿ��ʼʱ�ͼ���
		int		iWaitTime;			//	�����ȴ�����ʱ��ſ�ʼ���ɶ���
		int		iStopTime;			//	�����ȴ�����ʱ����Զ�ֹͣ���� int  �����������Ч
		
		bool	bActiveTimeInvalid;
		bool	bStopTimeInvalid;
		int		iActiveTimeRange;	//	�����ʱ�䷶Χֵ,��λ����
		bool	bRepeatActived;		//	�Ƿ���ظ�����

		NPCCTRLTIME ActiveTime;		//	�Ƿ���ڶ�ʱ�����ʱ��,�Լ�ʱ��ֵ�� ����ṹ 
		NPCCTRLTIME StopTime;		//	�Ƿ���ڶ�ʱֹͣ��ʱ��,�Լ�ʱ��ֵ   ����ṹ

		int		  nTimeSegNum;			// ��16λΪʱ��θ�������16λΪʱ����߼���ϵ��0Ϊ������1Ϊ����
		abase::vector<NPCCTRLTIME>	aTimeSeg;

		int GetTimeSegNum() const { return (nTimeSegNum & 0xffff); }
		int GetTimeSegLogic() const { return (nTimeSegNum/0xffff); }	// 0:����  1:����
	};

	typedef abase::hashtab<AREA, int, abase::_hash_function> AreaTable;
	typedef abase::hashtab<RESAREA, int, abase::_hash_function> ResAreaTable;

public:		//	Constructor and Destructor

	CNPCGenMan();
	virtual ~CNPCGenMan();

public:		//	Attributes

public:		//	Operations

	//	Load data from file
	bool Load(const char* szFileName);

	//	Get generate area number
	int GetGenAreaNum() { return m_aAreas.size(); }
	//	Get generate area by index
	const AREA& GetGenArea(int n) { return m_aAreas[n]; }
	//	Get generator number
	int GetGeneratorNum() { return m_aNPCGens.size(); }
	//	Get generator by index
	const NPCGEN& GetGenerator(int n) { return m_aNPCGens[n]; }
	//	Get generator attaching area by id
	const AREA* GetGenAttachArea(int id);

	//	Get resource area number
	int GetResAreaNum() { return m_aResAreas.size(); }
	//	Get resource area by index
	const RESAREA& GetResArea(int n) { return m_aResAreas[n]; }
	//	Get resource number
	int GetResNum() { return m_aRes.size(); }
	//	Get resource by index
	const RES& GetRes(int n) { return m_aRes[n]; }
	//	Get resources attaching area by id
	const RESAREA* GetResAttachArea(int id);
	
	//	Get dynamic object number
	int GetDynObjectNum() { return m_aDynObjs.size(); }
	//	Get dynamic object
	const DYNOBJ& GetDynObject(int n) { return m_aDynObjs[n]; }

	//	Get npc controller number
	int GetNPCCtrlNum() { return m_aControllers.size(); }
	//	Get npc controller
	const CONTROLLER& GetController(int n) { return m_aControllers[n]; }

protected:	//	Attributes

	abase::vector<AREA>			m_aAreas;			//	Generate area data
	AreaTable					m_aAAreaTab;		//	Attaching areas
	abase::vector<NPCGEN>		m_aNPCGens;			//	NPC generator data
	abase::vector<RESAREA>		m_aResAreas;		//	Resource areas
	ResAreaTable				m_aResAAreaTab;		//	Attaching resource areas
	abase::vector<RES>			m_aRes;				//	Resources
	abase::vector<DYNOBJ>		m_aDynObjs;			//	Dynamic objects
	abase::vector<CONTROLLER>	m_aControllers;		//	Controllers

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

#endif	//	_NPCGENMAN_H_
