#ifndef __OpenGLRenderer_h__
#define __OpenGLRenderer_h__

#include "Graphics/Renderer.h"

namespace JustRenderIt
{
  class DLL_DECLARE OpenGLRenderer: public Renderer
  {
  protected:
  public:
    OpenGLRenderer();;
    virtual ~OpenGLRenderer();;

    virtual GRAPHIC_API_TYPE GetRendererType()
    {
      return Renderer::OPENGL;
    };
    virtual bool InitAPI(bool VSync = false);
    virtual void ExitAPI();

    ///Rendering
    virtual void SwapBuffer();
    virtual void Clear(Color c = Color(0,0,0,0) );
//    virtual void DrawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices);
    virtual void RenderText(int x, int y, STRING message);
    virtual void RenderText(int x, int y, const char *string, ...);
    virtual void Flush();
    virtual void Finish();

    virtual bool CheckError();
  };

  extern DLL_DECLARE OpenGLRenderer* g_glRenderer;
}

#endif // __OpenGLRenderer_h__
