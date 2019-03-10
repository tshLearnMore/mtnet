#ifndef MYNET_MTNET_SOCKET_H
#define MYNET_MTNET_SOCKET_H

#include "Uncopyable.h"

namespace mtnet
{
	class InetAddress;

	class Socket : Uncopyable
	{
	public:
		explicit Socket(int sockfd)
			:sockfd_(sockfd)
		{
		}
		~Socket();

		int fd() const
		{
			return sockfd_;
		}
		//如果addr在使用中就abort
		void bindAddress(const InetAddress& localaddr);
		//如果addr在使用中就abort
		void listen();
		//成功：返回一个非阻塞的文件描述符
		//失败：返回 -1
		int accept(InetAddress* peeraddr);
		//开启或者关闭SO_REUSEADDR
		void setReuseAddr(bool on);

		void shutdownWrite();

		void setTcpNoDelay(bool on);

	protected:
	private:
		const int sockfd_;
	};
}

#endif //MYNET_MTNET_SOCKET_H