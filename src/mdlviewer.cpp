/*
#include <osg/Node>
#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <string>
#include <iostream>
#include <typeinfo>
#include <sstream>

//#include <boost/property_tree/xml_parser.hpp>
//#include <boost/property_tree/ptree.hpp>

#include <memory>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
*/

#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include "model/ModelDefinition"
#include "model/Color"
#include "model/LODGeo"
#include "model/ResourceBase"

/*
void writeTestModelDefinition()
{
  ModelDefinition d;
  d.frame = 76.0f;

  d.lights.push_back( Light(Color(1.0f,0.0f,0.0f),Vector3(0.5f,2.3f,0.23f),0.5f,1.25f,0.1f,0.0f,0.0f) );
  d.lights.push_back( Light(Color(0.0f,1.0f,0.0f),Vector3(0.5f,-2.3f,0.23f),0.5f,1.25f,0.1f,0.0f,0.0f) );
  d.lights.push_back( Light(Color(1.0f,1.0f,1.0f),Vector3(0.5f,0.5f,1.2f),-2.4f,1.25f,0.1f,0.0f,0.0f) );
  d.lights.push_back( Light(Color(1.0f,1.0f,1.0f),Vector3(0.5f,-0.5f,1.2f),-2.4f,1.25f,0.1f,0.0f,0.0f) );
  d.lights.push_back( Light(Color(1.0f,1.0f,1.0f),Vector3(0.5f,0.0f,-0.81f),2.5f,1.25f,0.1f,0.0f,0.0f) );
  d.frames.push_back( Frame("lthrust", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("rthrust", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("fwepmnt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("twepmnt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("lwepmnt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("rwepemt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("lsmoke", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("rsmoke", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("trail", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("rwepatt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("lwepatt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );
  d.frames.push_back( Frame("fwepemt", Vector3(1.0f,-0.5f,0.058f), Vector3(1.0f,0.0f,0.0f), Vector3(-1.0f,0.0f,0.0f) ) );

  LODGeo* g = new LODGeo();
  GroupGeo* rgl = new GroupGeo();
  MeshGeo* rmg = new MeshGeo("fig02_5.obj");
  rgl->add( new TextureGeo(rmg,"fig02.png") );
  rgl->add( new MeshGeo("fig02_4.obj") );
  g->rootGeo = std::unique_ptr<Geo>(rgl);
  
  MeshGeo* fig02_0 = new MeshGeo("fig02_0.obj");
  g->lodGeo.insert( std::make_pair(8,std::shared_ptr<Geo>(new TextureGeo(fig02_0,"fig02.png"))));
  g->lodGeo.insert( std::make_pair(32,std::shared_ptr<Geo>( new MeshGeo("fig02_1.obj"))));

  GroupGeo* gl = new GroupGeo();
  MeshGeo* fig02_3 = new MeshGeo("fig02_3.obj");
  gl->add( new TextureGeo( fig02_3, "fig02.png" ) );
  gl->add( new MeshGeo( "fig02_2.obj" ) );
  g->lodGeo.insert( std::make_pair(64,std::shared_ptr<Geo>(gl)));
  
  d.object = std::unique_ptr<Geo>(g);
  
  saveModelDefinition( d, "test.json" );
}
*/

int main( int argc, char** argv )
{
  if( argc < 2 )
  {
    std::cout << "Need a file\n";
    return 0;
  }
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );
  std::string base("tools/decompiled/");
  std::string name(argv[1]);
  ResourceBase::setResourceBase(base); 

//  writeTestModelDefinition();

  ModelDefinition d2;
  d2.load( base + name + ".json" );
  d2.buildGraph( root.get() );

  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();
  
  return 0;
}
