SHELL=/bin/bash
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
CC=g++

all: fa

fa: main.o
	$(CC) -ofa main.o $(PC_LIBS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm fa main.o
