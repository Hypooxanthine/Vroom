#include <gtest/gtest.h>
#include <Vroom/Asset/Asset.h>
#include <Vroom/Core/Application.h>
#include <Vroom/Asset/AssetManager.h>

#include <fstream>

class AssetManagerTest : public testing::Test {
protected:
  void SetUp() override
  {
    app = new vrm::Application(0, nullptr);

    // Create a fake obj file
    std::ofstream file(pathOK, std::ios::out);
    file << "v 0.0 0.0 0.0\n";
    file << "v 1.0 0.0 0.0\n";
    file << "v 1.0 1.0 0.0\n";
    file << "f 1 2 3\n";
    file.close();
  }

  void TearDown() override
  {
    delete app;

    // Remove the fake obj file
    std::remove(pathOK.c_str());
  }

  vrm::Application* app;
  std::string pathOK = "test_mesh.obj";
  std::string pathFail = "test_mesh_fail.obj";
};

TEST_F(AssetManagerTest, IsAssetLoaded)
{
  EXPECT_FALSE(vrm::AssetManager::Get().isAssetLoaded(pathOK));
  EXPECT_NO_THROW(vrm::AssetManager::Get().loadAsset<vrm::MeshAsset>(pathOK));
  EXPECT_TRUE(vrm::AssetManager::Get().isAssetLoaded(pathOK));
}

TEST_F(AssetManagerTest, GetAssetFirstTime)
{
  EXPECT_NO_THROW(
    vrm::MeshAsset::Handle asset = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>(pathOK)
  );
}

TEST_F(AssetManagerTest, GetAssetSecondTime)
{
  vrm::MeshAsset::Handle asset = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>(pathOK);

  EXPECT_NO_THROW(
    vrm::MeshAsset::Handle asset2 = vrm::AssetManager::Get().getAsset<vrm::MeshAsset>(pathOK)
  );
}

TEST_F(AssetManagerTest, GetAssetDifferentType)
{
  EXPECT_NO_THROW(vrm::AssetManager::Get().loadAsset<vrm::MeshAsset>(pathOK));
  EXPECT_ANY_THROW(
    vrm::MaterialAsset::Handle mat = vrm::AssetManager::Get().getAsset<vrm::MaterialAsset>(pathOK)
  );
}
