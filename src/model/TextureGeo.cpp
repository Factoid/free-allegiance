class TextureGeo : public Geo
{
public:
  std::unique_ptr<Geo> geo;
  Image image;
   
public:
  TextureGeo() {}
  TextureGeo( Geo* geo, const std::string& img ) : geo( geo ), image( img ) {}

  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {
    osg::ref_ptr<osg::Node> model = geo->buildGraph(0);
    image.buildGraph( model );
    return model;
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(geo) );
    ar( CEREAL_NVP(image) );
  }
};
CEREAL_REGISTER_TYPE( TextureGeo );


