#ifndef _RAID_CONTROLLER_
#define _RAID_CONTROLLER_

#include <hashmap.h>
#include <string>
#include <vector>

#include "pathfinding/pathfinding.h"
#include "traceman.h"
#include "template/pathman.h"
#include "template/city_region.h"
#include "buffarea.h"

//-------------------------------------------------------------------
// IMPORTANT, gamed��factionlib.cpp�ļ�
// SendRaidServerRegister����Ҳ����raid_world_info, �޸��뱣��һ��
//-------------------------------------------------------------------
struct raid_world_info
{
	int raid_id;					//����id, Ҳ���ǵ�ͼID
	int raid_template_id;
	int raid_type;					//�������ͣ���ʱ����
	int queuing_time;				//�������ʱ��
	int raid_max_instance;			//���ʵ������
	struct 
	{
		int max_player_limit;		//�������
		int min_start_player_num;	//������ʼ�������
	} group_limit;
};

class raid_world_template
{
public:
	raid_world_info rwinfo;
	std::string servername;
	int cur_instance_num;
	int level_config_tid;			//�ؿ��ั���������ģ��

protected:
	trace_manager traceman;
	path_manager* pathman;
	path_finding::PathfindMap* movemap;
	city_region* region;
	buff_area*   barea;


public:
	raid_world_template() : pathman(0), movemap(0), region(0), barea(0)
	{
		memset(&rwinfo, 0, sizeof(rwinfo));
		rwinfo.raid_id = -1;
		cur_instance_num = 0;		//��ǰGS���Ѿ��еĸ���ʵ������
	}
	~raid_world_template();
	
	raid_world_template(const raid_world_template& rwt) 
	{
		memcpy(&rwinfo, &rwt.rwinfo, sizeof(rwinfo));
		servername = rwt.servername;	
		cur_instance_num = rwt.cur_instance_num;
	}

	raid_world_template& operator=(const raid_world_template& rwt)
	{
		memcpy(&rwinfo, &rwt.rwinfo, sizeof(rwinfo));
		servername = rwt.servername;	
		cur_instance_num = rwt.cur_instance_num;
		return *this;
	}

	bool Init(const char* sName, int raid_id);

	trace_manager& GetTraceMan() { return traceman; }

	path_finding::PathfindMap* GetMoveMap() { return movemap; }

	path_manager* GetPathMan() { return pathman; }

	city_region* GetCityRegion() { return region; }

	buff_area* GetBuffArea() { return barea; }

};

class raid_controller
{
public:
	typedef abase::hash_map<int, raid_world_template*> RaidWorldInfoMap;

private:
	int rwtmap_lock;
	RaidWorldInfoMap rwtMap;

public:
	raid_controller() : rwtmap_lock(0){}
	~raid_controller();

	bool AddRaidWorldTemplate(raid_world_template* t);
	raid_world_template* GetRaidWorldTemplate(int raid_id) const;	
	const RaidWorldInfoMap& GetRaidWorldInfoMap() const;
	void GetRaidWorldInfos(std::vector<raid_world_info>& infos);
	void IncRaidWorldInstance(int raid_id);
	void DecRaidWorldInstance(int raid_id);
};

#endif /*_RAID_CONTROLLER_*/
