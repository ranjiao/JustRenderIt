#include "Renderer.h"

using namespace JustRenderIt;

JustRenderIt::Renderer* JustRenderIt::g_renderer;

void JustRenderIt::Renderer::RenderDebugInfo( double timeElapsed )
{
//   char debugBuf[BUFFER_LENGTH];
// 
//   glm::vec3 cameraPos = g_app->GetCamera()->GetEye();
//   glm::vec3 centerPos = g_app->GetCamera()->GetCenter();
//   sprintf(debugBuf, "Camera pos: (%4.2f, %4.2f, %4.2f), looking at: (%4.2f, %4.2f, %4.2f)\n, FPS: %4.2f",
//     cameraPos.x, cameraPos.y, cameraPos.z, centerPos.x, centerPos.y,
//     centerPos.z, FrameTimer::InstanceRef().GetFPS());
// 
//   this->RenderText(0, 0, debugBuf);

}
