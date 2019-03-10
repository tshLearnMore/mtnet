/*
socket��װ
*/

#ifndef MYNET_MTNET_INETADDRESS_H
#define MYNET_MTNET_INETADDRESS_H

#include <string>
#include <netinet/in.h>

namespace mtnet
{
	class InetAddress
	{
	public:
		//Ĭ��ip + port ����һ��sockaddr_in
		explicit InetAddress(uint16_t port);
		//ָ��ip + port ����һ��sockaddr_in
		InetAddress(const std::string& ip, uint16_t port);

		InetAddress(const struct sockaddr_in& addr)
			:addr_(addr)
		{}

		//����ip���ַ�����ʽ
		std::string toHostPort() const;

		const struct sockaddr_in& getSockaddrInet() const
		{
			return addr_;
		}

		void setSockAddrInet(const struct sockaddr_in& addr)
		{
			addr_ = addr;
		}
	protected:
	private:
		struct sockaddr_in addr_;
	};
}

#endif //MYNET_MTNET_INETADDRESS_H