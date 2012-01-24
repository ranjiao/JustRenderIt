#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Common.h"
#include "Image.h"

namespace JustRenderIt
{
  class DLL_DECLARE Texture: public IObject
  {
  public:
    enum TexParam_t
    {
      NONE,
      MIPMAP_ONCE,
      MIPMAP_DYNAMIC,
      TEX_PARAM_COUNT,
    };

    Texture(TexParam_t param = MIPMAP_ONCE)
    {
      m_width = m_height = (size_t)-1;
      m_isOK = false;
      m_format = PIXEL_UNKNOWN;
      m_param = param;
    }
    virtual ~Texture(){};

    static Texture* GetDefaultTexture();
    static Texture* CreateFromImage(Image& img);
    static Texture* CreateFromFile(STRING filename);

    void Create(size_t width, size_t height, PixelFormat pf);
    void Create(Image &img);
    virtual void Upload(Image& img) = 0;
    virtual bool const Download(float* dest) = 0;
    virtual void Begin() = 0;
    virtual void End() = 0;

    size_t GetWidth(){ return m_width; };
    size_t GetHeight(){ return m_height; };
    PixelFormat GetPixelFormat(){ return m_format; };
    bool IsOK(){ return m_isOK; };
  protected:
    size_t m_width, m_height;
    PixelFormat m_format;
    TexParam_t m_param;
  };
}

#endif
