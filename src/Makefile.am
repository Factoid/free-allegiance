bin_PROGRAMS = fa
if DEBUG
  AM_CFLAGS = -ggdb -O0
  AM_CXXFLAGS = -ggdb -O0
else
  AM_CFLAGS = -O2
  AM_CXXFLAGS = -O2
endif

fa_SOURCES = main.cpp 
#renderer/ogl_renderer.cpp renderer/ogl_window.cpp renderer/ogl_model.cpp renderer/renderable.cpp renderer/transform.cpp renderer/ogl_shader.cpp renderer/ogl_texture.cpp 
fa_CPPFLAGS = -fPIC $(DEPS_CFLAGS) -pthread
fa_LDFLAGS = $(DEPS_LIBS) -pthread
#fa_LDADD = libSOIL.a
