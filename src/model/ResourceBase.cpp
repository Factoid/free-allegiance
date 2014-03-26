#include "ResourceBase"
#include <cereal/types/polymorphic.hpp>

namespace fa
{
  ResourceBase::ResourceBase( const std::string& path ) : resourcePath(path) {}
  ResourceBase::ResourceBase() {}
  ResourceBase::~ResourceBase() {}
}
CEREAL_REGISTER_TYPE_WITH_NAME(fa::ResourceBase,"ResourceBase");
//CEREAL_REGISTER_TYPE( ResourceBase );
