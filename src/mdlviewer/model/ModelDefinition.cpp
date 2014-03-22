#include "ModelDefinition"
#include <osg/Group>
#include <osg/MatrixTransform>
#include <algorithm> 
#include <iostream>
#include <fstream>

ModelDefinition::ModelDefinition()
{
}

void ModelDefinition::buildGraph( osg::ref_ptr<osg::Node> root )
{
  root->asGroup()->addChild(object->buildGraph(0));
  osg::ref_ptr<osg::MatrixTransform> tform( new osg::MatrixTransform );
  root->asGroup()->addChild(tform);
  tform->setMatrix( osg::Matrix::rotate( osg::inDegrees(90.0), osg::X_AXIS ) );
  std::for_each( lights.begin(), lights.end(), [&](Light l) { tform->addChild(l.buildGraph(0)); });
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
  if( !ifs.good() ) {
    std::cerr << "File " << path << " does not exist.\n";
    exit(1);
  }
  cereal::JSONInputArchive archive(ifs);
  archive(*this);
}
