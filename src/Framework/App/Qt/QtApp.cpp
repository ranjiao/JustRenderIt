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
  m_widget = new QtGlWidget;
  m_widget->show();

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

JustRenderIt::QtGlWidget::QtGlWidget( QWidget *parent /*= 0*/ ) :
QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  // TODO: is there other ways? Timer seems not efficient enough
  m_timer = new QTimer(this);
  connect( m_timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
  m_timer->start(10);
}

JustRenderIt::QtGlWidget::~QtGlWidget()
{

}

void JustRenderIt::QtGlWidget::SwapBuffer()
{

}

void JustRenderIt::QtGlWidget::initializeGL()
{

}

void JustRenderIt::QtGlWidget::paintGL()
{
  //glBegin(GL_TRIANGLES);
  //glVertex3f( 0.0f, 1.0f, 0.0f);
  //glVertex3f(-1.0f,-1.0f, 0.0f);
  //glVertex3f( 1.0f,-1.0f, 0.0f);
  //glEnd(); 
}

void JustRenderIt::QtGlWidget::resizeGL( int width, int height )
{

}

QSize JustRenderIt::QtGlWidget::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize JustRenderIt::QtGlWidget::sizeHint() const
{
  return QSize(400, 400);
}
