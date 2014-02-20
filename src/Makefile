SHELL=/bin/bash
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
DEP_INCLUDE=-I../deps/cereal/include
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0

objects = mdlviewer.o model/Color.o model/Frame.o model/Geo.o model/GroupGeo.o model/Image.o model/Light.o model/LODGeo.o model/MeshGeo.o model/ModelDefinition.o model/ResourceBase.o model/ResourceManager.o model/TextureGeo.o model/Vector3.o 

mdlviewer: $(objects) 
	$(CC) -o $@ $^ $(PC_LIBS)

mdlviewer.o: mdlviewer.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

install: mdlviewer
	cp mdlviewer ../

include model/Makefile
