DEP_INCLUDE+=-I../../deps/alleg_src/ -I../../deps/alleg_src/zlib -I../ -I../../deps/cereal/include
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer sdl2)
PC_CFLAGS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags openscenegraph-osgViewer sdl2)
CC=g++
CFLAGS+=-std=gnu++11 -ggdb3 -O0 -fvisibility=hidden -fno-inline-small-functions -fmax-errors=10 -fPIC -Wall -Wno-unused-function -Werror $(PC_CFLAGS)

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o) $(wildcard ../model/*.o)

all: client.bin

client.bin: $(objects)
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -L../../deps/alleg_src/igc/ $^ -o $@ -ligc $(PC_LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

install: client.bin
	cp client.bin ../../

clean:
	rm client.bin *.o
