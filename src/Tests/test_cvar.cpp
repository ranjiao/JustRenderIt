#include "Utils/CVar.h"
#include "Common.h"
#include "gtest/gtest.h"

using namespace std;
using namespace JustRenderIt;

TEST(CVar, LoadFromFile)
{
  const char* filename = "test_cvar.txt";

  CVarManager* mgr = CVarManager::Instance();
  mgr->Reset();
  mgr->LoadFromFile(filename);

  CVar cvar1 = mgr->Get("title");
  CVar cvar2 = mgr->Get("width");
  CVar cvar3 = mgr->Get("time");
}

TEST(CVar, LoadFromMemory)
{
}

TEST(CVar, Create)
{

}
