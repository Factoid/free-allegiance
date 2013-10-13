AC_INIT([free_allegiance],[0.0.1],[adrian.cheater@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
PKG_CHECK_MODULES(BOOST,boost >= 1.54.0)
PKG_CHECK_MODULES(GLFW,glfw3)
AC_PROG_CXX
AC_PROG_RANLIB([librenderer.a])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/renderer/Makefile
])
AC_OUTPUT
