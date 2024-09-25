#include "TaskServer.h"
#include "TaskTempl.h"

#ifdef LINUX
	#include "../template/elementdataman.h"
#else
	#include "elementdataman.h"
#endif

#include "TaskTemplMan.h"

#ifndef _TASK_CLIENT

// ���������ļ�
bool LoadTasksFromPack(elementdataman* pDataMan, const char* szPackPath, const char* szDynPackPath, const char* szForbiddenTaskPath, const char* szNPCInfoPath)
{
	if (!GetTaskTemplMan()->Init(pDataMan))
		return false;

	if (!GetTaskTemplMan()->LoadTasksFromPack(szPackPath, szForbiddenTaskPath, true))
		return false;

	if(!GetTaskTemplMan()->LoadDynTasksFromPack(szDynPackPath))
		return false;

	if(!GetTaskTemplMan()->LoadNPCInfoFromPack(szNPCInfoPath))
		return false;

	return true;
}

// �ͷ������ļ�
void ReleaseTaskTemplMan()
{
	GetTaskTemplMan()->Release();
}

// �߻������İ汾
int TaskGetExportVersion()
{
	return GetTaskTemplMan()->GetExportVersion();
}

/*
 *	������Ա����ʧ�ܵ���ȫ��ʧ��
 */
void OnTaskForceFail(TaskInterface* pTask, unsigned long ulTaskId, bool bNotifyMem)
{
	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "TeamForceFail");

	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	const ATaskTempl *pTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	
	if (pTempl)
	{
		pTempl = pTempl->GetTopTask();
	}	

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (CurEntry.m_ID == ulTaskId || CurEntry.m_uCapTaskId == ulTaskId)
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, bNotifyMem);
			return;
		}		

		if (pTempl && pTempl->m_bTeamwork && pTempl->m_bMPTask)
		{	
			for (unsigned long k = 0; k < pTempl->m_ulMPTaskCnt; k++)
			{
				if (pTempl->m_MPTask[k].m_ulTaskID == CurEntry.m_ID)
				{
					CurEntry.ClearSuccess();
					CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);
					return;
				}	
			}
		}
	}
}

void OnTaskForceSucc(TaskInterface* pTask, unsigned long ulTaskId)
{
	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "TeamForceSucc");

	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (CurEntry.m_ID == ulTaskId || CurEntry.m_uCapTaskId == ulTaskId)
		{
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);
			return;
		}
	}
}

// ����ʦ�������������Ʒ����
void OnMasterTaskUpdateItemNum(TaskInterface *pTask, unsigned long ulTaskID, bool bAdd)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;
	ActiveTaskEntry*  pEntry;
	const ATaskTempl* pTempl;
	bool bFlag = false;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		pEntry = &pEntries[i];		
		pTempl = pEntry->GetTempl();
		if (pTempl == NULL)
			return;
		
		if (pTempl->m_bTeamwork && pTempl->m_bMPTask)
		{
			if (pTempl->m_ID == ulTaskID)
			{
				bFlag = true;				
				break;
			}

			for (unsigned long k = 0; k < pTempl->m_ulMPTaskCnt; k++)
			{
				if (pTempl->m_MPTask[k].m_ulTaskID == ulTaskID)
				{
					bFlag = true;
					break;
				}
			}

			if (bFlag == true)
				break;
		}
	}

	if (bFlag)
	{
		if (bAdd)
		{
			pEntry->m_wItemsToGet++;
		}
		else if (pEntry->m_wItemsToGet > 0)
		{
			pEntry->m_wItemsToGet--;
			
			if (pEntry->m_wItemsToGet == 0)
			{
				pEntry->ClearSuccess();
				pEntry->GetTempl()->OnSetFinished(pTask, pLst, pEntry, false);

				return;
			}
		}
		
		pTempl->NotifyClient(pTask, pEntry, TASK_SVR_NOTIFY_ITEMS_TO_GET, 0);
	}
}

// ���齨��
void OnTeamSetup(TaskInterface* pTask)
{
	const int nMemNum = pTask->GetTeamMemberNum();
	task_team_member_info mi;
	int i;
	float pos[3];
	unsigned long ulWorldId = pTask->GetPos(pos);

	for (i = 1; i < nMemNum; i++)
	{
		pTask->GetTeamMemberInfo(i, &mi);
		if (mi.m_ulWorldId != ulWorldId) continue;
		OnTeamAddMember(pTask, &mi);
	}
}

/*
 *	����ӳ�����Ա�˳�
 */

bool _teammember_leave(TaskInterface* pTask)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (!CurEntry.IsSuccess())
			continue;

		const ATaskTempl* pCapTempl = CurEntry.GetCapOrSelf();
		const ATaskTempl* pTempl = CurEntry.GetTempl();

		pCapTempl = pCapTempl->GetTopTask();
		pTempl    = pTempl->GetTopTask();

		if (pCapTempl && pCapTempl->m_bTeamwork && pCapTempl->m_bDismAsSelfFail)
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry);
			return true;
		}

		if (pTempl && pTempl->m_bMarriage)
		{
			CurEntry.ClearSuccess();
			pTempl->OnSetFinished(pTask, pLst, &CurEntry);
			return true;
		}
		
		if (pTempl && pTempl->m_bPrenticeTask && !CurEntry.HasParent())
		{
			if (pTask->IsInTeam())
			{
				// ʦ��������Ʒ-1
				TaskNotifyPlayer(
					pTask,	
					pTask->GetTeamMemberId(0),
					pTempl->m_ID,
					TASK_PLY_NOTIFY_P2M_LEAVE_TEAM);
			}			
			
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);

			return true;
		}
	}

	return false;
}

void OnTeamMemberLeave(TaskInterface* pTask)
{
	while (_teammember_leave(pTask)) {}
}

void OnTeamDisband(TaskInterface* pTask)
{
	OnTeamMemberLeave(pTask);
}

bool _team_captain_change(TaskInterface* pTask)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	const ATaskTempl* pTempl;
	
	for (int i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (!CurEntry.IsSuccess())
			continue;
		
		pTempl = CurEntry.GetTempl();		
		if (!pTempl)
			continue;
	
		if (pTempl->m_bTeamwork && pTempl->m_bCapChangeAllFail
		 || pTempl->m_bPrenticeTask)
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);
			return true;
		}
	}

	return false;
}

void OnTeamCaptainChange(TaskInterface* pTask)
{
	while (_team_captain_change(pTask)){}	
}

void OnTeamAddMember(TaskInterface* pTask, const task_team_member_info* pInfo)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	const ATaskTempl* pTempl;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];
		pTempl = CurEntry.GetTempl();
		
		if (!pTempl)
			continue;
		
		if (pTempl->m_bTeamwork)
		{
			if (pTempl->m_bSharedTask && pTempl->GetMemTaskByInfo(pInfo, pTask->GetFamilyID()))
			{
				if (pTempl->m_bCheckTeammate)
				{
					float pos[3];
					unsigned long ulWorldId = pTask->GetPos(pos);
					
					if (ulWorldId != pInfo->m_ulWorldId)
						continue;
					
					float x = pos[0] - pInfo->m_Pos[0];
					float y = pos[1] - pInfo->m_Pos[1];
					float z = pos[2] - pInfo->m_Pos[2];
					float fDist = x * x + y * y + z * z;
					
					if (fDist > pTempl->m_fTeammateDist)
						continue;
				}
				
				TaskNotifyPlayer(
					pTask,
					pInfo->m_ulId,
					pTempl->m_ID,
					TASK_PLY_NOTIFY_NEW_MEM_TASK);				
			}
			
			//ʦͽ�����ֹ�³�Ա����
			if (pTempl->m_bMPTask)
			{
				const int nMemNum = pTask->GetTeamMemberNum();
				for (int nIndex=0; nIndex<nMemNum; nIndex++)
				{
					TaskNotifyPlayer(
						pTask,
						pTask->GetTeamMemberId(nIndex),
						pTempl->m_ID,
						TASK_PLY_NOTIFY_FORCE_FAIL
						);
				}
			}
		}
	}
}

unsigned long _team_dismiss_member(TaskInterface* pTask)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	const ATaskTempl* pTempl;
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (!CurEntry.IsSuccess())
			continue;

		pTempl = CurEntry.GetTempl();

		if (!pTempl || !pTempl->m_bTeamwork)
			continue;

		if (pTempl->HasAllTeamMemsWanted(pTask, false) != 0)
		{
			CurEntry.ClearSuccess();
			pTempl->OnSetFinished(pTask, pLst, &CurEntry, false);
			return pTempl->m_ID;
		}
	}

	return 0;
}

void OnTeamDismissMember(TaskInterface* pTask, unsigned long ulPlayerId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;

	const int nMemNum = pTask->GetTeamMemberNum();
	while (true)
	{
		unsigned long ulTask = _team_dismiss_member(pTask);

		if (ulTask)
		{
			for (int j = 1; j < nMemNum; j++) // �����ӳ�
			{
				unsigned long ulId = pTask->GetTeamMemberId(j);

				if (ulId == ulPlayerId)
					continue;

				TaskNotifyPlayer(
					pTask,
					ulId,
					ulTask,
					TASK_PLY_NOTIFY_FORCE_FAIL);
			}
		}
		else
			break;
	}	
}

void OnTaskLeaveFamilyNotify(TaskInterface* pTask)
{
	if (!pTask->IsCaptain())
		return;

	// ����ʽ�Ͷ�Ա���һ��
	OnTeamDismissMember(pTask, 0);
}

void OnTaskCheckAwardDirect(TaskInterface* pTask, unsigned long ulTaskId);
void OnTaskGiveUpOneTask(TaskInterface* pTask, unsigned long ulTaskId, bool bForce);
void OnTaskAutoDelv(TaskInterface* pTask, unsigned long ulTaskId);
void OnTaskReachSite(TaskInterface* pTask,unsigned long ulTaskId);
void OnTaskLeaveSite(TaskInterface* pTask,unsigned long ulTaskId);
void OnTaskManualTrig(TaskInterface* pTask,unsigned long ulTaskId);
void OnTaskManualTrigStorage(TaskInterface* pTask,unsigned long ulTaskId, unsigned long ulStorageId);//Added 2011-03-02
void OnTaskOutZone(TaskInterface* pTask, unsigned long ulTaskID);
void OnTaskEnterZone(TaskInterface* pTask, unsigned long ulTaskID);
void OnTaskGiveUpAutoDeliverTask(TaskInterface* pTask, unsigned long ulTaskID);

void OnPlayerNotify(TaskInterface* pTask, unsigned long ulPlayerId, const void* pBuf, size_t sz)
{
	if (sz != sizeof(task_player_notify)) return;
	const task_player_notify* pNotify = static_cast<const task_player_notify*>(pBuf);
	const ATaskTempl* pTempl;

	switch (pNotify->reason)
	{
	case TASK_PLY_NOTIFY_NEW_MEM_TASK:

		if (!pTask->IsInTeam())
			break;

		pTempl = GetTaskTemplMan()->GetTopTaskByID(pNotify->task);

		if (!pTempl)
			break;

		if (pNotify->param || pTempl->QueryGlobalRequired(pTask, 0, TASK_GLOBAL_CHECK_ADD_MEM))
		{
			if (!pTempl->OnDeliverTeamMemTask(pTask, NULL) && pNotify->param)
			{
				// ֪ͨ�ӳ�ʧ��
				TaskNotifyPlayer(
					pTask,
					pTask->GetTeamMemberId(0),
					pTempl->m_ID,
					TASK_PLY_NOTIFY_FORCE_FAIL,
					pNotify->param
					);
			}
		}
		break;

	case TASK_PLY_NOTIFY_M2P_NEW_TASK:
		{
			if (!pTask->IsInTeam())
				break;

			pTempl = GetTaskTemplMan()->GetTopTaskByID(pNotify->task);

			if (!pTempl)
				break;

			if (pTempl->OnDelvierMasterPrenticeTask(pTask))
			{
				// ֪ͨʦ����������ɹ�
				TaskNotifyPlayer(
					pTask,
					pTask->GetTeamMemberId(0),
					pTempl->m_ID,
					TASK_PLY_NOTIFY_P2M_REC_SUCC,
					true
					);
			}
		}
		break;
	case TASK_PLY_NOTIFY_P2M_REC_SUCC:
	case TASK_PLY_NOTIFY_P2M_TASK_FAIL:
	case TASK_PLY_NOTIFY_P2M_LEAVE_TEAM:
		{
			OnMasterTaskUpdateItemNum(pTask, pNotify->task, (pNotify->param) != 0);
		}

		break;

	case TASK_PLY_NOTIFY_FORCE_FAIL:

		OnTaskForceFail(pTask, pNotify->task, pNotify->param != 0);
		break;

	case TASK_PLY_NOTIFY_FORCE_SUCC:

		OnTaskForceSucc(pTask, pNotify->task);
		break;

	case TASK_PLY_NOTIFY_LEAVE_FAMILY:

		OnTaskLeaveFamilyNotify(pTask);
		break;

	default:
		break;
	}
}

void OnClientNotify(TaskInterface* pTask, const void* pBuf, size_t sz)
{
	if (sz < sizeof(task_notify_base)) return;//Modified 2011-03-02
	const task_notify_base* pNotify = static_cast<const task_notify_base*>(pBuf);

	// �ǿ�������ڿ���������ϣ�ֻ��Ӧ������������
	ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(pNotify->task);
	if(pTempl && !pTempl->IsCrossServerTask() && pTask->IsInCrossServer() && pNotify->reason != TASK_CLT_NOTIFY_CHECK_GIVEUP)
		return;

	switch (pNotify->reason)
	{
	case TASK_CLT_NOTIFY_CHECK_FINISH:
		OnTaskCheckAwardDirect(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_CHECK_GIVEUP:
		OnTaskGiveUpOneTask(pTask, pNotify->task, false);
		break;
	case TASK_CLT_NOTIFY_AUTO_DELV:
		OnTaskAutoDelv(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_REACH_SITE:
		OnTaskReachSite(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_LEAVE_SITE:
		OnTaskLeaveSite(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_DYN_TIMEMARK:
		GetTaskTemplMan()->OnTaskGetDynTasksTimeMark(pTask);
		break;
	case TASK_CLT_NOTIFY_SPECIAL_AWARD:
		GetTaskTemplMan()->OnTaskGetSpecialAward(pTask);
		break;
	case TASK_CLT_NOTIFY_DYN_DATA:
		GetTaskTemplMan()->OnTaskGetDynTasksData(pTask);
		break;
	case TASK_CLT_NOTIFY_STORAGE:
		GetTaskTemplMan()->OnTaskUpdateStorage(pTask, pTask->GetCurTime());
		break;
	case TASK_CLT_NOTIFY_OUT_ZONE:
		OnTaskOutZone(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_ENTER_ZONE:
		OnTaskEnterZone(pTask, pNotify->task);
		break;
	case TASK_CLT_NOTIFY_GIVEUP_AUTO:
		OnTaskGiveUpAutoDeliverTask(pTask, pNotify->task);
		break;

	case TASK_CLT_NOTIFY_MANUAL_TRIG:
		OnTaskManualTrig(pTask, pNotify->task);
		break;

	case TASK_CLT_NOTIFY_MANUAL_TRIG_STORAGE:
		{
		if (sz != sizeof(task_notify_storage))
		{
			return;
		}
		const task_notify_storage* pNotifyStorage = static_cast<const task_notify_storage*>(pBuf);
		OnTaskManualTrigStorage(pTask, pNotifyStorage->task, pNotifyStorage->ulStorageId);
		break;
		}
	}
}

void OnTaskAutoDelv(TaskInterface* pTask, unsigned long ulTaskId)
{
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskId);

	if (!pTempl)
		return;

	if (pTempl->m_bAutoDeliver || pTempl->m_enumMethod == enumTMTinyGame)
	{
		pTempl->CheckDeliverTask(pTask, 0, NULL, false);
	}	
}


void OnTaskReachSite(TaskInterface* pTask,unsigned long ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry) return;

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl || pTaskTempl->m_enumMethod != enumTMReachSite) return;

	float pos[3];
	unsigned long ulWorldId = pTask->GetPos(pos);

	if (ulWorldId != pTaskTempl->m_ulReachSiteId ||
	   !is_in_zone(
			pTaskTempl->m_ReachSiteMin,
			pTaskTempl->m_ReachSiteMax,
			pos))
		return;

	pTaskTempl->OnSetFinished(
		pTask,
		pLst,
		pEntry);
}

void OnTaskLeaveSite(TaskInterface* pTask,unsigned long ulTaskId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry || pEntry->IsFinished()) return;

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl || pTaskTempl->m_enumMethod != enumTMLeaveSite) return;

	float pos[3];
	unsigned long ulWorldId = pTask->GetPos(pos);

	if (ulWorldId == pTaskTempl->m_ulLeaveSiteId &&
	    is_in_zone(
			pTaskTempl->m_LeaveSiteMin,
			pTaskTempl->m_LeaveSiteMax,
			pos))
		return;

	pTaskTempl->OnSetFinished(
		pTask,
		pLst,
		pEntry);
}

void OnTaskInterReachSite(TaskInterface* pTask,unsigned long ulInterObjId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();

	float pos[3];
	unsigned long ulWorldId = pTask->GetPos(pos);

	for(unsigned char i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry* pEntry = &(pLst->m_TaskEntries[i]);
		const ATaskTempl *pTempl = pEntry->GetTempl();

		if(pTempl && pTempl->m_enumMethod == enumTMInterReachSite && 	
			ulWorldId == pTempl->m_ulInterReachSiteId &&
			pTempl->m_iInterReachItemId == ulInterObjId &&
			!pEntry->IsFinished() &&
			is_in_zone(
			pTempl->m_InterReachSiteMin,
			pTempl->m_InterReachSiteMax,
			pos) 
			)
		{
			pTempl->OnSetFinished(pTask, pLst, pEntry);
			break;
		}
	}	
}

void OnTaskInterLeaveSite(TaskInterface* pTask,unsigned long ulInterObjId)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();

	float pos[3];
	unsigned long ulWorldId = pTask->GetPos(pos);

	for(unsigned char i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry* pEntry = &(pLst->m_TaskEntries[i]);
		const ATaskTempl *pTempl = pEntry->GetTempl();

		if(pTempl && pTempl->m_enumMethod == enumTMInterLeaveSite && 			
			pTempl->m_iInterLeaveItemId == ulInterObjId &&	!pEntry->IsFinished() &&	
			(ulWorldId != pTempl->m_ulInterLeaveSiteId  || !is_in_zone(pTempl->m_InterLeaveSiteMin, pTempl->m_InterLeaveSiteMax, pos))
			)
		{
			pTempl->OnSetFinished(pTask, pLst, pEntry);
			break;
		}
	}

}

void OnTaskOutZone(TaskInterface* pTask, unsigned long ulTaskID)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskID);
	if (!pEntry || pEntry->IsFinished()) return;
	
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskID);
	if (!pTaskTempl || !pTaskTempl->m_bOutZoneFail) return;

	float pos[3];
	unsigned long ulWorldID = pTask->GetPos(pos);

	if (ulWorldID == pTaskTempl->m_ulOutZoneWorldID &&
	    is_in_zone(
			pTaskTempl->m_OutZoneMinVert,
			pTaskTempl->m_OutZoneMaxVert,
			pos))
		return;

	pEntry->ClearSuccess();	
	pTaskTempl->OnSetFinished(
		pTask,
		pLst,
		pEntry);	
}

void OnTaskEnterZone(TaskInterface* pTask, unsigned long ulTaskID)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskID);
	if (!pEntry || pEntry->IsFinished()) return;
	
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskID);
	if (!pTaskTempl || !pTaskTempl->m_bEnterZoneFail) return;

	float pos[3];
	unsigned long ulWorldID = pTask->GetPos(pos);

	if (ulWorldID != pTaskTempl->m_ulEnterZoneWorldID ||
	    !is_in_zone(
			pTaskTempl->m_EnterZoneMinVert,
			pTaskTempl->m_EnterZoneMaxVert,
			pos))
		return;

	pEntry->ClearSuccess();	
	pTaskTempl->OnSetFinished(
		pTask,
		pLst,
		pEntry);	
}

bool OnTaskCheckDeliver(TaskInterface* pTask, unsigned long ulTaskId, unsigned long ulStorageId)
{
	char log[1024];
	sprintf(log, "TaskCheckDeliverAtNPC, Storage id = %d", ulStorageId);
	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), ulTaskId, 1, log);

	if (ulStorageId == 0 && GetTaskTemplMan()->IsStorageTask(ulTaskId))
	{
		TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "TaskCheckDeliver: wrong storage id");
		return false;
	}

	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());

	// �����������ܳ����ܷ��ŵ�����
	if (ulStorageId)
	{
		int nStorageTaskCount = 0;

		for (unsigned char i = 0; i < pLst->m_uTaskCount; i++)
		{
			if (GetTaskTemplMan()->IsStorageTask(pLst->m_TaskEntries[i].m_ID))
				nStorageTaskCount++;
		}

		if (nStorageTaskCount >= TASK_MAX_DELIVER_COUNT)
		{
			svr_task_err_code ret;

			ret.reason		= TASK_SVR_NOTIFY_ERROR_CODE;
			ret.task		= ulTaskId;
			ret.err_code	= TASK_PREREQU_FAIL_FULL;

			pTask->NotifyClient(&ret, sizeof(ret));
			return false;
		}
	}

	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	if (!pTaskTempl) return false;

	if (pTaskTempl->m_pParent) pTaskTempl = pTaskTempl->m_pParent;
	else ulTaskId = 0;

	if (ulStorageId > TASK_STORAGE_COUNT)
		return false;

	if (ulStorageId)
	{
		unsigned short uId = (unsigned short)pTaskTempl->GetID();
		unsigned short* arr = pLst->m_Storages[ulStorageId-1];
		int i;

		for (i = 0; i < TASK_STORAGE_LEN; i++)
			if (arr[i] == uId)
				break;

		if (i == TASK_STORAGE_LEN)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "TaskCheckDeliver: Not In Cur Storage List");
			return false;
		}
	}

	if (pTaskTempl->m_bSharedByFamily)
	{
		if (!_is_header(pTask->GetFactionRole()))
			return false;

		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetSharedTasks, pTaskTempl->m_ID, enumTaskReasonDeliver, 0, 0, 0);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		return true;
	}

	if (pTaskTempl->NeedFamilySkillData())
	{
		if (!pTask->IsInFamily())
			return false;

		TaskPairData tp(pTaskTempl->m_nFamilySkillIndex, 0, 0);
		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetSkillInfo, pTaskTempl->m_ID, enumTaskReasonDeliver, 0, &tp, 1);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		return true;
	}

	if (pTaskTempl->m_nFamilyMonRecordIndex)
	{
		if (!pTask->IsInFamily())
			return false;

		TaskPairData tp(pTaskTempl->m_nFamilyMonRecordIndex-1, 0, 0);
		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetMonsterRecord, pTaskTempl->m_ID, enumTaskReasonDeliver, 0, &tp, 1);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		return true;
	}

	if (pTaskTempl->m_nFamilyValueMin || pTaskTempl->m_nFamilyValueMax)
	{
		TaskPairData tp(pTaskTempl->m_nFamilyValueIndex, 0, 0);
		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetCommonValue, pTaskTempl->m_ID, enumTaskReasonDeliver, 0, &tp, 1);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		return true;
	}

	if (pTaskTempl->m_ulPremCircleGroupMin || pTaskTempl->m_ulPremCircleGroupMax)
	{
		int nCircleGroupId = pTask->GetCircleGroupId();
		TaskCircleGroupAsyncData d(nCircleGroupId, pTaskTempl->m_ID);
		pTask->QueryCircleGroupData(d.GetBuffer(), d.GetSize());
		return true;
	}

	if (pTaskTempl->QueryGlobalRequired(pTask, ulTaskId, TASK_GLOBAL_CHECK_FINISH_COUNT)
	 && pTaskTempl->QueryGlobalRequired(pTask, ulTaskId, TASK_GLOBAL_CHECK_RCV_NUM))
		return pTaskTempl->CheckDeliverTask(pTask, ulTaskId, NULL) == 0;

	return true;
}

void OnTaskManualTrig(TaskInterface* pTask, unsigned long ulTask)
{
	// ֻ�в�ͨ��npc����������ֶ�����
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTopTaskByID(ulTask);
	if (!pTaskTempl || pTaskTempl->m_ulDelvNPC) return;

	if(pTaskTempl->m_bManualTrig)
		OnTaskCheckDeliver(pTask, ulTask, 0);
}

void OnTaskManualTrigStorage(TaskInterface* pTask,unsigned long ulTaskId, unsigned long ulStorageId)
{
	// ֻ�в�ͨ��npc����������ֶ�����
	const ATaskTempl* pTaskTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskId);
	if (!pTaskTempl || pTaskTempl->m_ulDelvNPC || !ulStorageId) 
	{
		return;
	}

	//�Ƿ�Ҫ��֤������???
	if( pTaskTempl->m_bManualTrig )
	{
		OnTaskCheckDeliver(pTask, ulTaskId, ulStorageId);
	}

}

void OnNPCDeliverTaskItem(TaskInterface* pTask, unsigned long ulNPCId, unsigned long ulTaskId)
{
	DATA_TYPE dt;
	elementdataman* pMan = GetTaskTemplMan()->GetEleDataMan();

	NPC_ESSENCE* pNPC = (NPC_ESSENCE*)pMan->get_data_ptr(
		ulNPCId,
		ID_SPACE_ESSENCE,
		dt
		);

	if (!pNPC || dt != DT_NPC_ESSENCE)
		return;

	if (!pNPC->id_task_matter_service) return;

	NPC_TASK_MATTER_SERVICE* pSvr = (NPC_TASK_MATTER_SERVICE*)pMan->get_data_ptr(
		pNPC->id_task_matter_service,
		ID_SPACE_ESSENCE,
		dt
		);

	if (!pSvr || dt != DT_NPC_TASK_MATTER_SERVICE)
		return;

	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	const int sz = sizeof(pSvr->tasks) / sizeof(pSvr->tasks[0]);

	int i, j;
	for (i = 0; i < sz; i++)
	{
		unsigned int id = pSvr->tasks[i].id_task;

		if (!id) break;
		if (ulTaskId != id || !pLst->GetEntry(id)) continue;

		const int matter_num = sizeof(pSvr->tasks[0].taks_matters) / sizeof(pSvr->tasks[0].taks_matters[0]);
		int actual_num = 0;

		for (j = 0; j < matter_num; j++)
		{
			unsigned int matter = pSvr->tasks[i].taks_matters[j].id_matter;
			if (!matter) break;
			actual_num++;
		}

		if (actual_num == 0 || !pTask->CanDeliverTaskItem(actual_num)) break;

		for (j = 0; j < actual_num; j++)
		{
			unsigned int matter = pSvr->tasks[i].taks_matters[j].id_matter;
			int matter_num = pSvr->tasks[i].taks_matters[j].num_matter;

			if (pTask->GetTaskItemCount(matter)) continue;
			pTask->DeliverTaskItem(matter, matter_num);
		}

		break;
	}
}

void OnTaskCheckAward(TaskInterface* pTask, unsigned long ulTaskId, int nChoice)
{
	ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned long ulCurTime = pTask->GetCurTime();

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		// �ǿ�������ڿ�����������޷��ӡ��޷����
		if(!pTempl->IsCrossServerTask() && pTask->IsInCrossServer())
		{
			return;
		}

		// �����������Ҫ���ȫ�ֱ��������޷��ӣ��޷����
		if(pTempl->IsCrossServerTask() && (pTempl->m_bPremNeedComp || pTempl->m_bFinNeedComp || pTempl->m_lChangeKeyArr.size() != 0 || pTempl->m_Award_S->m_lChangeKeyArr.size() != 0))
		{
			return;
		}

		if (pTempl->m_enumFinishType != enumTFTNPC)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckAward Not By NPC");
			return;
		}

		if (pTempl->m_bMarriage)
		{
			if (pTask->IsCaptain() && pTempl->CheckMarriage(pTask) == 0)
				pTask->CheckTeamRelationship(TASK_TEAM_RELATION_MARRIAGE);

			return;
		}

		if ((pTempl->m_bSharedByFamily || pTempl->m_bFamilyHeader) && !_is_header(pTask->GetFactionRole()))
			return;

		// �������Ѿ���̬�ر�, �޷����
		if (pTask->IsDynamicForbiddenTask(pTempl->m_ID))
		{
			pTempl->NotifyClient(
				pTask,
				NULL,
				TASK_SVR_NOTIFY_ERROR_CODE,
				0,
				TASK_PREREQU_FAIL_DYNFORBIDDEN
			);
			return;
		}

		if (pTempl->m_enumMethod == enumTMCollectNumArticle)
			pTempl->CheckCollectItem(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMTitle)
			pTempl->CheckCollectTitle(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMKillNumMonster)
			pTempl->CheckMonsterKilled(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMWaitTime)
			pTempl->CheckWaitTime(pTask, pList, &CurEntry, ulCurTime, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMTalkToNPC)
		{
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		}
		else if (pTempl->m_enumMethod == enumTMFixedTime)
			pTempl->CheckFixedTime(pTask, pList, &CurEntry, ulCurTime, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMAchievement)
			pTempl->CheckFinishAchievement(pTask, pList, &CurEntry, true, nChoice);
		else if (pTempl->m_enumMethod == enumTMTotalCaseAdd)
			pTempl->CheckTotalTreasure(pTask, pList, &CurEntry, true, nChoice);
		else if(pTempl->m_enumMethod == enumTMFactionProperty)
			pTempl->CheckFactionProperty(pTask, pList, &CurEntry, true, nChoice);
		else if(pTempl->m_enumMethod == enumTMCollectNumInterObj)
			pTempl->CheckInterObjCollected(pTask, pList, &CurEntry, true, nChoice);
		else if(pTempl->m_enumMethod == enumTMBuildingToLevel)
			pTempl->CheckBuildToLevel(pTask, pList, &CurEntry, true, nChoice);
		else if (CurEntry.IsFinished())
			pTempl->DeliverAward(pTask, pList, &CurEntry, nChoice);
		else
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "CheckAwardAtNPC: Task Not Finished");

		return;
	}
}

void OnTaskCheckAwardDirect(TaskInterface* pTask, unsigned long ulTaskId)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned long ulCurTime = pTask->GetCurTime();

	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "SvrCheckAwardDirect");

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		// �ǿ�������ڿ�����������޷��ӡ��޷����
		if(!pTempl->IsCrossServerTask() && pTask->IsInCrossServer())
		{
			return;
		}

		// �����������Ҫ���ȫ�ֱ��������޷��ӣ��޷����
		if(pTempl->IsCrossServerTask() && (pTempl->m_bPremNeedComp || pTempl->m_bFinNeedComp || pTempl->m_lChangeKeyArr.size() != 0 || pTempl->m_Award_S->m_lChangeKeyArr.size() != 0))
		{
			return;
		}

		if(pTempl->m_bScriptFinishTask) // �ű��������
		{
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			return;
		}

		if (CurEntry.IsFinished() && !CurEntry.IsSuccess()) // ʧ��
		{
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
			return;
		}
		
		if (pTempl->m_bTeamwork && pTempl->m_bMPTask && CurEntry.m_wItemsToGet == 0 && pTask->IsMaster())
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry, false);
			return;
		}

		if (pTempl->m_ulTimeLimit
		 && CurEntry.m_ulTaskTime + pTempl->m_ulTimeLimit < ulCurTime) // ��ʱ
		{			
			//ͽ������ʧ�ܣ�ʦ��������Ʒ-1
			const ATaskTempl *pTopTempl = pTempl->GetTopTask();
			if (pTopTempl && pTopTempl->m_bPrenticeTask && pTask->IsInTeam())
			{
				TaskNotifyPlayer(
					pTask,	
					pTask->GetTeamMemberId(0),
					pTopTempl->m_ID,
					TASK_PLY_NOTIFY_P2M_TASK_FAIL);
				
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry, false);
			}
			else
			{
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
			}
			
			return;
		}

		// ��������ʧЧ����
		if (pTempl->m_bAbsFail)
		{
			tm cur = *localtime((long*)&ulCurTime);
			
			if(pTempl->m_tmAbsFailTime.before(&cur))
			{
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
				return;	
			}
		}

		// �����л���û��ָ����Ʒ��ʧ��
		if(pTempl->m_bHaveItemFail && pTempl->HasFailItem(pTask) ||
			pTempl->m_bNotHaveItemFail && pTempl->NotHasFailItem(pTask))
		{
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
			return;	
		}

		if (pTempl->m_enumFinishType != enumTFTDirect)
		{
			if (!pTempl->m_ulTimeLimit)
				TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckAward Not Direct");

			return;
		}

		// ��������ֻ����npc�����
		if (pTempl->m_bMarriage)
			return;

		// �������Ѿ���̬�ر�, �޷����
		if (pTask->IsDynamicForbiddenTask(pTempl->m_ID))
		{
			pTempl->NotifyClient(
				pTask,
				NULL,
				TASK_SVR_NOTIFY_ERROR_CODE,
				0,
				TASK_PREREQU_FAIL_DYNFORBIDDEN
			);
			return;
		}

		if (pTempl->m_enumMethod == enumTMCollectNumArticle)
			pTempl->CheckCollectItem(pTask, pList, &CurEntry, false, -1);
		else if (pTempl->m_enumMethod == enumTMTitle)
			pTempl->CheckCollectTitle(pTask, pList, &CurEntry, false, -1);
		else if (pTempl->m_enumMethod == enumTMKillNumMonster)
			pTempl->CheckMonsterKilled(pTask, pList, &CurEntry, false, -1);
		else if (pTempl->m_enumMethod == enumTMWaitTime)
			pTempl->CheckWaitTime(pTask, pList, &CurEntry, ulCurTime, false, -1);
		else if (pTempl->m_enumMethod == enumTMTinyGame || pTempl->m_enumMethod == enumTMNPCAction || pTempl->m_enumMethod == enumTMSpecialTrigger)
			pTempl->OnSetFinished(pTask, pList, &CurEntry, false);
		else if (pTempl->m_enumMethod == enumTMFixedTime)
			pTempl->CheckFixedTime(pTask, pList, &CurEntry, ulCurTime, false, -1);
		else if (pTempl->m_enumMethod == enumTMAchievement)
			pTempl->CheckFinishAchievement(pTask, pList, &CurEntry, false, -1);	
		else if (pTempl->m_enumMethod == enumTMFriendNum)
			pTask->QueryFriendNum(pTempl->m_ID);
		else if (pTempl->m_enumMethod == enumTMTotalCaseAdd)
			pTempl->CheckTotalTreasure(pTask, pList, &CurEntry, false, -1);
		else if(pTempl->m_enumMethod == enumTMFactionProperty)
			pTempl->CheckFactionProperty(pTask, pList, &CurEntry, false, -1);
		else if(pTempl->m_enumMethod == enumTMCollectNumInterObj)
			pTempl->CheckInterObjCollected(pTask, pList, &CurEntry, false, -1);
		else if(pTempl->m_enumMethod == enumTMBuildingToLevel)
			pTempl->CheckBuildToLevel(pTask, pList, &CurEntry, false, -1);
		else if (CurEntry.IsFinished())
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);
		else if (!pTempl->m_ulTimeLimit)
			TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 0, "SvrCheckAward Wrong Method");

		return;
	}
}

inline void _on_collect_interobj(TaskInterface* pTask, unsigned long ulTemplId)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	
	for(unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];
		if(!CurEntry.m_ulTemplAddr) continue;
		
		if(CurEntry.GetTempl()->CheckCollectInterObj(pTask, pList, &CurEntry, ulTemplId))
			break;
	}

}

void OnTaskCollectInterObj(TaskInterface* pTask, unsigned long ulTemplId)
{
	_on_collect_interobj(pTask, ulTemplId);
}

inline void _on_kill_monster(
	TaskInterface* pTask,
	unsigned long ulTemplId,
	unsigned long ulLev,
	bool bTeam,
	float fRand,
	bool bShare = false)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];
		if (!CurEntry.m_ulTemplAddr) continue;
		if (CurEntry.GetTempl()->CheckKillMonster(
			pTask,
			pList,
			&CurEntry,
			ulTemplId,
			ulLev,
			bTeam,
			fRand,
			bShare))
			break;
	}
}

void OnTaskKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand)
{
	_on_kill_monster(pTask, ulTemplId, ulLev, false, fRand);
}

void OnTaskTeamKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand)
{
	_on_kill_monster(pTask, ulTemplId, ulLev, true, fRand);
}

void OnTaskShareKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand)
{
	_on_kill_monster(pTask, ulTemplId, ulLev, false, fRand, true); //����ӹ���ɱ������
}

void OnTaskMining(TaskInterface* pTask, unsigned long ulTaskId)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* pEntry = pList->GetEntry(ulTaskId);

	if (!pEntry || !pEntry->GetTempl()) return;
	pEntry->GetTempl()->CheckMining(pTask, pList, pEntry);
}

bool _on_player_killed(TaskInterface* pTask)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (!CurEntry.IsSuccess())
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (pTempl && pTempl->m_bFailAsPlayerDie)
		{
			//ͽ������ʧ�ܣ�ʦ��������Ʒ-1
			const ATaskTempl *pTopTempl = pTempl->GetTopTask();
			if (pTopTempl && pTopTempl->m_bPrenticeTask && pTask->IsInTeam())
			{
				TaskNotifyPlayer(
					pTask,	
					pTask->GetTeamMemberId(0),
					pTopTempl->m_ID,
					TASK_PLY_NOTIFY_P2M_TASK_FAIL);
				
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry, false);
			}
			else
			{
				CurEntry.ClearSuccess();
				CurEntry.GetTempl()->OnSetFinished(pTask, pList, &CurEntry);
			}

			return true;
		}
	}

	return false;
}

void OnTaskPlayerKilled(TaskInterface* pTask)
{
	while (_on_player_killed(pTask)) {}
	GetTaskTemplMan()->CheckDeathTrig(pTask);
}

void OnTaskGiveUpOneTask(TaskInterface* pTask, unsigned long ulTaskId, bool bForce)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pList->m_TaskEntries[i];
		if (!CurEntry.m_ulTemplAddr) continue;
		const ATaskTempl* pTempl = CurEntry.GetTempl();
		if (!pTempl || pTempl->m_ID != ulTaskId) continue;
		// ��ʱ�����������Ƿ���ɣ�����ɵ����������ʱ��ǰ�ص�NPC
		pTempl->GiveUpOneTask(pTask, pList, &CurEntry, bForce);

		return;
	}
}

void OnTaskGiveUpAutoDeliverTask(TaskInterface* pTask, unsigned long ulTaskID)
{
	TaskFinishTimeList* pList = static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList());
	if(!pList)
		return;

	pList->AddOrUpdateDeliverTime(ulTaskID, pTask->GetCurTime());
}

void OnTaskCheckAllTimeLimits(unsigned long ulCurTime)
{
	GetTaskTemplMan()->OnTaskCheckAllTimeLimits(ulCurTime);
}

void OnTaskReceivedGlobalData(
	TaskInterface* pTask,
	unsigned long ulTaskId,
	unsigned char pData[TASK_GLOBAL_DATA_SIZE],
	const void* pPreservedData,
	size_t size)
{
	const TaskPreservedData* pPreserve = (const TaskPreservedData*)pPreservedData;
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	TaskGlobalData* pGlobal = (TaskGlobalData*)pData;
	unsigned short reason = pPreserve->reason;
	unsigned long ulCurTime = pTask->GetCurTime();
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(pPreserve->m_ulSrcTask);

	if (!pTempl)
		return;

	pGlobal->CheckRcvUpdateTime(pTempl, ulCurTime);

	if (pGlobal->m_ulFnshUpdateTime && ulCurTime >= pGlobal->m_ulFnshUpdateTime)
		pGlobal->m_ulFinishCount = 0;

	const ATaskTempl* pCond = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);
	pGlobal->m_ulFnshUpdateTime = (pCond ? pCond->GetFinishTimeLimit(pTask, ulCurTime) : 0);

	switch (reason)
	{
	case TASK_GLOBAL_CHECK_RCV_NUM:
	case TASK_GLOBAL_CHECK_COTASK:

		if (pTempl->CheckGlobalRequired(pTask, pPreserve->m_ulSubTaskId, pPreserve, pGlobal))
			pTempl->CheckDeliverTask(pTask,	pPreserve->m_ulSubTaskId, pGlobal);
		else
		{
			pTempl->NotifyClient(
				pTask,
				NULL,
				TASK_SVR_NOTIFY_ERROR_CODE,
				0,
				TASK_PREREQU_FAIL_MAX_RCV);
		}

		break;

	case TASK_GLOBAL_CHECK_ADD_MEM:

		if (pTempl->CheckGlobalRequired(pTask, 0, pPreserve, pGlobal))
			pTempl->OnDeliverTeamMemTask(pTask, pGlobal);

		break;

	case TASK_GLOBAL_CHECK_FINISH_COUNT:

		if (pTempl->CheckGlobalRequired(pTask, pPreserve->m_ulSubTaskId, pPreserve, pGlobal))
			pTempl->CheckDeliverTask(pTask,	pPreserve->m_ulSubTaskId, pGlobal);
		else
		{
			pTempl->NotifyClient(
				pTask,
				NULL,
				TASK_SVR_NOTIFY_ERROR_CODE,
				0,
				TASK_PREREQU_FAIL_GLOBAL_COUNT);
		}

		break;

	case TASK_GLOBAL_ADD_FINISH_COUNT: {

		pGlobal->m_ulFinishCount++;
		TaskUpdateGlobalData(ulTaskId, pGlobal->buf);

		break; }

	case TASK_GLOBAL_CLEAR_FINISH_COUNT: {

		pGlobal->m_ulFinishCount = 0;
		TaskUpdateGlobalData(ulTaskId, pGlobal->buf);

		break; }

	case TASK_GLOBAL_CHECK_FINISH_COUNT_FOR_EXTRA_AWARD: 
		{
			AWARD_DATA ad;
			ActiveTaskEntry* pEntry = pLst->GetEntry( pPreserve->m_ulSrcTask );
			if( !pEntry )
			{
				return;
			}
			pEntry->GetTempl()->CalcAwardData(pTask, &ad, pEntry, pEntry->m_ulTaskTime,	ulCurTime);
			if( pGlobal->m_ulFinishCount >= ad.m_ulGlobalFinishCountPrecondition )//�ж�ȫ������ֵ�Ƿ�ﵽ��ֵ
			{
				pEntry->SetCanDeliverExtraAward();//�ﵽ��ֵ���������Է�����ض��⽱����
			}
			pEntry->SetHasDeliverExtraAward();//�����Ѿ����Ž�����־����׼�����Ž���
			int nChoice = pPreserve->m_ulRcvNum;
			bool bNotifyTeamMem = ( pPreserve->m_ulSubTaskId != 0 );
			pEntry->GetTempl()->DeliverAward( pTask, pLst, pEntry, nChoice, bNotifyTeamMem );
			break;
		}

	case TASK_GLOBAL_UPDATE_FINISH_COUNT://Ŀǰ�����ڵ������Added 2011-03-23.
		{
			int nCountDelta = static_cast<int>(pPreserve->m_ulRcvNum); //Ŀǰ��ʱ�ɸñ������洢����Ҫ�Ĳ�ֵ����
			pGlobal->m_ulFinishCount += nCountDelta;
			TaskUpdateGlobalData(ulTaskId, pGlobal->buf);
			break;
		}
														 
	}
}

void OnTaskReceiveFamilyData(TaskInterface* pTask, const void* pData, int nSize, int nRetCode)
{
	TaskFamilyAsyncData d;

	if (!d.Unmarshal(pData, nSize))
		return;

	TaskFamilyAsyncData::_header* header = d.GetHeader();

	switch (header->nReason)
	{
	case enumTaskFamilyGetSkillInfo: {

		if (header->nTaskReason == enumTaskReasonDeliver)
		{
			if (!pTask->IsInFamily())
				return;

			if (header->nPairCount <= 0)
				return;

			TaskPairData& tp = d.GetPair(0);
			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(header->nTask);

			if (!pTempl || pTempl->m_nFamilySkillIndex != tp.key)
				return;

			unsigned long ulRet = pTempl->CheckFamilySkill(tp.value1, tp.value2);

			if (ulRet)
			{
				pTempl->NotifyClient(pTask,	NULL, TASK_SVR_NOTIFY_ERROR_CODE, 0, ulRet);
				return;
			}

			if (pTempl->QueryGlobalRequired(pTask, header->nTask, TASK_GLOBAL_CHECK_RCV_NUM))
				pTempl->CheckDeliverTask(pTask, header->nTask, NULL);
		}

		break; }

	case enumTaskFamilyUpdateSkillInfo: {

		ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
		ActiveTaskEntry* pEntry = pList->GetEntry(header->nTask);

		if (pEntry)
		{
			pEntry->SetAwardFamily();

			if (pEntry->GetTempl())
				pEntry->GetTempl()->DeliverAward(pTask, pList, pEntry, 0);
		}

		break; }

	case enumTaskFamilyGetSharedTasks: {

		if (header->nTaskReason == enumTaskReasonGetSharedTasks)
		{
			abase::hash_map<int, int, _task_hash_func> tasks;
			int i;

			for (i = 0; i < header->nPairCount; i++)
			{
				TaskPairData& tp = d.GetPair(i);
				tasks[tp.key] = tp.value1;
			}

			ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
			unsigned char n = 0;
			bool bInFamily = pTask->IsInFamily();

			while (n < pList->m_uTaskCount)
			{
				ActiveTaskEntry& CurEntry = pList->m_TaskEntries[n];
				const ATaskTempl* pTempl = CurEntry.GetTempl();

				if (pTempl && pTempl->m_bSharedByFamily)
				{
					abase::hash_map<int, int, _task_hash_func>::iterator it = tasks.find(pTempl->m_ID);

					if (bInFamily && it != tasks.end())
					{
						CurEntry.m_ulTaskTime = it->second;
						it->second = 0;
					}
					else
					{
						pList->ClearTask(pTask, &CurEntry, false);
						pList->UpdateTaskMask(*pTask->GetTaskMask());
						pTempl->NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_GIVE_UP, 0);
						continue;
					}
				}

				n++;
			}

			if (bInFamily)
			{
				for (abase::hash_map<int, int, _task_hash_func>::iterator it = tasks.begin(); it != tasks.end(); ++it)
				{
					if (it->second)
					{
						const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(it->first);

						if (pTempl)
							pTempl->DeliverTaskNoCheck(pTask, pList, it->second);
					}
				}

				for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
				{
					ActiveTaskEntry& CurEntry = pList->m_TaskEntries[i];
					const ATaskTempl* pTempl = CurEntry.GetTempl();

					if (pTempl && pTempl->m_enumMethod == enumTMKillNumMonster && pTempl->m_bSharedByFamily)
					{
						TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetMonsterInfo, pTempl->m_ID, enumTaskReasonGetMonsterInfo, pTempl->m_ID, 0, 0);
						pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
					}
				}
			}
		}
		else if (header->nTaskReason == enumTaskReasonDeliver)
		{
			for (int i = 0; i < header->nPairCount; i++)
			{
				if (header->nTask == d.GetPair(i).key)
				{
					TaskInterface::WriteLog(pTask->GetPlayerId(), header->nTask, 1, "FamilyTask: already delivered");
					return;
				}
			}

			TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetFinishedTasks, header->nTask, enumTaskReasonDeliver, 0, 0, 0);
			pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		}
	
		break; }

	case enumTaskFamilyAddSharedTask: {

		if (!_is_header(pTask->GetFactionRole()) && header->nPairCount > 0)
		{
			ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
			TaskPairData& tp = d.GetPair(0);
			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(tp.key);

			if (pTempl)
				pTempl->DeliverTaskNoCheck(pTask, pList, tp.value1);
		}

		break; }

	case enumTaskFamilyGetFinishedTasks: {

		if (header->nTaskReason == enumTaskReasonDeliver)
		{
			FinishedTaskList* pFinishedList = (FinishedTaskList*)pTask->GetFinishedTaskList();
			TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();

			for (int i = 0; i < header->nPairCount; i++)
			{
				TaskPairData& tp = d.GetPair(i);
				pFinishedList->AddOneTask(tp.key, tp.value1 != 0);
				pTimeList->AddOrUpdateDeliverTime(tp.key, tp.value2);
			}

			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(header->nTask);

			if (pTempl)
				pTempl->CheckDeliverTask(pTask, 0, NULL);
		}

		break; }

	case enumTaskFamilyGetMonsterInfo:
	case enumTaskFamilyUpdateMonsterInfo: {

		ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
		ActiveTaskEntry* pEntry = pList->GetEntry(header->nTask);

		if (pEntry)
		{
			const ATaskTempl* pTempl = pEntry->GetTempl();

			if (pTempl && pTempl->m_enumMethod == enumTMKillNumMonster)
			{
				for (unsigned long i = 0; i < pTempl->m_ulMonsterWanted; i++)
				{
					const MONSTER_WANTED& mw = pTempl->m_MonsterWanted[i];

					if (mw.m_ulDropItemId == 0)
					{
						for (int j = 0; j < header->nPairCount; j++)
						{
							TaskPairData& tp = d.GetPair(j);

							if (tp.key == mw.m_ulMonsterTemplId)
							{
								pEntry->m_wMonsterNum[i] = (unsigned short)tp.value1;
								pTempl->NotifyClient(pTask, pEntry, TASK_SVR_NOTIFY_MONSTER_KILLED, 0, i);
								break;
							}
						}
					}
				}

				if (!pEntry->IsFinished() && pTempl->HasAllMonsterWanted(pTask, pEntry))
					pTempl->OnSetFinished(pTask, pList, pEntry);
			}
		}

		break; }

	case enumTaskFamilyNotifyFinish: {

		if (pTask->IsInFamily() && !_is_header(pTask->GetFactionRole()))
		{
			ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
			ActiveTaskEntry* pEntry = pList->GetEntry(header->nTask);

			if (pEntry && pEntry->GetTempl())
			{
				if (header->nTaskReason == enumTaskReasonNotifyFinishFail)
					pEntry->ClearSuccess();

				pEntry->GetTempl()->DeliverAward(pTask, pList, pEntry, 0);
			}
		}

		break; }

	case enumTaskFamilyGetMonsterRecord: {

		if (header->nTaskReason == enumTaskReasonDeliver)
		{
			if (!pTask->IsInFamily())
				return;

			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(header->nTask);

			if (!pTempl)
				return;

			for (int i = 0; i < header->nPairCount; i++)
			{
				TaskPairData& tp = d.GetPair(i);

				if (tp.key+1 == pTempl->m_nFamilyMonRecordIndex)
				{
					unsigned long ulRet = pTempl->CheckFamilyMonsterRecord(tp.value1);

					if (ulRet)
					{
						pTempl->NotifyClient(pTask,	NULL, TASK_SVR_NOTIFY_ERROR_CODE, 0, ulRet);
						return;
					}

					if (pTempl->QueryGlobalRequired(pTask, header->nTask, TASK_GLOBAL_CHECK_RCV_NUM))
						pTempl->CheckDeliverTask(pTask, header->nTask, NULL);

					break;
				}
			}
		}

		break; }

	case enumTaskFamilyGetCommonValue:

		if (header->nTaskReason == enumTaskReasonDeliver)
		{
			if (header->nPairCount <= 0)
				return;

			TaskPairData& tp = d.GetPair(0);
			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(header->nTask);

			if (!pTempl || pTempl->m_nFamilyValueIndex != tp.key)
				return;

			unsigned long ulRet = pTempl->CheckFamilyValue(tp.value1);

			if (ulRet)
			{
				pTempl->NotifyClient(pTask,	NULL, TASK_SVR_NOTIFY_ERROR_CODE, 0, ulRet);
				return;
			}

			if (pTempl->QueryGlobalRequired(pTask, header->nTask, TASK_GLOBAL_CHECK_RCV_NUM))
				pTempl->CheckDeliverTask(pTask, header->nTask, NULL);
		}

		break;
	}
}

void OnTaskPlayerJoinFamily(TaskInterface* pTask)
{
	TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyGetSharedTasks, 0, enumTaskReasonGetSharedTasks, 0, 0, 0);
	pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
}

void OnTaskPlayerLeaveFamily(TaskInterface* pTask)
{
	ActiveTaskList* pList = (ActiveTaskList*)pTask->GetActiveTaskList();
	pList->ClearFamilyTask(pTask);

	if (pTask->IsInTeam())
	{
		if (pTask->IsCaptain())
			OnTaskLeaveFamilyNotify(pTask);
		else
			TaskNotifyPlayer(pTask,	pTask->GetTeamMemberId(0), 0, TASK_PLY_NOTIFY_LEAVE_FAMILY);
	}
}

void OnForgetLivingSkill(TaskInterface* pTask)
{
	GetTaskTemplMan()->OnForgetLivingSkill(pTask);
}

void OnTaskPlayerLevelUp(TaskInterface* pTask)
{
	GetTaskTemplMan()->UpdateStorage(pTask, (ActiveTaskList*)pTask->GetActiveTaskList(), pTask->GetCurTime());
}

void OnTaskCheckState(TaskInterface* pTask)
{
	unsigned long ulCurTime = pTask->GetCurTime();
	TaskFinishTimeList* pFinishList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	
	unsigned short i=0;
	for (; i < pFinishList->m_uCount; i++)
	{
		TaskFinishTimeEntry& entry = pFinishList->m_aList[i];
		
		if (entry.m_ulUpdateTime == 0)
			continue;
		
		ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(entry.m_uTaskId);
		
		// �ǿ�������ڿ���������ϣ��������
		if(pTempl && !pTempl->IsCrossServerTask() && pTask->IsInCrossServer())
			continue;

		if (pTempl && pTempl->CheckFinishTimeState(pTask, &entry, ulCurTime))
			pTempl->NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_FINISH_COUNT, ulCurTime, entry.m_uFinishCount);
	}
	
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	i = 0;	
	while (i < pList->m_uTaskCount)
	{
		ActiveTaskEntry* pCurEntry = &aEntries[i];
		
		if (!pCurEntry->m_ulTemplAddr)
		{
			i++;
			continue;
		}
		
		unsigned short old = pCurEntry->m_ID;
		const ATaskTempl* pTempl = pCurEntry->GetTempl();

		// �ǿ�������ڿ���������ϣ��������
		if(pTempl && !pTempl->IsCrossServerTask() && pTask->IsInCrossServer())
		{
			i++;
			continue;
		}

		if (pTempl->m_ulTimeLimit && pCurEntry->m_ulTaskTime + pTempl->m_ulTimeLimit < ulCurTime)// ��ʱ
		{
			pCurEntry->ClearSuccess();
			pCurEntry->SetFinished();
			pTempl->NotifyClient(pTask, pCurEntry, TASK_SVR_NOTIFY_FINISHED, 0);
			pTempl->DeliverAward(pTask, pList, pCurEntry, -1, false);		
		}

		if (pTempl->m_enumMethod == enumTMWaitTime && pCurEntry->m_ulTaskTime + pTempl->m_ulWaitTime < ulCurTime
			&& pTempl->m_enumFinishType == enumTFTDirect)	// �ȴ�һ��ʱ��ֱ�����
		{
			// ǿ�Ƴɹ�
			if(!pTask->IsDeliverLegal() && !pTempl->m_bClearSomeIllegalStates)
			{
				i++;
				continue;
			}

			pCurEntry->SetFinished();
			pTempl->NotifyClient(pTask, pCurEntry, TASK_SVR_NOTIFY_FINISHED, 0);
			pTempl->DeliverAward(pTask, pList, pCurEntry, -1, false);		
		}

		if (i >= pList->m_uTaskCount)
			return;	
		
		if (old != pCurEntry->m_ID)
			continue;
		
		i++;
	}
}

void OnTaskReceiveCircleGroupData(TaskInterface* pTask, const void* pData, int nSize, int nRetCode)
{
	TaskCircleGroupAsyncData d;

	if (!d.Unmarshal(pData, nSize))
		return;

	TaskCircleGroupAsyncData::_data* data = d.GetData();

	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(data->nTask);

	if (!pTempl || (pTempl->m_ulPremCircleGroupMin == 0 && pTempl->m_ulPremCircleGroupMax == 0))
		return;

	int nCircleGroupPoints = data->nPoints;

	if(!(nCircleGroupPoints >= pTempl->m_ulPremCircleGroupMin && 
		(!pTempl->m_ulPremCircleGroupMax || nCircleGroupPoints <= pTempl->m_ulPremCircleGroupMax)))
	{
		pTempl->NotifyClient(pTask,	NULL, TASK_SVR_NOTIFY_ERROR_CODE, 0, TASK_PREREQU_FAIL_CIRCLEGROUP);
		return;
	}

	pTempl->CheckDeliverTask(pTask, data->nTask, NULL);
}


// ��ú�������
void OnTaskReceiveFriendNumData(TaskInterface* pTask, unsigned long ulTaskId, unsigned long ulFriendNum)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned long ulCurTime = pTask->GetCurTime();

	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "SvrReceiveFriendNumData");

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (!pTempl || (pTempl->m_enumMethod != enumTMFriendNum))
			return;

		CurEntry.SetFinished();
		pTempl->DeliverAward(pTask, pList, &CurEntry, -1);

		return;
	}
}

// ����NPC֪ͨ
void OnTaskProtectNPCNotify(TaskInterface* pTask, unsigned long ulTaskId, int nReason)
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned long ulCurTime = pTask->GetCurTime();

	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTaskId, 1, "SvrReceiveProtectNPCNotify");

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (!pTempl || (pTempl->m_enumMethod != enumTMProtectNPC))
			return;

		switch(nReason)
		{
		case enumTaskProtectNPCSuccess:
			CurEntry.SetFinished();
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1);

			break;

		case enumTaskProtectNPCDie:
		case enumTaskProtectNPCTooFar:
		case enumTaskProtectNPCTimeOut:
			CurEntry.ClearSuccess();
			CurEntry.SetFinished();
			pTempl->NotifyClient(pTask, &CurEntry, TASK_SVR_NOTIFY_FINISHED, 0);
			pTempl->DeliverAward(pTask, pList, &CurEntry, -1, false);		

			break;
		}

		return;
	}	
}

// ��ӻ����Ƴ�ĳ������������б��У�Ŀǰ��ʱ��������ʹ�õĽӿ� Added 2011-03-23.
void OnUpdateFinishTaskList( TaskInterface* pTask, unsigned long ulTaskId, int param1, int param2 )
{
	//���Ȼ�õ�ǰ����������б������ݲ���ֵ������Ӧ�Ĳ������Ƴ�����ӣ�
	FinishedTaskList* pFinishList = static_cast< FinishedTaskList* >(pTask->GetFinishedTaskList());

	//
	if( !param1 )//param1 = 0, ���ʾ����������б������ulTaskId������
	{
		//����������б����Ƴ�������
		bool bRet = pFinishList->RemoveTask( ulTaskId );

		if( bRet ) //�Ƴ��ɹ�
		{
			//֪ͨ�ͻ�����ͬ�����Ƴ�����
			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID( ulTaskId );
			if( pTempl )//���ڸ�����
			{
				pTempl->NotifyClient( pTask, NULL, TASK_SVR_NOTIFY_RM_FINISH_TASK, 0, 0 );
			}
		}
	}
	else if( 1 == param1 ) //������ʱֻ����param1 == 1��������Ժ������չʹ��
	{
		bool bTaskSuccess = false;
		if( !param2 )//param2 == 0�� ��Լ��Ϊ��Ӹ�ulTaskId��������������б��У����ø�����Ϊ��ɳɹ�
		{
			//����Ϊ�ɹ�
			bTaskSuccess = true;
		}

		//�жϸ������Ƿ���ڣ���������ڣ���ֱ�ӷ���
		const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID( ulTaskId );
		if ( !pTempl )
		{
			return;
		}

		//��Ӹ�������������б��� 
		pFinishList->AddOneTask( ulTaskId, bTaskSuccess );
		//֪ͨ�ͻ�����ͬ���Ĳ���
		if( pTempl )//����Ҫ���ж�Ҳ���ԣ������������Ű�
		{
			pTempl->NotifyClient( pTask, NULL, TASK_SVR_NOTIFY_ADD_FINISH_TASK, 0, bTaskSuccess );
		}
	}

}

// �ı���֪ID�������ɴ�����Ŀǰ��������ʹ�õĽӿ� Added 2011-03-23.
void OnUpdateFinishTaskCount( TaskInterface* pTask, unsigned long ulTaskId, int nFinishTaskCount )
{
	//���ȣ���õ�ǰ�������ʱ���б�Ȼ���ٸ��¸�������ID��������ɴ���
	TaskFinishTimeList* pFinishTimeList = static_cast< TaskFinishTimeList* >( pTask->GetFinishedTimeList() );

	//���Ҹ������Ƿ����
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID( ulTaskId );
	
	if( nFinishTaskCount < 0 || !pTempl )//������µ�������ɴ���Ϊ�������߸�������������ڣ����ٴ���
	{
		return;
	}

	//�Ȼ�ò���¼֮ǰ��������ɴ���
	int nCountTmp = 0;
	TaskFinishTimeEntry* pEntry = pFinishTimeList->Search(ulTaskId);//��������ID�����������ʱ���б�
	if ( pEntry )//�Ѿ����ڸ������ˣ������ɴ���
	{
		nCountTmp = static_cast<int>( pEntry->m_uFinishCount );
	}
	//�����ֵ
	nCountTmp = nFinishTaskCount - nCountTmp;

	//���������б��и���ID��������ɴ���
	pFinishTimeList->AddOrUpdateFinishCountExt( ulTaskId, static_cast<unsigned long>(nFinishTaskCount) );

	//�����Ҫȫ����������ı�ȫ��������ֵ
	if ( pTempl->m_bRecFinishCountGlobal && pTempl->m_bRecFinishCount )
	{
		TaskPreservedData tpd;
		tpd.reason			= TASK_GLOBAL_UPDATE_FINISH_COUNT;
		tpd.m_ulSrcTask		= ulTaskId;
		tpd.m_ulSubTaskId	= 0;
		tpd.m_ulRcvNum		= static_cast<unsigned long>(nCountTmp);//��ʱ���ñ�����¼��ǰ��Ҫ�޸ĵ�������ɴ���
		TaskQueryGlobalData(ulTaskId, pTask->GetPlayerId(), &tpd, sizeof(tpd));		
	}

	//֪ͨ�ͻ�����ͬ���Ĳ���
	pTempl->NotifyClient( pTask, NULL, TASK_SVR_NOTIFY_FINISH_COUNT, 0, nFinishTaskCount );
}

// ͨ���������������Ŀǰ��ʱ�ṩ�������жϺ��������жϵ����񷢷Ź��� Added 2011-04-21.
void OnDeliverTaskForDebug( TaskInterface* pTask, unsigned long ulTaskId, int param )
{
	// ���ȣ�ͨ������ID���������Ӧ������ģ��
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(ulTaskId);

	// �жϸ�����ģ���Ƿ����
	if ( !pTempl )
	{
		return;
	}

	// ����param�������в�ͬ�Ĳ���
	switch ( param )
	{
		case 0: // ����������ȡ����
		{
			pTempl->CheckDeliverTask(pTask, 0, NULL, false);
			break;
		}
		case 1:	// ����������ȡ���� 
		{
			ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
			unsigned long ulCurTime = pTask->GetCurTime();
			pTempl->DeliverTaskNoCheck( pTask, pList, ulCurTime );
			break;
		}
		default:
			break;
	}
}

// �ı���֪ID�����״̬��Ŀǰ��������ʹ�� Added 2013-02-21.
void OnUpdateClearTaskState( TaskInterface* pTask, unsigned long ulTaskId )
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	for (unsigned char i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (CurEntry.m_ID != ulTaskId
		|| !CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (!pTempl)
			return;

		CurEntry.m_uState = 0;
		// clear state
		CurEntry.SetSuccess();

		//֪ͨ�ͻ���
		pTempl->NotifyClient( pTask, NULL, TASK_SVR_NOTIFY_CLEAR_STATE, 0, 0);

		return;
	}
}

// ���ʧȥ�峤ְλʱ֪ͨ(��λ���˼���)
void OnTaskPlayerLeaveFamilyHeader( TaskInterface* pTask )
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	const ATaskTempl *pTempl;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		pTempl = GetTaskTemplMan()->GetTaskTemplByID(CurEntry.m_ID);
		if (pTempl->m_bFamilyHeader)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), CurEntry.m_ID, 1, "LeaveFamilyHeadFail");
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);
		}
	}
}

// ���ʧȥ����ְλʱ֪ͨ(��λ���˼���)
void  OnTaskPlayerLeaveFactionHeader( TaskInterface* pTask )
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	unsigned char i;

	const ATaskTempl *pTempl;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		pTempl = GetTaskTemplMan()->GetTaskTemplByID(CurEntry.m_ID);
		if (pTempl->m_bPremise_FactionMaster)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), CurEntry.m_ID, 1, "LeaveFactionHeadFail");
			CurEntry.ClearSuccess();
			CurEntry.GetTempl()->OnSetFinished(pTask, pLst, &CurEntry, false);
		}
	}
}

#endif
