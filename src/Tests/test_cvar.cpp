#include "Utils/CVar.h"
#include "Common.h"
#include "gtest/gtest.h"

using namespace std;
using namespace JustRenderIt;

TEST(CVar, LoadFromFile)
{
  const char* filename = "../src/Tests/test_cvar.txt";

  CVarManager* mgr = CVarManager::Instance();
  mgr->Reset();
  ASSERT_TRUE(mgr->LoadFromFile(filename));

  CVar* cvar1 = mgr->Get("title");
  EXPECT_TRUE(cvar1->GetValues() == "test title");

  CVar* cvar2 = mgr->Get("width");
  EXPECT_EQ(cvar2->GetValuei(), 123);

  CVar* cvar3 = mgr->Get("time");
  EXPECT_TRUE( JustRenderIt::equal(cvar3->GetValuef(), 111.254f) );
}

TEST(CVar, LoadFromMemory)
{
}

TEST(CVar, Create)
{

}
