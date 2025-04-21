#include <gtest/gtest.h>
#include <Vroom/DataStructure/LinearRegistry.h>

TEST(test_LinearRegistry, TestAddElement)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();

  registry.submit(1, 42);

  registry.endRegistering();

  const auto& element = registry.begin();
  EXPECT_EQ((*element).first, 1);
  EXPECT_EQ((*element).second, 42);
}

TEST(test_LinearRegistry, TestContains)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();

  registry.submit(1, 42);
  registry.submit(6, 43);

  registry.endRegistering();

  EXPECT_TRUE(registry.contains(1));
  EXPECT_TRUE(registry.contains(6));
  EXPECT_FALSE(registry.contains(3));
}

TEST(test_LinearRegistry, TestReset)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();
  registry.submit(1, 42);
  registry.submit(4, 43);
  registry.endRegistering();

  // Vérification avant reset
  EXPECT_TRUE(registry.contains(1));
  EXPECT_TRUE(registry.contains(4));

  // Reset
  registry.reset();

  // Vérification après reset
  EXPECT_FALSE(registry.contains(1));
  EXPECT_FALSE(registry.contains(4));
}

TEST(test_LinearRegistry, TestConstIterator)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();
  registry.submit(1, 42);
  registry.submit(4, 3);
  registry.endRegistering();

  // Utilisation de l'itérateur constant
  auto it = registry.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, 42);
  ++it;
  EXPECT_EQ((*it).first, 4);
  EXPECT_EQ((*it).second, 3);
}

TEST(test_LinearRegistry, TestMultipleSubmissions)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();
  registry.submit(1, 42);
  registry.submit(2, 43);
  registry.submit(3, 44);
  registry.endRegistering();

  // Vérification que les éléments ont été insérés
  auto it = registry.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, 42);
  ++it;
  EXPECT_EQ((*it).first, 2);
  EXPECT_EQ((*it).second, 43);
  ++it;
  EXPECT_EQ((*it).first, 3);
  EXPECT_EQ((*it).second, 44);
}

TEST(test_LinearRegistry, TestDoubleStartRegistering)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();
  EXPECT_ANY_THROW(registry.startRegistering());
}

TEST(test_LinearRegistry, TestElementReplacement)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();
  registry.submit(1, 42);
  registry.endRegistering();

  auto it = registry.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, 42);

  registry.startRegistering();
  registry.submit(1, 100);
  registry.endRegistering();

  it = registry.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, 100);
}

TEST(test_LinearRegistry, TestContainsAfterDeletion)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();
  registry.submit(1, 42);
  registry.submit(2, 43);
  registry.endRegistering();

  EXPECT_TRUE(registry.contains(1));
  EXPECT_TRUE(registry.contains(2));

  registry.startRegistering();
  registry.submit(1, 0);
  registry.submit(2, 0);
  registry.endRegistering();

  EXPECT_TRUE(registry.contains(1));
  EXPECT_TRUE(registry.contains(2));
}

