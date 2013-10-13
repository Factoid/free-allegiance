#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer/renderer.hpp"
#include <memory>
#include <iostream>
#include <boost/log/trivial.hpp>

void glErrorLogger( int code, const char* description )
{
  BOOST_LOG_TRIVIAL(error) << "GLFW Error(" << code << ") " << description;
}

int main( int argc, char** argv )
{
  BOOST_LOG_TRIVIAL(info) << "Starting free allegiance 0.0.1";

  glfwSetErrorCallback( glErrorLogger );
  if( !glfwInit() )
  {
    glfwTerminate();
    return 1;
  }

  Renderer* renderer = CreateOpenGLRenderer();
  renderer->CreateWindow( 640, 480, "Allegiance 0.0.1" );

  glfwTerminate();
  return 0;
}
