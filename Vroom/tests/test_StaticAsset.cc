#include <gtest/gtest.h>

#include <Vroom/Asset/StaticAsset/StaticAsset.h>

class TestClass : public vrm::StaticAsset
{
public:
    static std::string getExtensionPublic(const std::string& filePath)
    {
        return vrm::StaticAsset::getExtension(filePath);
    }
};

TEST(GetFileExtension, GetFileExtension)
{
    std::string filePath = "C:/Users/username/Documents/Project/Assets/Model/Model.obj";
    std::string extension = TestClass::getExtensionPublic(filePath);
    EXPECT_EQ(extension, "obj");
}

TEST(GetFileExtension, GetFileExtensionNoExtension)
{
    std::string filePath = "C:/Users/username/Documents/Project/Assets/Model/Model";
    std::string extension = TestClass::getExtensionPublic(filePath);
    EXPECT_EQ(extension, "");
}

TEST(GetFileExtension, GetFileExtensionEmpty)
{
    std::string filePath = "";
    std::string extension = TestClass::getExtensionPublic(filePath);
    EXPECT_EQ(extension, "");
}

TEST(GetFileExtension, GetFileExtensionDot)
{
    std::string filePath = ".";
    std::string extension = TestClass::getExtensionPublic(filePath);
    EXPECT_EQ(extension, "");
}

TEST(GetFileExtension, GetFileExtensionMultipleDots)
{
    std::string filePath = "C:/Users/username/Documents/Project/Assets/Model/Model.obj.blend";
    std::string extension = TestClass::getExtensionPublic(filePath);
    EXPECT_EQ(extension, "blend");
}