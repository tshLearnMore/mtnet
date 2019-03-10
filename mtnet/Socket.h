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
		//���addr��ʹ���о�abort
		void bindAddress(const InetAddress& localaddr);
		//���addr��ʹ���о�abort
		void listen();
		//�ɹ�������һ�����������ļ�������
		//ʧ�ܣ����� -1
		int accept(InetAddress* peeraddr);
		//�������߹ر�SO_REUSEADDR
		void setReuseAddr(bool on);

		void shutdownWrite();

		void setTcpNoDelay(bool on);

	protected:
	private:
		const int sockfd_;
	};
}

#endif //MYNET_MTNET_SOCKET_H