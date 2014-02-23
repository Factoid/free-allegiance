#include "Light"
#include "Image"
#include "ResourceManager"
#include <osg/Billboard>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/TexEnv>
#include <osg/BlendFunc>

Image lightImg( "f101bmp.png" );

Light::Light() : color( Color(1.0f,1.0f,1.0f) ), position( Vector3(0,0,0) ), hold(1.0f), period(1.0f), phase(0.0f), ramp_up(0.5f), ramp_down(0.5f) {}
Light::Light( const Color& c, const Vector3& pos, float period, float phase, float hold, float ramp_up, float ramp_down ) : color(c), position(pos), period(period), phase(phase), hold(hold), ramp_up(ramp_up), ramp_down(ramp_down) {}

osg::ref_ptr<osg::Node> Light::buildGraph( osg::ref_ptr<osg::Node> node )
{
  osg::ref_ptr<osg::PositionAttitudeTransform> tform( new osg::PositionAttitudeTransform );
  osg::ref_ptr<osg::Billboard> gnode( new osg::Billboard );
  tform->addChild(gnode);
  tform->setPosition(position.toVec3d());
  gnode->setMode(osg::Billboard::Mode::POINT_ROT_EYE);
  osg::ref_ptr<osg::Material> mat( new osg::Material );
  mat->setDiffuse( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
  mat->setSpecular( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
  mat->setAmbient( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
  mat->setEmission( osg::Material::FRONT, color.toVec4() );
  lightImg.buildGraph(tform);

  osg::ref_ptr<osg::BlendFunc> trans( new osg::BlendFunc );
  trans->setFunction( osg::BlendFunc::ONE, osg::BlendFunc::ONE );
  gnode->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
  gnode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
  gnode->getOrCreateStateSet()->setAttribute(mat);
  gnode->getOrCreateStateSet()->setAttribute(trans);
  gnode->addDrawable( ResourceManager::instance()->getPrimative( ResourceManager::PrimativeType::PRIMATIVE_QUAD ) );
  return tform;
}

std::string Light::toString() const
{
  std::ostringstream os;
  os << "  Hold " << hold << "\n  Period " << period << "\n  Phase " << phase << "\n  Ramp-Up " << ramp_up << "\n  Ramp-Down " << ramp_down << "\n";
  return os.str();
}
