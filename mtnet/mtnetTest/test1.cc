#include "EventLoop.h"
#include <stdio.h>


mtnet::EventLoop* g_loop;

void print()
{
	printf("exc runAfter(1.0,print) \n");
};

void printInterval()
{
	printf("exc runEvery(2.0,printInterval) \n");
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
	mtnet::Thread t(threadFunc);
	t.start();
	loop.loop();
	return 0;
}