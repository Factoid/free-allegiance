#include <osgViewer/Viewer>
#include <osgGA/UFOManipulator>
#include <igc/pch.h>
#include <igc/missionIGC.h>
#include <iostream>

#include "ClientIgcSite"
#include "model/ResourceManager"
#include "Geo"
#include "formatters"

using namespace fa;

osg::ref_ptr<osg::Group> createCluster( IclusterIGC* cluster )
{
  auto root = osg::ref_ptr<osg::Group>( new osg::Group );
  for( auto m : *cluster->GetModels() )
  {
    ThingSite* tSite = m->GetThingSite();
    Geo* geo = tSite->GetGeo();
    std::cout << "Getting Model " << m->GetName() << " " << tSite << " " << geo << "\n";
    if( geo->model )
    {
      root->addChild( geo->model );
    }
  }
  Vector lDir = cluster->GetLightDirection();
  Color lCol = cluster->GetLightColor();
//  lCol.SetGreen(0.0f);
//  lCol.SetBlue(0.0f);
//  lCol.SetRed(0.0f);
  osg::ref_ptr<osg::StateSet> sset( new osg::StateSet );
  root->setStateSet(sset);
  osg::ref_ptr<osg::Light> l( new osg::Light );
  l->setLightNum(1);
  l->setAmbient( osg::Vec4( lCol.R(), lCol.G(), lCol.B(), lCol.A() ) );
  l->setDiffuse( osg::Vec4( lCol.R(), lCol.G(), lCol.B(), lCol.A() ) );
  l->setDirection( osg::Vec3( lDir.X(), lDir.Y(), lDir.Z() ) );
  l->setConstantAttenuation( 1.0f );
  l->setLinearAttenuation( 0.0f );
  l->setQuadraticAttenuation( 0.0f );
  osg::ref_ptr<osg::LightSource> ls( new osg::LightSource );
  ls->setLight(l);
  ls->setLocalStateSetModes(osg::StateAttribute::ON);
  ls->setStateSetModes(*sset, osg::StateAttribute::ON);
  std::cout << "Light direction is " << lDir << " color is " << lCol << "\n";
  return root;
}

int main( int argc, char** argv )
{
  try
  {
    fa::ResourceManager::setPathBase("decompiled/");

    UTL::SetArtPath( "Artwork/" );
    ClientIgcSite clientIgc;
    CmissionIGC mission;
    Time start = Clock::now();

    mission.Initialize( start, &clientIgc );
    LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
    
    MissionParams mp;
    mp.strGameName = std::string("Flight Test");
    mp.nTeams = 2;
    mp.rgCivID = { 18, 18 };
    mission.SetMissionParams( &mp ); 

    const char sideNames[c_cSidesMax][c_cbSideName] = { "Team 1", "Team 2" };
    mission.UpdateSides( start, mission.GetMissionParams(), sideNames );   
    mission.GenerateMission( start, mission.GetMissionParams(), nullptr, nullptr );
    mission.SetStartTime( start );
    mission.EnterGame();

    osg::ref_ptr<osg::Group> root = createCluster( mission.GetSide(0)->GetStation(0)->GetCluster() ); 

    osgViewer::Viewer viewer;
    osg::ref_ptr<osgGA::UFOManipulator> cameraManip( new osgGA::UFOManipulator );
    cameraManip->setForwardSpeed( cameraManip->getForwardSpeed() * 10000.0f );
    cameraManip->setSideSpeed( cameraManip->getSideSpeed() * 10000.0f );
    cameraManip->setRotationSpeed( cameraManip->getRotationSpeed() * 1000.0f );

    double fovy, ar, zNear, zFar;
    viewer.getCamera()->getProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    fovy *= 2.0f;
    zFar *= 100.0f;
    viewer.getCamera()->setProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    viewer.getCamera()->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );
//    viewer.setCameraManipulator( cameraManip );
    viewer.setSceneData(root);
    viewer.run();
  } catch( std::exception& e )
  {
    std::cerr << "exception caught " << e.what() << "\n";
    return 1;
  }
    
  return 0; 
}
