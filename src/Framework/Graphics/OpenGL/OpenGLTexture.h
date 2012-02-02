#ifndef __OPENGLTEXTURE_H__
#define __OPENGLTEXTURE_H__

#include "Common.h"
#include "GL/glew.h"
#include "../Texture.h"
//#include "../RenderTarget.h"

namespace JustRenderIt
{
  class DLL_DECLARE OpenGLTexture: public Texture
  {
  protected:
    GLuint m_textureID;
    GLenum m_glFormat;
  protected:
    virtual void internalCreate();
    virtual void internalDestroy();

  public:
    OpenGLTexture(TexParam_t param = MIPMAP_ONCE): Texture(param) {};
    ~OpenGLTexture(){ internalDestroy(); };

    GLuint GetTextureID(){ return m_textureID; };

    virtual void Upload(Image& img);
    virtual bool const Download(float* dest);
    virtual void Begin();
    virtual void End();

    static GLenum GetOpenGLInternalPixelFormat(PixelFormat pf);
    static GLenum GetOpenGLPixelFormat(PixelFormat pf);
  };
}

#endif
