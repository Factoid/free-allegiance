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
  Geo() {}
  virtual ~Geo() {}
  virtual osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {}
  template <class Archive> void serialize( Archive& ar ) {}
};
CEREAL_REGISTER_TYPE( Geo );

class ResourceBase
{
public:
  ResourceBase( const std::string& path ) : resourcePath(path) {}
  ResourceBase() {}
  virtual ~ResourceBase() {}

  static void setResourceBase( const std::string& b )
  {
    resourceBase = b;
  }
  
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(resourcePath) );
  }

protected:
  static std::string resourceBase;
  std::string resourcePath;
};

std::string ResourceBase::resourceBase;

CEREAL_REGISTER_TYPE(ResourceBase);

class MeshGeo : public Geo, public ResourceBase
{
public:
  MeshGeo() {}
  MeshGeo( const std::string& v ) : ResourceBase(v) {}
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( resourceBase + resourcePath );
    return model;
  }
  template <class Archive> void serialize( Archive& ar ) {
    ResourceBase::serialize(ar);
    Geo::serialize(ar);
  }
};
CEREAL_REGISTER_TYPE( MeshGeo );

class Image : public ResourceBase
{
public:
  Image() {}
  Image( const std::string& v ) : ResourceBase(v) {}
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node )
  {
    osg::ref_ptr<osg::Image> img = osgDB::readImageFile( resourceBase + resourcePath );
    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
    node->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
    return node;
  }
};

class TextureGeo : public Geo
{
public:
  std::unique_ptr<Geo> geo;
  Image image;
   
public:
  TextureGeo() {}
  TextureGeo( Geo* geo, const std::string& img ) : geo( geo ), image( img ) {}

  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {
    osg::ref_ptr<osg::Node> model = geo->buildGraph(0);
    image.buildGraph( model );
    return model;
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(geo) );
    ar( CEREAL_NVP(image) );
  }
};
CEREAL_REGISTER_TYPE( TextureGeo );

class Color
{
private:
  float r,g,b,a;
public:
  Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
  Color( float r, float g, float b, float a = 1.0f ) : r(r), g(g), b(b), a(a) {}

  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(r) );
    ar( CEREAL_NVP(g) );
    ar( CEREAL_NVP(b) );
    ar( CEREAL_NVP(a) );
  }
};

class Vector3
{
private:
  float x,y,z;
public:
  Vector3() : x(0), y(0), z(0) {}
  Vector3( float x, float y, float z ) : x(x), y(y), z(z) { }

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
  Vector3 position;

public:
  Light() : color( Color(1.0f,1.0f,1.0f) ), position( Vector3(0,0,0) ), hold(1.0f), period(1.0f), phase(0.0f), ramp_up(0.5f), ramp_down(0.5f) {}
  Light( const Color& c, const Vector3& pos, float period, float phase, float hold, float ramp_up, float ramp_down ) : color(c), position(pos), period(period), phase(phase), hold(hold), ramp_up(ramp_up), ramp_down(ramp_down) {}

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
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node ) {
    std::vector<float> keys;
    std::transform( lodGeo.begin(), lodGeo.end(), std::back_inserter( keys ), [](std::pair<float,std::shared_ptr<Geo> > p ){ return p.first; });
    std::sort( keys.begin(), keys.end() );
    keys.push_back(300);

    osg::ref_ptr<osg::LOD> lod(new osg::LOD());
    osg::ref_ptr<osg::Node> root = rootGeo->buildGraph(0);
    lod->addChild(root.get(),0,keys[0]);

    for( int i = 0; i < keys.size()-1; ++i )
    {
      osg::ref_ptr<osg::Node> node = lodGeo[keys[i]]->buildGraph(0);
      lod->addChild(node,keys[i],keys[i+1]);
    }

    return lod; 
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(rootGeo) );
    ar( CEREAL_NVP(lodGeo) );
  }
};
CEREAL_REGISTER_TYPE( LODGeo );

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
  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> root) {
    osg::ref_ptr<osg::Group> grp( new osg::Group );
    grp->addChild( geo_list[0]->buildGraph(0) );
    for( int i = 0; i < geo_list.size(); ++i )
    {
      grp->addChild(geo_list[i]->buildGraph(0));
    }
    return grp;
  }

  void add( Geo* geo )
  {
    geo_list.push_back(std::unique_ptr<Geo>(geo));
  }
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(geo_list) );
  }
};
CEREAL_REGISTER_TYPE( GroupGeo );

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
  void buildGraph( osg::Node* root )
  {
    root->asGroup()->addChild(object->buildGraph(0));
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

void buildGraphFromModelDefinition( const ModelDefinition& def, osg::Group* root )
{
/*  
*/
}

osg::ref_ptr<osg::Node> getModel()
{
  osg::ref_ptr<osg::Node> model = osgDB::readNodeFile( "tools/decompiled/fig02_5.obj" );
  return model;
}

osg::ref_ptr<osg::Node> applyImage( osg::ref_ptr<osg::Node> model )
{
  osg::ref_ptr<osg::Image> img = osgDB::readImageFile( "tools/decompiled/fig02bmp.png" );
  osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D(img);
  model->getOrCreateStateSet()->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
  return model;
}

int main( int argc, char** argv )
{
  osgViewer::Viewer viewer;

  osg::ref_ptr<osg::Group> root( new osg::Group );
  std::string base("tools/decompiled/");
  std::string name(argv[1]);
  std::string offset(argv[2]);
  ResourceBase::setResourceBase(base); 

//  writeTestModelDefinition();
 
  ModelDefinition d2;
  loadModelDefinition( d2, base + name + ".json" );
  d2.buildGraph( root.get() );

//  root->addChild( applyImage( getModel() ) );

//  MeshGeo m( "fig02_5.obj" );
//  Image img( "fig02bmp.png" );
//  root->addChild( Image("fig02bmp.png").buildGraph( MeshGeo("fig02_5.obj").buildGraph(0) ) );

  osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
  viewer.setSceneData(root.get());
  viewer.run();
  
  return 0;
}
