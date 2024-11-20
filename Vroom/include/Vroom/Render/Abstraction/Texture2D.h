#pragma once

#include <vector>
#include <string>

namespace vrm
{

class Texture2D
{
public:
    enum class Format
    {
        RGB,
        RGBA
    };

public:
    Texture2D();
    Texture2D(const std::string& path);
    
    virtual ~Texture2D();

    Texture2D& operator=(const Texture2D& other);
    Texture2D(const Texture2D& other);

    Texture2D& operator=(Texture2D&& other) noexcept;
    Texture2D(Texture2D&& other) noexcept;

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    void create(int width, int height, Format format, const void* data = nullptr);

    void release();

	/**
	 * @brief Loads the texture as an image.
	 * @param path Image file path.
	 * @return true If loaded successfuly.
	 * @return false Otherwise.
	 */
	bool loadFromFile(const std::string& path);

	/**
	 * @brief Saves the texture to a file.
	 * @param path File path.
	 * @return true If saved successfuly.
	 * @return false Otherwise.
	 */
	bool saveToFile(const std::string& path);

    inline bool isCreated() const { return m_RendererID != 0; }

    inline unsigned int getRendererID() const { return m_RendererID; }

    /**
     * @brief Gets texture data from GPU.
     * 
     * @return std::vector<unsigned char> 
     */
    std::vector<unsigned char> getData() const;

	/**
	 * @brief Gets texture width.
	 * @return Texture width (in pixels).
	 */
	inline int getWidth() const { return m_Width; }

	/**
	 * @brief Gets texture height.
	 * @return Texture height (in pixels).
	 */
	inline int getHeight() const { return m_Height; }

    /**
     * @brief Gets texture bytes per pixel.
     * @return Texture bytes per pixel.
     */
    inline int getBPP() const { return m_BPP; }

    /**
     * @brief Gets texture format.
     * @return Texture format.
     */
    inline Format getFormat() const { return m_Format; }

private:
    unsigned int m_RendererID = 0;
	int m_Width = 0, m_Height = 0;
	int m_BPP;
    Format m_Format;
};

} // namespace vrm