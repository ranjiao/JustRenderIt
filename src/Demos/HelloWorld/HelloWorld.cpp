#include "HelloWorld.h"
#include "Utils/Util.h"
#include "Graphics/Renderer.h"
#include "Graphics/Pipeline.h"

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

