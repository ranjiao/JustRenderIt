#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Utils/Singleton.h"

namespace JustRenderIt
{
  class Renderer: Singleton<Renderer>
  {
  public:
    Renderer(){};
    virtual ~Renderer(){};

    DECL_SINGLETON(Renderer);
  };

  extern Renderer* g_renderer;
}

#endif /* __RENDERER_H__ */
