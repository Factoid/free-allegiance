DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)

all: $(objects)

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

clean:
	rm *.o
