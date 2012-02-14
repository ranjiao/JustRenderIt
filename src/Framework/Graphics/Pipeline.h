#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "Common.h"

namespace JustRenderIt
{
  class Camera;

  /// Context of a rendering pipeline
  class Pipeline
  {
  public:
    Pipeline();
    virtual ~Pipeline();

    virtual void Render(double timeElapsed);

    Camera* m_camera;
  };

  DLL_DECLARE extern Pipeline* g_pipeline;
};

#endif /* __PIPELINE_H__ */
