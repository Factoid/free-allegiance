#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <boost/math/quaternion.hpp>
#include <set>
#include "vector.hpp"

class Renderable;
class Texture;
class Model;
class Shader;

class Transform
{
  public:
    void add( Renderable& renderable );
    void remove( Renderable& renderable );
    void update( std::set<Texture*>& textures, std::set<Model*>& models, std::set<Shader*>& shaders, std::set<Renderable*>& renderables );

  private:
    std::set<Renderable*> renderables;
    Vector3D localTranslation;
    Vector3D localScale;
    boost::math::quaternion<float> localRotation;

    Transform* parent;
    std::set<Transform*> children;
};

#endif
