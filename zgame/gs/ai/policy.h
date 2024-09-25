#ifndef _AI_POLICY_H_
#define _AI_POLICY_H_


#include <vector.h>
#include "policytype.h"
#include <stdio.h>
#include <string.h>

#define F_POLICY_VERSION	0
#define F_TRIGGER_VERSION   11
#define F_POLICY_EXP_VERSION 0


//----------------------------------------------------------------------------------------
//CTriggerData
//----------------------------------------------------------------------------------------

class CTriggerData
{

public:	
	CTriggerData()
	{
		rootConditon = 0;
		bActive = false;
		runCondition = run_attack_effect;
	}

	//0-����, 1-��ʦ, 2-ɮ��, 3-����, 4-����, 5-����, 6-��â, 7-����
	enum _e_occupation
	{
		o_wuxia			= 1,
		o_fashi			= 2,
		o_senglv		= 4,
		o_yaojing		= 8,
		o_yaoshou		= 16,
		o_meiling		= 32,
		o_yumang		= 64,
		o_yuling		= 128,
	};

	enum _e_condition
	{
		c_time_come = 0,		//ָ����ʱ����ʱ 	��������ʱ��ID **ֻ�ܵ�����
		c_hp_less,				//Ѫ�����ڰٷֱ�	������Ѫ������
		c_start_attack,			//ս����ʼ			������������   **ֻ�ܵ�����
		c_random,				//���				�������������
		c_kill_player,			//ɱ����ҡ�������	������������   **ֻ�ܵ�����
		c_not,					//һ�������ķ�
		c_or,					//����������
		c_and,					//����������
		c_died,                 //����ʱ                           **ֻ�ܵ�����
		c_path_end_point,       //����·���յ㡣    ��������һ��·��ID
		c_enmity_reach,			//����б������ж�  ������1,������2,�ȼ�
		c_distance_reach,		//��ʾ���ﴥ��ս��λ���뵱ǰλ�õľ��룬�����볬���ò���ʱ���߼�����ʽΪ�� ����������
		
		c_plus,					//��
		c_minus,                //��
		c_multiply,             //��
		c_divide,               //��
		c_great,                //����
		c_less,                 //С��
		c_equ,					//����
		c_var,                  //������           ����������ID
		c_constant,             //������           ������int
		c_rank_level,			//����ȼ����а��X���Ƿ񵽴�Y�� ������int ����
		c_born,                 //����ʱ                           **ֻ�ܵ�����
		c_attack_by_skill,      //�ܵ�ĳID���ܹ���  ����������ID

		c_num,
	};
	

	enum _e_target
	{
		t_hate_first = 0,		//���������һλ
		t_hate_second,			//��������ڶ�λ
		t_hate_others,			//��������ڶ�λ�����Ժ����ж����е����һ��
		t_most_hp,				//���hp
		t_most_mp,				//���mp
		t_least_hp,				//����hp
		t_occupation_list,		//ĳЩְҵ�����	������ְҵ��ϱ�
		t_self,					//�Լ� 
		t_num,
	};

	enum _e_operation
	{
		o_attact = 0,			//���𹥻���	������0 �����ⲫ 1 �������� 2 ħ���� 3 �ⲫ��Զ��
		o_use_skill,			//ʹ�ü��ܡ�	������ָ�����ܺͼ���
		o_talk,					//˵��		    �����������������ݣ�unicode����
		o_reset_hate_list,		//���ó���б�
		o_run_trigger,			//ִ��һ���´�����
		o_stop_trigger,			//ֹͣһ��������	������id
		o_active_trigger,		//����һ��������	������id
		o_create_timer,		    //����һ����ʱ��	������id
		o_kill_timer,			//ֹͣһ����ʱ��	������id
		
		o_flee,                 //����
		o_set_hate_to_first,    //��ѡ��Ŀ�����������б���һλ
		o_set_hate_to_last,     //��ѡ��Ŀ��ĳ�޶ȵ��������
		o_set_hate_fifty_percent,     //��޶��б������ж���ĳ�޶ȼ���50%�����ټ��ٵ�1
		o_skip_operation,       //��������Ĳ�����Ŀ
		o_active_controller,
		o_summon,               //�¼��ٻ�����
		o_trigger_task,         //���������¼��� param �¼�ID
		o_change_path,          //�л�·��       param ·��ID 
		o_dispear,              //����ʧ
		o_sneer_monster,        //��������       param ��Χ
		o_use_range_skill,      //ʹ�ô�Χ���� param ����ID ���ܵȼ� ��Χ��1-512��
		o_reset,				//��"��������"������"��λ"�����ֲ����������ص�����ս����λ�ã������ս��״̬����Ѫ������buff��գ�
		
		o_set_global,           //����ȫ�ֱ���   param ����ID ��ֵ
		o_revise_global,        //����ȫ�ֱ���   param ����ID ��ֵ
		o_assign_global,        //ȫ�ֱ�����ֵ   param ����ֵ����ID ��ֵ����ID
		o_summon_mineral,       //�ٻ�����
		o_drop_item,			//������Ʒ
		o_change_hate,			//�ı���ֵ ��Χ����Ϊ������ Added 2012-08-21.
		o_start_event,			//��ʼһ���¼�	param id 2012-12-26
		o_stop_event,			//ֹͣһ���¼�	param id
		o_num,
	};
	
	enum _run_condition
	{
		run_attack_noeffect = 0,
		run_attack_effect,
	};
	
    
public:
	
	struct _s_condition
	{
		int iType;
		void *pParam;
	};

	struct _s_target
	{
		int iType;
		void *pParam;
	};

	struct _s_operation
	{
		int iType;
		void *pParam;
		_s_target mTarget;
	};

	struct _s_tree_item
	{
		_s_tree_item(){ mConditon.iType = 0; mConditon.pParam = 0; pLeft = 0; pRight = 0; }
		~_s_tree_item()
		{
			if(mConditon.pParam) free(mConditon.pParam);
			if(pLeft) delete pLeft;
			if(pRight) delete pRight;
		}
		_s_condition mConditon;
		_s_tree_item *pLeft;
		_s_tree_item *pRight;
	};
public:
	
	void			AddOperaion(unsigned int iType, void *pParam, _s_target *pTarget);
	void            AddOperaion(_s_operation*pOperation);
	int				GetOperaionNum(){ return listOperation.size(); }
	int				GetOperaion(unsigned int idx, unsigned int &iType, void **ppData, _s_target& target);
	_s_operation *	GetOperaion( unsigned int idx){ return listOperation[idx]; };
	void			SetOperation(unsigned int idx, unsigned int iType, void *pData, _s_target *pTarget);
	void			DelOperation(unsigned int idx);

	_s_tree_item *	GetConditonRoot();
	void            ReleaseConditionTree();
	void            SetConditionRoot( _s_tree_item *pRoot){ rootConditon = pRoot; }
	unsigned int GetID(){ return uID; }
	void         SetID( unsigned int id){ uID = id; }
	char*        GetName(){ return szName; };
	void         SetName(const char *name){ strcpy(szName,name); }

	
	
	bool        Save(FILE *pFile);
	bool        Load(FILE *pFile);

	
	void Release();
	bool IsActive(){ return bActive; };
	void ActiveTrigger(){ bActive = true; }
	void ToggleTrigger(){ bActive = false; }
	bool IsRun(){ return bRun; }
	void SetRunStatus(bool br){ bRun = br; }
	_run_condition GetRunCondition(){ return (_run_condition)runCondition; }
	void SetRunCondition( _run_condition condition ){  runCondition = condition; };
	
	CTriggerData*        CopyObject();
	static _s_tree_item* CopyConditonTree(_s_tree_item* pRoot);
	static _s_operation* CopyOperation(_s_operation *pOperation);

protected:
	bool				SaveConditonTree(FILE *pFile, _s_tree_item *pNode);
	bool				ReadConditonTree(FILE *pFile, _s_tree_item *pNode);
	
private:

	char         szName[128];
	bool         bActive;
	bool         bRun;
	char         runCondition;
	unsigned int uID;
	_s_tree_item* rootConditon;
	abase::vector<_s_operation *>	listOperation;

};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


class CPolicyData
{

public:

	inline unsigned int GetID(){ return uID; }
	inline void SetID( unsigned int id){ uID = id; }
	
	inline int GetTriggerPtrNum(){ return listTriggerPtr.size(); }
	inline CTriggerData *GetTriggerPtr( int idx ){ return listTriggerPtr[idx]; }
	int			GetIndex( unsigned int id);//�������ʧ�ܷ���-1
	inline void SetTriggerPtr( int idx, CTriggerData *pTrigger){ listTriggerPtr[idx] = pTrigger; }
	inline void AddTriggerPtr( CTriggerData *pTrigger){ listTriggerPtr.push_back(pTrigger); }
	void        DelTriggerPtr( int idx);
	bool Save(const char* szPath);
	bool Load(const char* szPath);
	bool Save(FILE *pFile);
	bool Load(FILE *pFile);

	void Release();

private:

	unsigned int uID;
	abase::vector<CTriggerData*> listTriggerPtr;
};


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


class CPolicyDataManager
{

public:
	
	inline int GetPolicyNum(){ return listPolicy.size(); }
	inline CPolicyData *GetPolicy( int idx) 
	{
		if( idx >= 0 && idx < (int)listPolicy.size())
			return listPolicy[idx];
		else return 0;
	}
	
	bool Load(const char* szPath);
	bool Save(const char* szPath);
	void Release();

private:
	
	abase::vector<CPolicyData *> listPolicy;
};

#endif