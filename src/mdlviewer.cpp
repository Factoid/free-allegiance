#include <osg/Node>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <string>

int main( int argc, char** argv )
{
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );
  std::string base("tools/decompiled/");
  std::string name(argv[1]);
  osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( base + name + ".obj" );
  osg::ref_ptr<osg::Image> img = osgDB::readImageFile( base + name + ".png" );
  osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
  model->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
  root->addChild(model.get());

  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();
  
  return 0;
}
