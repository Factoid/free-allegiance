SHELL=/bin/bash
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
DEP_INCLUDE=../deps/cereal/include
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0

all: ../fa

../mdlviewer: mdlviewer.o color.o frame.o geo.o group_geo.o image.o light.o lod_geo.o mesh_geo.o model_definition.o resource_base.o resource_manager.o texture_geo.o vector3.o
	$(CC) -o../mdlviewer mdlviewer.o $(PC_LIBS)

../fa: main.o
	$(CC) -o../fa main.o $(PC_LIBS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

*.o: *.cpp
	$(CC) $(CFLAGS) -I$(DEP_INCLUDE) -c mdlviewer.cpp
	
clean:
	rm ../fa main.o
