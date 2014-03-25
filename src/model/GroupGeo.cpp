#include "GroupGeo"
#include <cereal/types/polymorphic.hpp>
#include <osg/Group>

namespace fa
{
  GroupGeo::GroupGeo() {
#if TRACE
    std::cout << "Creating GroupGeo()\n";
#endif
    }
  osg::ref_ptr<osg::Node> GroupGeo::buildGraph( osg::ref_ptr<osg::Node> root) {
    osg::ref_ptr<osg::Group> grp( new osg::Group );
    grp->addChild( geo_list[0]->buildGraph(0) );
    for( int i = 0; i < geo_list.size(); ++i )
    {
      grp->addChild(geo_list[i]->buildGraph(0));
    }
    return grp;
  }

  void GroupGeo::add( Geo* geo )
  {
    geo_list.push_back(std::unique_ptr<Geo>(geo));
  }
}
CEREAL_REGISTER_TYPE_WITH_NAME( fa::GroupGeo, "GroupGeo" );
//CEREAL_REGISTER_TYPE( GroupGeo );
