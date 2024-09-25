#ifndef __QUESTION_LIB_H__
#define __QUESTION_LIB_H__

#include <vector>

#define QUESTION_SUM_OF_ARRAY 30

struct _QUESTION
{
	unsigned short* szQuestion;//������ַ���ָ��
	char            cAnswer;   //�𰸣�һ���ֽڣ���1λ����6λ�ֱ��ӦA��B��C��D��E��F����ȷ����1�������Ϊ0
	int             nAnswer;   //�𰸵ĸ��������6��
	unsigned short* pAnswer[6];//6���𰸵��ַ���ָ��
};

struct _QUESTION_ARRAY
{
	std::vector<_QUESTION*> listQuestion;
};

class CQuestionLib
{
public:
	CQuestionLib();
	~CQuestionLib();

public:
	bool Load( const char *szPath );
	int  GetQtArrayCount(){ return m_listLib.size();}
	const _QUESTION_ARRAY* GetQtArray( int idx ){ return m_listLib[idx]; }
	
protected:
	void Release();
	
	std::vector<_QUESTION_ARRAY*> m_listLib;
	
};

#endif 
