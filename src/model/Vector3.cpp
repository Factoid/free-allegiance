#include "Vector3"

Vector3::Vector3() : _x(0), _y(0), _z(0) {}
Vector3::Vector3( float x, float y, float z ) : _x(x), _y(y), _z(z) { }

float& Vector3::x() { return _x; }
float& Vector3::y() { return _y; }
float& Vector3::z() { return _z; }

osg::Vec3d Vector3::toVec3d() { return osg::Vec3d(_x,_y,_z); }
