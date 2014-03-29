#include <igc/pch.h>
#include "ClientIgcSite"
#include <iostream>
#include "model/ModelDefinition"
#include "model/ResourceManager"
#include "Geo"
#include <osg/MatrixTransform>
#include "formatters"
#include "MyThingSite"

class MyClusterSite : public ClusterSite
{
};

namespace fa
{
  std::ostream& operator<< (std::ostream& os, const TechTreeBitMask& bm )
  {
    for( unsigned long int i = 0; i < 400; ++i )
    {
      if( bm.GetBit(i) )
      {
        os << i << " ";
      }
    }
    return os;
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

  IObject* ClientIgcSite::LoadRadarIcon( const char* name )
  {
    if( name != nullptr )
    {
      std::string icon( name );
//      std::cout << "Load Radar Icon : " << name << "\n";
    }
    return nullptr;
  }

  void ClientIgcSite::CreateSideEvent( IsideIGC* pIsideIGC )
  {
    std::cout << "New side " << pIsideIGC->GetName() << " " << pIsideIGC->GetColor() << "\n";
  }

  void ClientIgcSite::SideDevelopmentTechChange(IsideIGC* s) {
    std::cout << "Side " << s->GetName() << " got new tech, now " << s->GetDevelopmentTechs() << "\n";
  }

  std::shared_ptr<ClusterSite> ClientIgcSite::CreateClusterSite( IclusterIGC* pCluster )
  {
    std::cout << "Creating cluster site for cluster " << pCluster->GetName() << "\n";
    return std::shared_ptr<ClusterSite>( new MyClusterSite );
  }

  std::shared_ptr<ThingSite> ClientIgcSite::CreateThingSite(ImodelIGC* pModel)
  {
    std::cout << "Creating thing site for model " << pModel->GetName() << "\n";
    return std::shared_ptr<ThingSite>( new MyThingSite );
  }

  void ClientIgcSite::SideBuildingTechChange(IsideIGC* s)
  {
    std::cout << "Building tech change for side " << s->GetName() << ", now " << s->GetDevelopmentTechs() << "\n";
  }

  void ClientIgcSite::StationTypeChange(IstationIGC* s)
  {
    std::cout << "Station type change for station " << s->GetName() << "\n";
  }

  void ClientIgcSite::ClusterUpdateEvent( IclusterIGC* c )
  {
  }

  void ClientIgcSite::LoadoutChangeEvent( IshipIGC* pship, IpartIGC* ppart, LoadoutChange lc )
  {
    std::cout << "Ship " << pship << " part " << ppart << " change " << lc << "\n";
  }

  void ClientIgcSite::ChangeCluster( IshipIGC* pship, IclusterIGC* pclusterOld, IclusterIGC* pclusterNew )
  {
    std::cout << "Ship " << pship << " moved from cluster " << pclusterOld << " to " << pclusterNew << "\n";
  }
}
