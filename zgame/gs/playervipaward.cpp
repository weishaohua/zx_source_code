#include "playervipaward.h"
#include "item_manager.h"
#include "dbgprt.h" 
#include "timer.h"
#include "gmatrix.h"
#include <spinlock.h>

#define MIN_VIP_LEVEL			((int)1)
#define MAX_VIP_LEVEL			((int)6)

#define MIN_ORDINARY_LEVEL		((int)1)
#define MAX_ORDINARY_LEVEL		((int)8)

//ÿ����������ܴ�16������(award_id),��ͨ������⽱����ֻ����һ�εģ�Ϊ�˱���
//���̵ı�����������������Ƴ����16����FIFO�Ĺ���
//����vip����ͨ��Ҷ�ֻ����16��������2011.12.19
#define MAX_COUNT_PER_LEVEL		((int)16)

//��player����������´μ��ʱ�䣬��λΪ��
#define MAX_CHECKTIME_INTERVAL	((int)600)

//ÿ��0�㵽6�㲻����ȡvip����
#define VIP_DAILY_DELAY_HOUR	(6)
#define VIP_DAILY_DELAY_SECONDS (VIP_DAILY_DELAY_HOUR * 3600)

//һ���µ�����
#define DAY_30_SECONDS   (30 * 24 * 60 * 60) 
//���������
#define YEAR_2_SECONDS   (2 * 365 * 24 * 60 * 60) 

//��ʷ�汾��Ϣ
#define DATA_VERSION_0			((int)0)
#define DATA_VERSION_1			((int)1)
//add by sunjunbo����������Ҫ�洢���⽱������ȡʱ��
#define DATA_VERSION_2			((int)2)

//��ǰ���̵����ݰ汾��Ҳ�����µİ汾
#define CUR_VIPAWARD_DATA_VERSION (DATA_VERSION_2)


extern abase::timer g_timer;


namespace vipgame{

/**
 * @brief ��ͨ��ҽ������ε���ҵȼ��Ķ�Ӧ��ϵ
 */
typedef struct Ordinarylevel_To_Playerlevel
{
	int 	rebornCnt;	//ת������
	int		minlevel;	//�õ��ε������ҵȼ�
	int		maxleval;	//�õ��ε������ҵȼ�
}Ordinarylevel_To_Playerlevel;

//�ȼ��������ص�,
//�����Ҫ�޸����棬��ͨ��ҵĻ񽱼���ע���޸�˳��֮��ԭ�����̵Ľ���������ܺ����ڵĲ�һ��
const Ordinarylevel_To_Playerlevel convert_table[MAX_ORDINARY_LEVEL+1] =
{
	{-1,	 -1,     -1 },
	{ 0,	  1,	 89 },
	{ 0,	 90,	119 },
	{ 0,	120,	134 },
	{ 0,	135,	150 },
	{ 1,	  1,	 89 },
	{ 1,	 90,	119 },
	{ 1,	120,	134 },
	{ 1,	135,	160 },
};

award_data::award_data()
{
	_lock = 0;
}

award_data::~award_data()
{
	_vip_specialmap.clear();
	_vip_dailymap.clear();
	_ordinary_specialmap.clear();
	_ordinary_dailymap.clear();
	_totalmap.clear();
}

bool award_data::AddAward(const node_t & node)
{
	spin_autolock keeper(_lock);

	if(!CheckParam(node)) 
	{
		__PRINTF("\n\n\nERROR:  VIP�����������ݲ�������, ����id=%d\n", node.award_id);
		return false;
	}

	TOTALMAP::iterator it_total = _totalmap.find(node.award_id);
	if(_totalmap.end() != it_total)
	{
		__PRINTF("\n\n\nERROR:  VIP�����������ݣ�����id���ظ��� id=%d\n", node.award_id);
		return false;
	}

	node_t data;
	NodeCopy(data, node);

	_totalmap[node.award_id] = data;

	//���յ��Ρ����ͷ��ౣ��
	switch(data.award_type)
	{
		case ORDINARY_AWARD_TYPE:
		{
			if(DAILY_AWARD == data.obtain_type)
			{
				AddAwardByLevel(data, _ordinary_dailymap); 
			}
			else
			{
				AddAwardByLevel(data, _ordinary_specialmap);
			}
		}
		break;

		case VIP_AWARD_TYPE:
		{
			if(DAILY_AWARD == data.obtain_type)
			{
				AddAwardByLevel(data, _vip_dailymap);
			}
			else
			{
				AddAwardByLevel(data, _vip_specialmap);
			}
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  vip�������ݶ�������data.award_type=%d\n\n\n", data.award_type);
			return false;
		}
		break;
	}
	
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(node.award_item_id);
	if(NULL != pItem && pItem->pile_limit == 1 && node.award_item_count != 1)
	{
		__PRINTF("\n\nERROR:  �ѵ�����Ϊ1�Ľ�����Ʒ������������Ϊ1��������д����award_id=%d, item_id=%d, count=%d\n\n\n", node.award_id, node.award_item_id, node.award_item_count);
		return false;
	}

	return true;
}

bool award_data::CheckLevelAwardCnt(void)
{
	//��������Ƿ񳬹��涨ֵ
	if( !CheckLevelAwardCnt(MIN_VIP_LEVEL, MAX_VIP_LEVEL, _vip_dailymap)
	 || !CheckLevelAwardCnt(MIN_VIP_LEVEL, MAX_VIP_LEVEL, _vip_specialmap)
	 || !CheckLevelAwardCnt(MIN_ORDINARY_LEVEL, MAX_VIP_LEVEL, _ordinary_dailymap)
	 || !CheckLevelAwardCnt(MIN_ORDINARY_LEVEL, MAX_VIP_LEVEL, _ordinary_specialmap) )
	{
		__PRINTF("\n\nERROR:  VIP����ÿһ�ȼ��Ľ�Ʒ�������ܳ���%d\n\n", MAX_COUNT_PER_LEVEL);
		return false;
	}

	return true;
}

void award_data::AddAwardByLevel(const node_t & node, LEVELMAP & levelmap)
{
	LEVELMAP::iterator it_level = levelmap.find(node.obtain_level);
	if(levelmap.end() == it_level)
	{
		std::vector<int> tmpvector;
		tmpvector.push_back(node.award_id);
		levelmap[node.obtain_level] = tmpvector;
	}
	else
	{
		it_level->second.push_back(node.award_id);
	}
}

bool award_data::QueryAward(int award_id, node_t & node)
{
	spin_autolock keeper(_lock);

	TOTALMAP::iterator it_total = _totalmap.find(award_id);
	if(_totalmap.end() == it_total)
	{
		return false;
	}

	NodeCopy(node, it_total->second);

	return true;
}

bool award_data::QueryAwardByTypeLevel(int obtainlevel,OBTAIN_TYPE obtaintype, AWARD_TYPE awardtype, std::vector<node_t> &node_list)
{
	spin_autolock keeper(_lock);

	bool ret = false;
	switch(awardtype)
	{
		case ORDINARY_AWARD_TYPE:
		{
			if(DAILY_AWARD == obtaintype)
			{
				ret = QeryByLevel(obtainlevel, _ordinary_dailymap, node_list); 
			}
			else if(SPECIAL_AWARD == obtaintype)
			{
				ret = QeryByLevel(obtainlevel, _ordinary_specialmap, node_list); 
			}
			else
			{
				return false;
			}
		}
		break;

		case VIP_AWARD_TYPE:
		{
			if(DAILY_AWARD == obtaintype)
			{
				ret = QeryByLevel(obtainlevel, _vip_dailymap, node_list); 
			}
			else if(SPECIAL_AWARD == obtaintype)
			{
				ret = QeryByLevel(obtainlevel, _vip_specialmap, node_list); 
			}
			else
			{
				return false;
			}
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  vip�������ݲ�ѯ����award_type=%d\n\n\n", awardtype);
			return false;
		}
		break;
	}

	return ret;
}

bool award_data::QeryByLevel(int obtainlevel, LEVELMAP & levelmap, std::vector<node_t> & node_list)
{
	LEVELMAP::iterator it_level = levelmap.find(obtainlevel);
	if(levelmap.end() == it_level)
	{
		return false;
	}

	TOTALMAP::iterator it_total;
	for(size_t i = 0; i < it_level->second.size(); i++)
	{
		it_total = _totalmap.find(it_level->second[i]);
		if(_totalmap.end() == it_total) 
		{
			return false;
		}
		
		node_list.push_back(it_total->second);
	}

	return true;
}

bool award_data::CheckLevelAwardCnt(int minlevel, int maxlevel, LEVELMAP & levelmap)
{
	if(minlevel > maxlevel)
	{
		return false;
	}

	for(int i = minlevel; i <= maxlevel; i++)
	{
		LEVELMAP::iterator it_level = levelmap.find(i);
		if(levelmap.end() == it_level) continue;

		if(it_level->second.size() > MAX_COUNT_PER_LEVEL)
		{
			return false;
		}
	}

	return true;
}

bool award_data::CheckParam(const node_t & node) const
{
	if(node.award_id <= 0 || node.award_item_id <= 0 || node.award_item_count <= 0)
	{
		return false;
	}

	if((ORDINARY_AWARD_TYPE != node.award_type) && (VIP_AWARD_TYPE != node.award_type))
	{
		return false;
	}

	if((DAILY_AWARD != node.obtain_type) && (SPECIAL_AWARD != node.obtain_type))
	{
		return false;
	}

	if(ORDINARY_AWARD_TYPE == node.award_type)
	{
		if(node.obtain_level < MIN_ORDINARY_LEVEL || node.obtain_level > MAX_ORDINARY_LEVEL)
		{
			return false;
		}
	}
	else
	{
		if(node.obtain_level < MIN_VIP_LEVEL || node.obtain_level > MAX_VIP_LEVEL)
		{
			return false;
		}
	}

	return true;
}

//-------------------------------------------------------------------------------
player_vip_award::award_info::award_info()
{
	tomorrow = 0;
}

bool player_vip_award::award_info::IsAwardExist(OBTAIN_TYPE level, int award_id)
{
	AWARD_DEQUE::iterator it;
	it = find(award_has_got[level].begin(), award_has_got[level].end(), award_id);
	if(award_has_got[level].end() == it)
	{
		return false;
	}

	return true;
}

void player_vip_award::award_info::ClearDeque(OBTAIN_TYPE level) 
{ 
	award_has_got[level].clear(); 
}

void player_vip_award::award_info::PopFrontDeque(OBTAIN_TYPE level)
{
	award_has_got[level].pop_front();
}

void player_vip_award::award_info::PushBackDeque(OBTAIN_TYPE level, int award_id)
{
	while(award_has_got[level].size() >= MAX_COUNT_PER_LEVEL)
	{
		PopFrontDeque(level);
	}

	award_has_got[level].push_back(award_id);
}

void player_vip_award::award_info::Swap(award_info & rhs) 
{ 
	tomorrow = rhs.tomorrow;

	award_has_got.clear();
	award_has_got.swap(rhs.award_has_got); 
}

player_vip_award::vip_info::vip_info()
{
	//vip���⽱������Խ����ȡ��������Ҫ�༶deque
	for(int i = 0; i <= MAX_VIP_LEVEL; i++)
	{
		AWARD_DEQUE deque; 
        award_has_got[i] = deque;
	}

	time_diff		= 0;
	start_time		= 0;
	end_time		= 0;
	level			= 0;
}

player_vip_award::vip_info::~vip_info()
{
	award_has_got.clear();
}

void player_vip_award::vip_info::PushBackDeque(OBTAIN_TYPE level, int award_id)
{
	if(SPECIAL_AWARD == level)
	{
		while( award_has_got[level].size() >= (MAX_COUNT_PER_LEVEL * MAX_VIP_LEVEL) )
		{
			PopFrontDeque(level);
		}
	}
	else
	{
		while(award_has_got[level].size() >= MAX_COUNT_PER_LEVEL)
		{
			PopFrontDeque(level);
		}
	}

	award_has_got[level].push_back(award_id);
}

void player_vip_award::vip_info::Swap(vip_info & rhs)
{
	time_diff		= rhs.time_diff;
	start_time		= rhs.start_time;
	end_time		= rhs.end_time;
	level			= rhs.level;

	award_info::Swap(rhs);
}

bool player_vip_award::vip_info::IsOverTomorrowInValidTime(int t, int tz_adjust)
{
	if(!CheckValidTime(t, tz_adjust))
	{
		return false;
	}

	if(t >= (GetTomorrow(tz_adjust) + VIP_DAILY_DELAY_SECONDS))
	{
		return true;
	}

	return false;
}

//ÿ��0�㵽6�㲻����ȡvip����
bool player_vip_award::vip_info::CheckValidTime(int t, int tz_adjust)
{
	time_t t1 = t;
	struct tm tm1;
	localtime_r(&t1, &tm1);
	if(tm1.tm_hour >= 0 && tm1.tm_hour < VIP_DAILY_DELAY_HOUR)
	{
		return false;
	}

	return true;
}

player_vip_award::ordinary_info::ordinary_info()
{
	//��ͨ���ֻ��Ҫ����deque
	for(int i = 0; i <= SPECIAL_AWARD; i++)
	{
		AWARD_DEQUE deque;
		award_has_got[i] = deque;
	}
}

player_vip_award::ordinary_info::~ordinary_info()
{
	award_has_got.clear();
}

void player_vip_award::ordinary_info::Swap(ordinary_info & rhs)
{
	award_info::Swap(rhs);
}

//---------------------------------------------------------------------------------
player_vip_award::player_vip_award()
{
	time_t t1			= time(NULL);
	struct tm tm1;
	localtime_r(&t1, &tm1);
	_tz_adjust			= -tm1.tm_gmtoff;
	_recent_total_cash	= 0;
	_is_hide			= false;
}

player_vip_award::~player_vip_award()
{
}

bool player_vip_award::FinishAwardObtention(int curtime, const node_t & node)
{
	bool ret = false;

	switch(node.award_type)
	{
		case ORDINARY_AWARD_TYPE:
		{
			ret = FinishGetOrdinaryAward(curtime, node, node.obtain_type);
		}
		break;

		case VIP_AWARD_TYPE:
		{
			ret = FinishGetVipAward(curtime, node, node.obtain_type);
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  VIP��������һ�ȡ�������ʹ��󣨲���vipҲ������ͨ��FinishAwardObtention()\n\n");
			//����������֤��Ҳ�������ȡ
			FinishGetOrdinaryAward(curtime, node, DAILY_AWARD);
			FinishGetOrdinaryAward(curtime, node, SPECIAL_AWARD);
			FinishGetVipAward(curtime, node, DAILY_AWARD);
			FinishGetVipAward(curtime, node, SPECIAL_AWARD);
		}
		return false;
	}

	return ret;
}

bool player_vip_award::TryGetAward(int curtime, const node_t & node, int playerlevel, int rebornCnt)
{
	bool ret = false;
	switch(node.award_type)
	{
		case ORDINARY_AWARD_TYPE:
		{
			ret = TryGetOrdinaryAward(curtime, node, node.obtain_type, playerlevel, rebornCnt);
		}
		break;

		case VIP_AWARD_TYPE:
		{
			ret = TryGetVipAward(curtime, node, node.obtain_type, node.obtain_level);
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  VIP��������һ�ȡ�������ʹ��󣨲���vipҲ������ͨ��\n\n");
		}
		return false;
	}

	return ret;
}

bool player_vip_award::IsLifeLongVip(int curtime)
{
	return _vip_info.level == MAX_VIP_LEVEL && _vip_info.GetEndTimeUTC(_tz_adjust) - curtime > YEAR_2_SECONDS;	
}
bool player_vip_award::IsLifeLongVipSpecialAwardAvailable(int curtime, int award_id)
{
	ASSERT (IsLifeLongVip(curtime));

	// ��ʱ�������ʱ�������30��
	std::map<int , int>::iterator timestamp_it = _special_award_time_stamps.find(award_id);
	return (timestamp_it == _special_award_time_stamps.end()) || (curtime - timestamp_it->second >= DAY_30_SECONDS);
}

bool player_vip_award::TryGetVipAward(int curtime, const node_t & node, int obtain_type, int level)
{
	//����Ƿ���vip
	if(!IsVipPlayer(curtime))
	{
		return false;
	}

	//vip������0�㵽6��֮�䲻����ȡ
	if(!_vip_info.CheckValidTime(curtime, _tz_adjust))
	{
		return false;
	}

	switch(obtain_type)
	{
		case DAILY_AWARD:
		{
			//vipÿ�ս�������Խ����
			if(level != _vip_info.level)
			{
				return false;
			}

			//����������죬�����������ȡÿ�ս���,��������tomorrow��ʱ��
			if( _vip_info.IsOverTomorrowInValidTime(curtime, _tz_adjust) )
			{
				return true;
			}
			else
			{
				if(_vip_info.IsAwardExist(DAILY_AWARD, node.award_id)) 
				{
					//__PRINTF("\n\n\n�����Ѿ���ȡ\n\n\n");
					return false;
				}
			}
		}
		break;

		case SPECIAL_AWARD:
		{
			// ���ȼ���vip����Խ���콱
			if (level > _vip_info.level) {
				return false;	
			}

			if (IsLifeLongVip(curtime)) 
			{ // A. ʣ��ʱ��������������vip�������飬ֻ����ȡʱ���
				if (!IsLifeLongVipSpecialAwardAvailable(curtime, node.award_id)) {
					return false;	
				}
			} else 
			{ // B. ���ֻ࣬���Ƿ���ȡ��
				if (_vip_info.IsAwardExist(SPECIAL_AWARD, node.award_id))
				{
					return false;	
				}	
			}
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  VIP������VIP��һ�ȡ���ʹ��󣨲������⽱��Ҳ����ÿ�ս�����\n\n");
		}
		return false;
	}

	return true;
}


bool player_vip_award::FinishGetVipAward(int curtime, const node_t & node, int obtain_type)
{
	switch(obtain_type)
	{
		case DAILY_AWARD:
		{
			//����������죬�����������ȡÿ�ս���,��������tomorrow��ʱ��
			if( _vip_info.IsOverTomorrowInValidTime(curtime, _tz_adjust) )
			{
				_vip_info.ClearDeque(DAILY_AWARD);
				_vip_info.PushBackDeque(DAILY_AWARD, node.award_id);

				int tmptomorrow = CalcTomorrow(curtime);
				_vip_info.SetTomorrow(tmptomorrow, _tz_adjust);
			}
			else
			{
				_vip_info.PushBackDeque(DAILY_AWARD, node.award_id);
			}
		}
		break;

		case SPECIAL_AWARD:
		{
			if (_vip_info.level == MAX_VIP_LEVEL && _vip_info.GetEndTimeUTC(_tz_adjust) - curtime > YEAR_2_SECONDS) 
			{ // ʣ��ʱ��������������vip�����⴦��ʱ���
				std::map<int , int>::iterator timestamp_it = _special_award_time_stamps.find(node.award_id);
				if (timestamp_it == _special_award_time_stamps.end()) 
				{ // 1. ֮ǰ��ʱ���������ʱ���
					_special_award_time_stamps.insert(std::make_pair(node.award_id, curtime));
				} else 
				{ // 2. ֮ǰ��ʱ���������ʱ���
					int new_time_stamp = timestamp_it->second + DAY_30_SECONDS;
					_special_award_time_stamps.erase(node.award_id);
					_special_award_time_stamps.insert(std::make_pair(node.award_id, new_time_stamp));
				}
			
			}
			_vip_info.PushBackDeque(SPECIAL_AWARD, node.award_id);
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  VIP������VIP��һ�ȡ���ʹ��󣨲������⽱��Ҳ����ÿ�ս�����FinishGetVipAward()\n\n");
			//����������֤��Ҳ�������ȡ
			_vip_info.PushBackDeque(DAILY_AWARD, node.award_id);
			_vip_info.PushBackDeque(SPECIAL_AWARD, node.award_id);
		}
		return false;
	}

	return true;
}

bool player_vip_award::TryGetOrdinaryAward(int curtime, const node_t & node, int obtain_type, int playerlevel, int rebornCnt)
{
	//��ͨ��������Խ���콱����4�����������4���Ľ�����
	int calclevel = CalcOrdinaryLevel(playerlevel, rebornCnt);
	if(node.obtain_level != calclevel || calclevel < MIN_ORDINARY_LEVEL || calclevel > MAX_ORDINARY_LEVEL) 
	{
		return false;
	}

	switch(obtain_type)
	{
		case DAILY_AWARD:
		{
			//����������죬�����������ȡÿ�ս���,��������tomorrow��ʱ��
			if( _ordinary_info.IsOutOfToday(curtime, _tz_adjust) )
			{
				return true;
			}
			else
			{
				if(_ordinary_info.IsAwardExist(DAILY_AWARD, node.award_id)) 
				{
					//__PRINTF("\n\n\n�����Ѿ���ȡ\n\n\n");
					return false;
				}
			}
		}
		break;

		case SPECIAL_AWARD:
		{
			if(_ordinary_info.IsAwardExist(SPECIAL_AWARD, node.award_id))
			{
				return false;
			}
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  VIP��������ͨ��һ�ȡ���ʹ��󣨲������⽱��Ҳ����ÿ�ս�����\n\n");
		}
		return false;
	}

	return true;
}

bool player_vip_award::FinishGetOrdinaryAward(int curtime, const node_t & node, int obtain_type)
{
	switch(obtain_type)
	{
		case DAILY_AWARD:
		{
			//����������죬�����������ȡÿ�ս���,��������tomorrow��ʱ��
			if( _ordinary_info.IsOutOfToday(curtime, _tz_adjust) )
			{
				_ordinary_info.ClearDeque(DAILY_AWARD);
				_ordinary_info.PushBackDeque(DAILY_AWARD, node.award_id);

				int tmptomorrow = CalcTomorrow(curtime);
				_ordinary_info.SetTomorrow(tmptomorrow, _tz_adjust);
			}
			else
			{
				_ordinary_info.PushBackDeque(DAILY_AWARD, node.award_id);
			}
		}
		break;

		case SPECIAL_AWARD:
		{
			_ordinary_info.PushBackDeque(SPECIAL_AWARD, node.award_id);
		}
		break;

		default:
		{
			__PRINTF("\n\nERROR:  VIP��������ͨ��һ�ȡ���ʹ��󣨲������⽱��Ҳ����ÿ�ս�����FinishGetOrdinaryAward()\n\n");
			//����������֤��Ҳ�������ȡ
			_ordinary_info.PushBackDeque(DAILY_AWARD, node.award_id);
			_ordinary_info.PushBackDeque(SPECIAL_AWARD, node.award_id);
		}
		return false;
	}

	return true;
}

int	player_vip_award::CalcOrdinaryLevel(int playerlevel, int rebornCnt)
{
	int ret_level = MAX_ORDINARY_LEVEL + 1;
	for(int i = MIN_ORDINARY_LEVEL; i <= MAX_ORDINARY_LEVEL; i++)
	{
		if(convert_table[i].rebornCnt == rebornCnt)
		{
			if(playerlevel >= convert_table[i].minlevel && playerlevel <= convert_table[i].maxleval)
			{
				ret_level = i;
				break;
			}
		}
	}

	return ret_level;
}

int player_vip_award::CalcTomorrow(time_t t)
{
	time_t tmptomorrow;
	struct tm tm1;
	localtime_r(&t, &tm1);
	tm1.tm_mday += 1;
	tm1.tm_hour  = 0;
	tm1.tm_min   = 0;
	tm1.tm_sec   = 0;
	tmptomorrow  = mktime(&tm1);

	return (int)tmptomorrow;
}

int	player_vip_award::GetVipAvailableAward(int curtime, std::vector<S2C::CMD::vip_award_info::award_data> & award_list)
{
	//�ж��ǲ���vip
	if(!IsVipPlayer(curtime)) 
	{
		return 0;
	}

	//vip������0�㵽6��֮�䲻����ȡ
	if(!_vip_info.CheckValidTime(curtime, _tz_adjust))
	{
		return 0;
	}

	std::vector<node_t> vip_node_list;
	vipgame::award_data & vip_award = item_manager::GetVipAwardData();

	//�ռ�
	vip_award.QueryAwardByTypeLevel(_vip_info.level, DAILY_AWARD, VIP_AWARD_TYPE, vip_node_list);
	for(int tmplevel = MIN_VIP_LEVEL; tmplevel <= _vip_info.level; tmplevel++)
	{
		vip_award.QueryAwardByTypeLevel(tmplevel, SPECIAL_AWARD, VIP_AWARD_TYPE, vip_node_list);
	}

	for(size_t i = 0; i < vip_node_list.size(); i++)
	{
		if( (DAILY_AWARD == vip_node_list[i].obtain_type) )
		{
			S2C::CMD::vip_award_info::award_data tempdata;
			tempdata.award_id		= vip_node_list[i].award_id;

			//�ж��Ƿ��Ѿ���ȡ���������ͻ���������ʾ
			if(_vip_info.IsOverTomorrowInValidTime(curtime, _tz_adjust))
			{
				tempdata.is_obtained		= S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
			}
			else
			{
				if(_vip_info.IsAwardExist(DAILY_AWARD, vip_node_list[i].award_id))
				{
					tempdata.is_obtained	= S2C::CMD::vip_award_info::IS_OBTAINED;
				}
				else
				{
					tempdata.is_obtained    = S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
				}
			}

			award_list.push_back(tempdata);
		}
		
		if( (SPECIAL_AWARD == vip_node_list[i].obtain_type) )
		{
			S2C::CMD::vip_award_info::award_data tempdata;
			tempdata.award_id		= vip_node_list[i].award_id;

			//��ʶ�Ƿ��Ѿ���ȡ������Ʒ�����ڿͻ�����ʾ
			if (IsLifeLongVip(curtime) && IsLifeLongVipSpecialAwardAvailable(curtime, vip_node_list[i].award_id)) 
			{ 
				tempdata.is_obtained = S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
			}
			else if( _vip_info.IsAwardExist(SPECIAL_AWARD, vip_node_list[i].award_id) )
			{
				tempdata.is_obtained	= S2C::CMD::vip_award_info::IS_OBTAINED;
			}
			else
			{
				tempdata.is_obtained	= S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
			}

			award_list.push_back(tempdata);
		}
	}

	return (award_list.size());
}


int	player_vip_award::GetOrdinaryAvailableAward(int curtime, std::vector<S2C::CMD::vip_award_info::award_data> & award_list, int playerlevel, int rebornCnt)
{
	int calclevel = CalcOrdinaryLevel(playerlevel, rebornCnt);
	if(calclevel < MIN_ORDINARY_LEVEL || calclevel > MAX_ORDINARY_LEVEL)
	{
		return 0;
	}

	std::vector<node_t> ordinary_node_list;
	vipgame::award_data & ordinary_award = item_manager::GetVipAwardData();
	ordinary_award.QueryAwardByTypeLevel(calclevel, DAILY_AWARD, ORDINARY_AWARD_TYPE, ordinary_node_list); 
	ordinary_award.QueryAwardByTypeLevel(calclevel, SPECIAL_AWARD, ORDINARY_AWARD_TYPE, ordinary_node_list);

	for(size_t i = 0; i < ordinary_node_list.size(); i++)
	{
		if( (DAILY_AWARD == ordinary_node_list[i].obtain_type) )
		{
			S2C::CMD::vip_award_info::award_data tempdata;
			tempdata.award_id = ordinary_node_list[i].award_id;

			//��ʶ�Ƿ��Ѿ���ȡ������Ʒ�����ڿͻ�����ʾ
			if(_ordinary_info.IsOutOfToday(curtime, _tz_adjust))
			{
				tempdata.is_obtained = S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
			}
			else
			{
				if( _ordinary_info.IsAwardExist(DAILY_AWARD, ordinary_node_list[i].award_id) )
				{
					tempdata.is_obtained = S2C::CMD::vip_award_info::IS_OBTAINED;
				}
				else
				{
					tempdata.is_obtained = S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
				}
			}
			
			award_list.push_back(tempdata);
		}

		if( (SPECIAL_AWARD == ordinary_node_list[i].obtain_type) )
		{
			S2C::CMD::vip_award_info::award_data tempdata;
			tempdata.award_id = ordinary_node_list[i].award_id;

			//��ʶ�Ƿ��Ѿ���ȡ������Ʒ�����ڿͻ�����ʾ
			if(_ordinary_info.IsAwardExist(SPECIAL_AWARD, ordinary_node_list[i].award_id) )
			{
				tempdata.is_obtained = S2C::CMD::vip_award_info::IS_OBTAINED;
			}
			else
			{
				tempdata.is_obtained = S2C::CMD::vip_award_info::IS_NOT_OBTAINED;
			}

			award_list.push_back(tempdata);
		}
	}

	return (award_list.size());
}
	

bool player_vip_award::CheckVipLevelChange(int roleid, int curlevel, int start_time, int end_time, int vipserver_curtime)
{
	//����end_time���ܱ�ͳһ����Ϊ�ڶ�����㣬��˲���ʹ��end_time���ж�vip�ȼ��Ƿ�仯
	//start_timeҲ������ȫ�ж�vip�ȼ��Ƿ�仯����Ϊ������1����vip�ȼ����˺ü���,vipϵͳ�����ٿӵ�һ����
	if(start_time == _vip_info.start_time && curlevel == _vip_info.level)
	{
		return false;
	}

	//��log
	GLog::log(GLOG_INFO, "�û�%dVIP�ȼ������仯:old_endtime=%d,old_level=%d,new_endtime=%d,new_level=%d", roleid, _vip_info.end_time, _vip_info.level, end_time, curlevel);

	//������������������ÿ�ս���
	if(curlevel != _vip_info.level)
	{
		_vip_info.ClearDeque(DAILY_AWARD);
	}

	//int curtime = g_timer.get_systime();
	_vip_info.ClearDeque(SPECIAL_AWARD);
	_vip_info.level			= curlevel;
	_vip_info.start_time	= start_time;
	_vip_info.end_time		= end_time;
	_vip_info.time_diff		= 0;     //curtime - vipserver_curtime;//���ڲ�����diff��VIPϵͳ�Ǳ߷�������ʱ��͵���localtime��ʹ��

	return true;
}

void player_vip_award::Swap(player_vip_award & rhs)
{
	_tz_adjust				= rhs._tz_adjust;
	_recent_total_cash		= rhs._recent_total_cash;
	_is_hide				= rhs._is_hide;

	_vip_info.Swap(rhs._vip_info);
	_ordinary_info.Swap(rhs._ordinary_info);
	_special_award_time_stamps = rhs._special_award_time_stamps;
}

/**
 * @brief Save : 
 *
 * �汾��Ϊ0�Ĵ洢˳��level, start_time, end_time, time_diff, viptomorrow, ordinarytomorrow, 
 *                      vipcount, viplist, ordinarycount, ordinarylist
 *
 * @param ar
 */
void player_vip_award::Save(archive & ar)
{
	ar << CUR_VIPAWARD_DATA_VERSION;
	ar << _vip_info.level;
	ar << _vip_info.start_time;
	ar << _vip_info.end_time;
	ar << _vip_info.time_diff;
	ar << (int)(_vip_info.GetTomorrow(_tz_adjust));
	ar << (int)(_ordinary_info.GetTomorrow(_tz_adjust));
	ar << (char)_is_hide;

	//vip��Ϣ
	__PRINTF("\n\n\nSave: vip DAILY_AWARD size=%d\n", _vip_info.award_has_got[DAILY_AWARD].size());
	ar << _vip_info.award_has_got[DAILY_AWARD].size();
	for(size_t i = 0; i < _vip_info.award_has_got[DAILY_AWARD].size(); i++)
	{
		ar << _vip_info.award_has_got[DAILY_AWARD][i];
	}

	__PRINTF("Save: vip SPECIAL_AWARD size=%d\n", _vip_info.award_has_got[SPECIAL_AWARD].size());
	ar << _vip_info.award_has_got[SPECIAL_AWARD].size();
	for(size_t i = 0; i < _vip_info.award_has_got[SPECIAL_AWARD].size(); i++)
	{
		ar << _vip_info.award_has_got[SPECIAL_AWARD][i];
	}

	//ordinary��Ϣ
	__PRINTF("Save: ordinary DAILY_AWARD size=%d\n", _ordinary_info.award_has_got[DAILY_AWARD].size());
	ar << _ordinary_info.award_has_got[DAILY_AWARD].size();
	for(size_t i = 0; i < _ordinary_info.award_has_got[DAILY_AWARD].size(); i++)
	{
		ar << _ordinary_info.award_has_got[DAILY_AWARD][i];
	}

	__PRINTF("Save: ordinary SPECIAL_AWARD size=%d\n\n\n", _ordinary_info.award_has_got[SPECIAL_AWARD].size());
	ar << _ordinary_info.award_has_got[SPECIAL_AWARD].size();
	for(size_t i = 0; i < _ordinary_info.award_has_got[SPECIAL_AWARD].size(); i++)
	{
		ar << _ordinary_info.award_has_got[SPECIAL_AWARD][i];
	}
	__PRINTF("Save: ordinary SPECIAL_AWARD_TIME_STAMPS size=%d\n\n\n", _special_award_time_stamps.size());
	ar << _special_award_time_stamps.size();
	for (std::map<int , int>::iterator it = _special_award_time_stamps.begin(); it != _special_award_time_stamps.end(); ++it) 
	{
		ar << it->first;	
		ar << it->second;
	}
}
	
void player_vip_award::Load(archive & ar, int roleid)
{
	int version;
	int count;
	int tmpid;
	int tmp_timestamp;
	int viptomorrow;
	int ordinarytomorrow;
	ar >> version;

	switch(version)
	{
		case DATA_VERSION_0:
		{
			ar >> _vip_info.level;
			ar >> _vip_info.start_time;
			ar >> _vip_info.end_time;
			ar >> _vip_info.time_diff;
			ar >> viptomorrow;
			ar >> ordinarytomorrow;
			
			_vip_info.SetTomorrow(viptomorrow, _tz_adjust);
			_ordinary_info.SetTomorrow(ordinarytomorrow, _tz_adjust);
			
			//vip��Ϣ
			ar >> count;
			__PRINTF("\n\n\nLoad: vip DAILY_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_vip_info.award_has_got[DAILY_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: vip SPECIAL_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_vip_info.award_has_got[SPECIAL_AWARD].push_back(tmpid);
			}

			//ordinary��Ϣ
			ar >> count;
			__PRINTF("Load: ordinary DAILY_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_ordinary_info.award_has_got[DAILY_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: ordinary SPECIAL_AWARD size=%d\n\n\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_ordinary_info.award_has_got[SPECIAL_AWARD].push_back(tmpid);
			}
		}
		break;

		case DATA_VERSION_1:
		{
			ar >> _vip_info.level;
			ar >> _vip_info.start_time;
			ar >> _vip_info.end_time;
			ar >> _vip_info.time_diff;
			ar >> viptomorrow;
			ar >> ordinarytomorrow;
			
			_vip_info.SetTomorrow(viptomorrow, _tz_adjust);
			_ordinary_info.SetTomorrow(ordinarytomorrow, _tz_adjust);

			char hide = 0;
			ar >> hide;
			_is_hide = (bool)hide;
			
			//vip��Ϣ
			ar >> count;
			__PRINTF("\n\n\nLoad: vip DAILY_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_vip_info.award_has_got[DAILY_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: vip SPECIAL_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_vip_info.award_has_got[SPECIAL_AWARD].push_back(tmpid);
			}

			//ordinary��Ϣ
			ar >> count;
			__PRINTF("Load: ordinary DAILY_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_ordinary_info.award_has_got[DAILY_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: ordinary SPECIAL_AWARD size=%d\n\n\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_ordinary_info.award_has_got[SPECIAL_AWARD].push_back(tmpid);
			}
		}
		break;

		case DATA_VERSION_2:
		{
			ar >> _vip_info.level;
			ar >> _vip_info.start_time;
			ar >> _vip_info.end_time;
			ar >> _vip_info.time_diff;
			ar >> viptomorrow;
			ar >> ordinarytomorrow;
			
			_vip_info.SetTomorrow(viptomorrow, _tz_adjust);
			_ordinary_info.SetTomorrow(ordinarytomorrow, _tz_adjust);

			char hide = 0;
			ar >> hide;
			_is_hide = (bool)hide;
			
			//vip��Ϣ
			ar >> count;
			__PRINTF("\n\n\nLoad: vip DAILY_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_vip_info.award_has_got[DAILY_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: vip SPECIAL_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_vip_info.award_has_got[SPECIAL_AWARD].push_back(tmpid);
			}

			//ordinary��Ϣ
			ar >> count;
			__PRINTF("Load: ordinary DAILY_AWARD size=%d\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_ordinary_info.award_has_got[DAILY_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: ordinary SPECIAL_AWARD size=%d\n\n\n", count);
			for(int i = 0; i < count; i++)
			{
				ar >> tmpid;
				_ordinary_info.award_has_got[SPECIAL_AWARD].push_back(tmpid);
			}

			ar >> count;
			__PRINTF("Load: SPECIAL_AWARD_TIME_STAMP size=%d\n\n\n", count);
			for (int i = 0; i < count; i++) 
			{
				ar >> tmpid;
				ar >> tmp_timestamp;
				_special_award_time_stamps.insert(std::make_pair(tmpid, tmp_timestamp));
			}
		}
		break;

		default:
		{
			__PRINTF("\n\n\nERROR:  ��ȡ���id:%d vip���������ݰ汾����data_version = %d\n\n\n\n", roleid, version);
			GLog::log(GLOG_ERR, "��ȡ���id:%d vip���������ݰ汾����data_version = %d", roleid, version);
			return;
		}
		break;
	}
}

void player_vip_award::GetVipAwardEndtime(int & end_time)
{
	end_time = 0;
	unsigned char templevel = 0;

	if(IsVipAwardPlayer(templevel))
	{
		end_time = _vip_info.GetEndTimeUTC(_tz_adjust) + _vip_info.time_diff;
	}
}

bool player_vip_award::IsVipAwardPlayer(unsigned char & vipaward_level)
{
	vipaward_level	= 0;
	int curtime		= g_timer.get_systime();

	if(IsVipPlayer(curtime))
	{
		vipaward_level = _vip_info.level;
		return true;
	}
	else
	{
		//��1������0����AU�����ٷ���Ϣ������������Ҫά�����levelֵ
		_vip_info.level = 0;
	}

	return false;
}

bool player_vip_award::IsVipPlayer(int curtime)
{
	//����Ƿ���
	if(_vip_info.IsOverdue(curtime) || _vip_info.level < MIN_VIP_LEVEL || _vip_info.level > MAX_VIP_LEVEL)
	{
		return false;
	}

	return true;
}

void player_vip_award::ClearAllAward()
{
	_vip_info.ClearAllAward();
	_ordinary_info.ClearAllAward();
}

void player_vip_award::ClearAllVipInfo()
{
	_vip_info.Clear();
	_ordinary_info.Clear();
}

bool player_vip_award::SetVipLevelHide(char is_hide)
{
	//ֻ��vip��ʯ������Ҳ�������vip�ȼ�(vip 6��)
	if(6 == _vip_info.level)
	{
		_is_hide = (bool)is_hide;
		return true;
	}

	return false;
}

void DebugCmdClearAllVipAward(player_vip_award & rhs, DEBUG_OPCODE opcode) 
{ 
	if(CLEAR_ALL_AWARD == opcode)
	{
		rhs.ClearAllAward(); 
	}
	else if(CLEAR_ALL_VIP_INFO == opcode)
	{
		rhs.ClearAllVipInfo();
	}
}

}


