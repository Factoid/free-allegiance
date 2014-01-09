#ifndef __OGL_SHADER_HPP__
#define __OGL_SHADER_HPP__

#include "shader.hpp"
#include <string>

class OGLShader : public Shader
{
  public:
    OGLShader( const std::string& path );
    ~OGLShader();

    void bind();

  private:
    unsigned int vertexID;
    unsigned int fragmentID;
    unsigned int programID;
};

#endif
