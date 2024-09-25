#ifndef __ONLINE_QGAME_SHOPPING_MALL_H__
#define __ONLINE_QGAME_SHOPPING_MALL_H__

#include <hashmap.h>
#include <amemory.h>
#include <amemobj.h>

namespace qgame{

class mall
{
public:
	enum
	{
		MAX_ENTRY = 4
	};

	class sale_time
	{
	public:
		enum
		{
			HAS_STARTTIME = 0x00000001,
			HAS_ENDTIME   = 0x00000002,
		};

		enum IDENTIFIECODE
		{
			RET_SUCCESS,
			RET_ERROR,
		};

		enum
		{
			TYPE_NOLIMIT = 0,   //����ʱ������:����
			TYPE_INTERZONE,     //ʱ��Σ���ȡ������ʱUTC��Ҫʱ������������(_param)��1�����п���ʱ�䣬2�����н���ʱ��
			TYPE_WEEK,          //ÿ�ܣ�����(_param) bit:1-7 ����-��������ʼ����ʱ���÷���Ϊ��λ������start_time��end_time
			TYPE_MONTH,         //ÿ�£�����(_param) bit:1-31 1-31�գ���ʼ����ʱ���Է���Ϊ��λ������start_time��end_time
		};

		sale_time():_type(0), _start_time(0), _end_time(0), _param(0) {}
		bool TrySetParam(int t, int start, int end, int p);
		bool IsTimeToSale(time_t t) { return CheckAvailable(t); }
		IDENTIFIECODE GetRemainingTime(time_t now, int & remain_secs);

	private:
		int _type;	
		int _start_time; //ʱ���ʱ��UTC��������ÿ��ÿ��ʱ�����(Сʱ + ���� + ��)������
		int _end_time;   //ͬ_start_time

		int _param;      //ʱ���ʱ��1��ʾ��_start_time��2��ʾ��_end_time
						 //ÿ�ܣ�����(_param) bit:1-7 ����-����
						 //ÿ�£�����(_param) bit:1-31 1-31��

		int  ConvertToSeconds(struct tm & tmtime) const; 
		bool CheckAvailable(time_t t) const;
		bool CheckParam(int type, int start_time, int end_time, int param);
	};

	struct node_t
	{
		int goods_id;
		int goods_count;
		int bonus_ratio;
		int props;
		int discount;
		sale_time _sale_time;   //����ʱ��
		bool sale_time_active;  //�Ƿ���ڷ���������ʱ��

		bool 	has_present;
		bool 	present_bind;
		unsigned int present_id;
		unsigned int present_count;
		unsigned int present_time;

		struct
		{
			int expire_time;
			int cash_need;
		} entry[MAX_ENTRY];
	};

	struct index_node_t             //���ڱ����̳��п��ܷ����仯��Ʒ�����ݽṹ
	{
		int _index;
		node_t _node;
		index_node_t(const node_t& node, int index):_index(index),_node(node){}
	};

private:
	//typedef abase::hash_map<int, node_t>  MAP;
	typedef abase::vector<node_t>  MAP;
	MAP _map;
	int _lock;

	typedef abase::vector<index_node_t, abase::fast_alloc<> > LIMIT_GOODS;
	LIMIT_GOODS _limit_goods;

public:
	enum
	{
		COIN_RESERVED_MONEY 		= -1123,
		COIN_RESERVED_RANK_POINT 	= -1124,
	};

	bool CheckReservedCoin(int coin)
	{
		return coin != COIN_RESERVED_MONEY && coin != COIN_RESERVED_RANK_POINT;
	}
	
public:
	mall()
	{
		_lock = 0;
	}
	
	bool AddGoods(const node_t & node);
	bool QueryGoods(size_t index, node_t & n);
	//size_t QueryGoods(const size_t * index, node_t * n, size_t count);

	LIMIT_GOODS & GetLimitGoods(){return _limit_goods;}
	bool AddLimitGoods(const node_t & node, int index);
};

/*
class mall_order : public abase::ASmallObject
{
	struct entry_t
	{
		int  item_id;
		int  item_count;
		int  cash_need;
		int  expire_time;
		entry_t(int id, int count, int cash, int expire_time)
			:item_id(id),item_count(count),cash_need(cash),expire_time(expire_time)
		{}
	};

	typedef abase::vector<entry_t,abase::fast_alloc<> > LIST;

	LIST _list;
	int _total_point;
	int _order_id;
public:

	mall_order():_total_point(0),_order_id(-1) {}
	mall_order(int order_id):_total_point(0),_order_id(order_id) {}

	void AddGoods(int id, int count,int point, int expire_time)
	{
		_list.push_back(entry_t(id,count,point,expire_time));
		_total_point += point;
	}

	int GetPointRequire()
	{
		return _total_point;
	}

	bool GetGoods(size_t index, int & id, int & count, int & point, int &expire_time)
	{
		if(index >= _list.size()) return false;
		
		id = _list[index].item_id;
		count = _list[index].item_count;
		point = _list[index].cash_need;
		expire_time = _list[index].expire_time;
		return true;
	}
	
};
*/

struct mall_invoice
{
	int order_id;		//���ν��׵���ˮ��
	int item_id;		//�������ƷID
	int item_count;		//ÿ����Ʒ������
	int price;		//����
	int expire_date;	//����ʱ��
	int guid1;		//����guid�����ι����guid
	int guid2;		//����guid�����ι����guid

	int total_unit;		//���ι���ķ���
	int total_cash;		//���ι������Ǯ��
	int total_count;	//���ι����������
	int timestamp;
	mall_invoice(int order_id, int item_id, int item_count, int price, int expire_date, int total_unit,int ts, int guid1, int guid2)
		:order_id(order_id),item_id(item_id), item_count(item_count), 
		 price(price), expire_date(expire_date),guid1(guid1),guid2(guid2),total_unit(total_unit),timestamp(ts)
	{
		total_cash = price * total_unit;
		total_count = item_count * total_unit;

	}
};

}
#endif

