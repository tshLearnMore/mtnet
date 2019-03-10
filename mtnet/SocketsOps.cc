#include "SocketsOps.h"
#include "Logging.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace mtnet;

namespace
{
	typedef struct sockaddr SA;
	const SA* sockaddr_cast(const struct sockaddr_in* addr)
	{
		return static_cast<const SA*>(static_cast<const void*>(addr));
	}

	SA* sockaddr_cast(struct sockaddr_in* addr)
	{
		return static_cast<SA*>(static_cast<void*>(addr));
	}

	void setNonBlockAndCloseOnExec(int sockfd)
	{
		//nonblock
		int flags = ::fcntl(sockfd, F_GETFL, 0);
		flags |= O_NONBLOCK;
		int ret = ::fcntl(sockfd, F_SETFL, flags);

		flags = ::fcntl(sockfd, F_GETFD, 0);
		flags |= FD_CLOEXEC;
		ret = ::fcntl(sockfd, F_SETFD, flags);
	}
}


int sockets::createNonblockingOrDie()
{
#if VALGRIND
	int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		LOG_SYSFATAL << "sockets::createNonblockingOrDie sockfd < 0";
	}
	setNonBlockAndCloseOnExec(sockfd);
#else
	int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,IPPROTO_TCP);
	if (sockfd < 0)
	{
		LOG_FATAL("sockets::createNonblockingOrDie sockfd < 0");
	}
#endif
	return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
	int ret = ::bind(sockfd,sockaddr_cast(&addr), sizeof(addr));
	if (ret < 0)
	{
		LOG_FATAL("sockets::bindOrDie");
	}
}

void sockets::listenOrDie(int sockfd)
{
	int ret = ::listen(sockfd, SOMAXCONN);
	if (ret < 0)
	{
		LOG_FATAL("sockets::listenOrDie");
	}
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
	socklen_t addrlen = sizeof(*addr);
#if VALGRIND
	int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
	setNonBlockAndCloseOnExec(connfd);
#else
	int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, 
		SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
	if (connfd < 0)
	{
		int saveErrno = errno;
		LOG_ERROR("socket::accept");
		switch(saveErrno)
		{
		case EAGAIN:
		case ECONNABORTED:
		case EINTR:
		case EPROTO:
		case EPERM:
		case EMFILE: //每个进程可以打开的文件描述符限制
			//暂时性错误，忽略处理
			errno = saveErrno;
			break;
		case EBADF:
		case EFAULT:
		case EINVAL:
		case ENFILE:
		case ENOBUFS:
		case ENOMEM:
		case ENOTSOCK:
		case EOPNOTSUPP:
			// 致命错误
			LOG_FATAL("unexpected error of ::accept %d ",saveErrno);
			break;
		default:
			LOG_FATAL("unknow error of ::accept %d ",saveErrno);
			break;
		}
	}
	return connfd;
}

void sockets::close(int sockfd)
{
	if (::close(sockfd) < 0)
	{
		LOG_ERROR("sockets::close");
	}
}

void sockets::shutdownWrite(int sockfd)
{
	if (::shutdown(sockfd,SHUT_WR) < 0)
	{
		LOG_ERROR("sockets::shutdownWrite");
	}
}

void sockets::toHostPort(char* buf, size_t size, const struct sockaddr_in& addr)
{
	char host[INET_ADDRSTRLEN] = "INVALID";
	::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof(host));
	uint16_t port = sockets::networkToHost16(addr.sin_port);
	snprintf(buf, size, "%s:%u", host, port);
}
void sockets::fromHostPort(const char* ip, uint16_t port, struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = hostToNetwork16(port);
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
	{
		LOG_ERROR("sockets::fromHostPort");
	}
}

struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
	struct sockaddr_in localAddr;
	bzero(&localAddr, sizeof(localAddr));
	socklen_t addrlen = sizeof(localAddr);
	if (::getsockname(sockfd,sockaddr_cast(&localAddr), &addrlen))
	{
		LOG_ERROR("sockets::getLocalAddr");
	}
	return localAddr;
}

int sockets::getSocketError(int sockfd)
{
	int optval;
	socklen_t optlen = sizeof(optval);
	if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval,  &optlen) < 0)
	{
		return errno;
	} 
	else
	{
		return optval;
	}
}