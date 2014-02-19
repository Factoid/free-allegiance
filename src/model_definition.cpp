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
    std::for_each( lights.begin(), lights.end(), [&](Light l) { root->asGroup()->addChild(l.buildGraph(0)); });
  } 
  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(frame) );
    ar( CEREAL_NVP(frames) );
    ar( CEREAL_NVP(object) );
    ar( CEREAL_NVP(lights) );
  }

  void save( const std::string& path )
  {
    std::ofstream os( path.c_str() );
    cereal::JSONOutputArchive archive(os);
    archive(*this);
  }

  void load( const std::string& path )
  {
    std::ifstream ifs( path.c_str() );
    cereal::JSONInputArchive archive(ifs);
    archive(*this);
  }
};



