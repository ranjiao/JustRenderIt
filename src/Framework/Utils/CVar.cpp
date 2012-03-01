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
  t.LoadFromFile(filename);

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
  }
  return false;
}

bool CVarManager::LoadFromMemory(const char* data, unsigned length)
{
  return false;
}

CVar CVarManager::Get(STRING name)
{
  CVar result;
  return result;
}

void CVarManager::Set(CVar v)
{

}
