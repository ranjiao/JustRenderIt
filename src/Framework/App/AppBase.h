#ifndef __APPBASE_H__
#define __APPBASE_H__

#include <string>
#include "Utils/Logger.h"
#include "../Common.h"

#if defined USE_GUI_QT
#define NATIVE_APP JustRenderIt::QtApp

#ifndef WIN32
#define APP_MAIN_IMPLEMENTATION(klass)          \
  int main(int argc, char *argv[])              \
  {                                             \
    JustRenderIt::QtApp *app = new klass();     \
    app->InitApp();                             \
    app->StartLoop();                           \
    delete app;                                 \
    return 0;                                   \
  }
#else
#define APP_MAIN_IMPLEMENTATION(klass)                          \
  int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hLastInst,  \
                     LPSTR lpszCmdLine, int nCmdShow)           \
  {                                                             \
    JustRenderIt::QtApp *app = new klass();                     \
    app->InitApp();                                             \
    app->StartLoop();                                           \
    delete app;                                                 \
    return 0;                                                   \
  }
#endif

#elif defined WIN32
#define NATIVE_APP JustRenderIt::WindowsApp
#define APP_MAIN_IMPLEMENTATION(klass)                                \
  int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hLastInst,        \
                     LPSTR lpszCmdLine, int nCmdShow)                 \
  {                                                                   \
    JustRenderIt::WindowsApp* app = new klass();                      \
    app->SetInitParams(hThisInst, hLastInst, lpszCmdLine, nCmdShow);  \
    app->InitApp();                                                   \
    app->StartLoop();                                                 \
    delete app;                                                       \
    return 0;                                                         \
  }
#elif defined LINUX

#else
#error "Unknown OS"
#endif

namespace JustRenderIt
{
  class FrameBase;

  enum MouseButton
  {
    MOUSE_NONE = 0,
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT,

    MOUSE_BUTTON_COUNT,
  };

  class AppBase;
  extern DLL_DECLARE AppBase* g_app;

  /// Base class for application
  class DLL_DECLARE AppBase
  {
  protected:
    // virtual bool loadRenderer(Renderer::GRAPHIC_TYPE type);
    FrameBase *m_frame;
  public:
    AppBase(){ g_app = this; };
    virtual ~AppBase(){};

    /// Everything is initialized in this function.
    virtual void InitApp();
    virtual FrameBase* InitFrame(){ return NULL; };
    virtual FrameBase* GetFrame(){ return m_frame; };
    virtual void StartLoop() = 0;
    virtual void Exit() = 0;

    /// Tick function called by Frame once a frame
    virtual bool Tick();

    /// This routine should be called every frame.
    virtual void Update(double timeElapsed);

    /// Caled before every frame is rendered
    virtual void BeforeRender(){};

    /// Actually render the scene, called once per frame.
    virtual void Render(double timeElapsed) = 0;

    /// Render 2D stuff like GUI, text, etc.
    virtual void Render2D(double timeElapsed){};

    /// Called after every frame is rendered
    virtual void AfterRender(){};

    /// TODO: Return arguments from command line
    virtual STRING GetCmdArguments()
    {
      LOG_WARNING1("GetCmdArguments not implemented!");
      return EMPTY_STRING;
    };

    virtual void OnClose() = 0;
    virtual bool OnKey(unsigned key, bool down);;

    /** Callback function for mouse move event.
    @param x x coordinate of the mouse's current position
    @param y y coordinate of the mouse's current position
    @param lastX x coordinate of the position of last frame
    @param lastY y coordinate of the position of last frame
    **/
    virtual bool OnMouseMove(int x, int y, int lastX, int lastY);;

    /** Callback function for mouse click event
    @param x x coordinate of the mouse's position
    @param y y coordinate of the mouse's position
    @param btn which button is clicked
    @param down is this button pressed down
    **/
    virtual void OnMouseButton(int x, int y, MouseButton btn, bool down);;

    /// Callback function of mouse wheel
    virtual void OnMouseWheel(int x, int y, int speed);;

    /// Callback function of windows resize event.
    virtual void OnResize(int width, int height){};

    /// Callback function of windows close event.
    void CloseWindow(const bool quit, const bool canUnload);
  public:
    /// Reset the camera to the default one.
    void ResetCamera();
  };

  /** Base class for an rendering window. It could be an widget embedded
  in another window, or a standalone window.
  **/
  class DLL_DECLARE FrameBase: public BaseObject
  {
  protected:
    glm::vec2 m_pos, m_size;
    STRING    m_caption;
    AppBase   *m_app;
  public:
    FrameBase();
    FrameBase(glm::vec2 position, glm::vec2 size, STRING caption);

    virtual void SwapBuffer() = 0;

    void RegisterApp(AppBase* app){ m_app = app; };

    STRING GetCaption(){ return m_caption; };
    void SetCaption(STRING caption){ m_caption = caption; };

    glm::vec2 GetPosition(){ return m_pos; };
    void SetPosition(glm::vec2 pos){ m_pos = pos; };

    glm::vec2 GetSize(){ return m_size; };
    void SetSize(glm::vec2 size){ m_size = size; };
  };
}

#endif
