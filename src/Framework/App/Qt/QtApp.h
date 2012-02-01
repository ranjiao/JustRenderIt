#ifndef __QTAPP_H__
#define __QTAPP_H__

#include "Common.h"
#include <QGLWidget>
#include "../AppBase.h"

namespace JustRenderIt
{
  class QtGlWidget;

  /// App wrapper for Qt
  class DLL_DECLARE QtApp: public AppBase
  {
  public:
    QtApp(void);
    ~QtApp(void);

    // AppBase Implementation
    virtual void InitApp();
    virtual FrameBase* InitFrame();
    virtual void Update(double timeElapsed);
    virtual void Render(double timeElapsed);
    virtual void StartLoop();
    virtual void Exit();
    virtual void OnClose();
  protected:
    QApplication  *m_app;
    int           m_argc;
    char          **m_argv;
    QtGlWidget    *m_widget;
  };

  extern QtApp *g_qtApp;

  /// QtWidget which actually get rendering events
  class DLL_DECLARE QtGlWidget : public QGLWidget
  {
	  Q_OBJECT
  public:
	  explicit QtGlWidget(QWidget *parent = 0);
    virtual ~QtGlWidget();

    virtual void SwapBuffer();  // qt will swap buffer automaticlly

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
  protected:
	  void initializeGL();
	  void paintGL();
	  void resizeGL(int width, int height);
  protected:
    QTimer *m_timer;
  signals:

	  public slots:

  };

}

#endif /* __QTAPP_H__ */
