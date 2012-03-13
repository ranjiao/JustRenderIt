#include "Common.h"
#include "CVar.h"
#include "App/AppBase.h"
#include "Utils/Util.h"
#include "Tokenizer.h"

using namespace JustRenderIt;

IMPL_SINGLETON(CVarManager);

CVarManager::CVarManager()
{

}

CVarManager::~CVarManager()
{

}

void CVarManager::Reset()
{
  m_cvars.clear();
}

bool CVarManager::LoadFromFile(STRING filename)
{
  Tokenizer t;
  if(!t.LoadFromFile(filename))
    return false;

  STRING key, value, eq;
  while(!t.EndOfFile())
  {
    key = t.NextToken();
    eq = t.NextToken();
    if(eq != "=")
    {
      LOG_WARNING2("CVar file syntax error in line %d.", t.CrtLine());
    }
    value = t.NextToken();

    CVar cvar(key, EMPTY_STRING, NULL);
    cvar.SetValue(value);
    m_cvars[key] = cvar;
  }
  return true;
}

bool CVarManager::LoadFromMemory(const char* data, unsigned length)
{
  return false;
}

CVar* CVarManager::Get(STRING name)
{
  IterCVar iter = m_cvars.find(name);
  if(iter != m_cvars.end())
    return &iter->second;
  return NULL;
}
