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

void Client::loadCore()
{
  mission.Initialize( start, &clientIgc );
  LoadIGCStaticCore( "cc_14", &mission, false, nullptr );
}

void Client::launchMission()
{
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
}

void Client::init()
{
  hud = std::shared_ptr<HUD>( new HUD() );
  renderManager.addRenderable(hud);

  fa::ResourceManager::setPathBase("decompiled/");
  UTL::SetArtPath( "Artwork/" );

  start = Clock::now();

  loadCore();
  launchMission();
  inputManager = osg::ref_ptr<InputManager>( new InputManager );
   
  clientIgc.addEventHandler( inputManager );
  renderManager.setViewer(&clientIgc);
  inputManager->setViewer(&clientIgc);

  IsideIGC* side0 = mission.GetSide(0);
  IshipIGC* ship = launchShip( mission, side0, 210, c_ptPlayer, "Factoid" );
  controlTarget = ship;
  MyThingSite* viewTarget = dynamic_cast<MyThingSite*>(ship->GetThingSite());
  renderManager.setViewTarget(viewTarget);
  inputManager->setShip(ship);

  IsideIGC* side1 = mission.GetSide(1);
  launchShip( mission, side1, 210, c_ptWingman, "Factoid2" );
}

void Client::run()
{
  std::cout << "Start game loop\n";
  while(!clientIgc.done())
  {
    inputManager->update();
    mission.Update( Clock::now() );
    renderManager.update( inputManager->chase() );
  }
  std::cout << "Shuttind down\n";
}

