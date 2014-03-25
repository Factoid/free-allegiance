#include "Geo"
#include <cereal/types/polymorphic.hpp>

namespace fa
{
  Geo::Geo() {
#if TRACE
    std::cout << "Creating Geo()\n";
#endif
    }
  Geo::~Geo() {}
  osg::ref_ptr<osg::Node> Geo::buildGraph( osg::ref_ptr<osg::Node> node ) {}
}
CEREAL_REGISTER_TYPE_WITH_NAME( fa::Geo, "Geo" );
//CEREAL_REGISTER_TYPE( Geo );
