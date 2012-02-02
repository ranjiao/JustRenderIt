#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Utils/Singleton.h"

namespace JustRenderIt
{
  class DLL_DECLARE Color
  {
  public:
    Color()
    {
      r = g = b = a = 1.0f;
    };

    Color(float red, float green, float blue, float alpha)
    {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    };

    Color& operator += (const Color& c)
    {
      Color* cc = new Color;
      cc->r += c.r;
      cc->g += c.g;
      cc->b += c.b;
      cc->a += c.a;
      return *cc;
    };

    Color& operator -= (const Color& );
    Color& operator *= (float );
    Color& operator /= (float );

    Color operator + ( const Color& ) const;
    Color operator - ( const Color& ) const;
    Color operator * ( float ) const;
    Color operator / ( float ) const;

    bool operator == ( const Color& ) const;
    bool operator != ( const Color& ) const;

    operator float* ()
    { 
      return rgba; 
    };

    union
    {
      struct{ float r, g, b, a; };
      float rgba[4];
    };
  };
  
  static const Color BLACK  = Color(0.f, 0.f, 0.f, 1.0f);
  static const Color WHITE  = Color(1.f, 1.f, 1.f, 1.0f);
  static const Color RED    = Color(1.f, 0.f, 0.f, 1.0f);
  static const Color GREEN  = Color(0.f, 1.f, 0.f, 1.0f);
  static const Color BLUE  = Color(0.f, 0.f, 1.f, 1.0f);
  static const Color SOFT_BLUE = Color(135/255.f, 135/255.f, 233/255.f, 1.f);

  class DLL_DECLARE Renderer
  {
  public:
    Renderer(){};
    virtual ~Renderer(){};

    typedef enum
    {
      OPENGL,
      DX9,
      DX10,
      DX11,
    } GRAPHIC_API_TYPE;
    virtual GRAPHIC_API_TYPE GetRendererType() = 0;
    virtual bool InitAPI(bool VSync = false) = 0;
    virtual void ExitAPI() = 0;

    /// Rendering
    virtual void SwapBuffer() = 0;
    virtual void RenderDebugInfo( double timeElapsed ); //< Render infomaiton at right top cornor
    virtual void Clear(Color c = Color(0,0,0,0) ) = 0;  //< Clear the buffer
//    virtual void DrawElements(const Primitives primitives, const int firstIndex, const int nIndices, const int firstVertex, const int nVertices) = 0;
    virtual void RenderText(int x, int y, STRING message) = 0;
    virtual void RenderText(int x, int y, const char *string, ...) = 0;
    virtual void Flush() = 0;
    virtual void Finish() = 0;

    /// return true if everything is ok
    virtual bool CheckError() = 0;
  };

  extern DLL_DECLARE Renderer* g_renderer;
}

#endif /* __RENDERER_H__ */
