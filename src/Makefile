TARGS=libyak
OBJS=Date.o
SRCS= $(OBJS:.o=.cpp)

CXX=clang++
CXXFLAGS:=$(CXXFLAGS) -std=c++11 -I/usr/local/include/
LDFLAGS:=$(LDFLAGS)  #-L/usr/local/lib/ #-lgtest #-lpthread

%.o: %.cpp
	$(CXX) -c $< $(LDFLAGS) $(CXXFLAGS)

all: $(TARGS)

$(TARGS):$(OBJS)
	rm -f $@
	ar rcs $(TARGS) $(OBJS) 

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGS)

