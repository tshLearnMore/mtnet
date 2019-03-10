/*
tcp连接的accept相关操作封装
*/
#ifndef MYNET_MTNET_ACCEPTOR_H
#define MYNET_MTNET_ACCEPTOR_H

#include "Uncopyable.h"
#include "Channel.h"
#include "Socket.h"

#include <functional>

namespace mtnet
{
	class EventLoop;
	class InetAddress;

	class Acceptor : Uncopyable
	{
	public:
		typedef std::function<void (int sockfd,const InetAddress&)> NewConnectionCallback;

		Acceptor(EventLoop* loop, const InetAddress& listenAddr);

		void setNewConnectionCallback(const NewConnectionCallback& cb)
		{
			newConnectionCallback_ = cb;
		}

		bool listenning() const
		{
			return listenning_;
		}

		void listen();
	protected:
	private:
		void handleRead();

		EventLoop* loop_;
		Socket acceptorSocket_;
		Channel acceptChannel_;
		NewConnectionCallback newConnectionCallback_;
		bool listenning_;
	};
}

#endif //MYNET_MTNET_ACCEPTOR_H