#include "worldmanager.h"
#include <glog.h>

/*
 *	��������������������
 */

namespace GNET
{
//�౶���飬������6����0��1��ʾû�п����౶����
void SetMultipleExp(unsigned char multiple)
{
	GLog::log(GLOG_INFO,"GM:������%d�Ķ౶���鿪��Ϊ(%s), ��������Ϊ:%d",
			gmatrix::GetServerIndex(),
			(1 != multiple || 0 != multiple)?"On":"Off",
			multiple);

	int temp_multiple = multiple;
	if(temp_multiple >= 0 && temp_multiple <= 6)
	{
		gmatrix::GetWorldParam().set_multiple_exp(multiple);	
	}
	else
	{
		GLog::log(GLOG_ERR, "GM:������%d�Ķ౶�����������󣬱���Ϊ%d", 
				gmatrix::GetServerIndex(), multiple);
	}
}

void SetNoTrade(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d���׿���Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_trade = blOn;	
}

void SetNoMail(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d�ʼ�����Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_mail = blOn;	
}

void SetNoAuction(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d��������Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_auction = blOn;	
}

void SetNoFaction(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d���ɿ���Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_faction = blOn;	
}

void SetDoubleMoney(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d˫����Ǯ����Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"On":"Off");
	gmatrix::GetWorldParam().double_money = blOn;	
}

void SetDoubleObject(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d˫�����ʿ���Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"On":"Off");
	gmatrix::GetWorldParam().double_drop = blOn;	
}


void SetDoubleSP(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d˫��Ԫ�񿪹�Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"On":"Off");
	gmatrix::GetWorldParam().double_sp = blOn;	
}

void SetNoSellPoint(unsigned char blOn)
{
	GLog::log(GLOG_INFO,"GM:������%d��ֹ�㿨���׿���Ϊ(%s)",
			gmatrix::GetServerIndex(),
			blOn?"Off":"On");
	gmatrix::GetWorldParam().forbid_cash_trade = blOn;	

}
}

