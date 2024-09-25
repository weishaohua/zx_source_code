/********************************************************************
	created:	2008/08/13
	author:		liudong
	
	purpose:	NPC�����߼���
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _FLEE_AGENT_H_
#define _FLEE_AGENT_H_

#include "PfSec.h"


#include <a3dvector.h>

using namespace SvrPF;
namespace SvrPF
{
	class CMoveMap;
	class CLayerMap;
	class CPathTrack;
}

class CFleeAgent
{
public:
	CFleeAgent(SvrPF::CMoveMap * pMoveMap);
	~CFleeAgent();

    //brief    : ��ʼ��NPC����.
	//pMoveMap : [in] �����ͼ.
	//vCurPos  : [in] ����NPC��ǰ��λ��.
	//return   : ��ʼ���ɹ�����true, ���򷵻�false.
	bool Init(const A3DVECTOR3& vCurPos);

	//brief    : ����NPCҪ�����Σ�յ�.
	//vFleePos : [in] ����NPCҪԶ���Σ�յ�.
	//fSafeDist: [in] ���밲ȫ���룬NPC��Σ�յ�ֱ�߾�����ڵ��ڸ���ֵʱ��NPC���ڰ�ȫ״̬.
	//return   : ���óɹ�����true, ���򷵻�false.
	bool SetFleePos(const A3DVECTOR3& vFleePos, float fSafeDist);

	//brief    : ����NPC����·��.
	//return   : ����·���ɹ�����true, ���򷵻�false.
	bool StartFlee();

	//brief    : �ж�NPC��ǰ�Ƿ��ڰ�ȫλ��.
	//return   : NPC��ǰ���ڰ�ȫλ�÷���true�����򷵻�false.
	bool FleeSuccess();

	//brief    : ����NPC���ߵ��ٶ�.
	//fMoveStep: [in] NPCÿ�����ߵľ���.
	void SetMoveStep(float fMoveStep);

	//brief    : ����NPC����һ��.
	void MoveOn();

	//brief    : ����NPC��ǰλ��.
	A3DVECTOR3 GetCurPos() const;

#ifdef SVRPF_EDITOR
	//brief    : ����NPC��ǰ����·��.
    void GetPath(CPathTrack& path);
#endif

protected:
	//goalPos: �������ܵ�Ŀ���
	//bCanGoStraight:Ŀ����Ƿ�ֱ�߿ɴ�.
	//curPos : ����NPC��ǰλ��
	//curPos : ����NPC���������λ��
	//pMap   : �����ͼ�ļ�
	//fSafeDist: ���밲ȫ���롣 
	//return : ���ر���Ѱ�Ұ�ȫλ�õĲ����Ƿ�ɹ���
	bool _GenerateGoalPos(APointF& goalPos,bool& bCanGoStraight,const APointF& curPos, const APointF& fleePos, SvrPF::CLayerMap* pMap, float fSafeDist);

	bool _IsVecZero(APointF& vec);
	
	//��ǰ����Ѱ·�Ȳ����Ƿ�Ϸ���
	bool _IsOperateLegal();


protected:
    SvrPF::CMoveMap * m_pMoveMap;
	float             m_fSafeDist;
	float             m_fMoveStep;

	A3DVECTOR3        m_vCurPos;
	APointF           m_ptCurPos;
	int               m_iLayerCur;
	A3DVECTOR3        m_vFleePos;
	APointF           m_ptFleePos;
	A3DVECTOR3        m_vGoalPos;
	APointF           m_ptGoalPos;
	CPathTrack        m_Path;

	int               m_iAgentState;
};


#endif


