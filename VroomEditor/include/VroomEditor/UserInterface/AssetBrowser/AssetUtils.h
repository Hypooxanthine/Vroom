#pragma once

#include <memory>
#include <filesystem>

#include <Vroom/Asset/Parsing/Json.h>

namespace vrm
{
  class AssetElement;

  struct MetaFile
  {
    enum class EType : uint8_t
    {
      eNone = 0, eScene, eMesh, eMaterial
    };

    EType Type = EType::eNone;
  };

  class AssetUtils
  {
  public:

  public:

    /**
     * @brief Create the correct AssetElement implementation for a given file path.
     * 
     * @param path The element in file system.
     * @return std::unique_ptr<AssetElement> Nullptr if it shouldn't appear in the browser.
     */
    static std::unique_ptr<AssetElement> CreateAssetElement(const std::filesystem::path& path);
    
    /**
     * @brief Create the correct AssetElement implementation for a given file meta data.
     * 
     * @param meta Meta data
     * @param meta filePath The path of the file the meta data is attached to
     * @return std::unique_ptr<AssetElement> Nullptr if it shouldn't appear in the browser.
     */
    static std::unique_ptr<AssetElement> CreateAssetElement(const MetaFile& meta, const std::filesystem::path& filePath);

    static std::filesystem::path GetMetaName(const std::filesystem::path& baseName);

    static bool CreateMetaFile(const MetaFile& meta, const std::filesystem::path& filePath);

    static void OpenNativeFileExplorer(const std::filesystem::path& path);

    static bool CreateDirectory(const std::filesystem::path& path);

    // File or directory. Will check for meta files and remove them if exist
    static bool DeleteAssetElement(const std::filesystem::path& path);

    // File or directory. Will check for meta files and rename them if exist
    static bool RenameAssetElement(const std::filesystem::path& assetPath, const std::filesystem::path& targetName);
  };

}

namespace nlohmann
{
  NLOHMANN_JSON_SERIALIZE_ENUM(vrm::MetaFile::EType,
  {
    { vrm::MetaFile::EType::eNone , "None" },
    { vrm::MetaFile::EType::eMaterial , "Material" },
    { vrm::MetaFile::EType::eMesh , "Mesh" },
    { vrm::MetaFile::EType::eScene , "Scene" },
  })

  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(vrm::MetaFile,
    Type
  )
}
