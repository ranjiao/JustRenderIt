#include "AppBase.h"
#include "Graphics/Renderer.h"
#include "Utils/Timer.h"

bool JustRenderIt::AppBase::Tick()
{
  Timer& timer = FrameTimer::InstanceRef();
  timer.Update();
  double timeElapsed = timer.TimeSinceLastUpdate();

  Update(timeElapsed);
  BeforeRender();
  Render(timeElapsed);
  Render2D(timeElapsed);
  g_renderer->SwapBuffer();
  AfterRender();

  return true;
}
