CXXFLAGS=-g -Wall

thread:
	g++ -g -o threadtest thread_test.cc ../Thread.cc  -std=c++11 -pthread
atomic:
	g++ -g -o atomic atomic_test.cc   -std=c++11
mutex:
	g++ -g -o mutextest mutex_test.cc  ../Thread.cc  -std=c++11 -pthread
conditiontest:
	g++ -g -o conditiontest condition_test.cc  ../Thread.cc  -std=c++11 -pthread
timestamp:
	g++ -g -o timestamp timestamp_test.cc  ../Timestamp.cc -std=c++11
clean:
	rm threadtest atomic mutextest conditiontest timestamp