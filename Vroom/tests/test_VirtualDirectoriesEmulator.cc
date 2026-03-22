#include <filesystem>
#include <format>
#include <fstream>
#include <gtest/gtest.h>
#include <vector>

#include "Vroom/Asset/VirtualDirectoriesEmulator.h"

class TestVirtualDirectoriesEmulator : public testing::Test
{
protected:

  void SetUp() override
  {
    try
    {
      if (std::filesystem::exists("tmp"))
        std::filesystem::remove_all("tmp");
      if (std::filesystem::exists("tmpVirtual"))
        std::filesystem::remove_all("tmpVirtual");
    } catch (...)
    {
      FAIL() << "Error while removing tmp dirs";
    }

    try
    {
      std::filesystem::create_directory("tmp");
      std::filesystem::create_directory("tmpVirtual");
      std::filesystem::current_path("tmp");
    } catch (...)
    {
      FAIL() << "Error while creating tmp dirs";
    }
  }

  void TearDown() override
  {
    try
    {
      if (std::filesystem::current_path().parent_path() == "tmp")
      {
        std::filesystem::current_path(
          std::filesystem::current_path().parent_path());
        std::filesystem::remove_all("tmp");
        std::filesystem::remove_all("tmpVirtual");
      }
    } catch (...)
    {
      FAIL() << "Error while removing tmp dirs";
    }
  }

  void createFile(const std::filesystem::path& filePath) const
  {
    ASSERT_FALSE(std::filesystem::exists(filePath))
      << std::format("{} already exists", filePath.string());

    createParentDirs(filePath);

    std::ofstream out;
    out.open(filePath);

    ASSERT_TRUE(out.is_open())
      << std::format("Could not create {}", filePath.string());
  }

  void createVirtualFile(const std::filesystem::path& filePath) const
  {
    createFile((getVirtualRoot() / filePath).lexically_normal());
  }

  void createParentDirs(const std::filesystem::path& dirPath) const
  {
    try
    {
      if (dirPath.parent_path() != ""
          && !std::filesystem::exists(dirPath.parent_path()))
        std::filesystem::create_directories(dirPath.parent_path());
    } catch (...)
    {
      FAIL() << std::format("Could not create directories {}",
                            dirPath.parent_path().string());
    }
  }

  std::filesystem::path getVirtualRoot() const
  {
    return "../tmpVirtual/";
  }
};

TEST_F(TestVirtualDirectoriesEmulator, RegularPathsNoVirtualDirsAdded)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> files = { "dummy1.txt",
                                               "dummyDir/dummy1.txt" };

  for (const auto& file : files)
    createFile(file);

  for (const auto& file : files)
    ASSERT_EQ(emulator.findPath(file), file);
}

TEST_F(TestVirtualDirectoriesEmulator, AddVirtualDirectories)
{
  vrm::VirtualDirectoriesEmulator emulator;

  ASSERT_FALSE(emulator.addDirectory("v1"));
  ASSERT_FALSE(emulator.addDirectory(getVirtualRoot() / "v1"));

  createVirtualFile("v1/dummy.txt");

  ASSERT_FALSE(emulator.addDirectory("v1"));
  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v1"));
}

TEST_F(TestVirtualDirectoriesEmulator, RegularPathsWithVirtualDirsAdded)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> files        = { "dummy1.txt",
                                                      "dummyDir/dummy1.txt" },
                                     virtualFiles = { "v1/v1.txt", "v1/v2.txt",
                                                      "v2/v1/v1.txt",
                                                      "v2/v1/v1/v1.txt" };

  for (const auto& file : files)
    createFile(file);

  for (const auto& virtualFile : virtualFiles)
    createVirtualFile(virtualFile);

  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v1"));
  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v2"));

  for (const auto& file : files)
    ASSERT_EQ(emulator.findPath(file), file);
}

TEST_F(TestVirtualDirectoriesEmulator, VirtualPaths)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> files        = { "dummy1.txt",
                                                      "dummyDir/dummy1.txt" },
                                     virtualFiles = { "v1/v1.txt", "v1/v2.txt",
                                                      "v2/v1/v1.txt",
                                                      "v2/v1/v1/v1.txt" };

  for (const auto& file : files)
    createFile(file);

  for (const auto& virtualFile : virtualFiles)
    createVirtualFile(virtualFile);

  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v1"));
  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v2"));

  for (const auto& file : files)
    ASSERT_EQ(emulator.findPath(file), file);

  for (const auto& virtualFile : virtualFiles)
  {
    std::filesystem::path resolvedPath = emulator.findPath(virtualFile);
    ASSERT_TRUE(std::filesystem::exists(resolvedPath));
    ASSERT_EQ(std::filesystem::canonical(resolvedPath),
              std::filesystem::canonical(getVirtualRoot() / virtualFile));
  }
}

TEST_F(TestVirtualDirectoriesEmulator, CustomVirtualPaths)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> files        = { "dummy1.txt",
                                                      "dummyDir/dummy1.txt" },
                                     virtualFiles = { "v1/v1.txt", "v1/v2.txt",
                                                      "v2/v1/v1.txt",
                                                      "v2/v1/v1/v1.txt" };

  for (const auto& file : files)
    createFile(file);

  for (const auto& virtualFile : virtualFiles)
    createVirtualFile(virtualFile);

  ASSERT_TRUE(emulator.addDirectory(
    getVirtualRoot() / "v1", std::filesystem::path("custom1/custom2") / "v1"));
  ASSERT_TRUE(emulator.addDirectory(
    getVirtualRoot() / "v2", std::filesystem::path("custom1/custom2") / "v2"));

  for (const auto& file : files)
    ASSERT_EQ(emulator.findPath(file), file);

  for (const auto& virtualFile : virtualFiles)
  {
    std::filesystem::path resolvedPath =
      emulator.findPath("custom1/custom2" / virtualFile);
    ASSERT_TRUE(std::filesystem::exists(resolvedPath));
    ASSERT_EQ(std::filesystem::canonical(resolvedPath),
              std::filesystem::canonical(getVirtualRoot() / virtualFile));
  }

  for (const auto& virtualFile : virtualFiles)
  {
    std::filesystem::path resolvedPath = emulator.findPath(virtualFile);
    ASSERT_EQ(resolvedPath, "");
  }
}

TEST_F(TestVirtualDirectoriesEmulator, GetChildrenRegularDirectory)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> files = { "dir1/file1.txt",
                                               "dir1/file2.txt",
                                               "dir1/subdir/file3.txt" };

  for (const auto& file : files)
    createFile(file);

  auto children = emulator.getChildren("dir1");

  ASSERT_EQ(children.size(), 3);

  std::vector<std::filesystem::path> childNames;
  for (const auto& child : children)
    childNames.push_back(child.filename());

  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "file1.txt")
              != childNames.end());
  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "file2.txt")
              != childNames.end());
  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "subdir")
              != childNames.end());
}

TEST_F(TestVirtualDirectoriesEmulator, GetChildrenVirtualDirectory)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> virtualFiles = { "v1/file1.txt",
                                                      "v1/file2.txt",
                                                      "v1/subdir/file3.txt" };

  for (const auto& file : virtualFiles)
    createVirtualFile(file);

  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v1"));

  auto children = emulator.getChildren("v1");

  ASSERT_EQ(children.size(), 3);

  std::vector<std::filesystem::path> childNames;
  for (const auto& child : children)
    childNames.push_back(child.filename());

  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "file1.txt")
              != childNames.end());
  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "file2.txt")
              != childNames.end());
  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "subdir")
              != childNames.end());
}

TEST_F(TestVirtualDirectoriesEmulator, GetChildrenEmptyDirectory)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::filesystem::create_directory("emptyDir");

  auto children = emulator.getChildren("emptyDir");

  ASSERT_EQ(children.size(), 0);
}

TEST_F(TestVirtualDirectoriesEmulator, GetChildrenNonExistentPath)
{
  vrm::VirtualDirectoriesEmulator emulator;

  auto children = emulator.getChildren("nonExistentPath");

  ASSERT_EQ(children.size(), 0);
}

TEST_F(TestVirtualDirectoriesEmulator, GetChildrenCustomVirtualPath)
{
  vrm::VirtualDirectoriesEmulator emulator;

  std::vector<std::filesystem::path> virtualFiles = { "v1/file1.txt",
                                                      "v1/file2.txt" };

  for (const auto& file : virtualFiles)
    createVirtualFile(file);

  ASSERT_TRUE(emulator.addDirectory(getVirtualRoot() / "v1", "customPath/v1"));

  auto children = emulator.getChildren("customPath/v1");

  ASSERT_EQ(children.size(), 2);

  std::vector<std::filesystem::path> childNames;
  for (const auto& child : children)
    childNames.push_back(child.filename());

  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "file1.txt")
              != childNames.end());
  ASSERT_TRUE(std::find(childNames.begin(), childNames.end(), "file2.txt")
              != childNames.end());
}
