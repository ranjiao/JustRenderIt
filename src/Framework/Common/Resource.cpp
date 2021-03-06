#include "Resource.h"
#include "Utils/Util.h"
#include "Utils/Logger.h"

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

HashHandle    JustRenderIt::g_handleResTable;
HashFilename  JustRenderIt::g_nameHandleTable;

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

  if(!filename.empty() &&
     g_nameHandleTable.find(filename) != g_nameHandleTable.end())
  {
    return Handle(g_nameHandleTable[filename]);
  }

  Handle h;
  h.Init(r->GetHash(), tag);
  g_nameHandleTable[filename] = h.GetHandle();
  g_handleResTable[h.GetHandle()] = r;

  return h;
}

char ResourceManager::RegisterResourceLoader(BaseResourceLoader* loader)
{
  static char counter = 0;
  return ++counter;
}

BaseResource* JustRenderIt::ResourceManager::GetResource( Handle h )
{
  if(g_handleResTable.find(h) == g_handleResTable.end())
    return false;
  return g_handleResTable[h];
}

bool JustRenderIt::ResourceManager::GetResourcePath(STRING& filename,
                                                    STRING folderName)
{
  Trim(filename);
  Trim(folderName);
  if(folderName[folderName.length()-1] == '/' ||
     folderName[folderName.length()-1] == '\\' )
    folderName = folderName.substr(0, folderName.length()-1);
  if(folderName[0] == '/')
    folderName = folderName.substr(1, folderName.length()-1);

  if(!FileExists(filename))
  {
    STRING newfilename = "../resources/";
    newfilename += folderName + "/" + filename;
    if(!FileExists(newfilename))
    {
      char str[256];
      sprintf(str, "Couldn't open \"%s\"", filename.c_str());
      LOG_ERROR1(str);
      return false;
    }
    filename = newfilename;
  }
  return true;
}
