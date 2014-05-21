DEP_INCLUDE+=-I../../deps/cereal/include
#CC=g++
CFLAGS+=-std=gnu++11 -ggdb3 -O0 -fpic

sources = $(wildcard *.cpp)
#objects = $(sources:.cpp=.o)

%.d: %.cpp
	@set -e; \
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

#-include $(sources:.cpp=.d)

#all: $(objects)

#libmodel.a: $(objects)
#	ar rvs $@ $^

#%.o: %.cpp
#	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

clean:
	rm *.o *.d
