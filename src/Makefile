SHELL=/bin/bash
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
CC=g++

all: ../fa

../mdlviewer: mdlviewer.o
	$(CC) -o../mdlviewer mdlviewer.o $(PC_LIBS)

../fa: main.o
	$(CC) -o../fa main.o $(PC_LIBS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

mdlviewer.o: mdlviewer.cpp
	$(CC) $(CFLAGS) -c mdlviewer.cpp
	
clean:
	rm ../fa main.o
