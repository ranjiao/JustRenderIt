#include "HelloWorld.h"
#include "Utils/Util.h"
#include "Graphics/Renderer.h"
#include "Graphics/Pipeline.h"
#include "Graphics/Effect.h"

using namespace JustRenderIt;

APP_MAIN_IMPLEMENTATION(HelloWorld)

HelloWorld::HelloWorld()
{

}

HelloWorld::~HelloWorld()
{

}

void HelloWorld::Render(double timeElapsed)
{
  g_renderer->SetCamera(g_pipeline->m_camera);

  DrawGrid();
}

void HelloWorld::InitApp()
{
  NATIVE_APP::InitApp();

  m_effect = new CGEffect("illum.cgfx");
}
