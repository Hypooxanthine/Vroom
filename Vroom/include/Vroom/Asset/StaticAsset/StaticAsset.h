#pragma once

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

protected:
    virtual bool loadImpl(const std::string& filePath) = 0;

protected:
    size_t m_InstanceCount = 0;
};

} // namespace vrm