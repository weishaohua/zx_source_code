#include <stdlib.h>
#include <ASSERT.h>
#include "grid.h"
#include "config.h"

grid::grid():pTable(0),grid_region(0.f,0.f,0.f,0.f),
			local_region(0.f,0.f,0.f,0.f),
			slice_step(0.f),inv_step(0.f)
{
}

grid::~grid()
{
	if(pTable) 
	{
		delete []pTable;
		pTable = NULL;
	}
}

void grid::Release()
{
	if(pTable)
	{
		delete []pTable;
		pTable = NULL;
	}
	grid_region = rect(0.f,0.f,0.f,0.f);
	local_region = rect(0.f,0.f,0.f,0.f);
	slice_step = 0.f;
	inv_step = 0.f;
}

const grid & 
grid::operator=(const grid & rhs)
{
	ASSERT(pTable == 0 && &rhs != this);
	ASSERT(rhs.pTable);

	grid_region 	= rhs.grid_region;
	local_region	= rhs.local_region;
	inner_region	= rhs.inner_region;
	slice_step	= rhs.slice_step;
	inv_step	= rhs.inv_step;
	slice_count	= rhs.slice_count;
	reg_row		= rhs.reg_row;
	reg_column	= rhs.reg_column;

	int total = reg_row * reg_column;
	pTable = new slice[total];
	memcpy(pTable,rhs.pTable,total * sizeof(slice));

#ifndef NDEBUG
	for(size_t i = 0; i < (size_t)total; i ++)
	{
		ASSERT(pTable[i].spinlock == 0);
		ASSERT(pTable[i].player_list == 0);
		ASSERT(pTable[i].npc_list == 0);
		ASSERT(pTable[i].matter_list == 0);
	}
#endif
	return *this;

}

bool grid::Create(int row,int column,float step,float sx,float sz)
{
	ASSERT(pTable == NULL);
	grid_region.left = sx;
	grid_region.top = sz;
	grid_region.right = sx + column * step;
	grid_region.bottom = sz + row * step;
	inv_step = 1.f/step;
	slice_step = step;
	reg_row = row;
	reg_column = column;

	int total = row * column;
	pTable = new slice[total];
	memset(pTable, 0,sizeof(slice) * total);
	int i;
	for(i = 0; i < row*column; i ++)
	{
		/*����û�����ˣ���Ϊgrid�ľ���Ҳ����һ��
		if(i <column || i > total - column || (i % column == 0) || (i % column == column - 1))
		{
			pTable[i].flag |= slice::GRID_EDGE;
		}*/
		float left = (i % column) * step + sx;
		float top = (i / column) * step + sz;
		pTable[i].slice_range = rect(left,top,left + step,top + step);
	}
	slice_count = row*column;
	return true;
}

static bool spec_overlap(const rect & large, const rect & small)
{
	return large.IsIn(small.left,small.top) || 
		large.IsIn(small.left,small.bottom) ||
		large.IsIn(small.right,small.top) ||
		large.IsIn(small.right,small.bottom);
}

bool grid::SetRegion(const rect &local_rt, float border_size)
{
	if(local_rt.Width() <=0 || local_rt.Height() <=0) return false;
	if(!grid_region.IsIn(local_rt)) return false;
	local_region = local_rt;

//���¼���ÿ�����������ĵ�λ
	rect ne_region = local_region;	//ȥ���߽������
	ne_region.left += slice_step;
	ne_region.top += slice_step;
	ne_region.bottom -= slice_step;
	ne_region.right -= slice_step;

	rect in_region = local_region;	//�ڲ����� ��������������һ�㲻���������Ⲣ��׼ȷ
	in_region.left += border_size + slice_step;
	in_region.top += border_size + slice_step;
	in_region.bottom -= border_size + slice_step;
	in_region.right -= border_size + slice_step;

	rect out_region = local_region;	//�ⲿ�������������������ľͲ����뱾��������أ�
	out_region.left -= border_size;
	out_region.top -= border_size;
	out_region.bottom -= border_size;
	out_region.right -= border_size;

	int c1=0,c2=0,c3=0;
	for(int i = 0; i < slice_count; i ++)
	{
		rect rt = pTable[i].slice_range;
		ASSERT(spec_overlap(local_region,rt)?1:0 == local_region.IsOverlap(rt)?1:0);  
		if(!local_region.IsOverlap(rt))
		{
			//���������
			pTable[i].flag |= slice::OUTSIDE;
			if(out_region.IsOverlap(rt))
			{
				pTable[i].flag |= slice::SENSITIVE;
			}
			continue; 
		}
		c1++;
		pTable[i].flag |= slice::INSIDE;
		if(in_region.IsIn(rt)) continue;	//���ڲ�
		c2++;

		pTable[i].flag |= slice::BORDER;	
		if(ne_region.IsIn(rt)) continue;	//�ڱ߽紦
		c3++;

		pTable[i].flag |= slice::EDGE;		//���ڱ���
	}
	return true;
}

slice * grid::Locate(float x,float z) const
{
	ASSERT(grid_region.IsIn(x,z));
	int ofx = (int)((x - grid_region.left) * inv_step);
	int ofz = (int)((z - grid_region.top) * inv_step);
	unsigned int offset  = (unsigned int)(ofx + ofz * reg_column);
	if(offset >= (unsigned int)slice_count) return NULL;
	return pTable + offset;
}

slice * grid::Locate(float x,float z,int &rx,int &rz) const
{
	ASSERT(grid_region.IsIn(x,z));
	int ofx = (int)((x - grid_region.left) * inv_step);
	int ofz = (int)((z - grid_region.top) * inv_step);
	rx = ofx;rz = ofz;
	unsigned int offset  = (unsigned int)(ofx + ofz * reg_column);
	if(offset >= (unsigned int)slice_count) return NULL;
	return pTable + offset;
}


