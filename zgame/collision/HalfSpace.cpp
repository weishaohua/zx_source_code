#include "HalfSpace.h"
#include "A3DMatrix.h"

namespace SvrCD
{
//////////////////////////////////////////////////////////////////////
// Definitions of Members of CHalfSpace
//////////////////////////////////////////////////////////////////////

void CHalfSpace::Transform(const A3DMATRIX4& mtxTrans)
{
	//��mtxTrans�ֽ��Scale,Rotate,Translate����
	A3DVECTOR3 vTranslate=mtxTrans.GetRow(3);
	float fScale=mtxTrans.GetCol(0).Magnitude();
	
	// ��mtxTransһ�����Զ���Ϊ�ҳ˾���
	A3DMATRIX3 mtx3Rotate;		
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			mtx3Rotate.m[i][j]=mtxTrans.m[i][j]/fScale;
	
	//����ƽ�淽��N.X=D�ı任(s,R,t)���£�

	// N'=NR
	SetNormal(m_vNormal*mtx3Rotate);

	//D'=s*D+N'.t
	SetD(fScale*m_d+DotProduct(m_vNormal,vTranslate));
}


}

