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

  protected:

    /**
     * @brief Get the extension of a file from its path file.
     *
     * @param filePath The file path.
     * @return std::string Lowercase extension of the file, without the dot. Empty string if no extension could be found.
     */
    static std::string getExtension(const std::string& filePath);

    virtual bool loadImpl(const std::filesystem::path& filePath) = 0;

    std::filesystem::path applyPathOrder(const std::filesystem::path& path) const;

  private:
    mutable size_t m_InstanceCount = 0;
    std::string m_filePath;
  };

} // namespace vrm