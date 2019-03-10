/* 
²âÊÔEventLoopThread¹¦ÄÜ
*/
#include "EventLoop.h"
#include "EventLoopThread.h"

#include <stdio.h>
#include <unistd.h>

using namespace mtnet;
void runInThread()
{
	printf("runInThread():pid = %d, tid = %d\n",
		getpid(), CurrentThread::tid());
}

int main()
{
	printf("main():pid = %d, tid = %d\n", getpid(), CurrentThread::tid());

	EventLoopThread loopThread;
	EventLoop* loop = loopThread.startLoop();
	loop->runInLoop(runInThread);
	sleep(1);
	loop->runAfter(2,runInThread);
	loop->runEvery(1,runInThread);
	sleep(3);
	loop->quit();
	sleep(3);
	printf("exit main(). \n");

	return 0;
}