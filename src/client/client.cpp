#include <osgViewer/Viewer>
#include <osgGA/UFOManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/CameraManipulator>
#include <igc/pch.h>
#include <igc/missionIGC.h>
#include <iostream>

#include "ClientIgcSite"
#include "model/ResourceManager"
#include "Geo"
#include "formatters"
#include "MyThingSite"

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
  return root;
}

IshipIGC* launchShip( ImissionIGC& mission )
{
  IsideIGC* side0 = mission.GetSide(0);
  DataShipIGC shipData;
  shipData.shipID = mission.GenerateNewShipID();
  shipData.hullID = 210; // The Factoid scout
  shipData.sideID = side0->GetObjectID();
  shipData.nKills = 0;
  shipData.nDeaths = 0;
  shipData.pilotType = c_ptPlayer;
  shipData.nEjections = 0;
  strcpy(shipData.name,"Factoid");
  shipData.nParts = 0;
  shipData.baseObjectID = NA;

  IshipIGC* ship = (IshipIGC*)mission.CreateObject(mission.GetLastUpdate(),OT_ship,&shipData,sizeof(shipData));
  IstationIGC* station = side0->GetStation(0);
  station->RepairAndRefuel(ship);
  station->Launch(ship);
  ControlData cd;
  cd.jsValues[c_axisThrottle] = 1.0f;
  ship->SetControls(cd);
  return ship;
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
    mission.SetMissionStage(STAGE_STARTED); 

    osgViewer::Viewer viewer;
    osg::ref_ptr<osgGA::UFOManipulator> cameraManip( new osgGA::UFOManipulator );
    cameraManip->setForwardSpeed( cameraManip->getForwardSpeed() * 10000.0f );
    cameraManip->setSideSpeed( cameraManip->getSideSpeed() * 10000.0f );
    cameraManip->setRotationSpeed( cameraManip->getRotationSpeed() * 1000.0f );

    double fovy, ar, zNear, zFar;
    viewer.getCamera()->getProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    zFar *= 100.0f;
    viewer.getCamera()->setProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    viewer.getCamera()->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );
    viewer.realize();
    IshipIGC* ship = launchShip( mission );
    MyThingSite* mts = dynamic_cast<MyThingSite*>(ship->GetThingSite());
    std::cout << "Create cluster\n";
    osg::ref_ptr<osg::Group> root = createCluster( mission.GetSide(0)->GetStation(0)->GetCluster() ); 
    viewer.setSceneData(root);
    std::cout << "Start game loop\n";
    while(!viewer.done())
    {
      mission.Update( Clock::now() );
      viewer.getCamera()->setViewMatrixAsLookAt( mts->GetPosition() - mts->GetForward() * (3*ship->GetRadius()), mts->GetPosition(), mts->GetUp() );
      viewer.frame();
    }
    std::cout << "Shuttind down\n";

  } catch( std::exception& e )
  {
    std::cerr << "exception caught " << e.what() << "\n";
    return 1;
  }
    
  return 0; 
}
