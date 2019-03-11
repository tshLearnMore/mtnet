/*
TcpConnect相关操作封装  每个连接建立后都分配一个TcpConnection单独处理
*/
#ifndef MYNET_MTNET_TCPCONNECT_H
#define MYNET_MTNET_TCPCONNECT_H

#include "Uncopyable.h"
#include "Callbacks.h"
#include "InetAddress.h"
#include "Buffer.h"

#include <functional>
#include <memory>

namespace mtnet
{
	class Channel;
	class EventLoop;
	class Socket;

	class TcpConnection : Uncopyable,
		public std::enable_shared_from_this<TcpConnection> 
	{
	public:
		TcpConnection(EventLoop* loop, const std::string& name, int sockfd,
			const InetAddress& localAddr, const InetAddress& peerAddr);
		~TcpConnection();

		EventLoop* getLoop() const
		{
			return loop_;
		}

		const std::string& name() const
		{
			return name_;
		}

		const InetAddress& localAddress()
		{
			return localAddr_;
		}

		const InetAddress& peerAddress()
		{
			return peerAddr_;
		}

		bool connected() const
		{
			return state_ == kConnected;
		}

		void setConnectionCallback(const ConnectionCallback& cb)
		{
			connectionCallback_ = cb;
		}

		void setMessageCallback(const MessageCallback& cb)
		{
			messageCallback_ = cb;
		}
		void setWriteCompleteCallback(const WriteCompleteCallback& cb)
		{
			writeCompleteCallback_ = cb;
		}

		//内部使用 
		void setCloseCallback(const CloseCallback& cb)
		{
			closeCallback_ = cb;
		}

		//void send(const void* message, size_t len);
		void send(const std::string& message);
		void shutdown();
		void setTcpNoDelay(bool on);

		//当TcpServer删除当前conn后会调用
		void connectDestroyed();

		//内部使用，当TCPServer接受一个新连接的时候
		void connectEstablished();
	protected:
	private:
		enum StateE {
			kConnecting,
			kConnected,
			kDisconnecting,
			kDisconnected
		};

		void setState(StateE s)
		{
			state_ = s;
		}

		void handleRead(Timestamp receiveTime);
		void handleWrite();
		void handleClose();
		void handleError();
		void sendInLoop(const std::string& message);
		void shutdownInLoop();

		EventLoop* loop_;
		std::string name_;
		StateE state_;
		std::shared_ptr<Socket> socket_;
		std::shared_ptr<Channel> channel_;
		InetAddress localAddr_;
		InetAddress peerAddr_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;//低水位回调，在全部数据发送完之后，回调这个函数
		CloseCallback   closeCallback_;
		Buffer inputBuffer_;
		Buffer outputBuffer_;
	};
	//typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
}

#endif //MYNET_MTNET_TCPCONNECT_H