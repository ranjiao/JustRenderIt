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
  AppBase::Update(timeElapsed);
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

  setMouseTracking(true);
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
  g_app->Tick();
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

void JustRenderIt::QtGlWidget::mouseMoveEvent( QMouseEvent * event )
{
  static int last_x = 0, last_y = 0;

  g_qtApp->OnMouseMove(event->x(), event->y(), last_x, last_y);

  last_x = event->x();
  last_y = event->y();
}

JustRenderIt::MouseButton MouseButtonConvert(Qt::MouseButton in_btn)
{
  JustRenderIt::MouseButton btn;
  switch(in_btn)
  {
  case Qt::NoButton:
    return JustRenderIt::MOUSE_NONE;
  case Qt::LeftButton:
    btn = JustRenderIt::MOUSE_LEFT;
    break;
  case Qt::MidButton:
    btn = JustRenderIt::MOUSE_MIDDLE;
    break;
  case Qt::RightButton:
    btn = JustRenderIt::MOUSE_RIGHT;
    break;
  }
  return btn;
}

void JustRenderIt::QtGlWidget::mousePressEvent( QMouseEvent * event )
{
  
  g_qtApp->OnMouseButton(event->x(), event->y(), 
    MouseButtonConvert(event->button()), true);
}

void JustRenderIt::QtGlWidget::mouseReleaseEvent( QMouseEvent * event )
{
  g_qtApp->OnMouseButton(event->x(), event->y(), 
    MouseButtonConvert(event->button()), false);
}
