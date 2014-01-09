#include "transform.hpp"
#include "renderable.hpp"
#include <deque>
#include <boost/foreach.hpp>

void Transform::add( Renderable& renderable )
{
  renderable.setTransform(this);
  renderables.insert(&renderable);
}

void Transform::remove( Renderable& renderable )
{
  renderable.setTransform(0);
  renderables.erase(&renderable);
}

void Transform::update( std::set<Texture*>& textures, std::set<Model*>& models, std::set<Shader*>& shaders, std::set<Renderable*>& renderers )
{
  std::deque<Transform*> queue;
  queue.push_back( this );
  while( !queue.empty() )
  {
    Transform* t = queue.front();
    BOOST_FOREACH( Renderable* r, t->renderables )
    {
      r->listTextures( textures );
      r->listModels( models );
      r->listShaders( shaders );
      renderers.insert(r);
    }
    queue.insert(queue.end(), t->children.begin(), t->children.end());
    queue.pop_front();
  }
}
