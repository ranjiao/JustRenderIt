#include "Pipeline.h"
#include "Camera.h"

using namespace JustRenderIt;

Pipeline* JustRenderIt::g_pipeline = NULL;

JustRenderIt::Pipeline::Pipeline()
{
  m_camera = new Camera();

  // TODO: how to arrange multiple pipeline?
  assert(!g_pipeline);
  g_pipeline = this;
}

JustRenderIt::Pipeline::~Pipeline()
{
  SAFE_DELETE(m_camera);
}

void JustRenderIt::Pipeline::Render( double timeElapsed )
{

}
