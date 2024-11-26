#pragma once

#include <algorithm>
#include <vector>
#include <string>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

namespace vrm
{

enum class TextureFormat : int
{
    Unsupported = -1,
    Grayscale = 1,
    RG = 2,
    RGB = 3,
    RGBA = 4
};

inline constexpr TextureFormat ChannelsToTextureFormat(int channels)
{
    return static_cast<TextureFormat>(channels);
}

inline constexpr int TextureFormatToChannels(TextureFormat format)
{
    return static_cast<int>(format);
}

template <typename T>
class TextureData
{
public:
    inline constexpr TextureData()
    {

    }

    ~TextureData()
    {
    }

    TextureData& operator=(const TextureData& other)
    {
        if (this != &other)
        {
            m_Data = other.m_Data;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_Channels = other.m_Channels;
        }

        return *this;
    }

    inline constexpr TextureData(const TextureData& other)
    {
        *this = other;
    }

    inline constexpr TextureData& operator=(TextureData&& other) noexcept
    {
        if (this != &other)
        {
            m_Data = std::move(other.m_Data);
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_Channels = other.m_Channels;
        }

        return *this;
    }

    inline constexpr TextureData(TextureData&& other) noexcept
    {
        *this = std::move(other);
    }

    inline constexpr void create(int width, int height, int channels, const T* data = nullptr)
    {
        m_Width = width;
        m_Height = height;
        m_Channels = channels;

        if (data != nullptr)
            m_Data.assign(data, data + width * height * channels);
        else
            m_Data.resize(width * height * channels);
    }

    inline bool loadFromFile(const std::string& path, bool flipVertically = true)
    {
        stbi_set_flip_vertically_on_load(flipVertically);
        int width, height;
        int channels;

        T* localBuffer = getSTBIImage(path.c_str(), &width, &height, &channels);

        if (localBuffer == nullptr)
            return false;

        create(width, height, channels, localBuffer);
        stbi_image_free(localBuffer);

        return true;
    }

    inline bool saveToFile(const std::string& path, bool flipVertically = true)
    {
        if (!isCreated())
            return false;

        stbi_flip_vertically_on_write(flipVertically);

        return saveSTBIImage(path.c_str());
    }

    inline constexpr void release()
    {
        m_Data.clear();
        m_Width = 0;
        m_Height = 0;
        m_Channels = 0;
    }

    inline constexpr const T* getData() const { return m_Data.data(); }
    inline constexpr int getWidth() const { return m_Width; }
    inline constexpr int getHeight() const { return m_Height; }
    inline constexpr int getChannels() const { return m_Channels; }
    inline constexpr int getBpp() const { return m_Channels * sizeof(T); }
    inline constexpr TextureFormat getFormat() const { return ChannelsToTextureFormat(m_Channels); }

    inline constexpr bool isCreated() const { return !m_Data.empty(); }
    inline constexpr bool isGrayscale() const { return m_Channels == 1; }
    inline constexpr bool isRGB() const { return m_Channels == 3; }
    inline constexpr bool isRGBA() const { return m_Channels == 4; }

private:
    inline T* getSTBIImage(const char* filePath, int* width, int* height, int* channels) const = delete;

    inline bool saveSTBIImage(const std::string& path) const = delete;

private:
    std::vector<T> m_Data;
    int m_Width = 0;
    int m_Height = 0;
    int m_Channels = 0;
};

using FloatTextureData = TextureData<float>;
using ByteTextureData = TextureData<unsigned char>;

template <>
inline float* TextureData<float>::getSTBIImage(const char* filePath, int* width, int* height, int* channels) const
{
    unsigned char* localBuffer = stbi_load(filePath, width, height, channels, 0);

    if (localBuffer == nullptr)
        return nullptr;
    
    float* data = new float[*width * *height * *channels];
    for (int i = 0; i < *width * *height * *channels; ++i)
    {
        data[i] = localBuffer[i] / 255.0f;
    }

    stbi_image_free(localBuffer);

    return data;
}

template <>
inline unsigned char* TextureData<unsigned char>::getSTBIImage(const char* filePath, int* width, int* height, int* channels) const
{
    return stbi_load(filePath, width, height, channels, 0);
}

template <>
inline bool TextureData<float>::saveSTBIImage(const std::string& path) const
{
    std::vector<unsigned char> convertedData(m_Width * m_Height * m_Channels);
    for (size_t i = 0; i < m_Data.size(); ++i)
    {
        convertedData[i] = static_cast<unsigned char>(std::clamp(m_Data[i] * 255.0f, 0.0f, 255.0f));
    }

    return stbi_write_png(path.c_str(), m_Width, m_Height, m_Channels, convertedData.data(), m_Width * m_Channels);
}

template <>
inline bool TextureData<unsigned char>::saveSTBIImage(const std::string& path) const
{
    return stbi_write_png(path.c_str(), m_Width, m_Height, m_Channels, m_Data.data(), m_Width * m_Channels);
}

} // namespace vrm