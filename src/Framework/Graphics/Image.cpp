#include "Common.h"
#include "Image.h"
#include "Utils/Util.h"
#include "Utils/Logger.h"
#include "FreeImage.h"

using namespace JustRenderIt;

IMPL_SINGLETON(ImageCodecFreeImage);

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif,  const char* message)
{
  if(fif != FIF_UNKNOWN)
  {
    // TODO
//    LOG_ERROR1( freeimage_GetFormatFromFIF(fif) );
  }
}

ImageCodecFreeImage::ImageCodecFreeImage()
{

}

ImageCodecFreeImage::~ImageCodecFreeImage()
{

}

bool ImageCodecFreeImage::InitLibrary()
{
  FreeImage_Initialise();
  FreeImage_SetOutputMessage(FreeImageErrorHandler);
  return true;
}

bool ImageCodecFreeImage::DestroyLibrary()
{
  FreeImage_DeInitialise();
  return true;
}

Image* ImageCodecFreeImage::LoadFromFile(STRING filename)
{
  Image* imgData = NULL;
  unsigned char* imgPtr = NULL;

  FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str(), 0);
  FIBITMAP* fiBitmap = FreeImage_Load(fif, filename.c_str());
  if( !fiBitmap )
  {
    throw new JRIException("Error while loading image.");
  }

  imgData = new Image();
  imgData->m_width = FreeImage_GetWidth(fiBitmap);
  imgData->m_height = FreeImage_GetHeight(fiBitmap);
  imgData->m_mipMaps = 0;

  FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(fiBitmap);
  FREE_IMAGE_COLOR_TYPE colourType = FreeImage_GetColorType(fiBitmap);
  unsigned bpp = FreeImage_GetBPP(fiBitmap);

  switch(imageType)
  {
  case FIT_BITMAP:
    if (colourType == FIC_MINISWHITE || colourType == FIC_MINISBLACK)
    {
      FIBITMAP* newBitmap = FreeImage_ConvertToGreyscale(fiBitmap);
      // free old bitmap and replace
      FreeImage_Unload(fiBitmap);
      fiBitmap = newBitmap;
      // get new formats
      bpp = FreeImage_GetBPP(fiBitmap);
      colourType = FreeImage_GetColorType(fiBitmap);
    }
    // Perform any colour conversions for RGB
    else if (bpp < 8 || colourType == FIC_PALETTE || colourType == FIC_CMYK)
    {
      FIBITMAP* newBitmap =  NULL;
      if (FreeImage_IsTransparent(fiBitmap))
      {
        // convert to 32 bit to preserve the transparency
        // (the alpha byte will be 0 if pixel is transparent)
        newBitmap = FreeImage_ConvertTo32Bits(fiBitmap);
      }
      else
      {
        // no transparency - only 3 bytes are needed
        newBitmap = FreeImage_ConvertTo24Bits(fiBitmap);
      }

      // free old bitmap and replace
      FreeImage_Unload(fiBitmap);
      fiBitmap = newBitmap;
      // get new formats
      bpp = FreeImage_GetBPP(fiBitmap);
      colourType = FreeImage_GetColorType(fiBitmap);
    }

    // by this stage, 8-bit is greyscale, 16/24/32 bit are RGB[A]
    switch(bpp)
    {
    case 8:
      imgData->m_format = PIXEL_L8;
      break;
    case 16:
      // Determine 555 or 565 from green mask
      // cannot be 16-bit greyscale since that's FIT_UINT16
      if(FreeImage_GetGreenMask(fiBitmap) == FI16_565_GREEN_MASK)
      {
        imgData->m_format = PIXEL_R5G6B5;
      }
      else
      {
        // FreeImage doesn't support 4444 m_format so must be 1555
        imgData->m_format = PIXEL_A1R5G5B5;
      }
      break;
    case 24:
      // FreeImage differs per platform
      //     PF_BYTE_BGR[A] for little endian (== PF_ARGB native)
      //     PF_BYTE_RGB[A] for big endian (== PF_RGBA native)
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
      imgData->m_format = PIXEL_R8G8B8;
#else
      imgData->m_format = PIXEL_B8G8R8;
#endif
      break;
    case 32:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
      imgData->m_format = PIXEL_R8G8B8A8;
#else
      imgData->m_format = PIXEL_B8G8R8A8;
#endif
      break;
    };
    break;
  default:
    throw new JRIException("Unsupported image format.");
    break;
  }

  unsigned char* srcData = FreeImage_GetBits(fiBitmap);

//   // Final data - invert image and trim pitch at the same time
//   size_t dstPitch = imgData->m_width * ImageManager::GetNumElemBytes(
//     imgData->m_format);
//   imgData->m_size = dstPitch * imgData->m_height;
//   imgPtr = new unsigned char[imgData->m_size];

//   unsigned char* pSrc = srcData;
//   unsigned char* pDst = imgPtr;
//   // TODO: do we need to revert it?
// //   for (size_t y = 0; y < imgData->m_height; ++y)
// //   {
// //     pSrc = srcData + (imgData->m_height - y - 1) * srcPitch;
// //     memcpy(pDst, pSrc, dstPitch);
// //     pDst += dstPitch;
// //   }
//   memcpy(pDst, pSrc, imgData->m_height * dstPitch);

//   FreeImage_Unload(fiBitmap);

//   imgData->m_data = imgPtr;
//   imgData->m_bpp = bpp;
//   return imgData;
  return NULL;
}

Image* ImageCodecFreeImage::LoadFromMemory(unsigned width,
                                           unsigned height, unsigned bpp)
{
  assert(false);
  return NULL;
}

bool ImageCodecFreeImage::SaveToFile(Image* image, STRING filename,
                                     ImageFileFormat iff)
{
  FREE_IMAGE_FORMAT fif = ImageCodecFreeImage::GetFreeImageFileFormat(iff);
  FREE_IMAGE_TYPE fit;

  switch( image->GetPixelFormat() )
  {
  // standard bitmap
  case PIXEL_L8:
  case PIXEL_R5G6B5:
  case PIXEL_A1R5G5B5:
  case PIXEL_R8G8B8:
  case PIXEL_B8G8R8:
  case PIXEL_R8G8B8A8:
  case PIXEL_B8G8R8A8:
    fit = FIT_BITMAP;
    break;

  // n x 16 bits
  case PIXEL_RGB16:
    fit = FIT_RGB16;
    break;
  case PIXEL_RGBA16:
    fit = FIT_RGBA16;
    break;

  // float image
  case PIXEL_F32:
    fit = FIT_FLOAT;
    break;
  case PIXEL_F64:
    fit = FIT_DOUBLE;
    break;
  case PIXEL_RGBF32:
    fit = FIT_RGBF;
    break;
  case PIXEL_RGBAF32:
    fit = FIT_RGBAF;
    break;

  default:
    LOG_ERROR1("Unsupported pixel format for image saving");
    return false;
  }

  FreeImage_SetOutputMessage(FreeImageErrorHandler);
  FIBITMAP *bitmap = FreeImage_AllocateT(
      fit,
      static_cast<int>(image->GetWidth()),
      static_cast<int>(image->GetHeight()),
      image->GetBpp() );

  size_t srcPitch, dstPitch;
  srcPitch = dstPitch = FreeImage_GetPitch(bitmap);

  // Copy data, invert scanlines and respect FreeImage pitch
  unsigned char* pSrc;
  unsigned char* pDst = FreeImage_GetBits(bitmap);
  for (size_t y = 0; y < image->GetHeight(); ++y)
  {
    pSrc = image->m_data + (image->GetHeight() - y - 1) * srcPitch;
    memcpy(pDst, pSrc, srcPitch);
    pDst += dstPitch;
  }

  bool result = FreeImage_Save(fif, bitmap, filename.c_str()) != (bool)NULL;
  if( !result )
    LOG_ERROR2("Error while saving image: %s", filename.c_str());

  FreeImage_Unload(bitmap);

  return result;
}

