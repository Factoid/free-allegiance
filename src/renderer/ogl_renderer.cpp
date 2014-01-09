#include "ogl_common.hpp"
#include "ogl_renderer.hpp"
#include "ogl_window.hpp"
#include "ogl_model.hpp"
#include "ogl_shader.hpp"
#include "ogl_texture.hpp"
#include "transform.hpp"
#include "resource_exception.hpp"
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include "renderable.hpp"

Renderer& CreateOpenGLRenderer()
{
  static OGLRenderer oglRenderer;
  return oglRenderer;
}

Window& OGLRenderer::createWindow( unsigned int width, unsigned int height, const std::string& name )
{
  if( windows.find( name ) == windows.end() )
  {
    windows[name] = new OGLWindow( width, height, name );
  }
  return *windows[name];
}

Model& OGLRenderer::createModel( const std::string& path )
{
  if( models.find( path ) == models.end() )
  {
    models[path] = new OGLModel( path );
  }
  return *models[path];
}

Texture& OGLRenderer::createTexture( const std::string& path )
{
  if( textures.find( path ) == textures.end() )
  {
    textures[path] = new OGLTexture( path );
  }
  return *textures[path];
}

Shader& OGLRenderer::createShader( const std::string& path )
{
  if( shaders.find( path ) == shaders.end() )
  {
    shaders[path] = new OGLShader( path );
  }
  return *shaders[path];
}

void OGLRenderer::render( Transform* root )
{
  std::set<Texture*> textures;
  std::set<Model*> models;
  std::set<Shader*> shaders;
  std::set<Renderable*> renderables;

  root->update(textures,models,shaders,renderables);
//  BOOST_LOG_TRIVIAL(debug) << "Found " << textures.size() << " textures, " << models.size() << " models, " << shaders.size() << " shaders, and " << renderables.size() << " render instances";
  BOOST_FOREACH( Renderable* r, renderables )
  {
    r->bind();
    r->draw();
  }
}

void OGLRenderer::pollEvents()
{
  glfwPollEvents();
}

OGLRenderer::OGLRenderer()
{
  if( !glfwInit() )
  {
    glfwTerminate();
    throw ResourceException();
  }
}

OGLRenderer::~OGLRenderer()
{
  BOOST_FOREACH( model_map::value_type& i, models )
  {
    delete i.second;
  }
  BOOST_FOREACH( window_map::value_type& i, windows )
  {
    delete i.second;
  }
  BOOST_FOREACH( shader_map::value_type& i, shaders )
  {
    delete i.second;
  }
  BOOST_FOREACH( texture_map::value_type& i, textures )
  {
    delete i.second;
  }

  BOOST_LOG_TRIVIAL(debug) << "glfwTerminate()";
  glfwTerminate();
}
