#ifndef __QTAPP_H__
#define __QTAPP_H__

#include "../AppBase.h"

namespace JustRenderIt
{
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
    QApplication *m_app;
    int m_argc;
    char **m_argv;
  };

  extern QtApp *g_qtApp;
}

#endif /* __QTAPP_H__ */
