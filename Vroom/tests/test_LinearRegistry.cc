#include <gtest/gtest.h>
#include <Vroom/DataStructure/LinearRegistry.h>

TEST(test_LinearRegistry, TestAddElement)
{
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();

  registry.submit(1, 42);

  registry.endRegistering();

  const auto& element = registry.begin();
  EXPECT_EQ((*element), 42);
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
  EXPECT_EQ((*it), 42);
  ++it;
  EXPECT_EQ((*it), 3);
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
  EXPECT_EQ((*it), 42);
  ++it;
  EXPECT_EQ((*it), 43);
  ++it;
  EXPECT_EQ((*it), 44);
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
  EXPECT_EQ((*it), 42);

  registry.startRegistering();
  registry.submit(1, 100);
  registry.endRegistering();

  it = registry.begin();
  EXPECT_EQ((*it), 100);
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

  EXPECT_TRUE(registry.getRegisteredKeys().size() == 2);
  EXPECT_TRUE(registry.getRegisteredKeys().contains(1));
  EXPECT_TRUE(registry.getRegisteredKeys().contains(2));
  EXPECT_TRUE(registry.getJustRemovedKeys().empty());

  registry.startRegistering();
  registry.notifyUsed(2);
  registry.endRegistering();

  EXPECT_FALSE(registry.contains(1));
  EXPECT_TRUE(registry.contains(2));

  EXPECT_TRUE(registry.getJustRemovedKeys().size() == 1);
  EXPECT_TRUE(registry.getJustRemovedKeys().contains(1));
  EXPECT_TRUE(registry.getRegisteredKeys().size() == 1);
  EXPECT_TRUE(registry.getRegisteredKeys().contains(2));
}

TEST(test_LinearRegistry, BigTest)
{
  static constexpr int itemCount = 1'000;
  vrm::LinearRegistry<int, int> registry;

  registry.startRegistering();

  for (int i = 0; i < itemCount; ++i)
  {
    registry.submit(i, 10 * i + i % 10);
  }

  registry.endRegistering();
  
  EXPECT_TRUE(registry.getRegisteredKeys().size() == itemCount);
  EXPECT_TRUE(registry.getJustRemovedKeys().size() == 0);

  for (int i = 0; i < itemCount; ++i)
  {
    EXPECT_TRUE(registry.contains(i));
    EXPECT_TRUE(registry.getRegisteredKeys().contains(i));
    EXPECT_TRUE(!registry.getJustRemovedKeys().contains(i));
    EXPECT_TRUE(std::find(registry.begin(), registry.end(), 10 * i + i % 10) != registry.end());
  }

  registry.startRegistering();

  for (int i = 0; i < itemCount; i += 2)
  {
    registry.notifyUsed(i);
  }

  registry.endRegistering();
  
  EXPECT_TRUE(registry.getRegisteredKeys().size() == itemCount / 2);
  EXPECT_TRUE(registry.getJustRemovedKeys().size() == itemCount / 2);

  for (int i = 0; i < itemCount; i += 2)
  {
    EXPECT_TRUE(registry.contains(i));
    EXPECT_TRUE(registry.getRegisteredKeys().contains(i));
    EXPECT_TRUE(!registry.getJustRemovedKeys().contains(i));
    EXPECT_TRUE(std::find(registry.begin(), registry.end(), 10 * i + i % 10) != registry.end());
  }

  for (int i = 1; i < itemCount; i += 2)
  {
    EXPECT_TRUE(!registry.contains(i));
    EXPECT_TRUE(!registry.getRegisteredKeys().contains(i));
    EXPECT_TRUE(registry.getJustRemovedKeys().contains(i));
    EXPECT_TRUE(std::find(registry.begin(), registry.end(), 10 * i + i % 10) == registry.end());
  }
}
