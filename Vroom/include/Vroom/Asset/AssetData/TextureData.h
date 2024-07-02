#pragma once

#include <vector>

namespace vrm
{

class TextureData
{
public:
    using DataType = unsigned char;
public:
    TextureData(const std::vector<DataType>& data, int width, int height, int channels);
    TextureData(std::vector<DataType>&& data, int width, int height, int channels);

    TextureData();
    TextureData(const TextureData& other);
    TextureData(TextureData&& other);
    TextureData& operator=(const TextureData& other);
    TextureData& operator=(TextureData&& other);

    ~TextureData() = default;

    void reset();

    void setData(const std::vector<DataType>& data, int width, int height, int channels);
    void setData(std::vector<DataType>&& data, int width, int height, int channels);

    inline const DataType* getData() const { return m_Data.data(); }
    inline int getWidth() const { return m_Width; }
    inline int getHeight() const { return m_Height; }
    inline int getChannels() const { return m_Channels; }

private:
    std::vector<DataType> m_Data;
    int m_Width;
    int m_Height;
    int m_Channels;
};

} // namespace vrm