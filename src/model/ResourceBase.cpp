#include "ResourceBase"
#include <cereal/types/polymorphic.hpp>

namespace fa
{
  ResourceBase::ResourceBase( const std::string& path ) : resourcePath(path) {}
  ResourceBase::ResourceBase() {}
  ResourceBase::~ResourceBase() {}
  void ResourceBase::setResourceBase( const std::string& b )
  {
    resourceBase = b;
  }

  std::string ResourceBase::resourceBase;

}
CEREAL_REGISTER_TYPE_WITH_NAME(fa::ResourceBase,"ResourceBase");
//CEREAL_REGISTER_TYPE( ResourceBase );
