class Frame
{
private:
  std::string name;
  Vector3 position;
  Vector3 up;
  Vector3 forward;

public:
  Frame() {}
  Frame( const std::string& name, const Vector3& pos, const Vector3& up, const Vector3& forward ) : name(name), position(pos), up(up), forward(forward) {} 
  template <class Archive> void serialize( Archive& ar )
  {
    ar(CEREAL_NVP(name));
    ar(CEREAL_NVP(position));
    ar(CEREAL_NVP(up));
    ar(CEREAL_NVP(forward));
  }
};
