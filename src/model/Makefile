DEP_INCLUDE+=-I../../deps/cereal/include
CFLAGS+=-std=gnu++11 -ggdb3 -O0 -fpic
CC=g++

.SUFFIXES:

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)

all: $(objects)

clean:
	rm *.o *.d

deps: $(sources:.cpp=.d)

-include $(sources:.cpp=.d)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -std=c++11 $(DEP_INCLUDE) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
  rm -f $@.$$$$

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

