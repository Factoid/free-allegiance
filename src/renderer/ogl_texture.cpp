#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "ogl_texture.hpp"
#include <boost/log/trivial.hpp>

OGLTexture::OGLTexture( const std::string& path )
{
  id = SOIL_load_OGL_texture( path.c_str(), 0, 0, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS ); 
}

void OGLTexture::bind()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,id);
}

OGLTexture::~OGLTexture()
{
  BOOST_LOG_TRIVIAL(debug) << "~OGLTexture()";
  glDeleteTextures(1,&id);
}
