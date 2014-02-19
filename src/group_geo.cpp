class GroupGeo : public Geo
{
private:
  std::vector< std::unique_ptr<Geo> > geo_list;
 
public:
  GroupGeo() {}
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> root) {
    osg::ref_ptr<osg::Group> grp( new osg::Group );
    grp->addChild( geo_list[0]->buildGraph(0) );
    for( int i = 0; i < geo_list.size(); ++i )
    {
      grp->addChild(geo_list[i]->buildGraph(0));
    }
    return grp;
  }

  void add( Geo* geo )
  {
    geo_list.push_back(std::unique_ptr<Geo>(geo));
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(geo_list) );
  }
};
CEREAL_REGISTER_TYPE( GroupGeo );


