#ifndef _REFERENCEMANAGER_H_o
#define _REFERENCEMANAGER_H_o

#include <map>
#include <set>
#include <vector>
#include "itimer.h"

namespace GNET
{
class GReferral;
class GReferrer;
class ReferralBrief;
class RefTrans;

class CountedDirty
{
	int dirty;
public:
	CountedDirty():dirty(0) {}
	void SetDirty(bool _dirty)
	{
		if (_dirty) 
			++dirty;
		else if (dirty > 0)
			--dirty;
	}
	bool IsDirty() { return dirty>0; }
};

class Referral					//������Ϣ
{
	int roleid;
	int referrer;				//����roleid
	int bonus_total1; 		//���ڱ�������Ԫ�������ߵ���ʷ�ܹ��׺���ֵ
	int bonus_total2; 		//���ڱ�����������Ԫ���Ա������ߵ���ʷ�ܹ��׺���ֵ
	int bonus_withdraw; 		//���ߴӸ�����Ѿ���ȡ�ĺ���ֵ
	int64_t exp_total1; 			//���ڱ������������ߵ���ʷ�ܹ��׾���
	int64_t exp_total2; 			//���ڱ����������������ߵ���ʷ�ܹ��׾���
	int64_t exp_withdraw; 		//���ߴӸ�����Ѿ���ȡ�ľ���ֵ
	CountedDirty dirty;
	bool logout;
	friend class ReferenceManager;
	friend class WithdrawExpTrans;
	friend class WithdrawBonusTrans;

public:
	Referral():roleid(0),referrer(0),bonus_total1(0),bonus_total2(0),bonus_withdraw(0),
				exp_total1(0),exp_total2(0),exp_withdraw(0),logout(false)
	{
	}
	void OnLoad(const GReferral &referral, int _referrer);
	void SetDirty(bool _dirty) { dirty.SetDirty(_dirty); }
	bool IsDirty() { return dirty.IsDirty(); }
	void SetLogout(bool _logout) { logout = _logout; }
	bool IsLogout() { return logout; }
	void OnUseCash(int bonus_cont);
	void OnLevelUp(int64_t exp_cont);
	void SyncDB();
	void ToGReferral(GReferral &gref);
};

class Referrer					//������Ϣ
{
	typedef std::set<int> ReferralSet;
	ReferralSet referrals;
	int roleid;
	int bonus_withdraw;			//������ʷ�ϴ�����������ȡ���ܵĺ���ֵ
	int64_t exp_withdraw_today; //���ո���Ҵ��������������Ѿ���ȡ���ܾ���ֵ
	int exp_withdraw_time; //���һ�δ�������ȡ�����ʱ��
	bool loaded;				//������Ϣδ����
	CountedDirty dirty;
	friend class ReferenceManager;
	friend class WithdrawExpTrans;
	friend class WithdrawBonusTrans;
	
public:
	Referrer():roleid(0), bonus_withdraw(0), exp_withdraw_today(0), exp_withdraw_time(0), loaded(false)
	{
	}
	void OnLoad(const GReferrer &referrer);
	void AddReferral(int _roleid) { referrals.insert(_roleid); }
	void DelReferral(int _roleid) { referrals.erase(_roleid); }
	bool IsEmpty() { return referrals.empty(); }
	void SetLoaded(bool _loaded) { loaded = _loaded; }
	bool IsLoaded() { return loaded; }
	void SetDirty(bool _dirty) { dirty.SetDirty(_dirty); }
	bool IsDirty() { return dirty.IsDirty(); }
	void SyncDB();
	void OnLogin();
	void ToGReferrer(GReferrer &gref);
private:
	void CheckExpWithdrawToday();
};

class ReferenceManager : public IntervalTimer::Observer
{
	enum{
		CHECKNUM_ONUPDATE = 10,
		WITHDRAW_MAX = 250, //ÿ�������250����������ȡ��������
	};
	typedef std::map<int, Referrer> ReferrerMap;
	ReferrerMap referrers;
	typedef std::map<int, Referral> ReferralMap;
	ReferralMap referrals;
	typedef std::map<int, RefTrans *> WithdrawTransMap;		//������roleid��������ȡ����map
	WithdrawTransMap transmap;
	int referrer_cursor;
	int referral_cursor;
	int trans_cursor;
	time_t double_time_begin;
	time_t double_time_end;
	bool open;
	static ReferenceManager instance;

	ReferenceManager():referrer_cursor(0),referral_cursor(0),trans_cursor(0),double_time_begin(0),double_time_end(0), open(false){ }

public:
	static ReferenceManager *GetInstance() { return &instance; }

	bool Initialize();
	void OnLogin(int roleid);
	void OnLogout(int roleid);
	void OnLoadReferrer(const GReferrer &referrer);
	void OnLoadReferral(const GReferral &referral);
	void OnReferralUseCash(int roleid, int bonus_cont);
	void OnReferralLevelUp(int roleid, int64_t exp, int level, int reborn_cnt);
	int  ListReferrals(int roleid, int start_index, int &total, int &bonus_avail_today, int64_t &exp_avail_today, std::vector<ReferralBrief> &referrals);
	void OnDBUpdateReferrer(int roleid);
	void OnDBUpdateReferral(int roleid);
	void OnDBWithdrawConfirm(int roleid);
	void OnDBWithdrawRollback(int roleid);
	bool Update();
	bool IsInTransaction(int referrer) { return transmap.find(referrer)!=transmap.end(); }
	Referrer *GetReferrer(int roleid)
	{
		ReferrerMap::iterator it = referrers.find(roleid);
		if (it != referrers.end())
			return &it->second;
		else 
			return NULL;
	}
	Referral *GetReferral(int roleid)
	{
		ReferralMap::iterator it = referrals.find(roleid);
		if (it != referrals.end())
			return &it->second;
		else
			return NULL;
	}
	int WithdrawExp(int roleid);
	int WithdrawBonus(int roleid);
	bool IsOpen() { return open; }
};

};
#endif
