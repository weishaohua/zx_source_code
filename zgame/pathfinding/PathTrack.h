/********************************************************************
	created:	2008/7/30
	author:		liudong
	
	purpose:	·���Ĺ켣��
	            ·�����������ڵ�֮��ĸ������ݶ������һ���㡣����layer:��3��layer��0����4����1����3��4֮��·����layer����4.
				����joint�㣬������������layer��ͬһ���㣬���������ͬ��λ�ò�ͬlayer�ĵ㶼�ӵ�·���С�
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/

#ifndef _PF_PATH_TRACK_H_ 
#define _PF_PATH_TRACK_H_

#include <vector.h>
#include <APoint.h>

#define REPULSION_DISTANCE 0.03f //С������������������Ժϲ�Ϊһ����.

class IFile;

namespace SvrPF
{
class CPathOptimize;
struct PathNode;
	
class CPathTrack
{
public:
	CPathTrack();
	~CPathTrack();
	CPathTrack(const CPathTrack& target);
	CPathTrack& operator=(const CPathTrack& target);
	CPathTrack * Clone() const;
	void Release();
	
	//����ԭʼ·�������еĵ��ȡѹ��·�����������ɵ�ѹ��·����������8�����������ϵ��߶Ρ�
	//pathOri     : ԭʼ·���е����е㡣
	bool Init(abase::vector<APointI>& pathOri, int iLayer);
	bool Init(abase::vector<PathNode>& path);
	void SetLayer(int iLayer);

	//��ȡ·����fStep���Ⱥ��һ���� .����õ��ڵڶ����ؼ���֮�󣬷��صڶ����ؼ��㡣
	//��ˣ���ȡ��һ���㵽��ǰλ�õľ������С�� fStep��
	//fStep<=0: ��ȡ��һ���ؼ��㡣
	//fStep ������һ���㵽��ǰ�����ʵ���롣
	bool GetNextPos(PathNode& node,float& fStep);

	//����·���ӵ�ǰ����ǰ��fStep���ȣ����õ�ǰ��Ϊ�µ���ĵ㣬ɾ���Ѿ��߹���·����
	//���·���ĳ���û��fStep��ô������������·������,����ȫ�̺�·���б������һ���������һ���㶼û���ˡ�
	//�������൱�� GetNextPos �� SetCurPos ������������ϡ�
	//node  : [out] ������·������ʼ�㡣
	//fStep : [in]  ����ϣ���ߵĳ���, >0.0f��
	//return: ����ʵ���ߵĳ���,�����·ʧ�ܷ���ֵ<=0.0f��
	float GoAcrossPath(PathNode& node,const float fStep);

	//��pos���浱ǰ·���еĵ�һ���ؼ��㡣(�û��ߵ��˵�һ���ؼ���͵ڶ����ؼ���֮��)
	//���pos��·���еڶ����ؼ���ǳ�����ɾ����һ���ؼ��㼴�ɡ�(�û��ߵ��˵ڶ����ؼ���)
	bool SetCurPos(PathNode& node);
	bool GetCurPos(PathNode& node);
	bool GetEndPos(PathNode& node);
	
	bool ModifyPos(const PathNode& node, const unsigned int index);

	//node  : [out] �����㡣
	//index : [in]  ������������
	//return: �õ��Ƿ���ڡ�
	bool GetNode(PathNode& node, const unsigned int index);

	//��ȡ·���йյ�������
	unsigned int GetNodeCount();

	//��ȡ·������.
	float GetPathLen();

	//about file.
	bool Save(IFile& iFile);
	bool Load(IFile& iFile);
protected:
	//���Ըĳ����飬�Խ�ʡ�ڴ档
	abase::vector<PathNode> m_Path;

	friend class CPathOptimize;
	friend bool g_PF_AppendPath(CPathTrack& pathDest,const CPathTrack& pathSource);
};

//������·��������һ��
bool g_PF_AppendPath(CPathTrack& pathDest,const CPathTrack& pathSource);

}//end of namespace

#endif//_PF_PATH_TRACK_H_



