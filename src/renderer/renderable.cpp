#include "renderable.hpp"
#include "transform.hpp"
#include <boost/foreach.hpp>
#include "shader.hpp"
#include "texture.hpp"
#include "model.hpp"

Renderable::Renderable() : textures(8)
{
}

void Renderable::setModel( Model& m )
{
  model = &m;
}

void Renderable::setShader( Shader& s )
{
  shader = &s;
}

void Renderable::setTexture( unsigned int stage, Texture& t )
{
  textures[stage] = &t;
}

void Renderable::setTransform( Transform* tf )
{
  if( transform != 0 && transform != tf && tf != 0 )
  {
    transform->remove(*this);
  }
  transform = tf;
}

void Renderable::listTextures( std::set<Texture*>& tSet )
{
  BOOST_FOREACH( Texture* t, textures )
  {
    if( t != 0 )
    {
      tSet.insert(t);
    }
  }
}

void Renderable::listModels( std::set<Model*>& mSet )
{
  mSet.insert(model);
}

void Renderable::listShaders( std::set<Shader*>& sSet )
{
  sSet.insert(shader);
}

void Renderable::bind()
{
  shader->bind();
  BOOST_FOREACH( Texture* t, textures )
  {
    if( t != 0 ) t->bind();
  }
  model->bind();
}

void Renderable::draw()
{
  model->draw();
}
