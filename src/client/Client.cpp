#ifdef _WINDOWS
#include "windows.h"
#endif

#include "Client"

#include <osgViewer/Viewer>
#include <osgGA/UFOManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/CameraManipulator>
#include <osgText/Text>
#include <igc/missionIGC.h>
#include <iostream>
#include <igc/pch.h>
#include "ClientIgcSite"
#include "model/ResourceManager"
#include "Geo"
#include "formatters"
#include "MyThingSite"
#include "MyClusterSite"
#include "Renderable"

using namespace fa;

IshipIGC* launchShip( ImissionIGC& mission, IsideIGC* side, int hullID, PilotType pt, const std::string& name )
{
  std::cout << "Launching ship " << name << "\n";  
  DataShipIGC shipData;
  shipData.shipID = mission.GenerateNewShipID();
  shipData.hullID = hullID;
  shipData.sideID = side->GetObjectID();
  shipData.nKills = 0;
  shipData.nDeaths = 0;
  shipData.pilotType = pt;
  shipData.nEjections = 0;
  strcpy(shipData.name,name.c_str());
  shipData.nParts = 0;
  shipData.baseObjectID = NA;

  std::cout << "Create ship\n";
  IshipIGC* ship = (IshipIGC*)mission.CreateObject(mission.GetLastUpdate(),OT_ship,&shipData,sizeof(shipData));
  const PartTypeListIGC* plist = ship->GetHullType()->GetPreferredPartTypes();
  std::cout << "Add parts\n";
  for( auto part : *plist )
  {
    std::cout << "Part name " << part->GetName() << ", type " << part->GetEquipmentType() << "\n";
    switch( part->GetEquipmentType() )
    {
      case ET_Weapon:
        IpartIGC* p = ship->CreateAndAddPart(part,0,0);
        p->Arm();
        break;
    }
  }

  std::cout << "Getting station\n";
  IstationIGC* station = side->GetStations()->front();
  station->RepairAndRefuel(ship);
  station->Launch(ship);
  std::cout << "Setting controls\n";
  ControlData cd;
  cd.jsValues[c_axisThrottle] = -1.0f;
  ship->SetControls(cd);
  if( pt == c_ptWingman )
  {
    ship->PickDefaultOrder(nullptr,ship->GetPosition(),false);
    ship->SetAutopilot(true);
  }
  return ship;
}

Client::Client()
{
}

void Client::init()
{
  hud = std::shared_ptr<HUD>( new HUD() );
  renderManager.addRenderable(hud);

  fa::ResourceManager::setPathBase("decompiled/");

  UTL::SetArtPath( "Artwork/" );
  Time start = Clock::now();

  mission.Initialize( start, &clientIgc );
  LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
  
  MissionParams mp;
  mp.strGameName = std::string("Flight Test");
  mp.nTeams = 2;
  mp.rgCivID = { 18, 18 };
  mp.mmMapType = c_mmBrawl;
  mission.SetMissionParams( &mp ); 

  const char sideNames[c_cSidesMax][c_cbSideName] = { "Team 1", "Team 2" };
  mission.UpdateSides( start, mission.GetMissionParams(), sideNames );   
  mission.GenerateMission( start, mission.GetMissionParams(), nullptr, nullptr );
  mission.SetStartTime( start );
  mission.EnterGame();
  mission.SetMissionStage(STAGE_STARTED); 

  viewer = &clientIgc;
  evh = osg::ref_ptr<InputManager>( new InputManager );
  evh->viewer = viewer;
  renderManager.setViewer(viewer);
  viewer->addEventHandler( evh );

  double fovy, ar, zNear, zFar;
  viewer->getCamera()->getProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
  zFar *= 100.0f;
  viewer->getCamera()->setProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
  viewer->getCamera()->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );
  viewer->realize();
#if MANIP
  launchShip( mission );
#else
  IsideIGC* side0 = mission.GetSide(0);
  IshipIGC* ship = launchShip( mission, side0, 210, c_ptPlayer, "Factoid" );
//  auto id = ship->GetObjectID();
  controlTarget = ship;
  viewTarget = dynamic_cast<MyThingSite*>(ship->GetThingSite());
  //MyClusterSite* mcs = dynamic_cast<MyClusterSite*>(ship->GetCluster()->GetClusterSite());
  evh->setShip(ship);
//    mcs->SetViewer(viewer);

  IsideIGC* side1 = mission.GetSide(1);
  launchShip( mission, side1, 210, c_ptWingman, "Factoid2" );
#endif
//    std::cout << "Create cluster\n";
//    osg::ref_ptr<osg::Group> root = createCluster( mission.GetSide(0)->GetStation(0)->GetCluster() ); 
//    viewer->setSceneData(root);

#if MANIP
  osg::ref_ptr<osgGA::TrackballManipulator> cManip( new osgGA::TrackballManipulator );
  viewer->setCameraManipulator(cManip); 
#endif
}

void Client::run()
{
  std::cout << "Start game loop\n";
  while(!viewer->done())
  {
    controlTarget->SetControls( evh->getControls() );
    for( auto part : *controlTarget->GetParts() )
    {
      if( part->GetEquipmentType() == ET_Weapon )
      {
        if( evh->fire() )
        {
          part->Activate();
        } else
        {
          part->Deactivate();
        }
      }
    }
    mission.Update( Clock::now() );
#if !MANIP
    osg::Vec3 e = viewTarget->GetCockpit();
    osg::Vec3 c = e + (viewTarget->GetForward()*10);
    if( evh->chase() )
    {
      e += (viewTarget->GetForward()*-20) + (viewTarget->GetUp()*5);
    }
    viewer->getCamera()->setViewMatrixAsLookAt( e, c, viewTarget->GetUp() );
#endif
    viewer->frame();
  }
  std::cout << "Shuttind down\n";
}

