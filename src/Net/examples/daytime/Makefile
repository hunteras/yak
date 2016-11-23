TARGS=daytimetcpcli daytimetcpserv daytimetcpsrv1

CXX=clang++
CXXFLAGS:=$(CXXFLAGS) -std=c++11 -I/usr/local/include/ -I../../
LDFLAGS:=$(LDFLAGS)  -L/usr/local/lib/ -lpthread

%: %.cpp
	$(CXX) -o $@ $< $(LDFLAGS) $(CXXFLAGS)

all: $(TARGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGS)

