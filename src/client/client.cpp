#ifdef _WINDOWS
#include "windows.h"
#endif

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
#include "MyClusterSite"

using namespace fa;

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
  const PartTypeListIGC* plist = ship->GetHullType()->GetPreferredPartTypes();
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

  IstationIGC* station = side0->GetStation(0);
  station->RepairAndRefuel(ship);
  station->Launch(ship);
  ControlData cd;
  cd.jsValues[c_axisThrottle] = 1.0f;
  ship->SetControls(cd);
  return ship;
}

bool chase = false;
CmissionIGC mission;

class MyEventHandler : public osgGA::GUIEventHandler
{
private:
  bool mouseStick = false;
  ControlData controlData;
  bool m_fire = false;

public:
  const bool fire() const { return m_fire; }
  const ControlData& getControls() const { return controlData; }

  osgViewer::Viewer* viewer;
  bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv )
  {
    auto et = (unsigned int)ea.getEventType();
    bool ok = true;
    while( et != 0 )
    {
      auto e = et & ~(et-1);
      et = et & (et-1);
      switch( e )
      {
        case osgGA::GUIEventAdapter::EventType::PUSH:
          std::cout << "Push event " << ea.getButtonMask() << "\n";
          m_fire = (ea.getButtonMask() & 1) != 0;
          break;
        case osgGA::GUIEventAdapter::EventType::RELEASE:
          std::cout << "Release event " << ea.getButtonMask() << "\n";
          m_fire = (ea.getButtonMask() & 1) != 0;
          break;
        case osgGA::GUIEventAdapter::EventType::DRAG:
        case osgGA::GUIEventAdapter::EventType::MOVE:
          if( ea.getXnormalized() == 0 && ea.getYnormalized() == 0 ) break;
          if( mouseStick )
          {
            controlData.jsValues[c_axisYaw] -= ea.getXnormalized();
            controlData.jsValues[c_axisPitch] -= ea.getYnormalized();
            aa.requestWarpPointer( (ea.getXmin() + ea.getXmax())/2, (ea.getYmin() + ea.getYmax())/2 );
          }
          break;
        case osgGA::GUIEventAdapter::EventType::KEYDOWN:
          switch( ea.getUnmodifiedKey() )
          {
            case osgGA::GUIEventAdapter::KeySymbol::KEY_C:
              chase = !chase;
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_S:
              for( auto s : *mission.GetShips() )
              {
                std::cout << "Ship " << s->GetObjectID() << " - " << s->GetName() << " - " << s->GetHullType()->GetObjectID() << "\n";
                std::cout << "  " << s->GetHullType()->GetCockpit() << ", " << s->GetPosition() << "\n";
              }
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_Equals:
              std::cout << "Throttle up\n";
              controlData.jsValues[c_axisThrottle] += 0.25f;
              if( controlData.jsValues[c_axisThrottle] > 1.0f ) controlData.jsValues[c_axisThrottle] = 1.0f;
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_Minus:
              std::cout << "Throttle down\n";
              controlData.jsValues[c_axisThrottle] -= 0.25f;
              if( controlData.jsValues[c_axisThrottle] < -1.0f ) controlData.jsValues[c_axisThrottle] = -1.0f;
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_Space:
              mouseStick = !mouseStick;
              controlData.jsValues[c_axisYaw] = 0;
              controlData.jsValues[c_axisPitch] = 0;
              if( viewer != nullptr ) {
                osgViewer::Viewer::Windows w;
                viewer->getWindows(w);
                for( auto win : w )
                {
                  win->useCursor( !mouseStick );
                }
              }
              if( mouseStick )
              {
                aa.requestWarpPointer( (ea.getXmin() + ea.getXmax())/2, (ea.getYmin() + ea.getYmax())/2 );
              }
              break;
          }
          break;
        case osgGA::GUIEventAdapter::EventType::KEYUP:
          break;
        case osgGA::GUIEventAdapter::EventType::SCROLL:
          break;
        case osgGA::GUIEventAdapter::EventType::RESIZE:
          break;
        case osgGA::GUIEventAdapter::EventType::FRAME:
          break;
        default:
          std::cout << "ET = " << e << "\n";
          ok = false;
      }
    }
//    std::cout << "Event " << ea.getEventType() << ", " << ea.getTime() << ", " << ea.getKey() << ", " << ea.getUnmodifiedKey() << ", " << ea.getButton() << ", " << ea.getScrollingDeltaX() << ", " << ea.getScrollingDeltaY() << "\n";
    return ok;
  }
};

#ifdef _WINDOWS
int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow )
#else
int main( int argc, char** argv )
#endif
{
  try
  {
	  fa::ResourceManager::setPathBase("decompiled/");

    UTL::SetArtPath( "Artwork/" );
    ClientIgcSite clientIgc;
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

    osgViewer::Viewer& viewer = clientIgc;
    osg::ref_ptr<MyEventHandler> evh( new MyEventHandler );
    evh->viewer = &viewer;
    viewer.addEventHandler( evh );

    double fovy, ar, zNear, zFar;
    viewer.getCamera()->getProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    zFar *= 100.0f;
    viewer.getCamera()->setProjectionMatrixAsPerspective( fovy, ar, zNear, zFar );
    viewer.getCamera()->setClearColor( osg::Vec4( 0.0, 0.0, 0.0, 0.0 ) );
    viewer.realize();
#if MANIP
    launchShip( mission );
#else
    IshipIGC* ship = launchShip( mission );
    auto id = ship->GetObjectID();
    MyThingSite* mts = dynamic_cast<MyThingSite*>(ship->GetThingSite());
    MyClusterSite* mcs = dynamic_cast<MyClusterSite*>(ship->GetCluster()->GetClusterSite());
    mcs->SetViewer(viewer);
#endif
//    std::cout << "Create cluster\n";
//    osg::ref_ptr<osg::Group> root = createCluster( mission.GetSide(0)->GetStation(0)->GetCluster() ); 
//    viewer.setSceneData(root);

#if MANIP
    osg::ref_ptr<osgGA::TrackballManipulator> cManip( new osgGA::TrackballManipulator );
    viewer.setCameraManipulator(cManip); 
#endif
    std::cout << "Start game loop\n";
    while(!viewer.done())
    {
      ship->SetControls( evh->getControls() );
      for( auto part : *ship->GetParts() )
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
      ship = mission.GetShip(id);
      mts = dynamic_cast<MyThingSite*>(ship->GetThingSite());
      osg::Vec3 e = mts->GetCockpit();
      osg::Vec3 c = e + (mts->GetForward()*3);
      if( chase )
      {
        e += (mts->GetForward()*-20) + (mts->GetUp()*5);
      }
      viewer.getCamera()->setViewMatrixAsLookAt( e, c, mts->GetUp() );
#endif
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
