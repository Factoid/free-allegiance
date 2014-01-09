#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <string>
class Window;
class Model;
class Texture;
class Shader;
class Transform;

class Renderer
{
  public:
    virtual Window& createWindow( unsigned int width, unsigned int height, const std::string& name ) = 0;
    virtual Model& createModel( const std::string& path ) = 0;
    virtual Texture& createTexture( const std::string& path ) = 0;
    virtual Shader& createShader( const std::string& path ) = 0;
    virtual void pollEvents() = 0;
    virtual void render( Transform* root ) = 0;

    virtual ~Renderer() {};
};

Renderer& CreateOpenGLRenderer();

#endif

