#include "MeshGeo"

#include <osgDB/ReadFile>
#include <cereal/types/polymorphic.hpp>

MeshGeo::MeshGeo() {}
MeshGeo::MeshGeo( const std::string& v ) : ResourceBase(v) {}
osg::ref_ptr<osg::Node> MeshGeo::buildGraph( osg::ref_ptr<osg::Node> node ) {
  osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( resourceBase + resourcePath );
  return model;
}
CEREAL_REGISTER_TYPE( MeshGeo );


