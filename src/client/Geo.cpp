#include "Geo"
#include <osg/MatrixTransform>
#include <model/ResourceManager>
#include <osg/Billboard>
#include <osg/Material>
#include <osg/BlendFunc>
#include <model/Image>

Geo::Geo() {}

void Geo::setModel( const std::string& mName )
{
  model = osg::ref_ptr<osg::MatrixTransform>( new osg::MatrixTransform );
  osg::ref_ptr<osg::Group> mdl = fa::ResourceManager::instance()->getModel( mName + ".json" );
  radius = mdl->getBound().radius();
  model->addChild( mdl );
}

void Geo::setDecal( const std::string& dName )
{
  model = osg::ref_ptr<osg::MatrixTransform>( new osg::MatrixTransform );
  osg::ref_ptr<osg::Billboard> gnode( new osg::Billboard );
  model->addChild(gnode);
  gnode->setMode(osg::Billboard::Mode::POINT_ROT_EYE);
  osg::ref_ptr<osg::Material> mat( new osg::Material );
  mat->setDiffuse( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
  mat->setSpecular( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
  mat->setAmbient( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
  mat->setEmission( osg::Material::FRONT, osg::Vec4(1,1,1,1) );
  fa::Image lightImg( dName + "bmp.png" );    
  lightImg.buildGraph(model);
  radius = 1.0f;

  osg::ref_ptr<osg::BlendFunc> trans( new osg::BlendFunc );
  trans->setFunction( osg::BlendFunc::ONE, osg::BlendFunc::ONE );
  gnode->getOrCreateStateSet()->setMode(GL_BLEND,osg::StateAttribute::ON);
  gnode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
  gnode->getOrCreateStateSet()->setAttribute(mat);
  gnode->getOrCreateStateSet()->setAttribute(trans);
  gnode->addDrawable( fa::ResourceManager::instance()->getPrimative( fa::ResourceManager::PrimativeType::PRIMATIVE_QUAD ) );
}

float Geo::getRadius() { return radius; }
