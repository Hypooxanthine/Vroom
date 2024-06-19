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
    std::string assetPath = "An/asset";
    EXPECT_FALSE(assetManager->isAssetLoaded(assetPath));
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath);
    EXPECT_TRUE(assetManager->isAssetLoaded(assetPath));
}

TEST_F(AssetManagerTest, GetAssetFirstTime)
{
    std::string assetPath = "An/asset";
    EXPECT_NO_THROW(
        vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath)
    );
}

TEST_F(AssetManagerTest, GetAssetSecondTime)
{
    std::string assetPath = "An/asset";
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(assetPath);
    
    EXPECT_NO_THROW(
        vrm::MeshInstance instance2 = assetManager->getAsset<vrm::MeshAsset>(assetPath)
    );
}
