#ifndef MYNET_MTNET_TIMERQUEUE_H
#define MYNET_MTNET_TIMERQUEUE_H

#include "Timestamp.h"
#include "Mutex.h"
#include "Callbacks.h"
#include "channel.h"
#include "Uncopyable.h"

#include <set>
#include <vector>

namespace mtnet
{
	class EventLoop;
	class Timer;
	class TimerId;

	class TimerQueue : Uncopyable
	{
	public:
		TimerQueue(EventLoop* loop);
		~TimerQueue();
		//��ָ��ʱ��ص�����interval > 0ʱ���������ظ�����
		TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
	protected:
	private:
		typedef std::pair<Timestamp, Timer*> Entry;
		typedef std::set<Entry> TimerList;

		void addTimerInloop(Timer* timer);
		//��ʱ����ʱ����
		void handleRead();
		//�������е��ڵĶ�ʱ��
		std::vector<Entry> getExpired(Timestamp now);
		void reset(const std::vector<Entry>& expired, Timestamp now);
		bool insert(Timer* timer);

		EventLoop* loop_;
		const int timerfd_;
		Channel timerfdChannel_;
		//��ʱ���б��յ���ʱ������
		TimerList timers_;
	};
}

#endif //MYNET_MTNET_TIMERQUEUE_H