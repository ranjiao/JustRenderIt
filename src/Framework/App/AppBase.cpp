#include "AppBase.h"
#include "Graphics/Renderer.h"
#include "Graphics/OpenGL/OpenGLRenderer.h"
#include "Utils/Timer.h"
#include "../Graphics/Pipeline.h"
#include "../Graphics/Camera.h"

using namespace JustRenderIt;

JustRenderIt::AppBase* JustRenderIt::g_app = NULL;

bool JustRenderIt::AppBase::Tick()
{
  assert(g_renderer);

  Timer& timer = FrameTimer::InstanceRef();
  timer.Update();
  double timeElapsed = timer.TimeSinceLastUpdate();

  Update(timeElapsed);
  BeforeRender();

  g_renderer->Clear();
  Render(timeElapsed);
  Render2D(timeElapsed);
  g_renderer->SwapBuffer();
  AfterRender();

  return true;
}

void JustRenderIt::AppBase::InitApp()
{
  // TODO
  Pipeline* pipeline = new Pipeline();

  // for now we only have opengl renderer
  OpenGLRenderer::InstanceRef();

}

void JustRenderIt::AppBase::Update( double timeElapsed )
{
  Camera* camera = g_pipeline->m_camera;
  camera->Update();
}

bool JustRenderIt::AppBase::OnMouseMove( int x, int y, int lastX, int lastY )
{
  g_pipeline->m_camera->OnMouseMove(x, y);
  return true;
}

void JustRenderIt::AppBase::OnMouseButton( int x, int y, MouseButton btn, bool down )
{
  g_pipeline->m_camera->OnMouseClick(x, y, btn, down);
}

bool JustRenderIt::AppBase::OnKey( unsigned key, bool down )
{
  return false;
}

void JustRenderIt::AppBase::OnMouseWheel( int x, int y, int speed )
{

}
