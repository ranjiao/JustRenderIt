#include "Utils/Tokenizer.h"
#include "Common.h"
#include "gtest/gtest.h"

using namespace std;
using namespace JustRenderIt;

const char* filename = "../src/Tests/test_tokenizer.txt";
const unsigned length = FileLength(filename);

TEST(Tokenizer, LoadFromMemory)
{
  char in_string[] = "this is a test string = 123;";

  Tokenizer t1, t2, t3;
  t1.LoadFromMemory(in_string, strlen(in_string));
  t2.LoadFromMemory(in_string);
  t3.LoadFromMemory(in_string, 4);

  EXPECT_TRUE(t1.GetLength() == strlen(in_string));
  EXPECT_TRUE(strlen(t1.GetBuffer()) == t1.GetLength());
  EXPECT_TRUE(t1.GetLength() == t2.GetLength());
  EXPECT_TRUE(strlen(t2.GetBuffer()) == t2.GetLength());

  EXPECT_TRUE(strlen(t3.GetBuffer()) == t3.GetLength());
  EXPECT_TRUE(strcmp(t3.GetBuffer(), "this") == 0);
}

TEST(Tokenizer, LoadFromFile)
{
  Tokenizer t;
  EXPECT_TRUE(t.LoadFromFile(filename));

  EXPECT_EQ(length, t.GetLength());
}

TEST(Tokenizer, SkipDelim)
{
  char in_string[] = "    this is a test string = 123;";

  Tokenizer t;
  t.LoadFromMemory(in_string, strlen(in_string));

  char* ptr = t.GetBuffer();
  unsigned count = t.SkipDelim(&ptr);

  EXPECT_TRUE( strcmp( in_string+count, ptr ) == 0 );
}

TEST(Tokenizer, SkipNoDelim)
{
  const char in_string[] = "this is a test string = 123;";

  Tokenizer t;
  t.LoadFromMemory(in_string, strlen(in_string));

  char* ptr = t.GetBuffer();
  unsigned count = t.SkipNoDelim(&ptr);
  EXPECT_TRUE(count == 4);

  const char in_string2[] = "    another string";
  Tokenizer t2;
  t2.LoadFromMemory(in_string2);
  char* ptr2 = t2.GetBuffer();
  unsigned count2 = t2.SkipNoDelim(&ptr2);
  EXPECT_TRUE(count2 == 0);
}

TEST(Tokenizer, SkipLine)
{
  const char in_string[] = "123456\n11\n11";

  Tokenizer t;
  t.LoadFromMemory(in_string);

  char* ptr = t.GetBuffer();
  t.SkipLine(&ptr);
  EXPECT_TRUE( strcmp(ptr, "11\n11") == 0);

  t.SkipLine(&ptr);
  EXPECT_TRUE( strcmp(ptr, "11") == 0);
}

TEST(Tokenizer, CrtToken)
{
  const char in_string[] = " 1this is \n exam # and comment \n other content";

  Tokenizer t;
  t.LoadFromMemory(in_string);

  t.Next();
  STRING result = t.CrtToken();
  EXPECT_TRUE( result == "1this" );

  t.Next();
  result = t.CrtToken();
  EXPECT_TRUE( result == "is" );

  t.Next();
  result = t.CrtToken();
  EXPECT_TRUE( result == "exam" );

  t.Next();
  result = t.CrtToken();
  EXPECT_TRUE( result == "other" );
}
