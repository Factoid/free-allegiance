#include <osg/Node>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/ReadFile>

int main( int argc, char** argv )
{
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );
  osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("tools/int01.obj" );
  root->addChild(model.get());

  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();
  
  return 0;
}
