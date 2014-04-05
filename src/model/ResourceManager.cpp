#include "ResourceManager"
#include "ModelDefinition"
#include <osg/Geometry>
#include <osg/Vec3>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>

namespace fa
{
  std::shared_ptr<ResourceManager> ResourceManager::instance() {
    if( _instance == nullptr )
    {
      _instance = std::shared_ptr<ResourceManager>(new ResourceManager);
    }
    return _instance;
  }

  osg::ref_ptr<osg::Drawable> ResourceManager::getPrimative( ResourceManager::PrimativeType type )
  {
    return primatives[type];
  }

  osg::ref_ptr<osg::Image> ResourceManager::getImage( const std::string& path )
  {
    ImageMap::iterator it = images.find(path);
    if( it != images.end() )
    {
      return it->second;
    }

    osg::ref_ptr<osg::Image> img = osgDB::readImageFile( pathBase + path );
    images[path] = img;
    return img;
  }

  osg::ref_ptr<osg::Group> ResourceManager::getModel( const std::string& path )
  {
    auto it = models.find(path);
    if( it != models.end() )
    {
      return it->second;
    }

    osg::ref_ptr<osg::MatrixTransform> root( new osg::MatrixTransform );
    ModelDefinition mDef;	
    mDef.load( pathBase + path );
    mDef.buildGraph(root.get());
    root->setMatrix( osg::Matrix::rotate( osg::Vec3( 0, 0, 1 ), osg::Vec3(0, -1, 0) ) * osg::Matrix::rotate( osg::Vec3( 0, -1, 0 ), osg::Vec3( 0, 1, 0 ) ) );
    models[path] = root;
    return root;
  }

  osg::ref_ptr<osg::Geometry> ResourceManager::createQuad()
  {
    osg::ref_ptr<osg::Geometry> quad( new osg::Geometry );
    osg::ref_ptr<osg::Vec3Array> quadVerts( new osg::Vec3Array );
    quadVerts->push_back( osg::Vec3(-0.5f, 0.0f, 0.5f) );
    quadVerts->push_back( osg::Vec3( 0.5f, 0.0f, 0.5f) );
    quadVerts->push_back( osg::Vec3( 0.5f, 0.0f,-0.5f) );
    quadVerts->push_back( osg::Vec3(-0.5f, 0.0f,-0.5f) );
    quad->setVertexArray(quadVerts);

    osg::ref_ptr<osg::Vec2Array> uvVerts( new osg::Vec2Array );
    uvVerts->push_back( osg::Vec2(0,0) );
    uvVerts->push_back( osg::Vec2(1,0) );
    uvVerts->push_back( osg::Vec2(1,1) );
    uvVerts->push_back( osg::Vec2(0,1) );
    quad->setTexCoordArray( 0, uvVerts );
      
    osg::ref_ptr<osg::DrawElementsUInt> quadIndices( new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0) );
    quadIndices->push_back(3);
    quadIndices->push_back(2);
    quadIndices->push_back(1);
    quadIndices->push_back(0);
    quad->addPrimitiveSet(quadIndices);

    return quad;
  }

  ResourceManager::ResourceManager() {
    primatives[ PRIMATIVE_QUAD ] = createQuad();
  }

  std::shared_ptr<ResourceManager> ResourceManager::_instance;
  void ResourceManager::setPathBase( const std::string& base )
  {
    pathBase = base;
  }

  std::string ResourceManager::getPathBase() {
    return pathBase;
  }

  std::string ResourceManager::pathBase;


}
