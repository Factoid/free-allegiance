#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <string>

class Renderer
{
  public:
    virtual bool CreateWindow( unsigned int width, unsigned int height, const std::string& name ) = 0;
    virtual ~Renderer() {};
};

Renderer* CreateOpenGLRenderer();

#endif

