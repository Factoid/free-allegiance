#include "RenderManager"

namespace fa {
  RenderManager::RenderManager() : root( new osg::Group )
  {
  }

  void RenderManager::addRenderable( std::shared_ptr<Renderable> r )
  {
    root->addChild( r->getRoot() );
  }

  void RenderManager::setViewer( osgViewer::Viewer* viewer )
  {
    viewer->setSceneData( root );
  }
}
