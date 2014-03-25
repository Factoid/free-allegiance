#include "Frame"

namespace fa {

Frame::Frame() {}
Frame::Frame( const std::string& name, const Vector3& pos, const Vector3& up, const Vector3& forward ) : name(name), position(pos), up(up), forward(forward) {} 

}
