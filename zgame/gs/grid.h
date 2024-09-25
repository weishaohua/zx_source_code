#include <spinlock.h>
#include <math.h>
#include <common/types.h>
#include "slice.h"

#ifndef __ONLINEGAME_GS_GRID_H__
#define __ONLINEGAME_GS_GRID_H__

struct grid
{

	slice * pTable;		//���з�����ɵı�
	rect  	grid_region;	//������������Ͻ������
	rect  	local_region;	//�������򣬳������������Ӧ�ý��з�����ת��
	rect  	inner_region;	//�ڲ������������ֻ���Լ�����
	float 	slice_step;	//�����Ĵ�С
	float 	inv_step;	//�����Ĵ�С�ĵ���
	int 	slice_count;
	int	reg_row;
	int	reg_column;
	
public:
	grid();
	~grid();
	const grid & operator=(const grid & rhs);
	bool Create(int row,int column,float step,float startX,float startZ);
	void Release();
	//regiion (startX,startY) - (startX + step * colum, startY + step * row)
	//
	bool SetRegion(const rect &local_rt, float border_size);
	inline slice* 	GetSlice(int index) const { return pTable + index;}
	inline slice* 	GetSlice(int x,int z) const { return pTable + x + z * reg_column;}
	inline int 	GetSliceIndex(slice *pPiece) const {return pPiece - pTable;}
	inline void	GetSlicePos(slice *pPiece, int &x,int &z) const 
	{
		x =(int)( (pPiece->slice_range.left - grid_region.left + 0.1f) * inv_step );
		z =(int)( (pPiece->slice_range.top - grid_region.top + 0.1f) * inv_step );
	}
	inline void Index2Pos(int index, int &x,int &z) const { x = index % reg_column; z = index / reg_column;}
	inline bool IsOutsideGrid(float x,float z) const { return grid_region.IsOut(x,z);}
	inline bool IsLocal(float x,float z) const { return local_region.IsIn(x,z);}
public:
	slice * Locate(float x,float z) const;
	slice * Locate(float x,float z,int &rx,int &rz) const;
};

#endif

