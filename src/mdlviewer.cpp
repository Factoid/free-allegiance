#include <osg/Node>
#include <osg/Group>
#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <string>
#include <iostream>
#include <typeinfo>

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
  virtual std::string getResources() { return ""; }
  template <class Archive> void serialize( Archive& ar ) {}
};

class ModelGeo : public Geo
{
public:
  ModelGeo() {}
  ModelGeo( const std::string& v ) : resourcePath(v) {}
  template <class Archive> void serialize( Archive& ar )
  {
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
    ar( CEREAL_NVP(resourcePath) );
  }
};

class TextureGeo : public Geo
{
public:
  ModelGeo geo;
  Image image;
   
public:
  TextureGeo()
  {
  }

  std::string getResources() { return geo.resourcePath + " : " + image.resourcePath; }

  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(geo) );
    ar( CEREAL_NVP(image) );
  }
};

class Color
{
public:
  float r,g,b,a;

  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(r) );
    ar( CEREAL_NVP(g) );
    ar( CEREAL_NVP(b) );
    ar( CEREAL_NVP(a) );
  }
};

class Vector
{
public:
  float x,y,z;

  template <class Archive> void serialize( Archive& ar )
  {
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
  Vector position;

public:
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(hold) );
    ar( CEREAL_NVP(period) );
    ar( CEREAL_NVP(phase) );
    ar( CEREAL_NVP(ramp_up) );
    ar( CEREAL_NVP(ramp_down) );
    ar( CEREAL_NVP(color) );
    ar( CEREAL_NVP(position) );
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
    ar( CEREAL_NVP(rootGeo) );
    ar( CEREAL_NVP(lodGeo) );
  }
  std::string getResources() { return rootGeo->getResources(); }
};

class Frame
{
public:
  template <class Archive> void serialize( Archive& ar )
  {
  }
};

class ModelDefinition
{
public:
  float frame;
  std::vector<Frame> frames;
  std::shared_ptr<Geo> object;
  std::vector<Light> lights;

public:
  ModelDefinition()
  {
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(frame) );
    ar( CEREAL_NVP(frames) );
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

CEREAL_REGISTER_TYPE( Geo );
CEREAL_REGISTER_TYPE( ModelGeo );
CEREAL_REGISTER_TYPE( TextureGeo );
CEREAL_REGISTER_TYPE( LODGeo );

int main( int argc, char** argv )
{
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );
  std::string base("tools/decompiled/");
  std::string name(argv[1]);
  std::string offset(argv[2]);
  ModelDefinition d;
  d.lights.push_back( Light() );
  d.frames.push_back( Frame() );
  LODGeo* g = new LODGeo();
  TextureGeo* tg = new TextureGeo();
  tg->image = Image("fig02.png");
  tg->geo = ModelGeo("fig02_5.obj");
  g->rootGeo = std::unique_ptr<Geo>(tg);
  g->lodGeo[8] = std::shared_ptr<Geo>(new ModelGeo("fig02_1.obj"));
  g->lodGeo[16] = std::shared_ptr<Geo>( new ModelGeo("fig02_3.obj"));
  d.object = std::unique_ptr<Geo>(g);
  
  saveModelDefinition( d, "test.json" );
  ModelDefinition d2;
  if(d2.object != 0)
  {
    std::cout << "Mdl2 resources are " << typeid(*d2.object).name() << " value " << d2.object->getResources() << "\n";
  } else
  {
    std::cout << "Mdl2 resources are null\n";
  }
  loadModelDefinition( d2, "test2.json" );
  std::cout << "Mdl2 resources are " << typeid(*d2.object).name() << " value " << d2.object->getResources() << "\n";

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
