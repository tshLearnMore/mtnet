/*
TcpServer��ز�����װ  ͨ��tcpConnect�����µ�����
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

		//���ô�����������¼����߳���
		//accept������loop�߳��У�Ȼ��tcpconnection���������io�߳�
		//������start()����ǰ����
		//������numThreads
		//0 ��ʾ����������һ���߳�����
		//1 ��ʾaccept����һ���̣߳��������Ӷ�����һ���߳�����
		//N ��ʾaccept����һ���̣߳��������Ӷ�����N���߳�����----���䷽ʽ��ѭ������
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
		WriteCompleteCallback writeCompleteCallback_;//��ˮλ�ص�����ȫ�����ݷ�����֮�󣬻ص��������
		bool started_;
		int nextConnId_;
		ConnectionMap connections_;
	};
}

#endif //MYNET_MTNET_TCPSERVER_H