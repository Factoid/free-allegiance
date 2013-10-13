#include "renderer.hpp"
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OGLRenderer : public Renderer
{
  public:
    bool CreateWindow( unsigned int width, unsigned int height, const std::string& name );
    ~OGLRenderer();

  private:
    GLFWwindow* window;

};

Renderer* CreateOpenGLRenderer()
{
  return new OGLRenderer();
}

bool OGLRenderer::CreateWindow( unsigned int width, unsigned int height, const std::string& name )
{
  if( window )
  {
    glfwDestroyWindow(window);
    window = 0;
  }

  window = glfwCreateWindow( width, height, name.c_str(), 0, 0 );
  return window != 0;
}

OGLRenderer::~OGLRenderer()
{
  if( window )
  {
    glfwDestroyWindow(window);
    window = 0;
  }
}
