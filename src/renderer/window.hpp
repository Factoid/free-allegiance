#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

class Window
{
  public:
    virtual bool isClosed() = 0;
    virtual void swapBuffers() = 0; 
    virtual ~Window() {};
};

#endif
