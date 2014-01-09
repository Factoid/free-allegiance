#ifndef __OGL_MODEL_HPP__
#define __OGL_MODEL_HPP__

#include "model.hpp"
#include <string>
#include <vector>

class OGLModel : public Model
{
  public:
    OGLModel( const std::string& path );
    void bind();
    void draw();
    ~OGLModel();

  private:
    bool loaded;
    unsigned int vertexArrayObject;
    unsigned int vertexBuffer;
    unsigned int uvBuffer;
    unsigned int normalBuffer;
    unsigned int indexBuffer;
    unsigned int nIndices;
};

#endif
