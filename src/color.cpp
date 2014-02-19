class Color
{
private:
  float r,g,b,a;
public:
  Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
  Color( float r, float g, float b, float a = 1.0f ) : r(r), g(g), b(b), a(a) {}
  osg::Vec4 toVec4() { return osg::Vec4(r,g,b,a); }

  template <class Archive> void serialize( Archive& ar )
  {
    ar( CEREAL_NVP(r) );
    ar( CEREAL_NVP(g) );
    ar( CEREAL_NVP(b) );
    ar( CEREAL_NVP(a) );
  }
};


