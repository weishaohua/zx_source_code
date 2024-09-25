/********************************************************************
	created:	2008/08/21
	author:		liudong
	
	purpose:	NPCѲ���߼���
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#ifndef _RAMBLE_AGENT_H_
#define _RAMBLE_AGENT_H_

#include "PathTrack.h"

#include <a3dvector.h>
#include <APoint.h>

using namespace SvrPF;
namespace SvrPF
{
	class CMoveMap;
}


class CRambleAgent
{
public:
	CRambleAgent(SvrPF::CMoveMap * pMoveMap);
	~CRambleAgent();

	//brief    : ��ʼ��NPCѲ��.
	//pMoveMap : [in] �����ͼ.
	//vCurPos  : [in] ����NPC��ǰ��λ��.
	//return   : ��ʼ���ɹ�����true, ���򷵻�false.
	bool Init(const A3DVECTOR3& vCurPos);

	//brief    : ����NPC��Ѳ�߷�Χ.
	//vCenter  : [in] ����NPCѲ����������ĵ�.
	//fRange   : [in] ����NPCѲ������߳���һ��. NPCѲ����������2*fRangeΪ�߳��������Ρ�
	//return   : ���óɹ�����true, ���򷵻�false.
	bool SetRambleInfo(const A3DVECTOR3& vCenter, float fRange);

	//brief    : ����NPCѲ�ߵ���һ��Ŀ����Ѳ��·��.
	//return   : ����һ��Ŀ���ɹ�����true�����򷵻�false.
	bool StartRamble();

	//brief    : ����NPC���ߵ��ٶ�.
	//fMoveStep: [in] NPCÿ�����ߵľ���.
	void SetMoveStep(float fStep);

	//brief    : �ж�NPC��ǰ�Ƿ��ھ�ֹ״̬,��Ѳ��״̬���Ǿ�ֹ״̬.
	//return   : ��ֹ״̬����true��Ѳ��״̬����false.
	bool IsStopped();

	//brief    : ����NPC����һ��.
	void MoveOn();

	//brief    : ����NPC��ǰλ��.
	A3DVECTOR3 GetCurPos() const;

#ifdef SVRPF_EDITOR
	//brief    : ����NPC��ǰѲ��·��.
	void  GetPath(CPathTrack& path);
#endif

protected:
	void _GeneratePath();
private:
	SvrPF::CMoveMap*   m_pMoveMap;
	float              m_fRange;
	float              m_fMoveStep;

	A3DVECTOR3         m_vCenter;
	APointI            m_ptCenter;     //center pos in map
	A3DVECTOR3         m_vCurPos;
	APointF            m_ptCurPos;     //cur pos in map
	APointF            m_ptGoalPos;    //goal pos in map
	int                m_iLayerCur;    // cur layer
	SvrPF::CPathTrack  m_Path;
	int                m_iAgentState;
    int                m_iDirLast;//��û�к��ʵ�·���ߵ�ʱ�����ѡ��һ��������һ�����ñ�����¼�ϴ��ߵķ����Է�NPC�ڼ�����֮����������ѭ����
};

#endif

