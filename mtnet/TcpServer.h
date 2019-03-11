/*
TcpServer相关操作封装  通过tcpConnect接受新的连接
*/
#ifndef MYNET_MTNET_TCPSERVER_H
#define MYNET_MTNET_TCPSERVER_H

#include "Callbacks.h"
#include "TcpConnection.h"
#include "Uncopyable.h"

#include <map>
#include <memory>

namespace mtnet
{
	class Acceptor;
	class EventLoop;
	class EventLoopThreadPool;

	class TcpServer : Uncopyable
	{
	public:
		TcpServer(EventLoop* loop, const InetAddress& listenAddr);
		~TcpServer();

		//设置处理输入输出事件的线程数
		//accept总是在loop线程中，然后将tcpconnection分配给给个io线程
		//必须在start()函数前调用
		//参数：numThreads
		//0 表示所有任务都在一个线程中做
		//1 表示accept单独一个线程，其他连接都在另一个线程中做
		//N 表示accept单独一个线程，其他连接都在另N个线程中做----分配方式是循环分配
		void setThreadNum(int numThreads);
		void start();

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

	protected:
	private:
		void newConnection(int sockfd, const InetAddress& peerAddr);
		void removeConnection(const TcpConnectionPtr& conn);
		void removeConnectionInLoop(const TcpConnectionPtr& conn);

		typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

		EventLoop* loop_;
		const std::string name_;
		std::shared_ptr<Acceptor> acceptor_;
		std::shared_ptr<EventLoopThreadPool> threadPool_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;//低水位回调，在全部数据发送完之后，回调这个函数
		bool started_;
		int nextConnId_;
		ConnectionMap connections_;
	};
}

#endif //MYNET_MTNET_TCPSERVER_H