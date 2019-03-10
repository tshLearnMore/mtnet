/*
socket封装
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
		//默认ip + port 生成一个sockaddr_in
		explicit InetAddress(uint16_t port);
		//指定ip + port 生成一个sockaddr_in
		InetAddress(const std::string& ip, uint16_t port);

		InetAddress(const struct sockaddr_in& addr)
			:addr_(addr)
		{}

		//返回ip的字符串形式
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