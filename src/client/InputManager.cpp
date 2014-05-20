#include "InputManager"

namespace fa
{
  void InputManager::setShip( IshipIGC* ship ) { this->shipID = ship->GetObjectID(); }
  void InputManager::setMission( ImissionIGC* mission ) { this->mission = mission; }
  const bool InputManager::fire() const { return m_fire; }
  const bool InputManager::chase() const { return m_chase; }
  const ControlData& InputManager::getControls() const { return controlData; }
  void InputManager::setViewer( osgViewer::Viewer* viewer ) { this->viewer = viewer; }

  bool InputManager::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv )
  {
    auto et = (unsigned int)ea.getEventType();
    bool ok = true;
    IshipIGC* ship = mission->GetShip(shipID);
    while( et != 0 )
    {
      auto e = et & ~(et-1);
      et = et & (et-1);
      switch( e )
      {
        case osgGA::GUIEventAdapter::EventType::PUSH:
//          std::cout << "Push event " << ea.getButtonMask() << "\n";
          m_fire = (ea.getButtonMask() & 1) != 0;
          break;
        case osgGA::GUIEventAdapter::EventType::RELEASE:
//          std::cout << "Release event " << ea.getButtonMask() << "\n";
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
            case osgGA::GUIEventAdapter::KeySymbol::KEY_W:
              ship->SetStateBits(upButtonIGC,upButtonIGC);
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_S:
              ship->SetStateBits(downButtonIGC,downButtonIGC);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_A:
              ship->SetStateBits(leftButtonIGC,leftButtonIGC);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_D:
              ship->SetStateBits(rightButtonIGC,rightButtonIGC);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_X:
              ship->SetStateBits(backwardButtonIGC,backwardButtonIGC);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_C:
              m_chase = !m_chase;
              break;
/*
            case osgGA::GUIEventAdapter::KeySymbol::KEY_I:
              for( auto s : *mission.GetShips() )
              {
                std::cout << "Ship " << s->GetObjectID() << " - " << s->GetName() << " - " << s->GetHullType()->GetObjectID() << "\n";
                std::cout << "  " << s->GetHullType()->GetCockpit() << ", " << s->GetPosition() << "\n";
              }
              break;
*/
            case osgGA::GUIEventAdapter::KeySymbol::KEY_Equals:
//              std::cout << "Throttle up\n";
              controlData.jsValues[c_axisThrottle] += 0.25f;
              if( controlData.jsValues[c_axisThrottle] > 1.0f ) controlData.jsValues[c_axisThrottle] = 1.0f;
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_Minus:
//              std::cout << "Throttle down\n";
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
          switch( ea.getUnmodifiedKey() )
          {
            case osgGA::GUIEventAdapter::KeySymbol::KEY_W:
              ship->SetStateBits(upButtonIGC,0);
              break;
            case osgGA::GUIEventAdapter::KeySymbol::KEY_S:
              ship->SetStateBits(downButtonIGC,0);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_A:
              ship->SetStateBits(leftButtonIGC,0);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_D:
              ship->SetStateBits(rightButtonIGC,0);
              break;              
            case osgGA::GUIEventAdapter::KeySymbol::KEY_X:
              ship->SetStateBits(backwardButtonIGC,0);
              break;             
          }
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

  void InputManager::update()
  {
    IshipIGC* ship = mission->GetShip(shipID);
    if( ship == nullptr ) return;

    ship->SetControls( getControls() );
    for( auto part : *ship->GetParts() )
    {
      if( part->GetEquipmentType() == ET_Weapon )
      {
        if( fire() )
        {
          part->Activate();
        } else
        {
          part->Deactivate();
        }
      }
    }
  }
}
