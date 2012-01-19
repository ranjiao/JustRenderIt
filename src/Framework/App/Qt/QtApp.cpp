#include "Common.h"
#include "QtApp.h"


JustRenderIt::QtApp* JustRenderIt::g_qtApp = NULL;

JustRenderIt::QtApp::QtApp(void)
{
  assert(!g_qtApp);
  g_qtApp = this;
}

JustRenderIt::QtApp::~QtApp(void)
{
  SAFE_DELETE(m_app);
}

void JustRenderIt::QtApp::Update( double timeElapsed )
{

}


void JustRenderIt::QtApp::Render( double timeElapsed )
{

}

void JustRenderIt::QtApp::StartLoop()
{
  m_app->exec();
}

void JustRenderIt::QtApp::Exit()
{

}

void JustRenderIt::QtApp::OnClose()
{

}

void JustRenderIt::QtApp::InitApp()
{
  m_app = new QApplication(m_argc, m_argv);

  AppBase::InitApp();
}

JustRenderIt::FrameBase* JustRenderIt::QtApp::InitFrame()
{
  return NULL;
}
