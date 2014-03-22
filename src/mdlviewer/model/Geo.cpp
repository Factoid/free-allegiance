#include "Geo"
#include <cereal/types/polymorphic.hpp>

Geo::Geo() {}
Geo::~Geo() {}
osg::ref_ptr<osg::Node> Geo::buildGraph( osg::ref_ptr<osg::Node> node ) {}
CEREAL_REGISTER_TYPE( Geo );


