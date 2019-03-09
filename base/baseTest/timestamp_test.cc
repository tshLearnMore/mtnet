#include "../Timestamp.h"
#include <stdio.h>

using namespace mtnet;

void printReference(const Timestamp& time)
{
	printf("%s \n",time.toString().c_str());
	printf("%s \n",time.toFormattedString().c_str());
}

void printValue(Timestamp time)
{
	printf("%s \n",time.toString().c_str());
	printf("%s \n",time.toFormattedString().c_str());
}

int main()
{
	Timestamp now(Timestamp::now());
	//Timestamp now = Timestamp::now();
	printf("%s\n",now.toString().c_str());
	printReference(now);
	printValue(now);

	Timestamp newone = addTime(now,1000);
	printValue(newone);
	return 0;
}