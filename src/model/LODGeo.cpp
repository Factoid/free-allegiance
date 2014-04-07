#include "LODGeo"
#include <osg/LOD>
#include <algorithm>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>

namespace fa
{
  LODGeo::LODGeo() {
#if TRACE
    std::cout << "Creating LODGeo\n";
#endif
    }
  osg::ref_ptr<osg::Node> LODGeo::buildGraph( osg::ref_ptr<osg::Node> node ) {
    std::vector<float> keys;
//    std::transform( lodGeo.begin(), lodGeo.end(), std::back_inserter( keys ), [](std::pair<float,std::shared_ptr<Geo> > p ){ return p.first; });
    std::sort( keys.begin(), keys.end() );
    keys.push_back(3000);

    osg::ref_ptr<osg::LOD> lod(new osg::LOD());
    osg::ref_ptr<osg::Node> root = rootGeo->buildGraph(0);
    lod->addChild(root.get(),0,keys[0]);

    for( unsigned int i = 0; i < keys.size()-1; ++i )
    {
      osg::ref_ptr<osg::Node> node = lodGeo[keys[i]]->buildGraph(0);
      lod->addChild(node,keys[i],keys[i+1]);
    }

    return lod; 
  }

}
CEREAL_REGISTER_TYPE_WITH_NAME( fa::LODGeo, "LODGeo" );
//CEREAL_REGISTER_TYPE( LODGeo );
