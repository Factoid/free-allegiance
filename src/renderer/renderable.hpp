#ifndef __RENDERABLE_HPP__
#define __RENDERABLE_HPP__

#include <vector>
#include <set>

class Model;
class Shader;
class Texture;
class Transform;

class Renderable
{
  public:
    Renderable();
    void setModel( Model& model );
    void setShader( Shader& shader );
    void setTexture( unsigned int stage, Texture& texture );

    void setTransform( Transform* tf );
    void listTextures( std::set<Texture*>& textures );
    void listModels( std::set<Model*>& models );
    void listShaders( std::set<Shader*>& shaders );

    void bind();
    void draw();
  private:
    Transform* transform;
    Model* model;
    Shader* shader;
    std::vector<Texture*> textures;
};

#endif
