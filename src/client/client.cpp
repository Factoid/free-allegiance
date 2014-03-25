#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include <igc/pch.h>
#include <igc/missionIGC.h>
#include <iostream>

#include "ClientIgcSite"
#include "model/ResourceBase"

using namespace fa;

int main( int argc, char** argv )
{
  try
  {
    fa::ResourceBase::setResourceBase("decompiled/");

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

    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Group> root( new osg::Group );
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> cameraManip( new osgGA::KeySwitchMatrixManipulator );
    viewer.run();
  } catch( std::exception& e )
  {
    std::cerr << "exception caught " << e.what() << "\n";
    return 1;
  }
    
  return 0; 
}
