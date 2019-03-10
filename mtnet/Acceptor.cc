#include "Acceptor.h"

#include "Logging.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

#include <functional>

using namespace mtnet;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
	:loop_(loop),
	acceptorSocket_(sockets::createNonblockingOrDie()),
	acceptChannel_(loop_,acceptorSocket_.fd()),
	listenning_(false)
{
	acceptorSocket_.setReuseAddr(true);
	acceptorSocket_.bindAddress(listenAddr);
	acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead,this));
}

void Acceptor::listen()
{
	loop_->assertInLoopThread();
	listenning_ = true;
	acceptorSocket_.listen();
	acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
	loop_->assertInLoopThread();
	InetAddress peerAddr(0);

	int connfd = acceptorSocket_.accept(&peerAddr);
	if (connfd >= 0)
	{
		if (newConnectionCallback_)
		{
			newConnectionCallback_(connfd,peerAddr);
		}
	} 
	else
	{
		sockets::close(connfd);
	}
}