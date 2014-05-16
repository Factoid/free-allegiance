#include <igc/pch.h>
#include "ClientIgcSite"
#include <iostream>
#include "model/ModelDefinition"
#include "model/ResourceManager"
#include "Geo"
#include <osg/MatrixTransform>
#include "formatters"
#include "MyThingSite"
#include "MyClusterSite"

namespace fa
{
  void ClientIgcSite::DevelopmentCompleted(IbucketIGC* b, IdevelopmentIGC* d, Time now)
  {
    std::cout << "Development Completed " << b << ", " << d << "\n";
  }

  void ClientIgcSite::StationTypeCompleted(IbucketIGC*  b, IstationIGC* pstation, IstationTypeIGC* st, Time now)
  {
    std::cout << "Station Completed bucket " << b << ", station " << pstation << " type " << st << "\n";
  }

  void ClientIgcSite::HullTypeCompleted(IsideIGC* pside, IhullTypeIGC* pht)
  {
    std::cout << "Hull type completed, side " << pside << ", hull type " << pht << "\n";
  }

  void ClientIgcSite::PartTypeCompleted(IsideIGC* pside, IpartTypeIGC* ppt)
  {
    std::cout << "Part type completed, side " << pside << ", part type " << ppt << "\n";
  }

  void ClientIgcSite::DroneTypeCompleted(IbucketIGC* b, IstationIGC* pstation, IdroneTypeIGC* dt, Time now)
  {
    std::cout << "Drone completed bucket " << b << ", station " << pstation << ", drone type " << dt << "\n";
  }

  void ClientIgcSite::BucketChangeEvent(BucketChange bc, IbucketIGC* b)
  {
    std::cout << "Bucket change " << bc << " bucket " << b << "\n";
  }

  void ClientIgcSite::SideBuildingTechChange(IsideIGC* s)
  {
    std::cout << "Building tech change for side " << s->GetName() << ", now " << s->GetDevelopmentTechs() << "\n";
  }

  void ClientIgcSite::SideDevelopmentTechChange(IsideIGC* s) {
    std::cout << "Side " << s->GetName() << " got new tech, now " << s->GetDevelopmentTechs() << "\n";
  }

  void ClientIgcSite::SideGlobalAttributeChange(IsideIGC* s)
  {
    std::cout << "Side GA changed " << s << "\n";
  }

  void ClientIgcSite::StationTypeChange(IstationIGC* s)
  {
    std::cout << "Station type change for station " << s->GetName() << "\n";
  }

  void ClientIgcSite::ClusterUpdateEvent( IclusterIGC* c )
  {
    //std::cout << "Cluster update event " << c->GetName() << "\n";
  }

  void ClientIgcSite::BuildStation(IasteroidIGC* pasteroid, IsideIGC* pside, IstationTypeIGC* pstationtype, Time now, bool pbseensides[])
  {
    std::cout << "Station built on asteroid " << pasteroid << " for side " << pside << " type " << pstationtype << "\n";
  }

  std::shared_ptr<ThingSite> ClientIgcSite::CreateThingSite(ImodelIGC* pModel)
  {
//    std::cout << "Creating thing site for model " << pModel->GetName() << "\n";
    return std::shared_ptr<ThingSite>( new MyThingSite(pModel) );
  }

  std::shared_ptr<ClusterSite> ClientIgcSite::CreateClusterSite( IclusterIGC* pCluster )
  {
    std::cout << "Creating cluster site for cluster " << pCluster->GetName() << "\n";
    return std::shared_ptr<ClusterSite>( new MyClusterSite(pCluster) );
  }

  void ClientIgcSite::ReceiveChat(IshipIGC* pshipSender, ChatTarget ctRecipient, ObjectID oidRecipient, SoundID voiceOver, const char* szText, CommandID cid, ObjectType otTarget, ObjectID oidTarget, ImodelIGC* pmodelTarget )
  {
    std::cout << "Got chat\n";
  }

  void ClientIgcSite::SendChat(IshipIGC* pshipSender, ChatTarget chatTarget, ObjectID oidRecipient, SoundID soVoiceOver, const char* szText, CommandID cid, ObjectType otTarget, ObjectID oidTarget, ImodelIGC* pmodelTarget, bool bObjectModel)
  {
    std::cout << "Send chat : " << szText << " cmd " << cid << "\n";
  }

  void ClientIgcSite::SendChatf(IshipIGC* pshipSender, ChatTarget ctRecipient, ObjectID oidRecipient, SoundID soVoiceOver, const char* pszText, ...) {
    const int c_cbBfr = 256;
    char szChatBfr[c_cbBfr];

    va_list vl;
    va_start(vl, pszText);
    _vsnprintf(szChatBfr, c_cbBfr, pszText, vl);
    va_end(vl);

    SendChat(pshipSender, ctRecipient, oidRecipient, soVoiceOver, szChatBfr);
  }

  void ClientIgcSite::GetMoneyRequest(IshipIGC*  pshipSender, Money amount, HullID hidFor)
  {
    std::cout << "Money request from " << pshipSender << " for " << amount << " hulltype " << hidFor << "\n";
  }

  void ClientIgcSite::PlaySoundEffect(SoundID soundID, ImodelIGC* model)
  {
    std::cout << "Play sound effect " << soundID << " from " << model << "\n";
  }

  void ClientIgcSite::PlaySoundEffect(SoundID soundID, ImodelIGC* model, const Vector& vectOffset)
  {
    std::cout << "Play sound effect with offset " << soundID << " from " << model << "\n";
  }

  void ClientIgcSite::PlayNotificationSound(SoundID soundID, ImodelIGC* model)
  {
    std::cout << "Play notification sound " << soundID << " from " << model << "\n";
  }

  void ClientIgcSite::PlayFFEffect(ForceEffectID effectID, ImodelIGC* model, LONG lDirection )
  {
    std::cout << "Play force effect " << effectID << " from " << model << " dir " << lDirection << "\n";
  }
  
  void ClientIgcSite::PlayVisualEffect(VisualEffectID effectID, ImodelIGC* model, float fIntensity)
  {
    std::cout << "Play visual effect " << effectID << " from " << model << " intensity " << fIntensity << "\n";
  }

  int ClientIgcSite::MessageBox(const ZString& strText, const ZString& strCaption, UINT nType)
  {
    std::cout << "Message box " << strText << " caption " << strCaption << " type " << nType << "\n";
    return 0;
  }

  void ClientIgcSite::TerminateModelEvent(ImodelIGC* model)
  {
//    std::cout << "Terminate model event " << model << "\n";
  }

  void ClientIgcSite::TerminateMissionEvent(ImissionIGC* pMission)
  {
    std::cout << "Terminate mission event " << pMission << "\n";
  }

  void ClientIgcSite::KillAsteroidEvent(IasteroidIGC* pasteroid, bool explodeF)
  {
    std::cout << "Kill Asteroid " << pasteroid << " explode " << explodeF << "\n";
  }

  void ClientIgcSite::KillAsteroidEvent(AsteroidID roid, SectorID soid, IsideIGC* side)
  {
    std::cout << "Kill asteroid " << roid << " in sector " << soid << " by side " << side << "\n";
  }

  void ClientIgcSite::DrainAsteroidEvent(IasteroidIGC* pasteroid)
  {
    std::cout << "Drain asteroid event " << pasteroid << "\n";
  }

  void ClientIgcSite::MineAsteroidEvent(IasteroidIGC* pasteroid, float newOre)
  {
    std::cout << "Mine asteroid event " << pasteroid << " ore " << newOre << "\n";
  }

  void ClientIgcSite::KillProbeEvent(IprobeIGC* pprobe)
  {
    std::cout << "Kill probe " << pprobe << "\n";
  }

  void ClientIgcSite::KillMissileEvent(ImissileIGC* pmissile, const Vector& position)
  {
    std::cout << "Kill missile " << pmissile << "\n";
  }

  void ClientIgcSite::KillBuildingEffectEvent(IbuildingEffectIGC* pbe)
  {
    std::cout << "Kill building effect " << pbe << "\n";
  }

  void ClientIgcSite::KillMineEvent(ImineIGC* pmine)
  {
    std::cout << "Kill mine event " << pmine << "\n";
  }

  void ClientIgcSite::KillTreasureEvent(ItreasureIGC* ptreasure)
  {
    std::cout << "Kill treasure event " << ptreasure << "\n";
  }

  void ClientIgcSite::PostText(bool bCritical, const char* pszText, ...)
  {
    std::cout << "Post text, critical " << bCritical << " format string " << pszText << "\n";
  }

  void ClientIgcSite::PostNotificationText(ImodelIGC* pmodel, bool bCritical, const char* pszText, ...)
  {
    std::cout << "Post notification text model " << pmodel << " critical " << bCritical << " text " << pszText << "\n";
  }

  void ClientIgcSite::EjectPlayer(ImodelIGC* pmodelCredit)
  {
    std::cout << "Player ejected " << pmodelCredit << "\n";
  }

  void ClientIgcSite::TargetKilled(ImodelIGC* pmodel)
  {
    std::cout << "Target killed " << pmodel << "\n";
  }

  void ClientIgcSite::ShipWarped(IshipIGC* pship, SectorID sidOld, SectorID sidNew)
  {
    std::cout << "Ship warped " << pship << " from " << sidOld << " to " << sidNew << "\n";
  }

  void ClientIgcSite::DamageShipEvent(Time now, IshipIGC* ship, ImodelIGC* launcher, DamageTypeID type, float amount, float leakage, const Vector& p1, const Vector& p2)
  {
    std::cout << "Damage event " << ship << " amount " << amount << " leak " << leakage << "\n";
  }

  void ClientIgcSite::KillShipEvent(Time now, IshipIGC* ship, ImodelIGC* launcher, float amount, const Vector& position, const Vector& p2)
  {
    std::cout << "Kill ship " << ship << " amount " << amount << "\n";
    //IshipIGC* credit;
    ship->Kill(now,nullptr,amount,position,p2);
    if( ship->GetStation() )
    {
      std::cout << "Relaunching destroyed ship\n";
      ship->SetBaseHullType(ship->GetMission()->GetHullType(210)); // New scout

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

      ship->GetStation()->RepairAndRefuel(ship);
      ship->GetStation()->Launch(ship);
    }
  }

  void ClientIgcSite::DamageStationEvent(IstationIGC* station, ImodelIGC* launcher, DamageTypeID type, float amount)
  {
    std::cout << "Damage station " << station << " amount " << amount << "\n";
  }

  void ClientIgcSite::KillStationEvent(IstationIGC* station, ImodelIGC* launcher, float amount)
  {
    std::cout << "Kill station " << station << " amount " << amount << "\n";
  }

  bool ClientIgcSite::DockWithStationEvent(IshipIGC* ship, IstationIGC* station)
  {
    std::cout << "Dock with station " << ship << " at " << station << "\n";
    if( ship->GetBaseHullType() != ship->GetMission()->GetHullType(210) )
    {
      ship->SetBaseHullType(ship->GetMission()->GetHullType(210)); // New scout

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
    }

    station->RepairAndRefuel(ship);
    station->Launch(ship);
    return true;
  }

  void ClientIgcSite::CaptureStationEvent(IshipIGC* ship, IstationIGC* station)
  {
    std::cout << "Ship captured station " << ship << " station " << station << "\n";
  }

  bool ClientIgcSite::LandOnCarrierEvent(IshipIGC* shipCarrier, IshipIGC* ship, float tCollision)
  {
    std::cout << "Landed on carrier " << shipCarrier << " by " << ship << " at " << tCollision << "\n";
    return true;
  }

  bool ClientIgcSite::RescueShipEvent(IshipIGC* shipRescued, IshipIGC* shipRescuer)
  {
    std::cout << "Rescued ship " << shipRescued << " by " << shipRescuer << "\n";
    return true;
  }

  void ClientIgcSite::WarpBombEvent(IwarpIGC* pwap, ImissileIGC* pmissile)
  {
    std::cout << "Warp bomb " << pwap << " missile " << pmissile << "\n";
  }

  void ClientIgcSite::HitWarpEvent(IshipIGC* ship, IwarpIGC* warp)
  {
    std::cout << "Hit warp " << warp << " by " << ship << "\n";
    IwarpIGC* dest = warp->GetDestination();

    ship->SetCluster( dest->GetCluster() );
    ship->SetPosition( dest->GetPosition() + (dest->GetOrientation().GetForward() * ship->GetRadius()) );
    ship->SetVelocity( dest->GetOrientation().GetForward() * (ship->GetVelocity().Length()+10.0f) );
    ship->SetOrientation( dest->GetOrientation() );
  }

  bool ClientIgcSite::HitTreasureEvent(Time now, IshipIGC* ship, ItreasureIGC* treasure)
  {
    std::cout << "Hit treasure " << treasure << " by " << ship << "\n";
    return true;
  }

  void ClientIgcSite::PaydayEvent(IsideIGC* pside, float money)
  {
    std::cout << "Payday event " << pside << " amount " << money << "\n";
  }

  void ClientIgcSite::RequestRipcord(IshipIGC* ship, IclusterIGC* pcluster)
  {
    std::cout << "Request ripcord " << ship << " to " << pcluster << "\n";
  }

  bool ClientIgcSite::ContinueRipcord(IshipIGC* ship, ImodelIGC* pmodel)
  {
    std::cout << "Continue ripcord " << ship << " model " << pmodel << "\n";
    return true;
  }

  bool ClientIgcSite::UseRipcord(IshipIGC* ship, ImodelIGC* pmodel)
  {
    std::cout << "Use ripcord " << ship << " model " << pmodel << "\n";
    return true;
  }

  void ClientIgcSite::FireMissile(IshipIGC* pship, ImagazineIGC* pmagazine, Time timeFired, ImodelIGC* pTarget, float lock)
  {
    std::cout << "Fired missile " << pship << " from mag " << pmagazine << " at " << pTarget << " lock " << lock << "\n";
  }

  void ClientIgcSite::FireExpendable(IshipIGC* pship, IdispenserIGC* pdispenser, Time timeFired)
  {
    std::cout << "Fire expendable " << pship << " dispenser " << pdispenser << "\n";
  }

  void * ClientIgcSite::GetDroneFromShip(IshipIGC * pship)
  {
    std::cout << "Get drone from ship " << pship << "\n";
    return nullptr;
  }

  void ClientIgcSite::CreateSideEvent( IsideIGC* pIsideIGC )
  {
    std::cout << "New side " << pIsideIGC->GetName() << " " << pIsideIGC->GetColor() << "\n";
  }

  void ClientIgcSite::DestroySideEvent(IsideIGC * pIsideIGC)
  {
    std::cout << "Destroy side event " << pIsideIGC << "\n";
  }

  void ClientIgcSite::ChangeStation(IshipIGC* pship, IstationIGC* pstationOld, IstationIGC* pstationNew)
  {
    std::cout << "Change station " << pship << " from " << pstationOld << " to " << pstationNew << "\n";
  }

  void ClientIgcSite::ChangeCluster( IshipIGC* pship, IclusterIGC* pclusterOld, IclusterIGC* pclusterNew )
  {
    std::cout << "Ship " << pship << " moved from cluster " << pclusterOld << " to " << pclusterNew << "\n";
    if( pclusterOld )
    {
      MyClusterSite* mcs = dynamic_cast<MyClusterSite*>( pclusterOld->GetClusterSite() );
      mcs->RemoveViewer(*this);
    }
    if( pclusterNew )
    {
      MyClusterSite* mcs = dynamic_cast<MyClusterSite*>( pclusterNew->GetClusterSite() );
      mcs->SetViewer(*this);
    }
  }

  void ClientIgcSite::CommandChangedEvent(Command i, IshipIGC * pship, ImodelIGC* ptarget, CommandID cid)
  {
    std::cout << "Command " << i << " to ship " << pship << " target " << ptarget << " cid " << cid << "\n";
  }

  void ClientIgcSite::Preload( const char* modelName, const char* textureName )
  {
    if( modelName != nullptr )
    {
      std::string model( modelName );
      //      ResourceManager::instance()->getModel( model + ".json" );
    }

    if( textureName != nullptr )
    {
      std::string texture( textureName );
      //      ResourceManager::instance()->getImage( texture + ".png" );
    }
  }

  void ClientIgcSite::ActivateTeleportProbe(IprobeIGC* pprobe)
  {
    std::cout << "Teleport probe active " << pprobe << "\n";
  }

  void ClientIgcSite::DestroyTeleportProbe(IprobeIGC* pprobe)
  {
    std::cout << "Teleport probe destroyed " << pprobe << "\n";
  }

  void ClientIgcSite::LoadoutChangeEvent( IshipIGC* pship, IpartIGC* ppart, LoadoutChange lc )
  {
    std::cout << "Ship " << pship << " part " << ppart << " change " << lc << "\n";
  }

  IObject* ClientIgcSite::LoadRadarIcon( const char* name )
  {
    if( name != nullptr )
    {
      std::string icon( name );
      //      std::cout << "Load Radar Icon : " << name << "\n";
    }
    return nullptr;
  }

  void ClientIgcSite::ChangeGameState(void)
  {
    std::cout << "Game state changed\n";
  }

  void ClientIgcSite::CreateBuildingEffect(Time now, IasteroidIGC* pasteroid, IshipIGC* pshipBuilder)
  {
    std::cout << "Create building effect " << pasteroid << " builder " << pshipBuilder << "\n";
  }
  
  void ClientIgcSite::LayExpendable( Time now, IexpendableTypeIGC* pet, IshipIGC* pshipLayer)
  {
    std::cout << "Lay expendable " << pet << " from " << pshipLayer << "\n";
  }

  IclusterIGC* ClientIgcSite::GetCluster(IshipIGC* pship, ImodelIGC* target)
  {
//    std::cout << "Get cluster " << pship << " model " << pmodel << "\n";
    return target->GetCluster();
  }

  IclusterIGC* ClientIgcSite::GetRipcordCluster(IshipIGC* pship, HullAbilityBitMask habm)
  {
    std::cout << "Get rip cluster " << pship << " mask " << habm << "\n";
    return nullptr;
  }

  bool ClientIgcSite::Reload(IshipIGC* pship, IlauncherIGC* plauncher, EquipmentType type)
  {
    std::cout << "Reload " << pship << " launcher " << plauncher << " type " << type << "\n";
    return true;
  }
}
