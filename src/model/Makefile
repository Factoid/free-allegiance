DEP_INCLUDE+=-I../../deps/cereal/include
CC=g++
CFLAGS+=-std=gnu++11 -ggdb3 -O0 -fpic

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)

all: $(objects)

#libmodel.a: $(objects)
#	ar rvs $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

clean:
	rm libmodel.a *.o
