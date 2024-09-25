#ifndef __GNET_FACBASE_MANAGER_H__
#define __GNET_FACBASE_MANAGER_H__

#include "log.h"
#include <map>
#include "gproviderserver.hpp"
#include "gtoptable"
namespace GNET
{
	/*����һ�����ϵ�ʵ����*/
	class FBaseLineNumber
	{
		const int max_number;//max instance number, this line
		int running_number;
//		int applying_number;
	public:	
		FBaseLineNumber(int max):max_number(max), running_number(0)/*, applying_number(0)*/{}
		void Free()
		{
			running_number--;
			LOG_TRACE("FacBase after free, running_number=%d max_number=%d", running_number, max_number);
		}
		bool Alloc()
		{
			if (GetFree() > 0)
			{
				running_number++;
				LOG_TRACE("alloc facbase, running_number %d max_number %d", running_number, max_number);
				return true;
			}
			else
				return false;
		}
		int GetFree()
		{
			return max_number - running_number;
		}
		int GetMax()
		{
			return max_number;
		}
	};
	
	class FacBaseInfo
	{
		enum
		{
			SWITCH_TIMEOUT = 40,
			CLOSING_TIME = 180,	//gs Ҫ������ �ٹر�
			DISCONNECT_TIMEOUT = 180, //���ߺ����Զ��ָ�״̬
		};
		enum
		{
			STATUS_INIT = 0,//��ռ�ÿ�λ����δ����gs ����gsһ��������100�����ء�
			STATUS_ALLOCATING = 1, //���ڷ���gs
			STATUS_ALLOCATE = 2,//�ѷ���gs
			STATUS_CLOSING = 3, //������Ϊɾ��״̬ �رջ���			
			STATUS_DISCONNECT = 4,///����� gs ���ӶϿ���ά�ִ�״̬�ȴ����� gs ���̳ɹ�
		};
		int gsid;
		int index;
		int fid;
		int status;
		int status_timeout;
		int noti_roleid;
		std::set<int> mall_indexes;
	public:
		FacBaseInfo(int faction) : gsid(0), index(-1), fid(faction), status(STATUS_INIT), status_timeout(0), noti_roleid(0) {}
		void CheckStatusChange();
		bool IsAllocated()
		{
			CheckStatusChange();
			return status == STATUS_ALLOCATE;
		}
		bool IsFree() 
		{
			CheckStatusChange();
			return status == STATUS_INIT;
		}
		void TryStart(int gs, int roleid);
		void SetStatus(int st);
		void OnStart(int retcode, int faction, int gs, int in, const std::set<int> & mall);
//		int Enter(int & gs, int & in);
		int GetIndex(int & gs, int & in);
		int GetGS() { return gsid; }
		void OnDisconnect(int gs);
		void OnMallChange(const std::set<int> & mall);
		const std::set<int> & GetMall() const { return mall_indexes; }
		void OnFactionDel();
		void Reset()
		{
			gsid = 0;
			index = -1;
			noti_roleid = 0;
			mall_indexes.clear();
		}
		int GetStatus() { return status; }
		bool OnStop(int retcode, int faction, int gs, int in);
	};
	class FacBaseManager //���ɻ��ع�����
	{
	public:
		enum
		{
			CARE_TOP_NUM = 100, //���а�ǰ100���İ�����ǿ�ƽ�����ص�Ȩ��
		};
	private:
		typedef std::map<int/*gsid*/, FBaseLineNumber> LineNumber;
		LineNumber line_num; //ÿ�����ϵ�����������
		int total_num; //�������Ͽɷ���Ļ����ܺ�
		typedef std::map<int/*fid*/, FacBaseInfo> BaseMap;
		BaseMap base_map; //�������δ���� gs �Ļ���
//		typedef std::set<int/*fid*/> FactionInTop; //���ɻ�Ծ�������а�ǰ 100 ��
//		FactionInTop fac_top;
		bool load_top;

		FacBaseManager() : total_num(0), load_top(false) {};
	public:
		void RegisterServer(int gs_id, int count);
		bool TryAllocGS(int & gs);
		void OnActTopUpdate(const GTopTable & weektop);
		int TryStartBase(int faction, int noti_roleid=0);
		FacBaseInfo * FindBase(int faction);
		void OnBaseStart(int retcode, int fid, int gsid, int index, const std::set<int> & mall);
		static FacBaseManager * GetInstance()
		{
			static FacBaseManager inst;
			return &inst;
		}
		void OnDisconnect(int gs);
		int GetBaseIndex(int fid, int & gs, int & index);
		/*
		int EnterBase(int fid, int & gs, int & index);
		int OnBasePropChange(int fid, int & base_gs, int & base_index);
		int OnBaseBuildingProgress(int fid, int & base_gs, int & base_index);
		*/
		void OnMallChange(int fid, const std::set<int> & mall);
		void OnGSConnect(unsigned int sid);
		void OnFactionDel(int fid);
		void OnBaseStop(int retcode, int fid, int gsid, int index);
		void DebugSetLimit(int n) { total_num = n; }
		int GetCurNum() { return base_map.size(); }
	};
} // namespace GNET

#endif
