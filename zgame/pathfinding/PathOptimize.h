/********************************************************************
	created:	2008/7/30
	author:		liudong
	
	purpose:	ʹ·���Ĺ켣ƽ����Ȼ; ����·������.
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _PF_PATH_OPTIMIZE_H_ 
#define _PF_PATH_OPTIMIZE_H_

#include <math.h>
#include <vector.h>
#include "PfCommon.h"

namespace SvrPF
{
class  CLayerMap;
class  CPathTrack;

class CPathOptimize
{
public:
	CPathOptimize();
	~CPathOptimize();
	
	//�����������ǰ��Ӧ�����ȵ���CPathTrack�� SetCurPos(APointF& pos) �������Դӵ�ǰλ�ÿ�ʼ·���Ż���
	bool OptimizePath(CPathTrack& pathTrack, CLayerMap* pMap);

protected:

	struct CPathTrack_PosByStep
	{
		PathNode  node;
		int       iPrevBasePos;
	};

	bool GetPathByStep(abase::vector<CPathTrack_PosByStep>& pathByStep, CPathTrack& pathTrack, float stepLen, float maxLen);

	bool AddLine(CPathTrack& pathTrack, CPathTrack_PosByStep& pos);
};

}

#endif//_PF_PATH_OPTIMIZE_H_


