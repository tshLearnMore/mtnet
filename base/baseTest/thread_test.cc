#include "../Thread.h"

#include <string>
#include <functional>
#include <stdio.h>
#include <unistd.h>

void threadFunc1()
{
	printf("thread tid=%d \n",mtnet::CurrentThread::tid());
}

void threadFunc2(int a)
{
	printf("thread tid=%d, a=%d \n",mtnet::CurrentThread::tid(), a);
}

void threadFunc3()
{
	printf("thread tid=%d \n",mtnet::CurrentThread::tid());
	sleep(1);
}

class TestThread
{
public:
	explicit TestThread(double a)
		:a_(a)
	{}
	void memberFunc1()
	{
		printf("thread tid=%d, TestThread::a=%f \n",mtnet::CurrentThread::tid(), a_);
	}
	void memberFunc2(const std::string& str)
	{
		printf("thread tid=%d, TestThread::a=%f, str=%s \n",mtnet::CurrentThread::tid(), a_,str.c_str());
	}
protected:
private:
	double a_;
};

int main()
{
	printf("pid=%d, tid=%d\n",::getpid(),mtnet::CurrentThread::tid());

	mtnet::Thread thread1(threadFunc1);
	thread1.start();
	thread1.join();

	mtnet::Thread thread2(std::bind(threadFunc2, 56),"normal func with one argument");
	thread2.start();
	thread2.join();

	TestThread testThread(633.52);
	mtnet::Thread thread3(std::bind(&TestThread::memberFunc1,&testThread),"class func without argument");
	thread3.start();
	thread3.join();

	mtnet::Thread thread4(std::bind(&TestThread::memberFunc2,&testThread,"class func with string argument"),"class func with one argument");
	thread4.start();
	thread4.join();

	{
		mtnet::Thread thread5(threadFunc3);
		thread5.start();
	}
	sleep(2);
	printf("haved create num of threads %d \n",mtnet::Thread::numCreated());

	return 0;
}