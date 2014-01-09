#ifndef __MODEL_HPP__
#define __MODEL_HPP__

class Model
{
  public:
    virtual void bind() = 0;
    virtual void draw() = 0;
    virtual ~Model() {};
};

#endif
