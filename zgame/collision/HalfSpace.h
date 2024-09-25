#ifndef _HALF_SPACE_H_
#define _HALF_SPACE_H_

#include <a3dvector.h>

class A3DMATRIX4;

namespace SvrCD
{

//class halfspace
////////////////////////////////////////////////////////////////////////////
// 
// ƽ�淽�̵Ķ���ΪN.X=D���ڱ����е���ʽΪ m_vNormal.X=m_d
// ע�⣺����һ���ƽ�淽��Ax+By+Cz+D=0��һ������������D��������
// 
////////////////////////////////////////////////////////////////////////////

class CHalfSpace
{
public:

	CHalfSpace(){}
	virtual ~CHalfSpace() {}
	CHalfSpace(const CHalfSpace& hs):m_vNormal(hs.m_vNormal) { m_d=hs.m_d; }

	A3DVECTOR3 GetNormal() const {return m_vNormal;}
	float GetDist() const { return m_d;}
	
	
	//ֱ�����÷���
	void SetNormal(const A3DVECTOR3& n) { m_vNormal=n;m_vNormal.Normalize();}
	//ֱ�����þ���d
	void SetD(float d) { m_d=d;}

	// �Ծ���Halfspace��ƽ����б任���任����ΪmtxTrans
	virtual void Transform(const A3DMATRIX4& mtxTrans);

//Attributes
protected:
	A3DVECTOR3 m_vNormal;
	float m_d;

};

}
#endif
