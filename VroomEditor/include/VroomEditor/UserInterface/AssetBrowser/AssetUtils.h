#pragma once

#include <memory>
#include <filesystem>

namespace vrm
{
  class AssetElement;

  struct MetaFile
  {
    enum class EType : uint8_t
    {
      eNone = 0, eScene, eMesh, eMaterial
    };

    MetaFile(const std::filesystem::path& metaPath, const std::filesystem::path& filePath);

    bool load();

    std::filesystem::path metaPath, filePath;
    EType type = EType::eNone;
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
     * @return std::unique_ptr<AssetElement> Nullptr if it shouldn't appear in the browser.
     */
    static std::unique_ptr<AssetElement> CreateAssetElement(const MetaFile& meta);
  };

}