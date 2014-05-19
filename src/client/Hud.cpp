#include "Hud"

#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osgText/Text>

namespace fa
{
  HUD::HUD()
  {
    root = new osg::Projection();
    root->setMatrix(osg::Matrix::ortho2D(0,1024,0,768));

    osg::MatrixTransform* HUDView = new osg::MatrixTransform();
    root->addChild(HUDView);
    HUDView->setMatrix(osg::Matrix::identity());
    HUDView->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

    osg::Geode* HUDGeode = new osg::Geode();
    HUDView->addChild(HUDGeode);

    osgText::Text* text = new osgText::Text();
    HUDGeode->addDrawable( text );
    text->setText("Hello world\nLine 2\nLine 3");
  }

  osg::ref_ptr<osg::Group> HUD::getRoot()
  {
    return root;
  }
}
