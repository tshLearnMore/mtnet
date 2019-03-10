

BINARIES = test1

all: $(BINARIES)

CXXFLAGS = -O0 -g  -Wall -I ../../base/   -pthread
LDFLAGS = -std=c++11 -lpthread
BASE_SRC = ../../base/Thread.cc ../../base/Timestamp.cc
LIB_SRC =  ../../base/Channel.cc  ../EventLoop.cc  ../Poller.cc   ../TimerQueue.cc
$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(filter %.cc,$^) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

test1:    test1.cc

clean:
	rm -f $(BINARIES) core
