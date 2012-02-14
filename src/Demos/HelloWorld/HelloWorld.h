#ifndef __HELLOWORLD_H__
#define __HELLOWORLD_H__

#include "Common.h"
#include "App/AppBase.h"
#include "App/Qt/QtApp.h"

using namespace JustRenderIt;

class HelloWorld: public NATIVE_APP
{
public:
  HelloWorld();
  ~HelloWorld();

  virtual void Render(double timeElapsed);
};

#endif /* __HELLOWORLD_H__ */
