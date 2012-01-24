#include "Resource.h"

using namespace JustRenderIt;

void Handle::Init(unsigned int index, unsigned char tag)
{
  static int s_magic = 0;
  s_magic = (s_magic + 1) & MAX_MAGIC;
  if(s_magic == 0) // 0 is used as invalid handle
    s_magic++;

  m_magic = s_magic;
  m_index = index & MAX_INDEX;
  m_tag = tag & MAX_TAG;
}

IMPL_SINGLETON(ResourceManager);

ResourceManager* JustRenderIt::g_resourceMgr;

ResourceManager::ResourceManager()
{
  g_resourceMgr = this;
}

ResourceManager::~ResourceManager()
{

}

Handle ResourceManager::NewResource(BaseResource* r, char tag)
{
  assert(r);

  STRING filename = r->GetFilename();
  if(!filename.empty() and m_filenames.find(filename) != m_filenames.end())
  {
    return Handle(m_filenames[filename]);
  }

  Handle h;
  h.Init(r->GetHash(), tag);
  m_filenames[filename] = h.GetHandle();
  m_resources[h.GetHandle()] = r;

  return h;
}

char ResourceManager::RegisterResourceLoader(BaseResourceLoader* loader)
{
  static char counter = 0;
  return ++counter;
}
