#include "OpenGLRenderer.h"

using namespace JustRenderIt;

JustRenderIt::OpenGLRenderer* JustRenderIt::g_glRenderer;

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
  return false;
}
