#include <osgViewer/Viewer>
#include <osgGA/UFOManipulator>
#include "model/ModelDefinition"
#include "model/Color"
#include "model/LODGeo"
#include "model/ResourceManager"
#include <osg/MatrixTransform>
#include <osg/DisplaySettings>

using namespace fa;

class EventHandler : public osgGA::GUIEventHandler
{
  public:
    bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv )
    {
      switch( ea.getEventType() )
      {
        case osgGA::GUIEventAdapter::EventType::KEYDOWN:
        switch( ea.getUnmodifiedKey() )
        {
          case osgGA::GUIEventAdapter::KeySymbol::KEY_Q:
          {
            osg::Texture::Extensions* ext = osg::Texture::getExtensions(0,true);
            ext->setMaxTextureSize(64);
            ResourceManager::instance()->reloadImages();
          }
            break;
          case osgGA::GUIEventAdapter::KeySymbol::KEY_W:
          {
            osg::Texture::Extensions* ext = osg::Texture::getExtensions(0,true);
            ext->setMaxTextureSize(128);
            ResourceManager::instance()->reloadImages();
          }
            break;
          case osgGA::GUIEventAdapter::KeySymbol::KEY_E:
          {
            osg::Texture::Extensions* ext = osg::Texture::getExtensions(0,true);
            ext->setMaxTextureSize(256);
            ResourceManager::instance()->reloadImages();
          }
            break;
          case osgGA::GUIEventAdapter::KeySymbol::KEY_R:
          {
            osg::Texture::Extensions* ext = osg::Texture::getExtensions(0,true);
            ext->setMaxTextureSize(512);
            ResourceManager::instance()->reloadImages();
          }
            break;
          case osgGA::GUIEventAdapter::KeySymbol::KEY_T:
          {
            osg::Texture::Extensions* ext = osg::Texture::getExtensions(0,true);
            ext->setMaxTextureSize(1024);
            ResourceManager::instance()->reloadImages();
          }
            break;
          case osgGA::GUIEventAdapter::KeySymbol::KEY_Y:
          {
            osg::Texture::Extensions* ext = osg::Texture::getExtensions(0,true);
            ext->setMaxTextureSize(2048);
            ResourceManager::instance()->reloadImages();
          }
            break;
        }
        break;
        return true;
    }
    return false;
  }
};

int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    std::cout << "Need a file\n";
    return 0;
  }
  EventHandler* handler = new EventHandler;
  osgViewer::Viewer viewer;
  viewer.addEventHandler(handler);
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

  osg::ref_ptr<osg::StateSet> sset( new osg::StateSet );
  osg::ref_ptr<osg::Light> l( new osg::Light );
  osg::Vec4 c( 1.0f, 0.0f, 0.0f, 1.0f );

  l->setLightNum(0);
  l->setAmbient( c );
  l->setDiffuse( c );
  l->setSpecular( osg::Vec4(0,0,0,0) );
  l->setDirection( osg::Vec3( 1.0f, 0.0f, 0.0f ) );
  l->setConstantAttenuation( 1.0f );
  l->setLinearAttenuation( 0.0f );
  l->setQuadraticAttenuation( 0.0f );
  osg::ref_ptr<osg::LightSource> ls( new osg::LightSource );
  ls->setLight(l);
  ls->setLocalStateSetModes(osg::StateAttribute::ON);
  ls->setStateSetModes(*sset, osg::StateAttribute::ON);
  mdl->setStateSet(sset);

  viewer.setSceneData(mdl);
  viewer.run();
  
  return 0;
}
