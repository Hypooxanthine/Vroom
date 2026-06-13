#pragma once

#include "AssetManager/Api.h"

#include <filesystem>
#include <string>

namespace vrm
{

  class VRM_ASSETMANAGER_API StaticAsset
  {
  public:
    StaticAsset() = default;
    StaticAsset(const StaticAsset&) = delete;
    StaticAsset& operator=(const StaticAsset&) = delete;
    StaticAsset(StaticAsset&&) = delete;
    StaticAsset& operator=(StaticAsset&&) = delete;
    virtual ~StaticAsset();

    size_t getInstanceCount() const;

    void notifyNewInstance() const;
    void notifyDeleteInstance() const;

    bool load(const std::string& filePath);

    inline std::filesystem::path getFilePath() const { return m_filePath; }
    size_t getGeneration() const;

    /**
     * @brief Tries to resolve a path from this asset: local first, relative to the project origin then.
     * 
     * @param path 
     * @return std::filesystem::path 
     */
    std::filesystem::path applyPathOrder(const std::filesystem::path& path) const;

  protected:

    /**
     * @brief Get the extension of a file from its path file.
     *
     * @param filePath The file path.
     * @return std::string Lowercase extension of the file, without the dot. Empty string if no extension could be found.
     */
    static std::string getExtension(const std::string& filePath);

    virtual bool loadImpl(const std::filesystem::path& filePath) = 0;

  private:
    mutable size_t m_InstanceCount = 0;
    std::string m_filePath;
    size_t m_generation = 0;
  };

} // namespace vrm