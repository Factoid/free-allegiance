class Light
{
private:
  float hold;
  float period;
  float phase;
  float ramp_up;
  float ramp_down;
  Color color;
  Vector3 position;

public:
  Light() : color( Color(1.0f,1.0f,1.0f) ), position( Vector3(0,0,0) ), hold(1.0f), period(1.0f), phase(0.0f), ramp_up(0.5f), ramp_down(0.5f) {}
  Light( const Color& c, const Vector3& pos, float period, float phase, float hold, float ramp_up, float ramp_down ) : color(c), position(pos), period(period), phase(phase), hold(hold), ramp_up(ramp_up), ramp_down(ramp_down) {}

  osg::ref_ptr<osg::Node> buildGraph( osg::ref_ptr<osg::Node> node )
  {
    std::cout << "Creating light at position " << position.x() << ", " << position.y() << ", " << position.z() << "\n";
    osg::ref_ptr<osg::PositionAttitudeTransform> tform( new osg::PositionAttitudeTransform );
    osg::ref_ptr<osg::Billboard> gnode( new osg::Billboard );
    tform->addChild(gnode);
    tform->setPosition(position.toVec3d());
    gnode->setMode(osg::Billboard::Mode::POINT_ROT_EYE);
    //gnode->setAxis( osg::Vec3(0.0f,1.0f,0.0f));
    //gnode->setNormal( osg::Vec3(0.0f,0.0f,-1.0f));
    osg::ref_ptr<osg::Material> mat( new osg::Material );
    mat->setDiffuse( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
    mat->setSpecular( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
    mat->setAmbient( osg::Material::FRONT, osg::Vec4(0,0,0,1) );
    mat->setEmission( osg::Material::FRONT, color.toVec4() );
    gnode->getOrCreateStateSet()->setAttribute(mat);
    gnode->addDrawable( ResourceManager::instance()->getPrimative( ResourceManager::PrimativeType::PRIMATIVE_QUAD ) );
    return tform;
  }

  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(hold) );
    ar( CEREAL_NVP(period) );
    ar( CEREAL_NVP(phase) );
    ar( CEREAL_NVP(ramp_up) );
    ar( CEREAL_NVP(ramp_down) );
    ar( CEREAL_NVP(color) );
    ar( CEREAL_NVP(position) );
  }

  std::string toString() const
  {
    std::ostringstream os;
    os << "  Hold " << hold << "\n  Period " << period << "\n  Phase " << phase << "\n  Ramp-Up " << ramp_up << "\n  Ramp-Down " << ramp_down << "\n";
    return os.str();
  }
};

