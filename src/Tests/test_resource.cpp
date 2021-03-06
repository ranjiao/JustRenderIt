#include "Common.h"
#include "Common/Resource.h"
#include "gtest/gtest.h"

using namespace std;
using namespace JustRenderIt;

class TestResource: public BaseResource
{
  int m_index;
public:
  TestResource()
  {
    static int index = 0;
    m_index = index++;

    m_filename = "filename 1";
  };

  virtual char GetHash()
  {

    return m_index;
  }
};

class TestResourceLoader: public BaseResourceLoader
{
  int m_tag;
public:
  TestResourceLoader()
  {
    m_tag = g_resourceMgr->RegisterResourceLoader(this);
  }

  char GetTag()
  {
    return m_tag;
  }

  STRING GetTagName()
  {
    return "Test resource";
  }

  virtual BaseResource* doLoadFromFile(STRING filename)
  {
    return new TestResource;
  }

  virtual void UnLoad(BaseResource* r)
  {
    SAFE_DELETE(r);
  };
};

TEST(Handle, Create)
{
  const unsigned int index = 123;
  const unsigned char tag = 2;
  Handle h1, h2;

  h1.Init(index, tag);
  h2.Init(index+1, tag);
  EXPECT_TRUE(h1.GetIndex() == index);
  EXPECT_TRUE(h1.GetTag() == tag);
  EXPECT_TRUE(!h1.IsNull());

  EXPECT_TRUE(h2.GetIndex() == h1.GetIndex()+1);
  EXPECT_TRUE(h2.GetTag() == h1.GetTag());
  EXPECT_TRUE(h2.GetMagic() == h1.GetMagic()+1);
  EXPECT_TRUE(h2.GetHandle() != h1.GetHandle());
  EXPECT_TRUE(h2 != h1);
};

TEST(Resource, Create)
{
  TestResourceLoader* loader = new TestResourceLoader();

  Handle h1 = loader->LoadFromFile("file1");
  TestResource* r = (TestResource*)(g_resourceMgr->GetResource(h1));

  delete loader;
  EXPECT_TRUE(r != NULL);
};

TEST(ResourceManager, LoadFile)
{
  STRING filename = "illum.cgfx", filename2 = "notexists",
    type1 = "shaders", type2 = "shaders/", type3 = "shaders\\";

  STRING result1 = filename;
  EXPECT_TRUE(ResourceManager::GetResourcePath(result1, type1));

  STRING result2 = filename;
  EXPECT_TRUE(ResourceManager::GetResourcePath(result2, type2));

  STRING result3 = filename;
  EXPECT_TRUE(ResourceManager::GetResourcePath(result3, type3));

  EXPECT_TRUE(result1 == result2);
  EXPECT_TRUE(result2 == result3);
};
