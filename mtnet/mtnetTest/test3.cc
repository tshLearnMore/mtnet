/*
²âÊÔAcceptor¹¦ÄÜ
*/
#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"
#include <stdio.h>
#include <unistd.h>

using namespace mtnet;

void newConnection(int sockfd, const InetAddress& peerAddr)
{
	printf("newConnection(): accepted a new connection from %s \n",
		peerAddr.toHostPort().c_str());
	::write(sockfd,"How are you?\n",13);
	sockets::close(sockfd);
}

int main()
{
	printf("main() : pid = %d\n",getpid());
	EventLoop loop;
	//¼àÌý9981
	InetAddress listenAddr(9981);
	Acceptor acceptor(&loop,listenAddr);
	acceptor.setNewConnectionCallback(newConnection);
	acceptor.listen();
	//¼àÌý9982
	InetAddress listenAddr2(9982);
	Acceptor acceptor2(&loop,listenAddr2);
	acceptor2.setNewConnectionCallback(newConnection);
	acceptor2.listen();

	loop.loop();
}