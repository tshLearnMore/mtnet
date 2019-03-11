#include "TcpConnection.h"
#include "Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"

#include <functional>
#include <memory>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

using namespace mtnet;

TcpConnection::TcpConnection(EventLoop* loop, const std::string& name, int sockfd,
	const InetAddress& localAddr, const InetAddress& peerAddr)
	:loop_(loop),
	name_(name),
	state_(kConnecting),
	socket_(new Socket(sockfd)),
	channel_(new Channel(loop, sockfd)),
	localAddr_(localAddr),
	peerAddr_(peerAddr)

{
	LOG_DEBUG("TcpConnection::ctor[ %s ] at %x fd= %d",name_,this,sockfd);
	channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this,std::placeholders::_1));
	channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
	channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
	channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
}

TcpConnection::~TcpConnection()
{
	LOG_DEBUG("TcpConnection::dtor[ %s ] at %x  fd= %d",name_,this,channel_->fd());
}

void TcpConnection::send(const std::string& message)
{
	if (state_ == kConnected)
	{
		if (loop_->isInLoopThread())
		{
			sendInLoop(message);
		} 
		else
		{
			loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this,message));
		}
	}
}

void TcpConnection::sendInLoop(const std::string& message)
{
	loop_->assertInLoopThread();
	ssize_t nwrote = 0;
	//先尝试一次性发送，若不能怎将数据放入outputBuffer中，并开始关心writable事件发送剩余的数据
	if (!channel_->isWriting() && outputBuffer_.readableBytes()==0)
	{
		nwrote = ::write(channel_->fd(),message.data(),message.size());
		if (nwrote>=0)
		{
			if (static_cast<size_t>(nwrote) < message.size())
			{
				LOG_TRACE("I am going to write more data");
			}
			else if (writeCompleteCallback_)//全部数据发送完回调
			{
				loop_->queueInLoop(std::bind(writeCompleteCallback_,shared_from_this()));
			}
		} 
		else
		{
			nwrote = 0;
			if (errno != EWOULDBLOCK)
			{
				LOG_ERROR("TcpConnection::sendInLoop");
			}
		}
	}

	assert(nwrote>=0);
	if (static_cast<size_t>(nwrote) < message.size())
	{
		outputBuffer_.append(message.data()+nwrote,message.size()-nwrote);
		if (!channel_->isWriting())
		{
			channel_->enableWriting();
		}
	}
}

void TcpConnection::shutdown()
{
	if (state_ == kConnected)
	{
		setState(kDisconnecting);
		loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop,this));
	}
}

void TcpConnection::setTcpNoDelay(bool on)
{
	socket_->setTcpNoDelay(on);
}

void TcpConnection::shutdownInLoop()
{
	loop_->assertInLoopThread();
	if (!channel_->isWriting())
	{
		socket_->shutdownWrite();
	}
}

void TcpConnection::connectDestroyed()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnected || state_==kDisconnecting);
	setState(kDisconnected);
	channel_->disableAll();
	connectionCallback_(shared_from_this());

	loop_->removeChannel(channel_.get());
}

void TcpConnection::connectEstablished()
{
	loop_->assertInLoopThread();
	assert(state_ == kConnecting);
	setState(kConnected);
	channel_->enableReading();

	connectionCallback_(shared_from_this());
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
	int savedErrno = 0;
	ssize_t n = inputBuffer_.readFd(channel_->fd(),&savedErrno);
	if (n > 0)
	{
		messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
	} 
	else if (n == 0)
	{
		handleClose();
	} 
	else 
	{
		errno = savedErrno;
		LOG_ERROR("TcpConnection::handleRead");
		handleError();
	}
}

void TcpConnection::handleWrite()
{
	loop_->assertInLoopThread();
	if (channel_->isWriting())
	{
		ssize_t n = ::write(channel_->fd(),outputBuffer_.peek(),outputBuffer_.readableBytes());
		if (n>0)
		{
			outputBuffer_.retrieve(n);
			if (outputBuffer_.readableBytes()==0)
			{
				channel_->disableWriting();
				if (writeCompleteCallback_)
				{
					loop_->queueInLoop(std::bind(writeCompleteCallback_,shared_from_this()));
				}
				if (state_ ==kDisconnecting)
				{
					shutdownInLoop();
				}
			} 
			else
			{
				LOG_TRACE("I am going to write more data");
			}
		} 
		else
		{
			LOG_ERROR("TcpConnection::handleWrite");
		}
	} 
	else
	{
		LOG_TRACE("Connection is down, no more writing");
	}

}

void TcpConnection::handleClose()
{
	loop_->assertInLoopThread();
	LOG_TRACE("TcpConnection::handleClose state = %d", state_);
	assert(state_ == kConnected || state_ == kDisconnecting);
	channel_->disableAll();
	closeCallback_(shared_from_this());
}

void TcpConnection::handleError()
{
	int err = sockets::getSocketError(channel_->fd());
	LOG_ERROR("TcpConnection::handleError [ %s ] - SO_ERROR = %d" ,name_,err);
}