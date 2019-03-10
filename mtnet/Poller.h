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

	//ʹ��poll����IO����
	//Poller��ӵ��channel����

	class Poller : private Uncopyable
	{
	public:
		typedef std::vector<Channel*> ChannelList;

		Poller(EventLoop* loop);
		~Poller();
		//Poll I/O events
		//ֻ����loop�̵߳���
		Timestamp poll(int timeoutMs, ChannelList* activeChannels);

		//�ı���ע��I/O�¼� ���ӻ����޸�
		//ֻ����loop�̵߳���
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
		ChannelMap channels_; //fd �� channel��ӳ��
	};

}

#endif // MYNET_MTNET_POLLER_H
