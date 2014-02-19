class LODGeo : public Geo
{
public:
  std::unique_ptr<Geo> rootGeo;
  std::map< float, std::shared_ptr<Geo> > lodGeo;

public:
  LODGeo() {}
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {
    std::vector<float> keys;
    std::transform( lodGeo.begin(), lodGeo.end(), std::back_inserter( keys ), [](std::pair<float,std::shared_ptr<Geo> > p ){ return p.first; });
    std::sort( keys.begin(), keys.end() );
    keys.push_back(300);

    osg::ref_ptr<osg::LOD> lod(new osg::LOD());
    osg::ref_ptr<osg::Node> root = rootGeo->buildGraph(0);
    lod->addChild(root.get(),0,keys[0]);
//    lod->addChild(root,0,3000);
//    return lod;
  
    for( int i = 0; i < keys.size()-1; ++i )
    {
      osg::ref_ptr<osg::Node> node = lodGeo[keys[i]]->buildGraph(0);
      lod->addChild(node,keys[i],keys[i+1]);
    }

    return lod; 
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(rootGeo) );
    ar( CEREAL_NVP(lodGeo) );
  }
};
CEREAL_REGISTER_TYPE( LODGeo );


