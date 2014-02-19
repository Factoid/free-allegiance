class Image : public ResourceBase
{
public:
  Image() {}
  Image( const std::string& v ) : ResourceBase(v) {}
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node )
  {
    osg::ref_ptr<osg::Image> img = osgDB::readImageFile( resourceBase + resourcePath );
    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
    node->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
    return node;
  }
};
