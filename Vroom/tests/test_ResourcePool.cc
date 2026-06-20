#include <gtest/gtest.h>
#include <string>

#include <Tools/ResourcePool.h>

using vrm::ResourcePool;

namespace
{

struct Resource
{
  int value;
  explicit Resource(int v) : value(v) {}
  virtual ~Resource() = default;
};

struct DerivedResource : Resource
{
  explicit DerivedResource(int v) : Resource(v * 2) {}
};

} // namespace

TEST(test_ResourcePool, EmplaceAndGet)
{
  ResourcePool<Resource> pool;

  Resource* r = pool.emplace("a", 5);
  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->value, 5);
  EXPECT_TRUE(pool.contains("a"));
  EXPECT_EQ(pool.get("a"), r);
}

TEST(test_ResourcePool, TryGetReturnsNullWhenMissing)
{
  ResourcePool<Resource> pool;

  EXPECT_EQ(pool.tryGet("missing"), nullptr);

  pool.emplace("a", 1);
  EXPECT_NE(pool.tryGet("a"), nullptr);
}

TEST(test_ResourcePool, GetMissingAsserts)
{
  ResourcePool<Resource> pool;

  EXPECT_ANY_THROW(pool.get("missing"));
}

TEST(test_ResourcePool, DuplicateEmplaceAsserts)
{
  ResourcePool<Resource> pool;

  pool.emplace("a", 1);
  EXPECT_ANY_THROW(pool.emplace("a", 2));
}

TEST(test_ResourcePool, PolymorphicEmplace)
{
  ResourcePool<Resource> pool;

  Resource* r = pool.emplace<DerivedResource>("d", 5);
  ASSERT_NE(r, nullptr);
  EXPECT_EQ(r->value, 10);
  EXPECT_NE(dynamic_cast<DerivedResource*>(pool.get("d")), nullptr);
}

TEST(test_ResourcePool, ClearEmptiesPool)
{
  ResourcePool<Resource> pool;

  pool.emplace("a", 1);
  pool.emplace("b", 2);
  pool.clear();

  EXPECT_FALSE(pool.contains("a"));
  EXPECT_FALSE(pool.contains("b"));
}
