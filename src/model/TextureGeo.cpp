#include "TextureGeo"

#include <cereal/types/polymorphic.hpp>

TextureGeo::TextureGeo() {}
TextureGeo::TextureGeo( Geo* geo, const std::string& img ) : geo( geo ), image( img ) {}

osg::ref_ptr<osg::Node> TextureGeo::buildGraph( osg::ref_ptr<osg::Node> node ) {
  osg::ref_ptr<osg::Node> model = geo->buildGraph(0);
  image.buildGraph( model );
  return model;
}
CEREAL_REGISTER_TYPE( TextureGeo );


