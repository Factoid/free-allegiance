#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

class Texture
{
  public:
    virtual void bind() = 0;
    virtual ~Texture() {};
};

#endif
