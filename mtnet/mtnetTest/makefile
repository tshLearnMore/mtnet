LIB_SRC =               \
	../Channel.cc         \
	../EventLoop.cc       \
	../Poller.cc          \
	../TimerQueue.cc

BINARIES = test1

all: $(BINARIES)

CXXFLAGS = -O0 -g  -Wall -I ../../base/  -I ../  -pthread
LDFLAGS = -std=c++11 -lpthread
BASE_SRC = ../../base/Thread.cc ../../base/Timestamp.cc ../../base/Logging.cc

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

test1:    test1.cc

clean:
	rm -f $(BINARIES) core
