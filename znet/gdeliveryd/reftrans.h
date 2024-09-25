#ifndef _REF_TRANS_H_
#define _REF_TRANS_H_
#include "dbrefwithdrawtrans.hrp"
#include "gamedbclient.hpp"

namespace GNET
{

class RefTrans			//�����ƹ���������
{
public:
	virtual ~RefTrans() {}
	virtual bool TrySubmit() = 0;
	virtual void Confirm() = 0;
	virtual void Rollback() = 0;
};

//һ����ȡ����
template <typename T>
class WithdrawTrans : public RefTrans
{
protected:
	typedef std::map<int, T> ReferralOldWithdrawMap;//����ȡ�����漰�����������ϵ�bonus_withdraw��exp_withdrawֵ
	ReferralOldWithdrawMap referral_withdraws;
	int referrer;									//���ߵ�roleid
	T total_withdraw;								//�����ϵ�bonus_withdraw����exp_withdraw_today;
	T total2;										//��������߻������ߣ���¼�����ϵ�total2ֵ
	T to_withdraw;									//������ȡ��ֵ
	friend class ReferenceManager;

public:
	WithdrawTrans():referrer(0),total_withdraw(0),total2(0) {}
	bool TrySubmit();
	void Confirm();
protected:
	virtual void OnConfirm() = 0;
};

class WithdrawBonusTrans : public WithdrawTrans<int>
{
public:
	void Rollback();
protected:
	void OnConfirm();
};

class WithdrawExpTrans : public WithdrawTrans<int64_t>
{
public:
	void Rollback();
protected:
	void OnConfirm();
};

template <typename T>
bool WithdrawTrans<T>::TrySubmit()
{
	ReferenceManager *refman = ReferenceManager::GetInstance();
	DBRefWithdrawTransArg arg;

	Referrer *pref = refman->GetReferrer(referrer);
	if (pref == NULL) return false;
	pref->ToGReferrer(arg.referrer);

	int index = 0;
	Referral *pfal = refman->GetReferral(referrer);
	if (pfal != NULL) 
	{
		arg.referrals.resize(referral_withdraws.size()+1);
		pfal->ToGReferral(arg.referrals[index++]);
	}
	else
		arg.referrals.resize(referral_withdraws.size());


	typename ReferralOldWithdrawMap::iterator it, ie = referral_withdraws.end();
	for (it = referral_withdraws.begin(); it != ie; ++it)
	{
		pfal = refman->GetReferral(it->first);
		if (pfal != NULL) pfal->ToGReferral(arg.referrals[index++]);
	}

	DBRefWithdrawTrans *rpc = (DBRefWithdrawTrans *)Rpc::Call(RPC_DBREFWITHDRAWTRANS, arg);
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return true;
}

template <typename T>
void WithdrawTrans<T>::Confirm()
{
	ReferenceManager *refman = ReferenceManager::GetInstance();
	Referrer *pref = refman->GetReferrer(referrer);
	if (pref != NULL) pref->SetDirty(false);

	Referral *pfal = refman->GetReferral(referrer);
	if (pfal != NULL) pfal->SetDirty(false);

	typename ReferralOldWithdrawMap::iterator it, ie = referral_withdraws.end();
	for (it = referral_withdraws.begin(); it != ie; ++it)
	{
		pfal = refman->GetReferral(it->first);
		if (pfal != NULL) pfal->SetDirty(false);
	}

	OnConfirm();
}

};
#endif
