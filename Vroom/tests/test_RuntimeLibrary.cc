#include <gtest/gtest.h>
#include <Vroom/Tools/RuntimeLibrary.h>

#include <filesystem>
#include <string_view>

static constexpr std::string_view libPath = DUMMYLIB_FILENAME;

using namespace vrm;

class RuntimeLibraryTest : public testing::Test
{
protected:
  RuntimeLibrary lib;

  void SetUp() override
  {
    if (!RuntimeLibrary::IsPlatformSupported())
      GTEST_SKIP() << "Platform not supported for dynamic loading.";
    
    ASSERT_TRUE(lib.load(libPath)) << "Failed to load library: " << libPath;
  }

  void TearDown() override
  {
    lib.unload();
    ASSERT_FALSE(lib.isLoaded()) << "Library should be unloaded.";
  }
};

TEST_F(RuntimeLibraryTest, LoadLib)
{
  EXPECT_TRUE(lib.isLoaded()) << "Library should be loaded.";
}

TEST_F(RuntimeLibraryTest, GetSymbol_Identity)
{
  auto identity = lib.getSymbol<int(int)>("Identity");
  ASSERT_NE(identity, nullptr);
  EXPECT_EQ(identity(42), 42);
}

TEST_F(RuntimeLibraryTest, GetSymbol_TimesTwo)
{
  auto timesTwo = lib.getSymbol<int(int)>("TimesTwo");
  ASSERT_NE(timesTwo, nullptr);
  EXPECT_EQ(timesTwo(21), 42);
}

TEST_F(RuntimeLibraryTest, GetSymbol_GetSeven)
{
  auto getSeven = lib.getSymbol<int()>("GetSeven");
  ASSERT_NE(getSeven, nullptr);
  EXPECT_EQ(getSeven(), 7);
}


TEST_F(RuntimeLibraryTest, GetSymbol_InvalidSymbolReturnsNullptr)
{
  ASSERT_TRUE(lib.load(libPath));

  auto missing = lib.getSymbol<int()>("DoesNotExist");
  EXPECT_EQ(missing, nullptr) << "Expected nullptr for missing symbol.";
}

TEST_F(RuntimeLibraryTest, Unload_CorrectlyResetsState)
{
  RuntimeLibrary lib;
  ASSERT_TRUE(lib.load(libPath));
  ASSERT_TRUE(lib.isLoaded());

  lib.unload();

  EXPECT_FALSE(lib.isLoaded());

  auto getSeven = lib.getSymbol<int()>("GetSeven");
  EXPECT_EQ(getSeven, nullptr);
}
