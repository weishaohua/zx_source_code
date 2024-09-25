#ifndef __ONLINEGAME_GS_PLAYERONLINEAWARD_H__
#define __ONLINEGAME_GS_PLAYERONLINEAWARD_H__

#include "actobject.h"
#include <map>


class player_online_award
{
	enum
	{
		TIME_INTERVAL_AWARD = 0,
		PER_DAY_AWARD		= 1,
	};

	struct onlineawardinfo
	{
		short	index;				//��ʶ�Ѿ�����ʱ���ڼ���,��0��ʼ��

		int		time_passed;		//��¼���ֵ���ʱ�ѹ��˶����룬ֻ�ڴ��̵�ʱ��������
		bool	is_receive_awards;	//��ʶ���ֵĽ�Ʒ�Ƿ��Ѿ���ȡ
		bool	is_closed;			//�Ѿ���ȡ���еĽ���,�������ܴ��б���ɾ�������Ϊclosed


		//��ȡ������ʱ��֣������ʱ��
		inline int  GetTomorrow(int tz_adjust) const { return (tomorrow + tz_adjust); } 
		inline int  SetTomorrow(int tomorrow_secs, int tz_adjust) { return (tomorrow = tomorrow_secs - tz_adjust); }

		//��ȡ���ֵ���ʱ�Ŀ�ʼʱ�䣬�����̲���Ҫ����ʱ��
		inline int  GetStartTime() const { return (start_time); }
		inline int  SetStartTime(int starttime) { return (start_time = starttime); }

		onlineawardinfo()
		{
			index				= 0;
			time_passed			= 0;
			is_receive_awards	= false;
			tomorrow			= 0;
			start_time			= 0;
			is_closed			= false;
		}

		~onlineawardinfo()
		{
			index				= 0;
			time_passed			= 0;
			is_receive_awards	= false;
			tomorrow			= 0;
			start_time			= 0;
			is_closed			= false;
		}
	
	private:
		int		tomorrow;			//ÿ�յ���ʱ����ÿ��Ҫ���㽱������ֵ
									//��ֵ���������ʱ��������UTCʱ�䣬ʹ��ʱ�����ʱ��

		int		start_time;			//���ֵ���ʱ�Ŀ�ʼʱ�䣬Ϊ����ʱ�䣬��ֵ��¼ʱ����time_passed���¼���
									//��ֵ����ΪUTCʱ�䣬ʹ��ʱ��Ҫ����ʱ��,��ֵ������20111212
	};

	typedef std::map<int, onlineawardinfo> ONLINEAWARDINFO_MAP;


public:
	struct player_info
	{
		int		rebornCnt;		//ת������
		int		level;			//�ȼ�
		bool	gender;			//�Ա�
		short	cls;			//ְҵ
	};

	enum TRYGET_CODE
	{
		ONLINE_ERR_OVER_DEADLINE		= -9,
		ONLINE_ERR_EXHAUST				= -8,
		ONLINE_ERR_COUNTDOUWN			= -7,
		ONLINE_ERR_INVALID_INDEX		= -6,
		ONLINE_ERR_INVALID_PLAYER		= -5,
		ONLINE_ERR_INVALID_SMALL_GIFT	= -4,
		ONLINE_ERR_NOT_FOUND			= -3,
		ONLINE_ERR_HAS_OBTAINED			= -2,
		ONLINE_ERR_NOT_ACTIVE			= -1, 
		ONLINE_SUCCESS					= 0,
		ONLINE_OBTAINED					= 1,	//��Ʒ���ս�������
	};


public:
	player_online_award();
	~player_online_award();

	//-----------------------����ӿ�----------------------
	/**
	 * @brief CheckTimeToActive: ��ȡ��һ������ʱ��㣬��player��Heartbeat�е��� 
	 * force_flag: ��־�Ƿ���Ҫǿ�ƿͻ��˸���״̬
	 * @return true: ��ʾ��ҵ����߽����б仯����Ҫ��ͻ��˷��͵�ǰ�����߽�����Ϣ
	 */
	bool		CheckTimeToActive(time_t t, const player_info & playerinfo, bool & force_flag);
	inline bool IsTimeToCheck(void) { return (--_check_counter_down <= 0); }

	/**
	 * @brief TryGetOnlineAward FinishOnlineAwardObtain: 
	 *         ��ȡ�����������׶�
	 *         (1) �����ǵ���TryGetOnlineAward��������Ƿ�����
	 *         (2) Ȼ��ȴ��������ɹ���ʱ���ڵ���FinishOnlineAwardObtain
	 *         FinishOnlineAwardObtain������ʲô������ܰ�id��ӵ���������У���ֹ�ڴ���������������ظ���ȡ
	 */
	TRYGET_CODE	TryGetOnlineAward(int id, int index, time_t t, int small_giftbag_id);
	bool		FinishOnlineAwardObtain(int roleid, int id, int index, time_t t, bool & is_closed);

	/**
	 * @brief GetCurOnlineAwardInfo: ��ȡ��ҵ�ǰ�����Ѽ�������߽���
	 */
	int 		GetCurOnlineAwardInfo(std::vector<S2C::CMD::online_award_info::award_data> & info_list);

	//DB��ȡ
	void		Save(archive & ar);
	void		Load(archive & ar, int roleid);

	//player_imp swap
	void		Swap(player_online_award & rhs);

	bool		GetSmallGiftbagID(int awardid, int index, int & id);

	//������
	static void DebugCmdClearOnlineAward(player_online_award & rhs);
	static bool DebugIsActive(int id, player_online_award & rhs);
	static void DebudDumpActiveAward(std::vector<int> & active_list, player_online_award & rhs);
	bool		GetTimePassed(int awardid, time_t t, int & index, int & time_passed);
	bool		GetNextIndex(int id, int index, int & nextindex) const;


private:
	ONLINEAWARDINFO_MAP _info_map;
	int			_tz_adjust;				//ʱ����Ϣ
	int			_check_counter_down;	//��鵹��ʱ

	int			ConvertToSeconds(struct tm & tmtime) const;
	bool		CheckNextTimeToActive(time_t t, const player_template::online_giftbag_config & config, int & next_acttime);
	bool		CheckOverdueOnlineAward(time_t t, const player_template::OnlineGiftbagConfigMap & config_list, const player_info & playerinfo);
	void		InitOnlineAwardInfo(int id, onlineawardinfo & info, time_t t);

	//ȡ��Ч��С�����index
	int			GetSmallGiftBagsBegin(int id, const player_template::OnlineGiftbagConfigMap & config_list);
	bool		GetSmallGiftBagsNext(int id, const player_template::OnlineGiftbagConfigMap & config_list, int index, int & nextindex) const;

	inline bool ValidPlayer(const player_info & playerinfo, const player_template::online_giftbag_config & config);
	inline bool IsActive(int id) { return (_info_map.find(id) != _info_map.end()); }

	//clear
	void		Clear(void);

	//����Ƿ����
	inline bool	IsPerDayAwardPastDue(time_t t, onlineawardinfo & info)
	{
		return (t >= info.GetTomorrow(_tz_adjust));
	}

	//�ȼ�
	inline bool __CheckLevel(int curlevel, int minlevel, int maxlevel)
	{
		if(minlevel > 0 && curlevel < minlevel)
		{
			return false;
		}
		
		if(maxlevel > 0 && curlevel > maxlevel)
		{
			return false;
		}

		return true;
	}

	//ת������
	inline bool __CheckRebornCnt(int cur_rebornCnt, int rebornCnt_required)
	{
		return (cur_rebornCnt >= rebornCnt_required);
	}

	//�Ա�
	inline bool __CheckGender(bool cur_gender, unsigned int gender_required)
	{
		if(2 == gender_required) 
			return true;
		if(cur_gender && gender_required) 
			return true;
		if(!cur_gender && !gender_required) 
			return true;

		return false;
	}

	//ְҵ
	inline bool __CheckCls(short cur_cls, int64_t mask1, int64_t mask2)
	{
		return (object_base_info::CheckCls(cur_cls, mask1, mask2));
	}

	//���壺���塢����
	inline bool __CheckRace(short cur_cls, int required_race)
	{
		if( required_race == 0 && !(cur_cls <= 0x1F || (cur_cls > 0x3F && cur_cls <= 0x5F)) )
			return false;

		if( required_race == 1 && !((cur_cls > 0x1F && cur_cls <= 0x3F) || (cur_cls > 0x5F && cur_cls <= 0x7F)) )
			return false;

		return true;
	}
};


#endif
