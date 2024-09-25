#ifndef __ONLINEGAME_GS_PLAYERVIPAWARD_H__
#define __ONLINEGAME_GS_PLAYERVIPAWARD_H__

#include "common/base_wrapper.h"
#include "common/protocol.h"
#include "vector.h"
#include <vector>
#include <map>
#include <deque>


namespace vipgame{

struct node_t
{
	int		award_id;			//�༭����ÿ��������Ʒ��Ӧ��id����id��Ψһ�ģ�������
	short	award_type;			//��Ʒ���ͣ�0����ͨ��1��vip

	int		award_item_id;		//ÿ�������ľ�����Ʒid�����ڽ�����ʱ������Ʒ
								//�����ظ�(��5�����������10��������ǲ�ͬ�Ľ���)
	int		award_item_count;	//��Ʒ����

	short	obtain_level;		//��ȡ��Ʒ����ĵ��Σ�vip���δ�1��ʼ����ͨ��ҵ��θ��ݵȼ��ּ�����1��ʼ
	short	obtain_type;		//��ȡ��Ʒ�����ͣ�0��ÿ����ȡ��1��������ȡ

	int		expire_time;		//ʱ�ޣ���Ʒ�е�����ʱ��
};

enum AWARD_TYPE
{
	ORDINARY_AWARD_TYPE = 0,
	VIP_AWARD_TYPE      = 1,
};

enum OBTAIN_TYPE
{
	DAILY_AWARD			= 0,	//ÿ�ս�������ֵ���ܸģ���Ϊ�����ʱ��map[0]��Ӧÿ��
	SPECIAL_AWARD		= 1,	//���⽱������ֵ���ܸģ���Ϊ��ͨ��ұ�����map[1]��λ��
								//��ͨ��Ҳ���Խ���콱������ֻ��һ��level����Ʒ����
};

//���ڵ�������
enum DEBUG_OPCODE
{
	CLEAR_ALL_AWARD		= 0,
	CLEAR_ALL_VIP_INFO	= 1,
};


class award_data
{
	typedef std::map<int, std::vector<int> >		LEVELMAP;
	typedef std::map<int, node_t>					TOTALMAP;

	
public:
	award_data();
	~award_data();

	//����ȫ�������������ɺ���Ҫ����CheckLevelAwardCnt����Ƿ񳬹��������
	bool		AddAward(const node_t & node);
	bool		CheckLevelAwardCnt(void);

	bool		QueryAward(int award_id, node_t & n);
	bool		QueryAwardByTypeLevel(int obtainlevel,OBTAIN_TYPE obtaintype, AWARD_TYPE awardtype, std::vector<node_t> &node_list);

private:
	//�����Ρ����ͷ����map
	LEVELMAP	_vip_specialmap;		
	LEVELMAP	_vip_dailymap;
	LEVELMAP	_ordinary_specialmap;
	LEVELMAP	_ordinary_dailymap;
	
	//����award_id����
	TOTALMAP	_totalmap;				

	int			_lock;

	bool		CheckParam(const node_t & node) const;
	void		AddAwardByLevel(const node_t & node, LEVELMAP & levelmap);
	bool		QeryByLevel(int obtainlevel, LEVELMAP & levelmap, std::vector<node_t> & node_list);
	bool		CheckLevelAwardCnt(int minlevel, int maxlevel, LEVELMAP & levelmap);

	inline void NodeCopy(node_t & data, const node_t & node)
	{
		data.award_id			= node.award_id;
		data.award_type			= node.award_type;
		data.award_item_id		= node.award_item_id;
		data.award_item_count	= node.award_item_count;
		data.obtain_level		= node.obtain_level;
		data.obtain_type		= node.obtain_type;
		data.expire_time		= node.expire_time;
	}
};


class player_vip_award
{
	friend void DebugCmdClearAllVipAward(player_vip_award & rhs, DEBUG_OPCODE opcode);

	typedef std::map<int, std::deque<int> >		AWARDGOT_MAP;
	typedef std::deque<int>						AWARD_DEQUE;

	class award_info
	{
	public:
		award_info();
		virtual ~award_info() {}

		/***********************������ҽ�����Ϣʱ*******************
		 * @brief: map[DAILY_AWARD]����ÿ������Ľ�����map[SPECIAL_AWARD]������������⽱��
		 *			ÿ������ֻ��¼�����ȡ��MAX_COUNT_PER_LEVEL������
		 */
		AWARDGOT_MAP	award_has_got;	

		
		//��ȡ������ʱ��֣������ʱ��
		inline  int		GetTomorrow(int tz_adjust) const { return (tomorrow + tz_adjust); }
		inline  int		SetTomorrow(int tomorrow_secs, int tz_adjust) { return (tomorrow = tomorrow_secs - tz_adjust); }
		inline  bool	IsOutOfToday(int t, int tz_adjust) const { return (t >= GetTomorrow(tz_adjust)); }

		virtual bool	IsAwardExist(OBTAIN_TYPE level, int award_id);
		virtual void	ClearDeque(OBTAIN_TYPE level);
		virtual void	PushBackDeque(OBTAIN_TYPE level, int award_id);

		virtual void	Swap(award_info & rhs);
		virtual void	Clear() 
		{
			award_has_got.clear();
			tomorrow = 0;
		}

		virtual void	ClearAllAward()
		{
			award_has_got.clear();
			tomorrow = 0;
		}
	
	protected:
		virtual void	PopFrontDeque(OBTAIN_TYPE level);

	private:
		int				tomorrow;		//��¼�ϴ���ȡÿ�ս����Ĵ����賿������룬����ΪUTCʱ��

	};

	class vip_info : public award_info
	{
	public:
		vip_info();
		~vip_info();

		int				time_diff;		//�յ�Э��ʱget_systime��ʱ���AU������curtime�Ĳ�ֵ
		int				start_time;		//AU�������Ŀ�ʼʱ�䣬ʹ��ʱ��Ҫtime_diff
		int				end_time;		//AU�������Ľ���ʱ�䣬ʹ��ʱ��Ҫtime_diff
		int				level;			//vip����

		virtual void	PushBackDeque(OBTAIN_TYPE level, int award_id);
		virtual void	Swap(vip_info & rhs);

		virtual void	Clear()
		{
			time_diff       = 0;
			start_time      = 0;
			end_time        = 0;
			level           = 0;

			award_info::Clear();
		}
		
		//vipÿ�ս�����6:00 AM�����������ȡ��0�㵽6��֮���ǲ�����ȡ��
		bool IsOverTomorrowInValidTime(int t, int tz_adjust);
		bool CheckValidTime(int t, int tz_adjust);

		//�����ͻ��˵���UTCʱ��
		inline int		GetEndTimeUTC(int tz_adjust)
		{
			return (end_time - tz_adjust);
		}

		inline bool		IsOverdue(int curtime)
		{
			//������ÿ�������м��
			time_t t1 = end_time + time_diff;
			struct tm tm1;
			localtime_r(&t1, &tm1);
			tm1.tm_mday += 1;
			tm1.tm_hour  = 0;
			tm1.tm_min   = 0;
			tm1.tm_sec   = 0;
			int tmptime  = mktime(&tm1); 

			if(curtime < (start_time + time_diff) || curtime >= tmptime)
			{
				return true;
			}

			return false;
		}

	};


	class ordinary_info : public award_info
	{
	public:
		ordinary_info();
		~ordinary_info();

		virtual void    Swap(ordinary_info & rhs);
		virtual void	Clear()
		{
			award_info::Clear();
		}
	};


public:
	player_vip_award();
	~player_vip_award();

	//-------------------����ӿ�------------------------
	/**
	 * @brief TryGetAward FinishAwardObtention:
	 *         ��ȡ�����������׶�:
	 *         (1) �����ǵ���TryGetAward��������Ƿ�����
	 *         (2) Ȼ��ȴ��������ɹ���ʱ���ڵ���FnishAwardObtention
	 *         FinishAwardObtention() ������ʲô������ܰ�id�����ȡ�������ֹ�ڴ���������������ظ���ȡ
	 */
	bool			TryGetAward(int curtime, const node_t & node, int playerlevel, int rebornCnt);
	bool			FinishAwardObtention(int curtime, const node_t & node);

	void			GetVipAwardEndtime(int & end_time);

	// ����Ƿ�Ϊ����VIP������Ϊ��������ʣ��ʱ�����2��
	bool IsLifeLongVip(int curtime);

	// ����VIP����VIP�������͵Ľ�������ʱ�����������award_has_got�������µ�30�����ù���
	// 30������VIP SPECIAL��������:
	//					(a)��ʱ��� --> ���콱��������ȡ������ʱ�������Ϊ��ǰ��
	//                  (b)��ʱ����Ҹ���ǰʱ������30�� --> ���콱��������ȡ������ʱ���+30��
	//                  (c)��ʱ����Ҹ���ǰʱ��С��30�� --> �����콱��
	bool IsLifeLongVipSpecialAwardAvailable(int curtime, int award_id);

	/**
	 * @brief IsVipAwardPlayer ��1������0����AU�����ٷ���Ϣ���������Ըú�������vip���ڵ�ʱ���_vip_info.level��Ϊ0
	 */
	bool			IsVipAwardPlayer(unsigned char & vipaward_level);
	
	/**
	 * @brief CheckVipLevelChange: 
	 *         deliveryת��������vip��Ϣ��Ҫ�жϣ�vip�ȼ��Ƿ��б䣨�������������������ڿ�ʼ����������ȡ����
	 */
	bool			CheckVipLevelChange(int roleid, int curlevel, int start_time, int end_time, int vipserver_curtime);

	int				GetVipAvailableAward(int curtime, std::vector<S2C::CMD::vip_award_info::award_data> & award_list);
	int				GetOrdinaryAvailableAward(int curtime, std::vector<S2C::CMD::vip_award_info::award_data> & award_list, int playerlevel, int rebornCnt);

	//���̶���
	void			Save(archive & ar);
	void			Load(archive & ar, int roleid);

	//player_imp swap
	void			Swap(player_vip_award & rhs);

	//����AU��������recenttotalcashֵ�����ڿͻ�����ʾ
	void			SetRecentTotalCash(int recent_total_cash) { _recent_total_cash = recent_total_cash; }
	int				GetRecentTotalCash() const { return _recent_total_cash; }

	bool			SetVipLevelHide(char is_hide);
	bool			IsHideVipLevel() { return _is_hide; }



protected:
	//���ڵ�������
	void			ClearAllAward();
	void			ClearAllVipInfo();



private:
	//time-zone
	int				_tz_adjust;
	int				_recent_total_cash;
	bool			_is_hide; //����Ƿ�ر�vip�ȼ�����ʾ��Ĭ��Ϊ��,��false


	//info-data
	vip_info		_vip_info;
	ordinary_info	_ordinary_info;
	std::map<int , int> _special_award_time_stamps;


	bool			TryGetVipAward(int curtime, const node_t & node, int obtain_type, int level);
	bool			TryGetOrdinaryAward(int curtime, const node_t & node, int obtain_type, int playerlevel, int rebornCnt);

	bool			FinishGetVipAward(int curtime, const node_t & node, int obtain_type);
	bool			FinishGetOrdinaryAward(int curtime, const node_t & node, int obtain_type);

	int				CalcOrdinaryLevel(int playerlevel, int rebornCnt);
	int				CalcTomorrow(time_t t);

	bool			IsVipPlayer(int curtime);
};

/**
 * @brief DebugCmdClearAllVipAward ���ڴ����������
 *
 * @param rhs
 * @param opcode: 0 ��ʾ����������������ȡ���Ľ�Ʒ�ļ�¼
 *                1 ��ʾ�������vip������Ϣ--����vip�ȼ���ÿ�յ���ʱ�䣬vip����ʱ��ȵ�
 */
void DebugCmdClearAllVipAward(player_vip_award & rhs, DEBUG_OPCODE opcode);

}

#endif
