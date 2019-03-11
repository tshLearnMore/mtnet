/*
TcpConnect��ز�����װ  ÿ�����ӽ����󶼷���һ��TcpConnection��������
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

		//�ڲ�ʹ�� 
		void setCloseCallback(const CloseCallback& cb)
		{
			closeCallback_ = cb;
		}

		//void send(const void* message, size_t len);
		void send(const std::string& message);
		void shutdown();
		void setTcpNoDelay(bool on);

		//��TcpServerɾ����ǰconn������
		void connectDestroyed();

		//�ڲ�ʹ�ã���TCPServer����һ�������ӵ�ʱ��
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
		WriteCompleteCallback writeCompleteCallback_;//��ˮλ�ص�����ȫ�����ݷ�����֮�󣬻ص��������
		CloseCallback   closeCallback_;
		Buffer inputBuffer_;
		Buffer outputBuffer_;
	};
	//typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
}

#endif //MYNET_MTNET_TCPCONNECT_H