#include <GLFW/glfw3.h>

#ifdef ADRIAN_RENDERER
#include <boost/log/trivial.hpp>
#include "renderer/renderer.hpp"
#include "renderer/model.hpp"
#include "renderer/window.hpp"
#include "renderer/transform.hpp"
#include "renderer/renderable.hpp"
#else
#include <iostream>
#include <osg/Node>
#include <osg/Image>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osg/Texture2D>
#endif

void glErrorLogger( int code, const char* description )
{
//  BOOST_LOG_TRIVIAL(error) << "GLFW Error(" << code << ") " << description;
}

#ifdef ADRIAN_RENDERER
void runAdrianRenderer()
{
  BOOST_LOG_TRIVIAL(info) << "Starting free allegiance 0.0.1";
  glfwSetErrorCallback( glErrorLogger );

  Renderer& renderer = CreateOpenGLRenderer();
  Window& window = renderer.createWindow( 640, 480, "Allegiance 0.0.1" );

  Texture& texture = renderer.createTexture( "img.png" );
  Shader& shader = renderer.createShader( "default" );
  Model& model = renderer.createModel( "suzanne.obj" );

  Transform root;
  Renderable renderable;

  renderable.setModel( model );
  renderable.setShader( shader );
  renderable.setTexture( 0, texture );
  
  root.add( renderable );

  while( !window.isClosed() )
  {
    renderer.render( &root );
    window.swapBuffers();
    renderer.pollEvents();
  }
  BOOST_LOG_TRIVIAL(info) << "Stopping free allegiance 0.0.1";
}
#else
void runOSGRenderer()
{
  std::cout << "Starting free allgiance 0.0.1\n";
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );

  osg::ref_ptr<osg::ClearNode> cNode( new osg::ClearNode );
  cNode->setClearColor(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
//  root->addChild(cNode.get());

  osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("../data/suzanne.obj" );
  osg::ref_ptr<osg::Image> img = osgDB::readImageFile("../data/img.png" );
  osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
  model->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
  root->addChild(model.get());

  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();

  std::cout << "Stopping free allgiance 0.0.1\n";
}
#endif

int main( int argc, char** argv )
{
#ifdef ADRIAN_RENDERER
  runAdrianRenderer();
#else
  runOSGRenderer();
#endif
  return 0;
}


