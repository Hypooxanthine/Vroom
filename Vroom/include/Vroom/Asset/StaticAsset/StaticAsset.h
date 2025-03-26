#pragma once

#include "Vroom/Asset/Macros.h"

#include <memory>
#include <string>

namespace vrm
{

class StaticAsset
{
public:
    StaticAsset() = default;
    StaticAsset(const StaticAsset&) = delete;
    StaticAsset& operator=(const StaticAsset&) = delete;
    StaticAsset(StaticAsset&&) = delete;
    StaticAsset& operator=(StaticAsset&&) = delete;
    virtual ~StaticAsset() = default;

    size_t getInstanceCount() const;

    void notifyNewInstance();
    void notifyDeleteInstance();

    bool load(const std::string& filePath);

    inline const std::string& getFilePath() const { return m_filePath; }

protected:

    /**
     * @brief Get the extension of a file from its path file.
     * 
     * @param filePath The file path.
     * @return std::string Lowercase extension of the file, without the dot. Empty string if no extension could be found.
     */
    static std::string getExtension(const std::string& filePath);

    virtual bool loadImpl(const std::string& filePath) = 0;

protected:
    size_t m_InstanceCount = 0;
    std::string m_filePath;
};

} // namespace vrm