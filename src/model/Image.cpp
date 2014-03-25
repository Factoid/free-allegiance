#include "Image"
#include "ResourceManager"
#include <osg/Image>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

namespace fa
{
  Image::Image() {}
  Image::Image( const std::string& v ) : ResourceBase(v) {}
  osg::ref_ptr<osg::Node> Image::buildGraph( osg::ref_ptr<osg::Node> node )
  {
    osg::ref_ptr<osg::Image> img = ResourceManager::instance()->getImage( resourceBase + resourcePath );
    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
    node->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex, osg::StateAttribute::ON );
    return node;
  }
}
