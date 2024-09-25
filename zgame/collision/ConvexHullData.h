#ifndef _CONVEX_HULL_DATA_H_
#define _CONVEX_HULL_DATA_H_

#include "ABaseDef.h"
#include <a3dvector.h>
#include "A3DGeometry.h"
#include <vector.h>

#ifdef BMAN_EDITOR
#include "A3DFlatCollector.h"
#endif

class A3DMATRIX4;

namespace SvrCD
{


// a set of DWORD flags
#define	CHDATA_BAD_FIT				0x00000001		//��־͹����ģ�͵Ľ���Ч�����ã���Ҫ����һ����ģ����ײ����CHEditor���õ���
#define CHDATA_NPC_CANNOT_PASS		0x00000002		//Ŀǰ���ڵ��α༭���У���־��͹���Ƿ���npc��ͨ����
#define CHDATA_SKIP_COLLISION		0x00000004		//��־�Ƿ�����ײ����к��Դ�͹��
#define CHDATA_SKIP_RAYTRACE		0x00000008		//��־�Ƿ������߸����к��Դ�͹��


class CFace;

class CConvexHullData  
{
public:
	
	CConvexHullData();
	CConvexHullData(const CConvexHullData& CHData);
	~CConvexHullData();
	

	bool LoadBinaryData(FILE* InFile);
	// ��ConvexHullData��������任���任����ΪmtxTrans
	// ע�����ܴ�������任��ֻ���Ǵ�����ͬscaleֵ�ĸ��Ա任
	void Transform(const A3DMATRIX4& mtxTrans);

	int GetVertexNum() const { return (int)m_arrVertices.size(); }
	A3DVECTOR3 GetVertex(int vid) const { return m_arrVertices[vid];}
	int GetFaceNum() const { return (int)m_arrFaces.size();}
	CFace* GetFacePtr(int fid)  const { return m_arrFaces[fid];}
	DWORD GetFlags() { return m_dwFlags; }

	// build the aabb at runtime.
	bool GetAABB(A3DAABB& aabb)
	{
		if ( GetVertexNum() < 2 ) return false;

		//aabb.Build( m_arrVertices.GetData(), m_arrVertices.GetSize());
		aabb.Build( m_arrVertices.begin(), (int)m_arrVertices.size());
		return true;
	}
	// get the aabb we precomputed.
	A3DAABB GetAABB() const
	{
		return m_aabb;
	}

	void AddVertex(const A3DVECTOR3& v) { m_arrVertices.push_back(v); }
	void AddFace(const CFace& f); 

#ifdef BMAN_EDITOR
	//���Ƶ�pFC,���У����ָ����pSpecialFace,������������ʾ
	void Render(A3DFlatCollector* pFC, bool bRenderV=false, CFace* pSpecialFace=NULL, DWORD dwVColor=0xffff0000, DWORD dwFColor=0xa0ffff00, const A3DVECTOR3& vRayDir=A3DVECTOR3(1.0f,1.0f,1.0f), const A3DMATRIX4* pTransMatrix=NULL)const;

#endif

private:
	bool _ReadFromBuf(char* buf);				//��buf�ж�������
	void _BuildAABB();

private:
	DWORD		m_dwFlags;
	//AArray<A3DVECTOR3,const A3DVECTOR3&> m_arrVertices;		//�����б�����
	//AArray<CFace*, CFace*> m_arrFaces;						//������������б�����ֻ���ṩ�����ֶμ���
	abase::vector<A3DVECTOR3> m_arrVertices;		//�����б�����
	abase::vector<CFace*> m_arrFaces;						//������������б�����ֻ���ṩ�����ֶμ���

	int m_iVer;			// �汾��Ϣ�����ڴ�ȡ����
	A3DAABB m_aabb;		// keep record of convex hull's AABB.
};


}
#endif
