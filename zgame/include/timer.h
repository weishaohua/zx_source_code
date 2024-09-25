#ifndef __CMLIB_TIMER_H__
#define __CMLIB_TIMER_H__

#include "spinlock.h"
#include <sys/time.h>

extern int g_use_old_move_policy;

namespace abase
{
typedef void (*timer_callback)(int index,void *object,int remain);

class  timer
{
	void * __imp;
public:
	enum 
	{
		TICKTIME = 50000,
		MINTIME = 10000
	};
	/*
		��ʼ��һ��timer����,һ������������
		1:idx_tab_size ��ʾ������timer������������Ĵ�С, 
			�������ṩ�˿��ٲ�ѯ��������ʹ��ÿ��timer�¼�����ʱ�����ٽ��и��Ӳ�ѯ
			���������Ĵ�С������Ҫ������ٸ�tick��timer�¼���
			���ֵԽ����ô�Ϳ��Զ�Խ���¼���Χ�Ķ�ʱ�����л���
		2:max_timer_count ��ʾ����ܹ�ͬʱ������ٸ�timer	
			ÿ�������timer entry�������44����ʱ���ֽ��ڴ�
	*/
	timer(int idx_tab_size,int max_timer_count);	
	~timer();


	unsigned int 	get_tick();
	long 	get_systime();
	void 	get_systime(timeval & tv);
	int 	get_free_timer_count();
	int 	get_timer_total_alloced();
	int	get_timer_left(int index,int & interval);
	int 	set_timer(int interval,int start_time,int times,timer_callback routine, void * obj);
	/*
	 * 	ɾ��һ��timer��������һ��tickʱɾ��������һ��tick�˶�ʱ�����ܱ�����Ӧ��
	 *	����tick��Ȼ���ܻᱻɨ�赽�����ʱ����
	 *	������timer��callback�����ᱻ����,��˲�������� 
	 *	ע�⣺��Ҫ��callback����OnTimer�е��ô˺���ɾ������,���Ҫɾ������������һ������
	 */
	int 		remove_timer(int index,void *object);
	//�˺���ֻ����callback����OnTimer����ò�����ɾ������
	int		callback_remove_self(int index);		
	int		get_next_interval(int index,void * obj, int * interval, int *rtimes);
	int		change_interval(int index ,void * obj, int interval,bool nolock/* in callback*/);
	int		change_interval_at_once(int index ,void * obj, int interval,bool nolock/* in callback*/);
	void 		timer_thread(int ticktime = TICKTIME,int mintime  = MINTIME);
	void 		stop_thread();	//stop֮��Ҫ�������¿�ʼ,�ȴ�һ��ʱ����������˳�
	void		pause_thread();	
	void		resume_thread();	
	void		reset();	// reset timer
	void		timer_tick();	// timer tick 

};

class timer_task
{
	virtual void OnTimer(int index,int rtimes, bool & is_delete) = 0; //is_delete������������ɾ��
	inline void DoTimer(int index,int rtimes)
	{
		if(g_use_old_move_policy)
		{
			if(rtimes == 0)
			{
				//��ʱ�����Զ�ɾ��
				extern void _t_RecordTimer(int , timer_task *,int,int);
				_t_RecordTimer(_timer_index, this, 2, 0);
				_timer_index = -1;
				_tm = NULL;
			}
		}

		bool is_delete = false;
		OnTimer(index,rtimes, is_delete);
		if(is_delete) return;	//����ɾ��������ִ������Ĵ��룬����������ڴ����

		if(!g_use_old_move_policy)
		{
			//Ϊ����·ʹ���µ��߼�������һ���з����� $$$$$$$$$$$
			if(rtimes == 0)
			{
				//��ʱ�����Զ�ɾ��
				extern void _t_RecordTimer(int , timer_task *,int,int);
				_t_RecordTimer(_timer_index, this, 2, 0);
				_timer_index = -1;
				_tm = NULL;
			}
		}
	}
protected:

protected:

	int _timer_index;
	timer * _tm;
	timer_task():_timer_index(-1),_tm(NULL){}
	virtual  ~timer_task()
	{
		//ASSERT(_timer_index == -1 && _tm == NULL);
	}
	int RemoveSelf()
	{
		int rst = _tm->callback_remove_self(_timer_index);
		_timer_index = -1;
		_tm = NULL;
		return rst;
	}

	int ChangeInterval(int new_interval)
	{
		ASSERT(new_interval > 0);
		if(_timer_index < 0) return -1;
		int rst = _tm->change_interval(_timer_index,this,new_interval,false);
		ASSERT(rst == 0);
		return rst;
	}

	int ChangeIntervalInCallback(int new_interval)
	{
		ASSERT(new_interval > 0);
		int rst = _tm->change_interval(_timer_index,this,new_interval,true);
		ASSERT(rst == 0);
		return rst;
	}
	
	int GetTaskData(int &interval, int & rtimes)
	{
		return _tm->get_next_interval(_timer_index, this,&interval,&rtimes);
	}

public:

	int SetTimer(timer & tm, int interval,int times,int start_time = -1)
	{
		//$$$$$$$$ ����û������ interval ����С�ڵ��� 0
		if(_timer_index != -1 &&  _tm != NULL )
		{
			ASSERT(false && "ͬ��һ�������ظ��ļ��뵽�˶�ʱ���У�����tagû�и�ֵ");
			return -1;
	
		}
		_tm = & tm;
		_timer_index = tm.set_timer(interval,start_time,times,_callback,this);
		extern void _t_RecordTimer(int , timer_task *,int,int);
		_t_RecordTimer(_timer_index, this, 0,_timer_index);
		return _timer_index;
	}

	int RemoveTimer()
	{
		timer * __tm = _tm;
		int __index = _timer_index;
		if(!__tm || __index < 0) return -1;
		int tmp = __tm->remove_timer(__index,this);
		extern void _t_RecordTimer(int , timer_task *,int,int);
		_t_RecordTimer(__index, this, 1, tmp);
		_timer_index = -1;
		_tm = NULL;
		return tmp;
	}
	inline int GetTimerIndex()
	{
		return _timer_index;
	}


	static void _callback(int index,void *object,int rtimes)
	{
		((timer_task*)object)->DoTimer(index,rtimes);
	}
};

}
#endif

