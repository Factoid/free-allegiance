class ResourceManager
{
public:
  enum PrimativeType { PRIMATIVE_QUAD };
  static std::shared_ptr<ResourceManager> instance() {
    _instance = std::shared_ptr<ResourceManager>(new ResourceManager);
    return _instance;
  }

  osg::ref_ptr<osg::Drawable> getPrimative( ResourceManager::PrimativeType type )
  {
    return primatives[type];
  }

private:
  static std::shared_ptr<ResourceManager> _instance;
  std::map< PrimativeType, osg::ref_ptr<osg::Geometry> > primatives;

  osg::ref_ptr<osg::Geometry> createQuad()
  {
    osg::ref_ptr<osg::Geometry> quad( new osg::Geometry );
    osg::ref_ptr<osg::Vec3Array> quadVerts(new osg::Vec3Array);
    quadVerts->push_back( osg::Vec3(-0.5f, 0.0f, 0.5f) );
    quadVerts->push_back( osg::Vec3( 0.5f, 0.0f, 0.5f) );
    quadVerts->push_back( osg::Vec3( 0.5f, 0.0f,-0.5f) );
    quadVerts->push_back( osg::Vec3(-0.5f, 0.0f,-0.5f) );
    quad->setVertexArray(quadVerts);

    osg::ref_ptr<osg::DrawElementsUInt> quadIndices( new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0) );
    quadIndices->push_back(3);
    quadIndices->push_back(2);
    quadIndices->push_back(1);
    quadIndices->push_back(0);
    quad->addPrimitiveSet(quadIndices);

//    osg::ref_ptr<osg::Vec4Array> colors( new osg::Vec4Array );
//    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
//    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
//    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
//    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
//    quad->setColorArray(colors);
//    quad->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    return quad;
  }

  ResourceManager() {
    primatives[ PRIMATIVE_QUAD ] = createQuad();
  }
};
std::shared_ptr<ResourceManager> ResourceManager::_instance;


