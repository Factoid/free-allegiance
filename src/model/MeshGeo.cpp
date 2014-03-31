#include "MeshGeo"
#include "ResourceManager"

#include <osgDB/ReadFile>
#include <cereal/types/polymorphic.hpp>
#include <osg/Material>

namespace fa
{
  MeshGeo::MeshGeo() {}
  MeshGeo::MeshGeo( const std::string& v ) : ResourceBase(v) {}

  osg::ref_ptr<osg::Node> MeshGeo::buildGraph( osg::ref_ptr<osg::Node> node ) {
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( ResourceManager::getPathBase() + resourcePath );
    osg::ref_ptr<osg::Material> mat( new osg::Material );
    mat->setDiffuse( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
    mat->setSpecular( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
    mat->setAmbient( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
    mat->setEmission( osg::Material::FRONT, osg::Vec4(1,1,1,1) );
    model->getOrCreateStateSet()->setAttribute(mat);

    return model;
  }
}
CEREAL_REGISTER_TYPE_WITH_NAME( fa::MeshGeo, "MeshGeo" );
//CEREAL_REGISTER_TYPE( MeshGeo );
