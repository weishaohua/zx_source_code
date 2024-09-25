
#include "maptaskdata.h"
#include "puttaskdatarpc.hrp"
#include "gettaskdatarpc.hrp"
#include "gamedbclient.hpp"

namespace GNET
{

TaskData TaskData::instance;

bool TaskData::GetTaskData(int taskid, GTaskData &data)
{
	Thread::Mutex::Scoped l(locker);
	Map::const_iterator it = map.find(taskid);
	if( it == map.end() ) return false;
	if( !Valid( it->second.settime ) ) return false;
	data = it->second;
	return true;
}

void TaskData::SetTaskData(int taskid, const Octets &td)
{
	Thread::Mutex::Scoped l(locker);
	map[taskid] = GTaskData(taskid, time(NULL), td) ;
}

void TaskData::GameSetTaskData(int taskid, const Octets &td )
{
	GTaskData data(taskid, time(NULL), td);
	{
		Thread::Mutex::Scoped l(locker);
		map[taskid] = data ;
	}
	// TODO save to gamedbd
    PutTaskDataRpc *rpc = (PutTaskDataRpc*)Rpc::Call(RPC_PUTTASKDATARPC, data);
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void TaskData::GameGetTaskData(int taskid, unsigned int sid,int playerid, const Octets &env)
{
    GetTaskDataRpc *rpc = (GetTaskDataRpc*)Rpc::Call(RPC_GETTASKDATARPC, GTaskData(taskid));
	rpc->save_sid = sid;
	rpc->save_playerid = playerid;
	rpc->save_env = env;
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

};


