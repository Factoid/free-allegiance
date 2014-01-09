#ifndef __SHADER_HPP__
#define __SHADER_HPP__

class Shader
{
  public:
    virtual void bind() = 0;
    virtual ~Shader() {};
};

#endif
