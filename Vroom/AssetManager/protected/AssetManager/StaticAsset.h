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

    /**
     * @brief Sets the source path used to resolve relative resources (see
     * applyPathOrder). load() sets this from the file path; assets that can be
     * (re)built from in-memory data set it themselves.
     */
    void setFilePath(const std::filesystem::path& filePath);

    /**
     * @brief Bumps the generation counter. load() does this automatically on a
     * successful file load; in-memory (re)builds must call it so consumers that
     * cache by generation refresh from the new content.
     */
    void incrementGeneration();

  private:
    mutable size_t m_InstanceCount = 0;
    std::string m_filePath;
    size_t m_generation = 0;
  };

} // namespace vrm