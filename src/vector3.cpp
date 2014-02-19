class Vector3
{
private:
  float _x,_y,_z;

public:
  Vector3() : _x(0), _y(0), _z(0) {}
  Vector3( float x, float y, float z ) : _x(x), _y(y), _z(z) { }

  float& x() { return _x; }
  float& y() { return _y; }
  float& z() { return _z; }

//  osg::Vec3d toVec3d() { return osg::Vec3d(_y,_z,_x); }
  osg::Vec3d toVec3d() { return osg::Vec3d(_x,0,0); }

  template <class Archive> void serialize( Archive& ar )
  {
    ar( _CEREAL_NVP("x",_x) );
    ar( _CEREAL_NVP("y",_y) );
    ar( _CEREAL_NVP("z",_z) );
  }
};


