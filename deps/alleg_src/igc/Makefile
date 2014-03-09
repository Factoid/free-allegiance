DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0 -fmax-errors=10 -fPIC

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)
depends = $(sources:.cpp=.d)

all: igc.so $(objects)

igc.so: $(objects)
	$(CC) -fPIC -shared -o $@ $^

%.d: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -MM -o $@ $<

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

deps: $(depends)

depclean:
	rm *.d

clean:
	rm *.o *.so
