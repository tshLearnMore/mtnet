/*
测试TcpConnection、TcpServer、Buffer功能
*/
#include "TcpServer.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "Timestamp.h"

#include <stdio.h>
#include <unistd.h>

void onConnection(const mtnet::TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		printf("onConnection(): tid=%d new connection [%s] from %s\n",
			mtnet::CurrentThread::tid(),conn->name().c_str(),conn->peerAddress().toHostPort().c_str());
	}
	else
	{
		printf("onConnection():tid=%d connection [%s] is down \n",
			mtnet::CurrentThread::tid(),conn->name().c_str());
	}
}

void onMessage(const mtnet::TcpConnectionPtr& conn, mtnet::Buffer* buf, mtnet::Timestamp time)
{
	printf("onMessage():tid=%d received %zd bytes from connection [%s]\n",
		mtnet::CurrentThread::tid(),buf->readableBytes(),conn->name().c_str());
	buf->retrieveAll();
}

int main()
{
	printf("main():pid = %d\n",getpid());

	mtnet::InetAddress listenAddr(9981);
	mtnet::EventLoop loop;

	mtnet::TcpServer server(&loop, listenAddr);
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.setThreadNum(3);//设置io线程数，总线程数是4= loop线程 + 3个io线程
	server.start();

	loop.loop();
}