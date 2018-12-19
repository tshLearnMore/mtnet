#include "../Mutex.h"
#include "../Thread.h"

#include <string>
#include <functional>
#include <stdio.h>
#include <unistd.h>


using namespace mtnet;
class TestMutex
{
public:
	void firstTake()
	{
		printf("in firstTake\n");
		MutexLockGuard guard(mutex_);
		printf("int firstTake sleep 5s start\n");
		sleep(5);
		printf("int firstTake sleep 5s end\n");
	}
	void secondTake()
	{
		printf("in secondTake\n");
		MutexLockGuard guard(mutex_);
		printf("int secondTake sleep 2s start\n");
		sleep(2);
		printf("int secondTake sleep 2s end\n");
	}
protected:
private:
	Mutex mutex_;
};

int main()
{
	TestMutex testMutex;

	mtnet::Thread thread1(std::bind(&TestMutex::firstTake,&testMutex));

	mtnet::Thread thread2(std::bind(&TestMutex::secondTake,&testMutex));
	thread1.start();
	thread2.start();
	thread1.join();
	thread2.join();

	sleep(2);
	printf("haved create num of threads %d \n",mtnet::Thread::numCreated());
	return 0;
}