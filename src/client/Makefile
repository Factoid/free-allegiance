DEP_INCLUDE+=-I../../deps/alleg_src/ -I../../deps/alleg_src/zlib -I../ -I../../deps/cereal/include
PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
PC_LIBS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs openscenegraph-osgViewer)
PC_CFLAGS=$(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags openscenegraph-osgViewer)
CC=g++
CFLAGS+=-std=gnu++11 -ggdb -O0 -fvisibility=hidden -fno-inline-small-functions -fmax-errors=10 -fPIC -Wall -Wno-unused-function -Werror $(PC_CFLAGS)

sources = $(wildcard *.cpp) $(wildcard ../model/*.cpp)
#include $(sources:.cpp=.d)
objects = $(sources:.cpp=.o)

all: client.bin

#%.d: %.cpp
#  @set -e; rm -f $@; \
#  $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
#  sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#  rm -f $@.$$$$

client.bin: $(objects)
	$(MAKE) -C ../../deps/alleg_src/igc/
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -L../../deps/alleg_src/igc/ $^ -o $@ -ligc $(PC_LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

install: client.bin
	cp client.bin ../../

clean:
	rm client.bin *.o
	$(MAKE) -C ../model/ clean
	$(MAKE) -C ../../deps/alleg_src/igc/ clean
