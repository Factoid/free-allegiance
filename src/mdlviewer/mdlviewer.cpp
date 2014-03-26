#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include "model/ModelDefinition"
#include "model/Color"
#include "model/LODGeo"
#include "model/ResourceManager"

using namespace fa;

int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    std::cout << "Need a file\n";
    return 0;
  }
  osgViewer::Viewer viewer;

  ResourceManager::setPathBase("decompiled/");
  osg::ref_ptr<osg::Group> root = ResourceManager::instance()->getModel( argv[1] );
/*
  std::string base("decompiled/");
  std::string name(argv[1]);
  ResourceBase::setResourceBase(base); 

  ModelDefinition d2;
  d2.load( base + name + ".json" );
  d2.buildGraph( root.get() );
*/
  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();
  
  return 0;
}
