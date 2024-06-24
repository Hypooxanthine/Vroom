#include <gtest/gtest.h>
#include <Vroom/Asset/StaticAsset/MeshAsset.h>
#include <Vroom/Asset/AssetManager.h>
#include <Vroom/Core/Application.h>

#include <fstream>

class TestMeshAsset : public testing::Test
{
protected:

    void SetUp() override
    {
        app = new vrm::Application(0, nullptr);
        meshAsset = new vrm::MeshAsset();
        assetManager = new vrm::AssetManager();

        // Create a fake obj file
        std::ofstream file(pathOK, std::ios::out | std::ios::trunc);
        file << "v 0.0 0.0 0.0\n";
        file << "v 1.0 0.0 0.0\n";
        file << "v 1.0 1.0 0.0\n";
        file << "f 1 2 3\n";
        file.close();
    }

    void TearDown() override
    {
        delete meshAsset;
        delete assetManager;
        delete app;

        // Remove the fake obj file
        std::remove(pathOK.c_str());
    }

    vrm::Application* app;
    vrm::MeshAsset* meshAsset;
    vrm::AssetManager* assetManager;
    std::string pathOK = "test_mesh.obj";
    std::string pathFail = "test_mesh_fail.obj";
};

TEST_F(TestMeshAsset, LoadObj)
{
    meshAsset->load(pathOK, *assetManager);
}

TEST_F(TestMeshAsset, LoadObjFail)
{
    EXPECT_FALSE(meshAsset->load(pathFail, *assetManager));
}

TEST_F(TestMeshAsset, LoadObjCorrect)
{
    meshAsset->load(pathOK, *assetManager);
    const vrm::MeshData& meshData = meshAsset->getSubMeshes().begin()->meshData;

    const auto vertices = meshData.getVertices();
    const auto indices = meshData.getIndices();

    EXPECT_EQ(vertices[0].position.x, 0.0f);
    EXPECT_EQ(vertices[0].position.y, 0.0f);
    EXPECT_EQ(vertices[0].position.z, 0.0f);

    EXPECT_EQ(vertices[1].position.x, 1.0f);
    EXPECT_EQ(vertices[1].position.y, 0.0f);
    EXPECT_EQ(vertices[1].position.z, 0.0f);

    EXPECT_EQ(vertices[2].position.x, 1.0f);
    EXPECT_EQ(vertices[2].position.y, 1.0f);
    EXPECT_EQ(vertices[2].position.z, 0.0f);

    EXPECT_EQ(indices[0], 0);
    EXPECT_EQ(indices[1], 1);
    EXPECT_EQ(indices[2], 2);
}

TEST_F(TestMeshAsset, GetRenderMesh)
{
    meshAsset->load(pathOK, *assetManager);
    EXPECT_NO_THROW(const vrm::RenderMesh& renderMesh = meshAsset->getSubMeshes().begin()->renderMesh;);
}
