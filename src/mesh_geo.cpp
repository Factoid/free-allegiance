class MeshGeo : public Geo, public ResourceBase
{
public:
  MeshGeo() {}
  MeshGeo( const std::string& v ) : ResourceBase(v) {}
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( resourceBase + resourcePath );
    return model;
  }
  template <class Archive> void serialize( Archive& ar ) {
    ResourceBase::serialize(ar);
    Geo::serialize(ar);
  }
};
CEREAL_REGISTER_TYPE( MeshGeo );


