/*
 * FILE: Terrain.h
 *
 * DESCRIPTION: header for terrain class on server side
 *
 * CREATED BY: Hedi, 2004/11/22 
 *
 * HISTORY:
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

typedef struct _TERRAINCONFIG
{
	int			nNumAreas;		// ��ͼһ�����ٿ�ѽ
	int			nNumRows;		// �ּ���ѽ
	int			nNumCols;		// �ּ���ѽ
	int			nAreaWidth;		// ÿ��Ŀ�ȣ��Ը���㣩
	int			nAreaHeight;	// ÿ��ĸ߶ȣ��Ը���㣩
	float		vGridSize;		// ÿС��ĳߴ�

	float		vHeightMin;		// 0.0 ��Ӧ�ĸ߶�
	float		vHeightMax;		// 1.0 ��Ӧ�ĸ߶�

	char		szMapPath[256];	// ��ͼ������ѽ����Ҫ���һ��б��ѽ

} TERRAINCONFIG;

class CTerrain
{
private:
	// height map buffer and width height of it
	float *				m_pHeights;			// height map points of this terrain object
	int					m_nNumVertX;		// how many points in one row of this terrain object
	int					m_nNumVertZ;		// how many points in one column of this terrain object

	// range of this terrain object
	float				m_ox;				// origin (left-top point) of this terrain object
	float				m_oz;
	
	float				m_vGridSizeInv;		// value to be multiplied to get grid coords.

	// configuration data
	TERRAINCONFIG		m_config;			// terrain configuration data

public:
	inline float * GetHeights()			{ return m_pHeights; }
	inline int GetNumVertX()			{ return m_nNumVertX; }
	inline int GetNumVertZ()			{ return m_nNumVertZ; }

protected:

public:
	CTerrain();
	~CTerrain();

	bool Init(const TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax);
	bool Release();

	float GetHeightAt(float x, float z) const;
};

#endif//_TERRAIN_H_

