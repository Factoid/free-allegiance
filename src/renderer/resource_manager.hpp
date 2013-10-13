#ifndef __RESOURCE_MANAGER_HPP__
#define __RESOURCE_MANAGER_HPP__

class ResourceManager
{
  public:
    Texture* LoadTexture( const std::string& path ) = 0;
    Shader* LoadShader( const std::string& path ) = 0;
    Model* LoadModel( const std::string& path ) = 0;

    void UnloadTexture( Texture* texture ) = 0;
    void UnloadShader( Shader* shader ) = 0;
    void UnloadModel( Model* model ) = 0;
};

#endif
