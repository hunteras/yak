TARGS=testAll
OBJS=main.o StringTest.o DateTest.o ThreadTest.o BSTreeTest.o RegexTest.o NetTest.o
SRCS= $(OBJS:.o=.cpp)

CXX=clang++
CXXFLAGS:=$(CXXFLAGS) -std=c++11 -I/usr/local/include/
LDFLAGS:=$(LDFLAGS)  -L/usr/local/lib/ -lgtest ../libyak -lpthread

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

all: $(TARGS)

$(TARGS):$(OBJS)
	rm -f $@
	$(CXX) -o $(TARGS) $(OBJS) $(LDFLAGS) $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGS)

