#include <gtest/gtest.h>
#include <Vroom/Asset/Asset.h>
#include <Vroom/Core/Application.h>

#include <fstream>

class AssetManagerTest : public testing::Test {
protected:
    void SetUp() override {
        app = new vrm::Application(0, nullptr);
        assetManager = new vrm::AssetManager();

        // Create a fake obj file
        std::ofstream file(pathOK, std::ios::out);
        file << "v 0.0 0.0 0.0\n";
        file << "v 1.0 0.0 0.0\n";
        file << "v 1.0 1.0 0.0\n";
        file << "f 1 2 3\n";
        file.close();
    }

    void TearDown() override {
        delete assetManager;
        delete app;
        
        // Remove the fake obj file
        std::remove(pathOK.c_str());
    }

    vrm::Application* app;
    vrm::AssetManager* assetManager;
    std::string pathOK = "test_mesh.obj";
    std::string pathFail = "test_mesh_fail.obj";
};

TEST_F(AssetManagerTest, IsAssetLoaded)
{
    EXPECT_FALSE(assetManager->isAssetLoaded(pathOK));
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(pathOK);
    EXPECT_TRUE(assetManager->isAssetLoaded(pathOK));
}

TEST_F(AssetManagerTest, GetAssetFirstTime)
{
    EXPECT_NO_THROW(
        vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(pathOK)
    );
}

TEST_F(AssetManagerTest, GetAssetSecondTime)
{
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(pathOK);
    
    EXPECT_NO_THROW(
        vrm::MeshInstance instance2 = assetManager->getAsset<vrm::MeshAsset>(pathOK)
    );
}

TEST_F(AssetManagerTest, GetAssetDifferentType)
{
    vrm::MeshInstance instance = assetManager->getAsset<vrm::MeshAsset>(pathOK);
    
    EXPECT_ANY_THROW(
        vrm::ImageInstance instance2 = assetManager->getAsset<vrm::ImageAsset>(pathOK)
    );
}
