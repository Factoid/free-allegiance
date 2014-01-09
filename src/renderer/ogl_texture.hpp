#ifndef __OGL_TEXTURE_HPP__
#define __OGL_TEXTURE_HPP__

#include "texture.hpp"
#include <string>

class OGLTexture : public Texture
{
  public:
    OGLTexture( const std::string& path );
    ~OGLTexture();
    void bind();

  private:
    unsigned int id;
};

#endif
