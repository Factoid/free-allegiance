#include "ModelManager"
#include "model/ModelDefinition"

namespace fa
{

  void ModelManager::load( const std::string& str )
  {
    if( models.find(str) != models.end() ) return;

    osg::ref_ptr<osg::Group> root( new osg::Group );
    ModelDefinition mDef;
    std::cout << "Loading model" << str << "\n";
    mDef.load( "decompiled/" + str + ".json" );
    mDef.buildGraph(root.get());
    models[str] = root;
  }

  void TextureManager::load( const std::string& str )
  {
  }

}
