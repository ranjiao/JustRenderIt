#include "gtest/gtest.h"
#define CLIENT_SINGLETON
#include "Utils/Singleton.h"

using namespace std;
using namespace JustRenderIt;

int magic_number = 21294;
class __declspec(dllexport) SingletonTest: public Singleton<SingletonTest>
{
public:
  SingletonTest(){};
  virtual ~SingletonTest(){};

  int TestFunc()
  {
    return magic_number;
  }

  DECL_SINGLETON(SingletonTest);
};

IMPL_SINGLETON(SingletonTest);

TEST(Singleton, Create)
{
  EXPECT_EQ(SingletonTest::InstanceRef().TestFunc(), magic_number);

  EXPECT_THROW(new SingletonTest(), JRIException);

  SingletonTest* p = SingletonTest::Instance();
  SAFE_DELETE(p);
}
