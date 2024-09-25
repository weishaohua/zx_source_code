#ifndef _FACE_H_
#define _FACE_H_

#include <a3dvector.h>
#include "HalfSpace.h"
#include <vector.h>

class A3DMATRIX4;

namespace SvrCD
{

class CFace:public CHalfSpace
{
public:
	CFace();
	CFace(const CFace& face);	

	virtual ~CFace();

	//���һ��Ԫ�أ�����ͬʱ���һ��vid�͸�vid����һ��id���ɵı�����Ӧ��HalfSpace
	void AddElement(int vid,const CHalfSpace& hs){ m_arrVIDs.push_back(vid);m_arrEdgeHSs.push_back(hs);}
	void AddExtraHS(const CHalfSpace& hs) { m_arrExtraHSs.push_back(hs); }
	// ����Ƭ���б任���任����ΪmtxTrans
	virtual	void Transform(const A3DMATRIX4& mtxTrans);
	int GetVNum() const { return  (int)m_arrVIDs.size();}
	int GetVID(int i) const { return m_arrVIDs[i];}
	CHalfSpace GetEdgeHalfSpace(int eid) const { return m_arrEdgeHSs[eid];}
	CHalfSpace GetExtraHalfSpace(int id) const { return m_arrExtraHSs[id];}
	int GetExtraHSNum() const { return (int)m_arrExtraHSs.size(); }

private:
	//AArray<int, int> m_arrVIDs;						//��˳���¼���������id��
	//AArray<CHalfSpace,const CHalfSpace&> m_arrEdgeHSs;		//��˳��ͬ�ϣ���¼һ���ߺ͸���ķ��������halfspace
	//AArray<CHalfSpace,const CHalfSpace&> m_arrExtraHSs;		//�������н����ʱ���ӵ�һ��Hs!

	abase::vector<int> m_arrVIDs;						//��˳���¼���������id��
	abase::vector<CHalfSpace> m_arrEdgeHSs;		//��˳��ͬ�ϣ���¼һ���ߺ͸���ķ��������halfspace
	abase::vector<CHalfSpace> m_arrExtraHSs;		//�������н����ʱ���ӵ�һ��Hs!
};


}
#endif

