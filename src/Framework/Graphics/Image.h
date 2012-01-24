#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "../Common.h"
#include "Utils/Singleton.h"
#include <map>
#include <string>
#include "FreeImage.h"

namespace JustRenderIt
{
  enum ImageFileFormat
  {
    IFF_UNKNOWN = -1,
    IFF_BMP    = 0,
    IFF_ICO    = 1,
    IFF_JPEG  = 2,
    IFF_JNG    = 3,
    IFF_KOALA  = 4,
    IFF_LBM    = 5,
    IFF_IFF = IFF_LBM,
    IFF_MNG    = 6,
    IFF_PBM    = 7,
    IFF_PBMRAW  = 8,
    IFF_PCD    = 9,
    IFF_PCX    = 10,
    IFF_PGM    = 11,
    IFF_PGMRAW  = 12,
    IFF_PNG    = 13,
    IFF_PPM    = 14,
    IFF_PPMRAW  = 15,
    IFF_RAS    = 16,
    IFF_TARGA  = 17,
    IFF_TIFF  = 18,
    IFF_WBMP  = 19,
    IFF_PSD    = 20,
    IFF_CUT    = 21,
    IFF_XBM    = 22,
    IFF_XPM    = 23,
    IFF_DDS    = 24,
    IFF_GIF     = 25,
    IFF_HDR    = 26,
    IFF_FAXG3  = 27,
    IFF_SGI    = 28,
    IFF_EXR    = 29,
    IFF_J2K    = 30,
    IFF_JP2    = 31,
    IFF_PFM    = 32,
    IFF_PICT  = 33,
    IFF_RAW    = 34
  };

  enum PixelFormat {
    PIXEL_UNKNOWN,

    // Gray scale image, all bits in lumiance
    PIXEL_L8,

    // 16 bits rgb image
    PIXEL_R5G6B5,
    PIXEL_A1R5G5B5,

    // 24 bits
    PIXEL_R8G8B8,
    PIXEL_B8G8R8,

    // 32 bits
    PIXEL_R8G8B8A8,
    PIXEL_B8G8R8A8,

    // n x 16 bits
    PIXEL_RGB16,
    PIXEL_RGBA16,

    // float
    PIXEL_F32,
    PIXEL_F64,
    PIXEL_RGBF32,
    PIXEL_RGBAF32,
  };

  /// This class keeps meta information and the pointer of actual image data
  class Image
  {
  public:
    unsigned m_width, m_height;
    int m_mipMaps;
    int m_bpp;
    PixelFormat m_format;
    STRING m_name, m_filename;
    bool m_isOK;
    unsigned m_size;
    unsigned char* m_data;
  public:
    Image()
    {
      m_isOK = false;
    };

    Image(const Image &img)
    {
      m_width = img.m_width;
      m_height = img.m_height;
      m_bpp = img.m_bpp;
      m_mipMaps = img.m_mipMaps;

      m_name = img.m_name;
      m_filename = img.m_filename;

      m_isOK = img.m_isOK;
      m_data = img.m_data;
    };

    ~Image()
    {
      SAFE_DELETE_ARRAY(m_data);
    };

    bool       IsOK(){ return m_isOK; };
    unsigned   GetWidth(){ return m_width; };
    unsigned   GetHeight(){ return m_height; };
    int        GetBpp(){ return m_bpp; };
    STRING     GetName(){ return m_name; };
    STRING     GetFilename(){ return m_filename; };
    PixelFormat  GetPixelFormat(){ return m_format; };
    unsigned   GetSize(){ return m_size; };
  };

  class DLL_DECLARE ImageCodec
  {
  public:
    ImageCodec()
    {
      InitLibrary();
    };
    virtual ~ImageCodec()
    {
      DestroyLibrary();
    };

    virtual Image* LoadFromMemory(
      unsigned width, unsigned height, unsigned bpp) = 0;
    virtual Image* LoadFromFile(STRING filename) = 0;
    virtual bool SaveToFile(
      Image* image, STRING filename, ImageFileFormat iff) = 0;

    virtual bool InitLibrary(){ return false; };
    virtual bool DestroyLibrary(){ return false; };
  };

  class DLL_DECLARE ImageCodecFreeImage:
    public Singleton<ImageCodecFreeImage>,
    public ImageCodec
  {
  private:
    virtual bool InitLibrary();
    virtual bool DestroyLibrary();

  public:
    ImageCodecFreeImage();
    virtual ~ImageCodecFreeImage();
    virtual Image* LoadFromMemory(unsigned width, unsigned height,
      unsigned bpp);
    virtual Image* LoadFromFile(STRING filename);
    virtual bool SaveToFile(Image* image, STRING filename,
                                        ImageFileFormat fif);

    DECL_SINGLETON(ImageCodecFreeImage);

    static FREE_IMAGE_FORMAT GetFreeImageFileFormat(ImageFileFormat iff)
    {
      return static_cast<FREE_IMAGE_FORMAT>(iff);
    };
  };
}

#endif
