#include <gtest/gtest.h>
#include "Vroom/Asset/Asset.h"

class AssetManagerTest : public testing::Test {
protected:
    void SetUp() override {
        assetManager = new vrm::AssetManager();
    }

    void TearDown() override {
        delete assetManager;
    }

    vrm::AssetManager* assetManager;
};

TEST_F(AssetManagerTest, IsAssetLoaded)
{
    std::string assetPath = "Resources/test_mesh.obj";
    EXPECT_FALSE(assetManager->isAssetLoaded(assetPath));
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath);
    EXPECT_TRUE(assetManager->isAssetLoaded(assetPath));
}

TEST_F(AssetManagerTest, GetAssetFirstTime)
{
    std::string assetPath = "Resources/test_mesh.obj";
    EXPECT_NO_THROW(
        vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath)
    );
}

TEST_F(AssetManagerTest, GetAssetSecondTime)
{
    std::string assetPath = "Resources/test_mesh.obj";
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath);
    
    EXPECT_NO_THROW(
        vrm::MeshInstance instance2 = assetManager->getAsset<vrm::MeshAsset>(assetPath)
    );
}

TEST_F(AssetManagerTest, GetAssetDifferentType)
{
    std::string assetPath = "Resources/test_mesh.obj";
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath);
    
    EXPECT_ANY_THROW(
        vrm::ImageInstance instance2 = assetManager->getAsset<vrm::ImageAsset>(assetPath)
    );
}
