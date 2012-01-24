#include "Common.h"
#include "Utils/Util.h"
#include "Image.h"
#include "Texture.h"
#include "Renderer.h"
#include "Utils/Logger.h"
#include "Utils/Util.h"

using namespace JustRenderIt;

Texture* Texture::GetDefaultTexture()
{
  // Texture* result = SearchTexture("DefaultTexture");
  // if(result)
  //   return result;
  // else
  // {
  //   result = g_renderer->CreateTexture(Texture::MIPMAP, "DefaultTexture");
  //   result->Create(*ImageManager::InstanceRef().GetDefaultImage());
  //   assert(result->IsOK());
  //   return result;
  // }
  return NULL;
}

void Texture::Create(size_t width, size_t height, PixelFormat pf)
{
  m_width = width;
  m_height = height;
  m_format = pf;

  internalCreate();
}

void Texture::Create(Image &img)
{
  Create(img.GetWidth(), img.GetHeight(), img.GetPixelFormat());

  if(m_isOK)
    Upload(img);
}

Texture* Texture::CreateFromImage(Image& img)
{
  // Texture* result = g_renderer->CreateTexture();
  // result->Create(img);
  // return result;
  return NULL;
}

Texture* Texture::CreateFromFile(STRING filename)
{
  // Image* img = ImageManager::InstanceRef().LoadFromFile("", filename);
  // assert(img);
  // return CreateFromImage(*img);

  return NULL;
}
