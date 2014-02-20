#include "ModelDefinition"
#include <osg/Group>
#include <algorithm> 
#include <iostream>
#include <fstream>

ModelDefinition::ModelDefinition()
{
}

void ModelDefinition::buildGraph( osg::ref_ptr<osg::Node> root )
{
  root->asGroup()->addChild(object->buildGraph(0));
  std::for_each( lights.begin(), lights.end(), [&](Light l) { root->asGroup()->addChild(l.buildGraph(0)); });
}

void ModelDefinition::save( const std::string& path ) const
{
  std::ofstream os( path.c_str() );
  cereal::JSONOutputArchive archive(os);
  archive(*this);
}

void ModelDefinition::load( const std::string& path )
{
  std::ifstream ifs( path.c_str() );
  cereal::JSONInputArchive archive(ifs);
  archive(*this);
}
