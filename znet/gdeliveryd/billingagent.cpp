#include "billingagent.h"
#include "log.h"
#include "billingrequest.hpp"
#include "billingrequest2.hpp"
#include "gproviderserver.hpp"
#include "gauthclient.hpp"

namespace GNET
{
bool BillingAgent::Initialize()
{
	char buf[128];
	char menuid[128];
	char usedurl[128];
	FILE *fp = fopen("menuid.txt", "r");

	if(!fp)
		return false;
	activated = 1;

	while( fgets(buf, 512, fp) )
	{
		ShopItem x;
		Octets   id, url;

		sscanf(buf, "%d%d%d%d\t%[^\t]\t%[^\n]", &x.id, &x.amount, &x.timeout, &x.number, menuid, usedurl);
		id.replace(menuid, strlen(menuid));
		url.replace(usedurl, strlen(usedurl));
		if(menuids.find(x)!=menuids.end())
			Log::log(LOG_ERR,"BillingAgent, duplicate item,id=%d,amount=%d,timeout=%d",x.id, x.amount, x.timeout);
		if(itemids.find(id)!=itemids.end())
			Log::log(LOG_ERR,"BillingAgent, duplicate menuid,id=%s",menuid);

		menuids[x]  = MenuItem(id,url);
		itemids[id] = x;
	}
	fclose(fp);
	LOG_TRACE("billing: menu loaded, menuids size=%d, itemids size=%d", menuids.size(), itemids.size());
	return true;
}

bool BillingAgent::Itemid2Menuid(int id, int number, int timeout, Octets& menuid, Octets& url)
{
	MenuidMap::iterator it = menuids.find(ShopItem(id,number,timeout));
	if(it==menuids.end())
	{
		Log::log(LOG_ERR,"BillingAgent, item not found,id=%d,number=%d,timeout=%d",id,number,timeout);
		return false;
	}
	menuid = it->second.menuid;
	url = it->second.url;
	return true;
}

bool BillingAgent::Menuid2Itemid(const Octets& menuid, int& id, int& number, int& timeout)
{
	ItemidMap::iterator it = itemids.find(menuid);
	if(it==itemids.end())
	{
		Log::log(LOG_ERR,"BillingAgent, menuid not found,menuid=%.*s",menuid.size(),menuid.begin());
		return false;
	}
	ShopItem &x = it->second;
	id = x.id;
	number = x.number;
	timeout = x.timeout;
	return true;
}

void BillingAgent::SendToGs(int gameid, const BillingRequest &rqst)
{
	BillingRequest2 rqst2;
	rqst2.userid = rqst.userid;
	rqst2.request = rqst.request;
	rqst2.result = rqst.result;
	rqst2.itemid = rqst.itemid;
	rqst2.itemnum = rqst.itemnum;
	rqst2.timeout = rqst.timeout;
	rqst2.amount = rqst.amount;
	rqst2.count = rqst.itemnum;
	rqst2.menuid = rqst.menuid;
	rqst2.bxtxno = rqst.bxtxno;
	rqst2.agtxno = rqst.agtxno;
	GProviderServer::GetInstance()->DispatchProtocol(gameid, rqst2);
}

void BillingAgent::SendToAu(const BillingRequest2 &rqst2)
{
	BillingRequest rqst;
	rqst.userid = rqst2.userid;
	rqst.request = rqst2.request;
	rqst.result = rqst2.result;
	rqst.itemid = rqst2.itemid;
	rqst.itemnum = rqst2.itemnum*rqst2.count;
	rqst.timeout = rqst2.timeout;
	rqst.amount = rqst2.amount;
	rqst.menuid = rqst2.menuid;
	rqst.bxtxno = rqst2.bxtxno;
	rqst.agtxno = rqst2.agtxno;
	GAuthClient::GetInstance()->SendProtocol(rqst);
}
}
