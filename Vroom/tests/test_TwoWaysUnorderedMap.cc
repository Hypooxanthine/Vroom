#include <gtest/gtest.h>
#include <string>

#include <Tools/TwoWaysUnorderedMap.h>

// Note: TwoWaysUnorderedMap is declared in the global namespace.

TEST(test_TwoWaysUnorderedMap, InsertAndLookupBothWays)
{
  TwoWaysUnorderedMap<int, std::string> map;
  map.insert({ 1, "one" });
  map.insert({ 2, "two" });

  EXPECT_TRUE(map.containsLeft(1));
  EXPECT_TRUE(map.containsLeft(2));
  EXPECT_FALSE(map.containsLeft(3));

  EXPECT_TRUE(map.containsRight("one"));
  EXPECT_TRUE(map.containsRight("two"));
  EXPECT_FALSE(map.containsRight("three"));

  EXPECT_EQ(map.getRight(1), "one");
  EXPECT_EQ(map.getRight(2), "two");
  EXPECT_EQ(map.getLeft("one"), 1);
  EXPECT_EQ(map.getLeft("two"), 2);
}

TEST(test_TwoWaysUnorderedMap, InitializerListConstructor)
{
  TwoWaysUnorderedMap<int, std::string> map = {
    { 1, "one" }, { 2, "two" }, { 3, "three" }
  };

  EXPECT_EQ(map.getRight(3), "three");
  EXPECT_EQ(map.getLeft("one"), 1);
}

TEST(test_TwoWaysUnorderedMap, DuplicateLeftKeyAsserts)
{
  TwoWaysUnorderedMap<int, std::string> map;
  map.insert({ 1, "one" });

  EXPECT_ANY_THROW(map.insert({ 1, "uno" }));
}

TEST(test_TwoWaysUnorderedMap, DuplicateRightKeyAsserts)
{
  TwoWaysUnorderedMap<int, std::string> map;
  map.insert({ 1, "one" });

  EXPECT_ANY_THROW(map.insert({ 2, "one" }));
}
