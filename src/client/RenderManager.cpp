#include "RenderManager"

namespace fa {
  RenderManager::RenderManager() : root( new osg::Group )
  {
  }

  void RenderManager::setMission( ImissionIGC* mission )
  {
    this->mission = mission;
  }

  void RenderManager::addRenderable( std::shared_ptr<Renderable> r )
  {
    renderables.push_back( r );
    root->addChild( r->getRoot() );
  }

  void RenderManager::setViewer( osgViewer::Viewer* viewer )
  {
    this->viewer = viewer;
    viewer->setSceneData( root );
    double fovy, ar, zNear, zFar;
    viewer->getCamera()->getProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    zFar *= 100.0f;
    viewer->getCamera()->setProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    viewer->getCamera()->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );
    viewer->realize();
  }

  void RenderManager::setViewTarget( IshipIGC* viewTarget )
  {
    this->viewTargetID = viewTarget->GetObjectID();
  }

  void RenderManager::update( bool chaseView )
  {
    for( auto r : renderables )
    {
      r->update();
    }
    MyThingSite* viewTarget = dynamic_cast<MyThingSite*>(mission->GetShip(viewTargetID)->GetThingSite());
    if( viewTarget == nullptr ) return;

    osg::Vec3 e = viewTarget->GetCockpit();
    osg::Vec3 c = e + (viewTarget->GetForward()*10);
    if( chaseView )
    {
      e += (viewTarget->GetForward()*-20) + (viewTarget->GetUp()*5);
    }
    viewer->getCamera()->setViewMatrixAsLookAt( e, c, viewTarget->GetUp() );
    viewer->frame();
  }
}
