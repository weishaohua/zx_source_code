#ifndef _GENERAL_INDEX_CFG_H_
#define _GENERAL_INDEX_CFG_H_

enum {
	GENERAL_INDEX_NULL =0,		//0, 无效

	GENERAL_INDEX_BEGIN =1,		//通用索引起始

	GENERAL_INDEX_GLOBAL_BEGIN =GENERAL_INDEX_BEGIN,	//全局索引起始，范围为所有服务器
	GENERAL_INDEX_GLOBAL_DESIGNER_BEGIN =GENERAL_INDEX_GLOBAL_BEGIN,//全局索引中，策划起始
	GENERAL_INDEX_TASK_BEGIN = GENERAL_INDEX_GLOBAL_DESIGNER_BEGIN,	//任务起始
	GENERAL_INDEX_TASK_END = 90000,					//任务结束


	GENERAL_INDEX_GLOBAL_DESIGNER_END=120000,   		//全局索引中，策划结束
	GENERAL_INDEX_GLOBAL_END=160000,			//全局索引结束

	GENERAL_INDEX_LINE_BEGIN = 160001,	   	//本线索引起始
	GENERAL_INDEX_LINE_DESIGNER_BEGIN = 160001,	   	//本线索引策划起始
	GENERAL_INDEX_LINE_DESIGNER_END = 170000,	   	//本线索引策划结束
	GENERAL_INDEX_LINE_END = 180000,	   	//本线索引结束

	GENERAL_INDEX_MAP_BEGIN = 180001,		//本地图索引起始
	GENERAL_INDEX_MAP_DESIGNER_BEGIN = 180001,		//本地图索引策划起始
	GENERAL_INDEX_MAP_DESIGNER_END= 220000,			//本地图索引策划结束
	GENERAL_INDEX_MAP_END= 280000,			//本地图索引结束


	GENERAL_INDEX_END=280000,	//通用索引结束
};

//策略或脚本可以读取的区段, 权限
const struct 
{
	int range_low;
	int range_high;
} access_table [] = 
{
   {GENERAL_INDEX_GLOBAL_DESIGNER_BEGIN,GENERAL_INDEX_GLOBAL_DESIGNER_END},
   {GENERAL_INDEX_MAP_DESIGNER_BEGIN,GENERAL_INDEX_MAP_DESIGNER_END},
};
#endif
