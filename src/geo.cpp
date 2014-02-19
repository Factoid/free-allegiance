class Geo
{
public:
  Geo() {}
  virtual ~Geo() {}
  virtual osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {}
  template <class Archive> void serialize( Archive& ar ) {}
};
CEREAL_REGISTER_TYPE( Geo );


