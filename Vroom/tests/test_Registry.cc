#include <gtest/gtest.h>

#include <Tools/Registry.h>

using vrm::Registry;

TEST(test_Registry, SubmitAndGet)
{
  Registry<int, int> reg;

  reg.startRegistering();
  reg.submit(1, 10);
  reg.submit(2, 20);
  reg.endRegistering();

  EXPECT_TRUE(reg.contains(1));
  EXPECT_TRUE(reg.contains(2));
  EXPECT_FALSE(reg.contains(3));
  EXPECT_EQ(reg.get(1), 10);
  EXPECT_EQ(reg.get(2), 20);
}

TEST(test_Registry, NewIdsTrackedOnFirstFrame)
{
  Registry<int, int> reg;

  reg.startRegistering();
  reg.submit(1, 10);
  reg.submit(2, 20);
  reg.endRegistering();

  EXPECT_EQ(reg.getLastFrameNewIds().size(), 2u);
  EXPECT_TRUE(reg.getLastFrameNewIds().contains(1));
  EXPECT_TRUE(reg.getLastFrameNewIds().contains(2));
  EXPECT_TRUE(reg.getLastFrameRemovedIds().empty());
}

TEST(test_Registry, UnusedEntriesAreEvictedNextFrame)
{
  Registry<int, int> reg;

  reg.startRegistering();
  reg.submit(1, 10);
  reg.submit(2, 20);
  reg.endRegistering();

  // Second frame: only refresh key 1 ; key 2 is left out and must be evicted.
  reg.startRegistering();
  reg.submit(1, 11);
  reg.endRegistering();

  EXPECT_TRUE(reg.contains(1));
  EXPECT_EQ(reg.get(1), 11);
  EXPECT_FALSE(reg.contains(2));

  EXPECT_TRUE(reg.getLastFrameRemovedIds().contains(2));
  EXPECT_TRUE(reg.getLastFrameNewIds().empty());
}

TEST(test_Registry, NotifyUsedKeepsEntryWithoutResubmitting)
{
  Registry<int, int> reg;

  reg.startRegistering();
  reg.submit(42, 100);
  reg.endRegistering();

  reg.startRegistering();
  reg.notifyUsed(42);
  reg.endRegistering();

  EXPECT_TRUE(reg.contains(42));
  EXPECT_EQ(reg.get(42), 100);
}

TEST(test_Registry, DoubleStartRegisteringAsserts)
{
  Registry<int, int> reg;

  reg.startRegistering();
  EXPECT_ANY_THROW(reg.startRegistering());
}

TEST(test_Registry, GetMissingAsserts)
{
  Registry<int, int> reg;

  EXPECT_ANY_THROW(reg.get(123));
}

TEST(test_Registry, ResetClearsEverything)
{
  Registry<int, int> reg;

  reg.startRegistering();
  reg.submit(1, 10);
  reg.submit(2, 20);
  reg.endRegistering();

  reg.reset();

  EXPECT_FALSE(reg.contains(1));
  EXPECT_FALSE(reg.contains(2));
}
