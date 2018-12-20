/************************************************************************/
/* �������߶�������ģ��   ����                                                                  */
/************************************************************************/
#include "../Mutex.h"
#include "../Thread.h"
#include "../Condition.h"

#include <deque>
#include <functional>
#include <stdio.h>
#include <unistd.h>


using namespace mtnet;

Mutex gMutex;
Condition gCond(gMutex);
std::deque<int> gDeq;

void getData()
{
	gMutex.lock();
	while(gDeq.empty())
	{
		printf("getData wait \n");
		gCond.wait();
		int cur = gDeq.front();
		gDeq.pop_front();
		printf("tid=%d getData cur = %d  leave size = %d\n",CurrentThread::tid(),cur,int(gDeq.size()));
		gMutex.unlock();
		gMutex.lock();
		
	}
	printf("getData end \n");
}
void pushData()
{
	sleep(1);//��ҪgetData�߳������У���ȻGdeq�������ݺ�getData��ֱ�ӽ���
	int curNum = 1;
	gMutex.lock();
	while (gDeq.empty())
	{
		gDeq.push_back(curNum++);
		printf("tid=%d pushData notify \n",CurrentThread::tid());
		gCond.notify();
		gMutex.unlock();
		sleep(1); //sleep��Ϊ�ˣ���getData����ȡ���ݣ���ȻpushData�����ٴ�����������
		gMutex.lock();
	}
	printf("pushData end \n");
}

int main()
{
	mtnet::Thread thread1(pushData);
	mtnet::Thread thread2(getData);
	mtnet::Thread thread3(getData);

	thread1.start();
	thread2.start();
	thread3.start();
	thread1.join();
	thread2.join();
	thread3.join();
	return 0;
}
