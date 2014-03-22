DEP_INCLUDE+=-I../../deps/alleg_src/ -I../../deps/alleg_src/zlib
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
CC=g++
CFLAGS=-std=gnu++11 -ggdb3 -O0 -fno-inline-small-functions -fmax-errors=10 -fPIC -Wall -Wno-unused-function

all: client

client: client.cpp ../../deps/alleg_src/igc/libigc.so
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -L../../deps/alleg_src/igc/ $< -o $@ -ligc $(PC_LIBS)

install: client
	cp client ../../

clean:
	rm client *.o
