#include "ogl_window.hpp"
#include "ogl_common.hpp"
#include "resource_exception.hpp"

OGLWindow::OGLWindow( unsigned int width, unsigned int height, const std::string& name )
{
  window = glfwCreateWindow( width, height, name.c_str(), 0, 0 );
  if( !window )
  {
    throw ResourceException();
  }
  glfwMakeContextCurrent(window);
  glewInit();
}

OGLWindow::~OGLWindow()
{
  BOOST_LOG_TRIVIAL(debug) << "~OGLWindow()";
  if( window )
  {
    BOOST_LOG_TRIVIAL(debug) << "glfwDestroyWindow()";
    glfwDestroyWindow(window);
  }
}

bool OGLWindow::isClosed()
{
  if( window == 0 ) return true;
  return glfwWindowShouldClose(window);
}

void OGLWindow::swapBuffers()
{
  glfwSwapBuffers(window);
}


