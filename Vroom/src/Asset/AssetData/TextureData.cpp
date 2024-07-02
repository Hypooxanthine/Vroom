#include "Vroom/Asset/AssetData/TextureData.h"

namespace vrm
{

TextureData::TextureData(const std::vector<DataType>& data, int width, int height, int channels)
    : m_Data(data), m_Width(width), m_Height(height), m_Channels(channels)
{
}

TextureData::TextureData(std::vector<DataType>&& data, int width, int height, int channels)
    : m_Data(std::move(data)), m_Width(width), m_Height(height), m_Channels(channels)
{
}

TextureData::TextureData()
    : m_Width(0), m_Height(0), m_Channels(0)
{
}

TextureData::TextureData(const TextureData& other)
    : m_Data(other.m_Data), m_Width(other.m_Width), m_Height(other.m_Height), m_Channels(other.m_Channels)
{
}

TextureData::TextureData(TextureData&& other)
    : m_Data(std::move(other.m_Data)), m_Width(other.m_Width), m_Height(other.m_Height), m_Channels(other.m_Channels)
{
}

TextureData& TextureData::operator=(const TextureData& other)
{
    m_Data = other.m_Data;
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Channels = other.m_Channels;

    return *this;
}

TextureData& TextureData::operator=(TextureData&& other)
{
    m_Data = std::move(other.m_Data);
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Channels = other.m_Channels;

    return *this;
}

void TextureData::reset()
{
    m_Data.clear();
    m_Width = 0;
    m_Height = 0;
    m_Channels = 0;
}

void TextureData::setData(const std::vector<DataType>& data, int width, int height, int channels)
{
    m_Data = data;
    m_Width = width;
    m_Height = height;
    m_Channels = channels;
}

void TextureData::setData(std::vector<DataType>&& data, int width, int height, int channels)
{
    m_Data = std::move(data);
    m_Width = width;
    m_Height = height;
    m_Channels = channels;
}

} // namespace vrm