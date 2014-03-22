#include "Color"

Color::Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
Color::Color( float r, float g, float b, float a ) : r(r), g(g), b(b), a(a) {}
osg::Vec4 Color::toVec4() const { return osg::Vec4(r,g,b,a); }
