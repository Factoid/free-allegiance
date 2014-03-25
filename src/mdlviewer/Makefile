SHELL=/bin/bash
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
DEP_INCLUDE=-I../../deps/cereal/include -I../
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0

model_objs = $(wildcard ../model/*.o)
objects = mdlviewer.o $(model_objs)

all: mdlviewer

mdlviewer: $(objects) $(model_objs)
	$(CC) -o $@ $^ $(PC_LIBS) 
mdlviewer.o: mdlviewer.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

clean:
	rm mdlviewer *.o

install: mdlviewer
	cp mdlviewer ../../
