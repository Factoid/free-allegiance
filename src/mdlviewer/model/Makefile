DEP_INCLUDE+=-I../../deps/cereal/include
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@
