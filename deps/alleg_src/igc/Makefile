DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0 -fmax-errors=10 -fPIC

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)

all: $(objects)

igc.so: $(objects)
	$(CC) -fPIC -shared -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

clean:
	rm *.o
