#include <osgViewer/Viewer>
#include <osgGA/UFOManipulator>
#include "model/ModelDefinition"
#include "model/Color"
#include "model/LODGeo"
#include "model/ResourceManager"
#include <osg/MatrixTransform>

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
  osg::ref_ptr<osg::Group> root( new osg::Group );
  osg::ref_ptr<osg::MatrixTransform> m1( new osg::MatrixTransform );
  osg::ref_ptr<osg::MatrixTransform> m2( new osg::MatrixTransform );
  osg::ref_ptr<osg::Group> mdl = ResourceManager::instance()->getModel( std::string(argv[1]) + ".json" );

//  root->addChild( m1 );
//  root->addChild( m2 );
//  m1->setMatrix( osg::Matrix::translate( 2, 0, 0 ) );
//  m1->addChild( mdl );
//  m2->setMatrix( osg::Matrix::translate( -3, 0, 0 ) );
//  m2->addChild( mdl );
  
  osg::ref_ptr<osgGA::CameraManipulator> cameraManip( new osgGA::UFOManipulator );
//  viewer.setCameraManipulator( cameraManip );
//  viewer.setSceneData(root);
  viewer.setSceneData(mdl);
  viewer.run();
  
  return 0;
}
