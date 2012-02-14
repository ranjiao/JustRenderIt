#include "OpenGLRenderer.h"
#include "../Camera.h"

using namespace JustRenderIt;

JustRenderIt::OpenGLRenderer* JustRenderIt::g_glRenderer;

IMPL_SINGLETON(OpenGLRenderer);

JustRenderIt::OpenGLRenderer::OpenGLRenderer()
{
  assert(!g_glRenderer);
  assert(!g_renderer);

  g_glRenderer = this;
  g_renderer = this;
}

JustRenderIt::OpenGLRenderer::~OpenGLRenderer()
{

}

bool JustRenderIt::OpenGLRenderer::InitAPI( bool VSync /*= false*/ )
{
  return false;
}

void JustRenderIt::OpenGLRenderer::ExitAPI()
{

}

void JustRenderIt::OpenGLRenderer::SwapBuffer()
{

}

void JustRenderIt::OpenGLRenderer::Clear( Color c /*= Color(0,0,0,0) */ )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void JustRenderIt::OpenGLRenderer::RenderText( int x, int y, STRING message )
{

}

void JustRenderIt::OpenGLRenderer::RenderText( int x, int y, const char *string, ... )
{

}

void JustRenderIt::OpenGLRenderer::Flush()
{

}

void JustRenderIt::OpenGLRenderer::Finish()
{

}

bool JustRenderIt::OpenGLRenderer::CheckError()
{
  return true;
}

void JustRenderIt::OpenGLRenderer::SetCamera(Camera* c)
{
  glViewport(0, 0, (GLsizei)c->GetViewportSize().x, 
    (GLsizei)c->GetViewportSize().y);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(c->GetMatrixModelView()));
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(c->GetMatrixProjection()));
}
