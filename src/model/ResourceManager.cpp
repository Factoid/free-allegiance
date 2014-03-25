#include "ResourceManager"

#include <osg/Vec3>
#include <osgDB/ReadFile>

namespace fa
{
  std::shared_ptr<ResourceManager> ResourceManager::instance() {
    _instance = std::shared_ptr<ResourceManager>(new ResourceManager);
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

    osg::ref_ptr<osg::Image> img = osgDB::readImageFile( path );
    images[path] = img;
    return img;
  }

  osg::ref_ptr<osg::Geometry> ResourceManager::createQuad()
  {
    osg::ref_ptr<osg::Geometry> quad( new osg::Geometry );
    osg::ref_ptr<osg::Vec3Array> quadVerts(new osg::Vec3Array);
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

}
