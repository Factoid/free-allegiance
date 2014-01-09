AC_INIT([free_allegiance],[0.0.1],[adrian.cheater@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
#PKG_CHECK_MODULES([DEPS],[boost gl glfw3 glew assimp glm])
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
  [enable debugging, default: no]),
  [case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
  esac],
  [debug=false])
                                                      
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")
AC_PROG_CXX
AC_PROG_RANLIB([librenderer.a])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
