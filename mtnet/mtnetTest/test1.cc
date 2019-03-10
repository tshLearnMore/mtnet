#include "EventLoop.h"
#include "Thread.h"
#include <stdio.h>
#include <unistd.h>

mtnet::EventLoop* g_loop;

void print()
{
	printf("exc runAfter(1.0,print) tid = %d\n",mtnet::CurrentThread::tid());
};

void printInterval()
{
	printf("exc runEvery(2.0,printInterval) tid = %d\n",mtnet::CurrentThread::tid());
}

void threadFunc()
{
	g_loop->runAfter(1.0,print);
	g_loop->runEvery(2,printInterval);

}

int main()
{
	mtnet::EventLoop loop;
	g_loop = &loop;
	mtnet::Thread t1(threadFunc);
	t1.start();
	mtnet::Thread t2(threadFunc);
	t2.start();
	sleep(2);
	loop.loop();
	return 0;
}