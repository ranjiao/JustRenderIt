#include "Common.h"
#include "OpenGLTexture.h"
#include "Graphics/OpenGL/OpenGLRenderer.h"
#include "Utils/Util.h"
#include "Utils/Logger.h"

#include <iostream>

using namespace JustRenderIt;

void OpenGLTexture::internalCreate()
{
  assert(m_width >0 && m_height >0);
  assert(m_format != PIXEL_UNKNOWN);

  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);

  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1 );

  // Set some misc default parameters so NVidia won't complain, these
  // can of course be changed later
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, Image::GetComponentCount(m_format),
               m_width, m_height, 0, GetOpenGLPixelFormat(m_format),
               GL_UNSIGNED_BYTE, 0);

  g_renderer->CheckError();

  switch(m_param)
  {
  case NONE:
    break;
  case MIPMAP_ONCE:
    glGenerateMipmapEXT(GL_TEXTURE_2D);
    break;
  case MIPMAP_DYNAMIC:
    // we don't use mipmap for dynamic texture, because this will
    // cause terrible performance issue on some video card.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
  default:
    assert(false);
    break;
  }

  m_isOK = g_renderer->CheckError();
}

void OpenGLTexture::internalDestroy()
{
  assert(m_isOK && m_textureID);

  if(! (m_isOK && m_textureID) )
    return;

  glDeleteTextures(1, &m_textureID);
  m_isOK = false;
}

void OpenGLTexture::Upload(Image& img)
{
  m_glFormat = OpenGLTexture::GetOpenGLPixelFormat(img.GetPixelFormat());

  GLenum error = gluBuild2DMipmaps(GL_TEXTURE_2D, Image::GetComponentCount(img.m_format),
                                   m_width, m_height, m_glFormat, GL_UNSIGNED_BYTE,
                                   img.m_data);

  if( error )
  {
    const char* string = (const char*)gluErrorString(error);
    LOG_ERROR1(string);
    //ErrorMsg(string);
  };

  glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::Begin()
{
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void OpenGLTexture::End()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

/* GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA,
GL_LUMINANCE, and GL_LUMINANCE_ALPHA.*/
GLenum OpenGLTexture::GetOpenGLPixelFormat(PixelFormat pf)
{
  GLenum format;
  switch(pf)
  {
  case PIXEL_L8:
  case PIXEL_F32:
  case PIXEL_F64:
    format = GL_LUMINANCE;
    break;
  case PIXEL_B8G8R8:
    format = GL_BGR;
    break;
  case PIXEL_B8G8R8A8:
  case PIXEL_R8G8B8A8:
    format = GL_BGRA;
    break;
  default:
    format = GL_BGRA;
    LOG_WARNING1("Unknown pixel format while uploading image");
    break;
  }

  return format;
}

bool const JustRenderIt::OpenGLTexture::Download( float* dest )
{
  Begin();
  glGetTexImage(GL_TEXTURE_2D, 0, GetOpenGLPixelFormat(m_format), GL_FLOAT, (void*)dest);
  return g_renderer->CheckError();
  End();
}
