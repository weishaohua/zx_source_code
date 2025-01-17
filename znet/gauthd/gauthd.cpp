#include "gauthserver.hpp"
#include "conf.h"
#include "log.h"
#include "thread.h"
#include <iostream>
#include <unistd.h>

using namespace GNET;

int main(int argc, char *argv[])
{
	if (argc != 2 || access(argv[1], R_OK) == -1)
	{
		std::cerr << "Usage: " << argv[0] << " configurefile" << std::endl;
		exit(-1);
	}

	Conf *conf = Conf::GetInstance(argv[1]);
	Log::setprogname("gauthd");
	{
		GAuthServer *manager = GAuthServer::GetInstance();
		manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
		std::string value=conf->find(manager->Identification(),"shared_key");
		manager->shared_key=Octets(value.c_str(),value.size());
		Protocol::Server(manager);
	}

	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();
	return 0;
}

