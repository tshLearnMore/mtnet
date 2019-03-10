#ifndef MYNET_MTNET_POLLER_H
#define MYNET_MTNET_POLLER_H

#include "Timestamp.h"
#include "EventLoop.h"
#include "Uncopyable.h"

#include <map>
#include <vector>

struct pollfd;

namespace mtnet
{
	class Channel;

	//使用poll处理IO复用
	//Poller不拥有channel对象

	class Poller : private Uncopyable
	{
	public:
		typedef std::vector<Channel*> ChannelList;

		Poller(EventLoop* loop);
		~Poller();
		//Poll I/O events
		//只能在loop线程调用
		Timestamp poll(int timeoutMs, ChannelList* activeChannels);

		//改变已注册I/O事件 增加或者修改
		//只能在loop线程调用
		void updateChannel(Channel* channel);

		void removeChannel(Channel* channel);

		void assertInLoopThread()
		{
			ownerLoop_->assertInLoopThread();
		}
	protected:
	private:
		void fillActiveChannels(int numEvents,ChannelList* activeChannels)const;

		typedef std::vector<struct pollfd> PollFdList;
		typedef std::map<int, Channel*> ChannelMap;

		EventLoop* ownerLoop_;
		PollFdList pollfds_;
		ChannelMap channels_; //fd 到 channel的映射
	};

}

#endif // MYNET_MTNET_POLLER_H
