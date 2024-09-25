#include "ConvexHullData.h"
#include "Face.h"
#include "A3DMatrix.h"

#ifdef BMAN_EDITOR
#include "A3DGDI.h"
#endif

namespace SvrCD
{

// �ļ�����İ汾���ƣ��ַ����ĳ��ȱ�����ͬ��
// old versions
#define CHDATA_HEAD_TAG_V00	"==========Convex_Hull_Data_Start========="
#define CHDATA_HEAD_TAG_V01	"V0.1======Convex_Hull_Data_Start========="

// current version
#define CHDATA_HEAD_TAG	       "V0.2======Convex_Hull_Data_Start========="



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvexHullData::CConvexHullData()
{
	m_dwFlags=0;
}

CConvexHullData::~CConvexHullData()
{
	for(int i=0;i<(int)m_arrFaces.size();i++)
		delete m_arrFaces[i];
}

CConvexHullData::CConvexHullData(const CConvexHullData& CHData)
{
	int i;
	for(i=0;i<CHData.GetVertexNum();i++)
		AddVertex(CHData.m_arrVertices[i]);
	for(i=0;i<CHData.GetFaceNum();i++)
		AddFace(*(CHData.m_arrFaces[i]));
	
	m_dwFlags=CHData.m_dwFlags;
}


void CConvexHullData::AddFace(const CFace& f)
{	
		CFace* pFace = new CFace(f);
		m_arrFaces.push_back(pFace);
}


bool CConvexHullData::LoadBinaryData(FILE* InFile)
{
	if(!InFile)
	{
		return false;
	}

	//������־���Ӷ����飡
	char *szCHDataFlag=CHDATA_HEAD_TAG;
	char szRead[60];
	memset(szRead,0,60);
	int iFlagLen=strlen(szCHDataFlag);
	
	fread(szRead, iFlagLen, sizeof(char), InFile);


	//�жϱ�־
	if(!strcmp(szRead,szCHDataFlag))	
	{
		//Ϊ���°汾��CHDATA_HEAD_TAG���汾2
		m_iVer=2;
	}
	else
	{
		szCHDataFlag=CHDATA_HEAD_TAG_V01;		
		if(!strcmp(szRead,szCHDataFlag))
		{
			//Ϊ�汾��CHDATA_HEAD_TAG_V01
			m_iVer=1;
		}
		else
		{
			szCHDataFlag=CHDATA_HEAD_TAG_V00;		
			if(!strcmp(szRead,szCHDataFlag))
			{
				//Ϊ�汾��CHDATA_HEAD_TAG_V00
				m_iVer=0;
			}
			else
				m_iVer=-1;
		}
	}

	switch(m_iVer) 
	{

	case 2:
	case 1:
		//������־λ
		fread(&m_dwFlags, sizeof(DWORD), 1, InFile);
		break;
	case 0:
		m_dwFlags=0;
		break;
	case -1:
		return false;
		break;
	}

	//������������С
	int BufSize;
	fread(&BufSize, sizeof(int), 1, InFile);
	
	//��ʼ��������
	char * buf= new char[BufSize];
	fread(buf, BufSize, sizeof(char), InFile);

	_ReadFromBuf(buf);
	delete [] buf;

	// build the convex hull's aabb
	_BuildAABB();


	return true;
}

bool CConvexHullData::_ReadFromBuf(char* buf)
{
	const int IntSize = sizeof(int);
	const int FloatSize = sizeof(float);

	if(!buf) return false;

	int iRead;
	int cur=0;
	float fRead;
	int i, j;
	//������������
	iRead=* (int *) (buf+cur);
	cur+=IntSize;
	int vNum=iRead;

	//��������������Ϣ
	for(i=0;i<vNum;i++)
	{
		A3DVECTOR3 v;
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		v.x=fRead;

		//memcpy(&fRead,buf+cur,FloatSize);
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		v.y=fRead;

		//memcpy(&fRead,buf+cur,FloatSize);
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		v.z=fRead;
		m_arrVertices.push_back(v);
	}

	//������Ƭ����
	iRead=* (int *) (buf+cur);
	cur+=IntSize;
	int fNum=iRead;
	for(i=0;i<fNum;i++)
	{
		CFace *pFace = new CFace;
		
		//������Ƭ��ƽ�淽�̲���
		A3DVECTOR3 n;
		float d;
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		n.x=fRead;

		//memcpy(&fRead,buf+cur,FloatSize);
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		n.y=fRead;

		//memcpy(&fRead,buf+cur,FloatSize);
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		n.z=fRead;

		//memcpy(&fRead,buf+cur,FloatSize);
		fRead=* (float *) (buf+cur);
		cur+=FloatSize;
		d=fRead;

		pFace->SetNormal(n);
		pFace->SetD(d);

		//�������������
		//memcpy(&iRead,buf+cur,IntSize);
		iRead=* (int *) (buf+cur);
		cur+=IntSize;
		int fvNum=iRead;
		
		for(j=0;j<fvNum;j++)
		{
			int id;
			CHalfSpace hs;
			A3DVECTOR3 n;
			float d;
			
			//��������id
			//memcpy(&iRead,buf+cur,IntSize);
			iRead=* (int *) (buf+cur);
			cur+=IntSize;
			id=iRead;
			
			//������Ӧ�ı�ƽ�淽��
			//memcpy(&fRead,buf+cur,FloatSize);
			fRead=* (float *) (buf+cur);
			cur+=FloatSize;
			n.x=fRead;

			//memcpy(&fRead,buf+cur,FloatSize);
			fRead=* (float *) (buf+cur);
			cur+=FloatSize;
			n.y=fRead;

			//memcpy(&fRead,buf+cur,FloatSize);
			fRead=* (float *) (buf+cur);
			cur+=FloatSize;
			n.z=fRead;

			//memcpy(&fRead,buf+cur,FloatSize);
			fRead=* (float *) (buf+cur);
			cur+=FloatSize;
			d=fRead;

			hs.SetNormal(n);
			hs.SetD(d);
			
			pFace->AddElement(id,hs);
		}
		
		if(m_iVer>1)
		{
			// �汾2���ϵģ�������������Ƭ��Ϣ
			
			// ����������Ƭ������
			iRead=* (int *) (buf+cur);
			cur+=IntSize;
			int iExtraFaceNum=iRead;
			
			// ����������Ƭ��Ϣ
			for(j=0; j<iExtraFaceNum; j++)
			{
				CHalfSpace hs;
				A3DVECTOR3 n;
				float d;
				
				//������Ӧ�ı�ƽ�淽��
				//memcpy(&fRead,buf+cur,FloatSize);
				fRead=* (float *) (buf+cur);
				cur+=FloatSize;
				n.x=fRead;

				//memcpy(&fRead,buf+cur,FloatSize);
				fRead=* (float *) (buf+cur);
				cur+=FloatSize;
				n.y=fRead;

				//memcpy(&fRead,buf+cur,FloatSize);
				fRead=* (float *) (buf+cur);
				cur+=FloatSize;
				n.z=fRead;

				//memcpy(&fRead,buf+cur,FloatSize);
				fRead=* (float *) (buf+cur);
				cur+=FloatSize;
				d=fRead;

				hs.SetNormal(n);
				hs.SetD(d);
				
				pFace->AddExtraHS(hs);
			}
			
		}

		m_arrFaces.push_back(pFace);
	}


	return true;
}

void CConvexHullData::_BuildAABB()
{
	m_aabb.Build( m_arrVertices.begin(), (int)m_arrVertices.size());	
}


void CConvexHullData::Transform(const A3DMATRIX4& mtxTrans)
{
	// �任����
	int i;
	for(i=0;i<(int)m_arrVertices.size();i++)
	{
		m_arrVertices[i]=m_arrVertices[i]*mtxTrans;
	}

	// �任��Ƭ
	for(i=0;i<(int)m_arrFaces.size();i++)
	{
		m_arrFaces[i]->Transform(mtxTrans);
	}

	// after transformation, we should rebuild the convex hull's aabb.
	_BuildAABB();
}

#ifdef BMAN_EDITOR
void CConvexHullData::Render(A3DFlatCollector* pFC,  bool bRenderV,CFace* pSpecialFace, DWORD dwVColor,DWORD dwFColor, const A3DVECTOR3& vRayDir, const A3DMATRIX4* pTransMatrix)const
{
	if(!pFC) return;

	if( m_arrVertices.size() == 0 ) return;

	int i;
	//���ƶ���
	if(bRenderV)
	{
		A3DVECTOR3 v;
		for(i=0;i<(int)m_arrVertices.size();i++)
		{
			if(pTransMatrix)
				v=m_arrVertices[i]*(*pTransMatrix);
			else
				v=m_arrVertices[i];
			g_pA3DGDI->Draw3DPoint(v,dwVColor,3.0f);
		}
	}

	//������Ƭ
	A3DVECTOR3 RayDir;
	//������Ƭɫ��dwFColor��alphaֵ
	DWORD ALPHA=dwFColor & 0xff000000;

	Normalize(vRayDir,RayDir);

	for(i=0;i< (int)m_arrFaces.size();i++)
	{
		int VNum=m_arrFaces[i]->GetVNum();
		A3DVECTOR3* Vs=new A3DVECTOR3[VNum];
		int IDNum=(VNum-2)*3;
		WORD* IDs=new WORD[IDNum];
		int cur=0;
		//���춥�������������
		for(int j=0;j<VNum;j++)
		{
			if(pTransMatrix)
				Vs[j]=m_arrVertices[m_arrFaces[i]->GetVID(j)]*(*pTransMatrix);
			else
				Vs[j]=m_arrVertices[m_arrFaces[i]->GetVID(j)];
			if(j<VNum-2)
			{
				IDs[cur]=0;
				IDs[cur+1]=j+1;
				IDs[cur+2]=j+2;
				cur+=3;
			}
		}
		
		//���ù��ռ�����ɫ
		A3DCOLORVALUE faceColorV(dwFColor);
		//˫���Դ��Ϊ�����ֲ�ͬ����Ƭ��
		float fdot = DotProduct(m_arrFaces[i]->GetNormal(),RayDir);
		if(fdot < 0.0f) fdot = 0.0f;
		float fWeight=fdot*0.8f;
		faceColorV*=0.2f+fWeight;
		DWORD faceColor;
		if(pSpecialFace==m_arrFaces[i])
		{
			faceColor=faceColorV.ToRGBAColor();
			faceColor=0xffffffff-faceColor;
			faceColor=faceColor & 0x00ffffff  | ALPHA;
		}
		else
			faceColor=faceColorV.ToRGBAColor() & 0x00ffffff  | ALPHA;

		//����
		pFC->AddRenderData_3D(Vs,VNum,IDs,IDNum,faceColor);

		delete [] IDs;
		delete [] Vs;
	}
}
#endif


}
