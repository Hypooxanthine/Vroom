#include <gtest/gtest.h>
#include <string>

#include <Tools/Maths.h>
#include <Tools/String.h>
#include <Tools/TypeList.h>

// ----- vrm::FindString --------------------------------------------------

TEST(test_StringSearch, FindsSubstring)
{
  std::string haystack = "hello world";
  EXPECT_EQ(vrm::FindString<std::string>(haystack, std::string("world")), 6u);
  EXPECT_EQ(vrm::FindString<std::string>(haystack, std::string("hello")), 0u);
}

TEST(test_StringSearch, NotFoundReturnsNpos)
{
  std::string haystack = "hello";
  EXPECT_EQ(vrm::FindString<std::string>(haystack, std::string("xyz")), std::string::npos);
}

TEST(test_StringSearch, CaseInsensitiveMatch)
{
  std::string haystack = "Hello World";
  EXPECT_EQ(vrm::FindString<std::string>(haystack, std::string("WORLD"), true), 6u);
  EXPECT_EQ(vrm::FindString<std::string>(haystack, std::string("WORLD"), false), std::string::npos);
}

// ----- vrm::maths::NextMultipleInclusive --------------------------------

TEST(test_Maths, NextMultipleInclusive)
{
  EXPECT_EQ(vrm::maths::NextMultipleInclusive(10, 4), 12u);
  EXPECT_EQ(vrm::maths::NextMultipleInclusive(12, 4), 12u);
  EXPECT_EQ(vrm::maths::NextMultipleInclusive(1, 4), 4u);
  EXPECT_EQ(vrm::maths::NextMultipleInclusive(0, 4), 0u);
  EXPECT_EQ(vrm::maths::NextMultipleInclusive(8, 8), 8u);

  static_assert(vrm::maths::NextMultipleInclusive(10, 4) == 12);
}

// ----- vrm::TypeList ----------------------------------------------------

TEST(test_TypeList, HasType)
{
  using TL = vrm::TypeList<int, float, char>;

  EXPECT_TRUE(TL::HasType<int>());
  EXPECT_TRUE(TL::HasType<char>());
  EXPECT_FALSE(TL::HasType<double>());

  static_assert(TL::HasType<float>());
  static_assert(!TL::HasType<double>());
}

TEST(test_TypeList, PushFrontAndBack)
{
  constexpr auto front = vrm::TypeList<int>::CreatePushFront<char>();
  constexpr auto back  = vrm::TypeList<int>::CreatePushBack<double>();

  static_assert(decltype(front)::HasType<char>());
  static_assert(decltype(front)::HasType<int>());
  static_assert(decltype(back)::HasType<double>());
  static_assert(decltype(back)::HasType<int>());

  EXPECT_TRUE(decltype(front)::HasType<char>());
  EXPECT_TRUE(decltype(back)::HasType<double>());
}
