#include "igc/pch.h"
#include "MyClusterSite"
#include "Geo"

MyClusterSite::MyClusterSite( IclusterIGC* cluster ) : cluster(cluster), root(new osg::Group) {}
void MyClusterSite::AddThingSite( ThingSite* ts )
{
  //    std::cout << "Adding thing site " << ts << " to cluster " << this << "\n";
  Geo* geo = ts->GetGeo();
  if( geo->model )
  {
    root->addChild( geo->model );
  }
}

void MyClusterSite::DeleteThingSite( ThingSite* ts )
{
  //    std::cout << "Removing thing site " << ts << " from cluster " << this << "\n";
  Geo* geo = ts->GetGeo();
  if( geo->model )
  {
    root->removeChild( geo->model );
  }
}

void MyClusterSite::RemoveViewer( osgViewer::Viewer& viewer )
{
  viewer.getSceneData()->asGroup()->removeChild(root);
}

void MyClusterSite::SetViewer( osgViewer::Viewer& viewer )
{
  viewer.getSceneData()->asGroup()->addChild(root);
}
