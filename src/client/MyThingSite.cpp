#include "MyThingSite"
#include "formatters"
#include <iostream>
#include <osg/Vec3>

MyThingSite::MyThingSite( ImodelIGC* model ) : model(model) {}
Geo* MyThingSite::GetGeo() { return &geo; }

HRESULT MyThingSite::LoadAleph( const char* textureName )
{
  std::cout << "ThingSite load aleph texture " << textureName << "\n";
  geo.setModel( "aleph_sphere" );
  return S_OK;
}
HRESULT MyThingSite::LoadDecal( const char* textureName, bool bDirectional, float width )
{
  //    std::cout << "ThingSite load decal " << textureName << " dir " << bDirectional << " width " << width << "\n";
  geo.setDecal( textureName );
  return S_OK;
}
HRESULT MyThingSite::LoadModel( int options, const char* modelName, const char* textureName )
{
  std::cout << "ThingSite load model ";
  if( modelName )
  {
    std::cout << " model = " << modelName;
    geo.setModel( modelName );
  }
  if( textureName )
  {
    std::cout << " texture = " << textureName;
  }
  std::cout << "\n";
  return S_OK;
}

osg::Vec3 MyThingSite::GetCockpit() const
{
  osg::Vec3 cp = GetPosition();
  auto ship = dynamic_cast<IshipIGC*>(model);
  if( ship )
  {
    Vector v = ship->GetHullType()->GetCockpit();
    cp += osg::Vec3( v.X(), v.Y(), v.Z() );
  }
  return cp;
}

osg::Vec3 MyThingSite::GetPosition() const
{
  Vector v = tform.GetPosition();
  return osg::Vec3( v.X(), v.Y(), v.Z() );
}

osg::Vec3 MyThingSite::GetForward() const
{
  Vector v = tform.GetOrientation().GetForward();
  return osg::Vec3( v.X(), v.Y(), v.Z() );
}

osg::Vec3 MyThingSite::GetUp() const
{
  Vector v = tform.GetOrientation().GetUp();
  return osg::Vec3( v.X(), v.Y(), v.Z() );
}
void MyThingSite::SetPosition( const Vector& pos )
{
  tform.SetPosition(pos);
  UpdateMatrix();
}
void MyThingSite::SetOrientation( const Orientation& orient )
{
  tform.SetOrientation(orient);
  UpdateMatrix();
}
void MyThingSite::SetRadius( float r )
{
  radius = r;
}
void MyThingSite::UpdateSideVisibility( ImodelIGC* pmodel, IclusterIGC* pcluster )
{
  //std::cout << "Model = " << pmodel->GetName() << ", cluster = " << pcluster->GetName() << "\n";
  for( auto m : *pcluster->GetShips() )
  {
    if( m->CanSee(pmodel) )
    {
      pmodel->SetSideVisibility( m->GetSide(), true );
      if( m->GetSide() != pmodel->GetSide() )
      {
        //          m->PickDefaultOrder(pcluster,m->GetPosition(),false);
      }
    }
  }
  for( auto m : *pcluster->GetStations() )
  {
    if( m->CanSee(pmodel) )
    {
      pmodel->SetSideVisibility( m->GetSide(), true );
    }
  }
  for( auto m : *pcluster->GetProbes() )
  {
    if( m->CanSee(pmodel) )
    {
      pmodel->SetSideVisibility( m->GetSide(), true );
    }
  }
}
void MyThingSite::SetVisible( unsigned char c )
{
  std::cout << "Set Visible = " << c << "\n";
}
void MyThingSite::SetSideVisibility( IsideIGC* side, bool visible )
{
  //    std::cout << "Set Side Visibility = " << side->GetName() << " v = " << visible << "\n";
}
bool MyThingSite::GetSideVisibility( IsideIGC* side )
{
  return true;
}

void MyThingSite::UpdateSeenBySide()
{
  std::cout << "Update seen by side\n";
}

void MyThingSite::UpdateMatrix()
{
  Vector v = tform.GetPosition();
  if( geo.model )
  {
    float scale = radius / geo.getRadius();
    float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    for( int i = 0; i < 3; ++i )
    {
      for( int j = 0; j < 3; ++j )
      {
        m[i*4+j] = tform.GetOrientation()[i][j];
      }
    }
    geo.model->setMatrix( osg::Matrix::scale( scale, scale, scale ) * osg::Matrix( m ) * osg::Matrix::translate( v.X(), v.Y(), v.Z() ) );
  }
}
