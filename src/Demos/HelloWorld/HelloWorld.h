#ifndef __HELLOWORLD_H__
#define __HELLOWORLD_H__

#include "Common.h"
#include "App/AppBase.h"
#include "App/Qt/QtApp.h"

namespace JustRenderIt
{
  class Effect;
};

class HelloWorld: public NATIVE_APP
{
public:
  HelloWorld();
  ~HelloWorld();

  virtual void InitApp();
  virtual void Render(double timeElapsed);
protected:
  JustRenderIt::Effect* m_effect;
};

#endif /* __HELLOWORLD_H__ */
