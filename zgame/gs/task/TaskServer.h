#ifndef _TASKSERVER_H_
#define _TASKSERVER_H_

#include "TaskInterface.h"

#ifndef _TASK_CLIENT

class elementdataman;

// ���������ļ�
bool LoadTasksFromPack(elementdataman* pDataMan, const char* szPackPath, const char* szDynPackPath, const char* szForbiddenTaskPath, const char* szNPCInfoPath);

// �ͷ������ļ�
void ReleaseTaskTemplMan();

// �߻������İ汾
int TaskGetExportVersion();

// ֪ͨ�ӳ�����
void OnTeamSetup(TaskInterface* pTask);

// ֪ͨ�ӳ���ɢ
void OnTeamDisband(TaskInterface* pTask);

// ֪ͨ�ӳ������Ա
void OnTeamAddMember(TaskInterface* pTask, const task_team_member_info* pInfo);

// ֪ͨ�ӳ���Ա�˳�
void OnTeamDismissMember(TaskInterface* pTask, unsigned long ulPlayerId);

// ֪ͨ��Ա�˳�
void OnTeamMemberLeave(TaskInterface* pTask);

// ֪ͨ��Ա����ӳ�����
void OnTeamCaptainChange(TaskInterface* pTask);

// �����������֪ͨ
void OnPlayerNotify(TaskInterface* pTask, unsigned long ulPlayerId, const void* pBuf, size_t sz);

// ���տͻ���֪ͨ
void OnClientNotify(TaskInterface* pTask, const void* pBuf, size_t sz);

// ��������ܷ񷢷�
bool OnTaskCheckDeliver(TaskInterface* pTask, unsigned long ulTaskId, unsigned long ulStorageId);

// NPC����������Ʒ
void OnNPCDeliverTaskItem(TaskInterface* pTask, unsigned long ulNPCId, unsigned long ulTaskId);

// �����NPC����ɵ�����
void OnTaskCheckAward(TaskInterface* pTask, unsigned long ulTasklId, int nChoice);

// ��ұ�ɱ
void OnTaskPlayerKilled(TaskInterface* pTask);

// ��ʱ�����������ϵͳ��ʱ���н��������������״̬
void OnTaskCheckAllTimeLimits(unsigned long ulCurTime);

//�����ռ�������Ʒ
void OnTaskCollectInterObj(TaskInterface* pTask, unsigned long ulTemplId);

//������Ʒ����ָ���ص�
void OnTaskInterReachSite(TaskInterface* pTask,unsigned long ulInterObjId);

//������Ʒ�뿪ָ���ص�
void OnTaskInterLeaveSite(TaskInterface* pTask,unsigned long ulInterObjId);


// ����ɱ��
void OnTaskKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand);

// ���ɱ��
void OnTaskTeamKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand);

// ����ӹ���ɱ�� Added 2012-04-09.
void OnTaskShareKillMonster(TaskInterface* pTask, unsigned long ulTemplId, unsigned long ulLev, float fRand);

// �������
void OnTaskMining(TaskInterface* pTask, unsigned long ulTaskId);

// ��������ȫ������
void TaskQueryGlobalData(
	unsigned long ulTaskId,
	unsigned long ulPlayerId,
	const void* pPreservedData,
	size_t size);

// ��������ȫ������
void TaskUpdateGlobalData(
	unsigned long ulTaskId,
	const unsigned char pData[TASK_GLOBAL_DATA_SIZE]);

// ���ȫ������
void OnTaskReceivedGlobalData(
	TaskInterface* pTask,
	unsigned long ulTaskId,
	unsigned char pData[TASK_GLOBAL_DATA_SIZE],
	const void* pPreservedData,
	size_t size);

// ��ü�������
void OnTaskReceiveFamilyData(TaskInterface* pTask, const void* pData, int nSize, int nRetCode);

// �������֪ͨ
void OnTaskPlayerJoinFamily(TaskInterface* pTask);

// �˳�����֪ͨ
void OnTaskPlayerLeaveFamily(TaskInterface* pTask);

// ���������
void OnForgetLivingSkill(TaskInterface* pTask);

// �������
void OnTaskPlayerLevelUp(TaskInterface* pTask);

// ��ʱ����������״̬
void OnTaskCheckState(TaskInterface* pTask);

// ���Ȧ������
void OnTaskReceiveCircleGroupData(TaskInterface* pTask, const void* pData, int nSize, int nRetCode);

// ��ú�������
void OnTaskReceiveFriendNumData(TaskInterface* pTask, unsigned long ulTaskId, unsigned long ulFriendNum);

// ����NPC֪ͨ
void OnTaskProtectNPCNotify(TaskInterface* pTask, unsigned long ulTaskId, int nReason);

// ��ӻ����Ƴ�ĳ������������б��У�Ŀǰ��ʱ��������ʹ�õĽӿ� Added 2011-03-23.
void OnUpdateFinishTaskList( TaskInterface* pTask, unsigned long ulTaskId, int param1, int param2 );

// �ı���֪ID�������ɴ�����Ŀǰ��������ʹ�õĽӿ� Added 2011-03-23.
void OnUpdateFinishTaskCount( TaskInterface* pTask, unsigned long ulTaskId, int nFinishTaskCount );

// ͨ���������������Ŀǰ��ʱ�ṩ�������жϺ��������жϵ����񷢷Ź��� Added 2011-04-21.
void OnDeliverTaskForDebug( TaskInterface* pTask, unsigned long ulTaskId, int param );

// �ı���֪ID�����״̬��Ŀǰ��������ʹ�� Added 2013-02-21.
void OnUpdateClearTaskState( TaskInterface* pTask, unsigned long ulTaskId );

// ���ʧȥ�峤ְλʱ֪ͨ(��λ���˼���)
void  OnTaskPlayerLeaveFamilyHeader( TaskInterface* pTask );

// ���ʧȥ����ְλʱ֪ͨ(��λ���˰���)
void  OnTaskPlayerLeaveFactionHeader( TaskInterface* pTask );

#endif

#endif
