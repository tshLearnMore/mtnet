#ifndef MTNET_BASE_CHANNEL_H
#define MTNET_BASE_CHANNEL_H

#include "Uncopyable.h"
#include "Timestamp.h"

#include <functional>

namespace mtnet
{
	class EventLoop;
	class Channel :private Uncopyable
	{
	public:
		typedef std::function<void ()> EventCallback;
		typedef std::function<void(Timestamp)> ReadEventCallback;

		Channel(EventLoop* loop, int fd);
		~Channel();
		//����io�¼�
		void handelEvent(Timestamp receiveTime);

		void setReadCallback(const ReadEventCallback& cb)
		{
			readCallback_ = cb;
		}
		void setWriteCallback(const EventCallback& cb)
		{
			writeCallback_ = cb;
		}
		void setErrorCallback(const EventCallback& cb)
		{
			errorCallback_ = cb;
		}

		void setCloseCallback(const EventCallback& cb)
		{
			closeCallback_ = cb;
		}

		int fd() const 
		{
			return fd_;
		}
		int events() const
		{
			return events_;
		}
		void set_revents(int revents)
		{
			revents_ = revents;
		}
		bool isNoneEvent() const
		{
			return events_ == kNoneEvent;
		}

		//����/ȡ�� ���ĵ��¼�����
		void enableReading()
		{
			events_ |= kReadEvent;
			update();
		}
		void enableWriting()
		{
			events_ |= KwriteEvent;
			update();
		}
		void disableWriting()
		{
			events_ &= ~KwriteEvent;
			update();
		}
		void disableAll()
		{
			events_ = kNoneEvent;
			update();
		}

		bool isWriting() const
		{
			return events_ & KwriteEvent;
		}

		//for  poller
		int index()
		{
			return index_;
		}
		void set_index(int index)
		{
			index_ = index;
		}
		EventLoop* ownerLoop()
		{
			return loop_;
		}
	protected:
	private:
		void update();

		//fd���¼�����
		static const int kNoneEvent;
		static const int kReadEvent;
		static const int KwriteEvent;

		EventLoop* loop_;
		const int  fd_;
		int        events_;      //���ĵ��¼�����
		int        revents_;     //��ǰ����¼�����
		int        index_;       //poller ������

		bool       eventHandling_;

		ReadEventCallback readCallback_;
		EventCallback writeCallback_;
		EventCallback errorCallback_;
		EventCallback closeCallback_;
	};
}

#endif //MTNET_BASE_CHANNEL_H