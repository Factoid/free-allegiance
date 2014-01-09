#ifndef __OGL_RENDERER_HPP__
#define __OGL_RENDERER_HPP__

#include "renderer.hpp"
#include "ogl_common.hpp"
#include <map>
#include <string>

class OGLRenderer : public Renderer
{
  public:
    OGLRenderer();
    ~OGLRenderer();

    Window& createWindow( unsigned int width, unsigned int height, const std::string& name );
    Model& createModel( const std::string& path );
    Shader& createShader( const std::string& path );
    Texture& createTexture( const std::string& path );
    void render( Transform* root );
    void pollEvents();

    bool isClosed();
    void swapBuffers();

  private:
    typedef std::map<std::string,Model*> model_map;
    model_map models;
    typedef std::map<std::string,Window*> window_map;
    window_map windows;
    typedef std::map<std::string,Shader*> shader_map;
    shader_map shaders;
    typedef std::map<std::string,Texture*> texture_map;
    texture_map textures;
};

#endif
