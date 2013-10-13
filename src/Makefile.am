SUBDIRS = renderer
bin_PROGRAMS = fa
fa_SOURCES = main.cpp
fa_CPPFLAGS = $(BOOST_CFLAGS) $(GLFW_CFLAGS) -pthread
fa_LDFLAGS = $(BOOST_LIBS) $(GLFW_LIBS) -pthread
fa_LDADD = renderer/librenderer.a
