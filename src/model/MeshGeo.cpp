#include "MeshGeo"
#include "ResourceManager"

#include <osgDB/ReadFile>
#include <cereal/types/polymorphic.hpp>

namespace fa
{
  MeshGeo::MeshGeo() {}
  MeshGeo::MeshGeo( const std::string& v ) : ResourceBase(v) {}
  osg::ref_ptr<osg::Node> MeshGeo::buildGraph( osg::ref_ptr<osg::Node> node ) {
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( ResourceManager::getPathBase() + resourcePath );
    return model;
  }
}
CEREAL_REGISTER_TYPE_WITH_NAME( fa::MeshGeo, "MeshGeo" );
//CEREAL_REGISTER_TYPE( MeshGeo );
