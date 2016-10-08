TARGS=StringTest

CXX=clang++
CXXFLAGS:=$(CXXFLAGS) -std=c++11 -I/usr/local/include/
LDFLAGS:=$(LDFLAGS)  -L/usr/local/lib/ -lgtest #-lpthread

%: %.cpp
	$(CXX) -o $@ $< $(LDFLAGS) $(CXXFLAGS)

all: $(TARGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGS)

