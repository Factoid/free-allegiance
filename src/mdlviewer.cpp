#include <osg/Node>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
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

class ModelObject
{
};

class Geo
{
public:
  virtual ~Geo() {}
  template <class Archive> void serialize( Archive& ar ) 
  {
    std::cout << "Loading Geo\n";
  }
};

class MeshGeo : public Geo
{
public:
  MeshGeo() {}
  MeshGeo( const std::string& v ) : resourcePath(v) {}
  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "MeshGeo\n";
    ar( CEREAL_NVP(resourcePath) );
  }

public:
  std::string resourcePath;
};

class Image
{
public:
  std::string resourcePath;
public:
  Image() {}
  Image( const std::string& v ) : resourcePath(v) {}
  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "Image\n";
    ar( CEREAL_NVP(resourcePath) );
  }
};

class TextureGeo : public Geo
{

public:
  std::unique_ptr<Geo> geo;
  Image image;
   
public:
  TextureGeo()
  {
  }
  TextureGeo( Geo* geo, const std::string& img ) : geo( geo ), image( img )
  {
  }

  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "TextureGeo\n";
    ar( CEREAL_NVP(geo) );
    std::cout << "Image Data\n";
    ar( CEREAL_NVP(image) );
  }
};

class Color
{
public:
  float r,g,b,a;
  Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
  Color( float r, float g, float b, float a = 1.0f ) : r(r), g(g), b(b), a(a) {}

  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "Color\n";
    ar( CEREAL_NVP(r) );
    ar( CEREAL_NVP(g) );
    ar( CEREAL_NVP(b) );
    ar( CEREAL_NVP(a) );
  }
};

class Vector3
{
public:
  float x,y,z;
  Vector3() : x(0), y(0), z(0) {}
  Vector3( float x, float y, float z ) : x(x), y(y), z(z) { }

  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "Vector3\n";
    ar( CEREAL_NVP(x) );
    ar( CEREAL_NVP(y) );
    ar( CEREAL_NVP(z) );
  }
};

class Light
{
private:
  float hold;
  float period;
  float phase;
  float ramp_up;
  float ramp_down;
  Color color;
  Vector3 position;

public:
  Light() : color( Color(1.0f,1.0f,1.0f) ), position( Vector3(0,0,0) ), hold(1.0f), period(1.0f), phase(0.0f), ramp_up(0.5f), ramp_down(0.5f) {}
  Light( const Color& c, const Vector3& pos, float period, float phase, float hold, float ramp_up, float ramp_down ) : color(c), position(pos), period(period), phase(phase), hold(hold), ramp_up(ramp_up), ramp_down(ramp_down) {}

  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "Light\n";
    ar( CEREAL_NVP(hold) );
    ar( CEREAL_NVP(period) );
    ar( CEREAL_NVP(phase) );
    ar( CEREAL_NVP(ramp_up) );
    ar( CEREAL_NVP(ramp_down) );
    ar( CEREAL_NVP(color) );
    ar( CEREAL_NVP(position) );
  }

  std::string toString() const
  {
    std::ostringstream os;
    os << "  Hold " << hold << "\n  Period " << period << "\n  Phase " << phase << "\n  Ramp-Up " << ramp_up << "\n  Ramp-Down " << ramp_down << "\n";
    return os.str();
  }
};

class LODGeo : public Geo
{
public:
  std::unique_ptr<Geo> rootGeo;
  std::map< float, std::shared_ptr<Geo> > lodGeo;

public:
  LODGeo() {}
  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "LODGeo\n";
    ar( CEREAL_NVP(rootGeo) );
    std::cout << "Loading LOD array\n";
    ar( CEREAL_NVP(lodGeo) );
  }
};

class Frame
{
private:
  std::string name;
  Vector3 position;
  Vector3 up;
  Vector3 forward;

public:
  Frame() {}
  Frame( const std::string& name, const Vector3& pos, const Vector3& up, const Vector3& forward ) : name(name), position(pos), up(up), forward(forward) {} 
  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "Frame\n";
    ar(CEREAL_NVP(name));
    ar(CEREAL_NVP(position));
    ar(CEREAL_NVP(up));
    ar(CEREAL_NVP(forward));
  }

};

class GroupGeo : public Geo
{
private:
  std::vector< std::unique_ptr<Geo> > geo_list;
 
public:
  GroupGeo() {}
  void add( Geo* geo )
  {
    geo_list.push_back(std::unique_ptr<Geo>(geo));
  }

  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "GroupGeo\n";
    ar( CEREAL_NVP(geo_list) );
  }
};

class ModelDefinition
{
public:
  float frame;
  std::vector<Frame> frames;
  std::unique_ptr<Geo> object;
  std::vector<Light> lights;

public:
  ModelDefinition()
  {
  }
  template <class Archive> void serialize( Archive& ar )
  {
    std::cout << "ModelDefinition\n";
    ar( CEREAL_NVP(frame) );
    ar( CEREAL_NVP(frames) );
    std::cout << "Loading object\n";
    ar( CEREAL_NVP(object) );
    ar( CEREAL_NVP(lights) );
  }
};
/*
void loadModelDefinition( const std::string& filePath )
{
  using boost::property_tree::ptree;
  std::ifstream ifs( filePath.c_str() );
  ptree pt;
  read_xml( ifs, pt );
}
*/
void saveModelDefinition( ModelDefinition& obj, const std::string& path )
{
  std::ofstream os( path.c_str() );
  cereal::JSONOutputArchive archive(os);
  archive(obj);
}

void loadModelDefinition( ModelDefinition& obj, const std::string& path )
{
  std::ifstream ifs( path.c_str() );
  cereal::JSONInputArchive archive(ifs);
  archive(obj);
}

CEREAL_REGISTER_TYPE( MeshGeo );
CEREAL_REGISTER_TYPE( LODGeo );
CEREAL_REGISTER_TYPE( Geo );
CEREAL_REGISTER_TYPE( TextureGeo );
CEREAL_REGISTER_TYPE( GroupGeo );
/*
CEREAL_REGISTER_TYPE_WITH_NAME( MeshGeo, "MeshGeo" `);
CEREAL_REGISTER_TYPE_WITH_NAME( LODGeo, "LODGeo" );
CEREAL_REGISTER_TYPE_WITH_NAME( Geo, "Geo" );
CEREAL_REGISTER_TYPE_WITH_NAME( TextureGeo, "TextureGeo" );
*/

int main( int argc, char** argv )
{
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );
  std::string base("tools/decompiled/");
  std::string name(argv[1]);
  std::string offset(argv[2]);
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
  ModelDefinition d2;
  if(d2.object != 0)
  {
  } else
  {
    std::cout << "Mdl2 resources are null\n";
  }
  loadModelDefinition( d2, "test2.json" );
  std::cout << "Mdl2 lights are " << d2.lights.size() << " in number\n";
  for( int i = 0; i < d2.lights.size(); ++i )
  {
    std::cout << "Light " << i << " is " << d2.lights[i].toString() << "\n";
  }

  osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( base + name + "_" + offset + ".obj" );
  osg::ref_ptr<osg::Image> img = osgDB::readImageFile( base + name + "bmp.png" );
  osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
  model->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
  root->addChild(model.get());

  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();
  
  return 0;
}
