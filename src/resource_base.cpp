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


