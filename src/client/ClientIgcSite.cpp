#include <igc/pch.h>
#include "ClientIgcSite"
#include <iostream>

namespace fa
{
  std::ostream& operator<< (std::ostream& os, const Color& c)
  {
    return os << c.R() << ", " << c.G() << ", " << c.B() << ", " << c.A();
  }

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
			std::cout << "Preload model : " << model << "\n";
		}

		if( textureName != nullptr )
		{
		  std::string texture( textureName );
			std::cout << "Preload texture : " << texture << "\n";
		}
	}

  IObject* ClientIgcSite::LoadRadarIcon( const char* name )
  {
    if( name != nullptr )
    {
      std::string icon( name );
      std::cout << "Load Radar Icon : " << name << "\n";
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
    return std::shared_ptr<ClusterSite>( new ClusterSite );
  }

  std::shared_ptr<ThingSite> ClientIgcSite::CreateThingSite(ImodelIGC* pModel)
  {
    std::cout << "Creating thing site for model " << pModel->GetName() << "\n";
    return std::shared_ptr<ThingSite>( new ThingSite );
  }

  void ClientIgcSite::SideBuildingTechChange(IsideIGC* s)
  {
    std::cout << "Building tech change for side " << s->GetName() << ", now " << s->GetDevelopmentTechs() << "\n";
  }

  void ClientIgcSite::StationTypeChange(IstationIGC* s)
  {
    std::cout << "Station type change for station " << s->GetName() << "\n";
  }

}
