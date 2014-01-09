#ifndef __OGL_WINDOW_HPP__
#define __OGL_WINDOW_HPP__

#include "window.hpp"
#include <string>

class GLFWwindow;

class OGLWindow : public Window
{
  public:
    OGLWindow( unsigned int width, unsigned int height, const std::string& name );
    ~OGLWindow();
    bool isClosed();
    void swapBuffers();

  private:
    GLFWwindow* window;
};

#endif


