#ifndef __PIPELINE_H__
#define __PIPELINE_H__

namespace JustRenderIt
{
  class Camera;

  /// Context of a rendering pipeline
  class Pipeline
  {
  public:
    Pipeline();
    virtual ~Pipeline();

    Camera* m_camera;
  };
};

#endif /* __PIPELINE_H__ */
