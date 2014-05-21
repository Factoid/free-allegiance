DEP_INCLUDE+=-I../../deps/cereal/include
#CC=g++
CFLAGS+=-std=c++11 -ggdb3 -O0 -fpic
#	@set -e; \

%.d: %.cpp
	$(CC) -std=c++11 $(DEP_INCLUDE) -MM $< > $@;
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \

.SUFFIXES:

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)

all: $(objects)

clean:
	rm *.o *.d

deps: $(sources:.cpp=.d)

-include $(sources:.cpp=.d)

#libmodel.a: $(objects)
#	ar rvs $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

