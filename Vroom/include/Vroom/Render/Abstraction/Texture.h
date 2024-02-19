#pragma once

#include <string>

class Texture
{
public:
	
	/**
	 * @brief Constructs a Texture object and loads it from system.
	 * @param path Image file path.
	 */
	Texture(const std::string& path);

	/**
	 * @brief Constructs an empty Texture object.
	 * Texture won't be valid; use Texture::loadFromFile(const std::string&) for loading an image in the texture.
	 */
	Texture();

	/**
	 * @brief Releases GPU memory.
	 */
	virtual ~Texture();

	/**
	 * @brief Loads the texture as an image.
	 * @param path Image file path.
	 * @return true If loaded successfuly.
	 * @return false Otherwise.
	 */
	bool loadFromFile(const std::string& path);

	/**
	 * @brief Check if Texture is loaded.
	 * @return true If loaded.
	 * @return false Otherwise.
	 */
	inline bool isLoaded() const { return m_Loaded; }

	/**
	 * @brief Binds this texture.
	 * @param slot Specify a slot to load the texture in.
	 */
	void bind(unsigned int slot = 0) const;

	/**
	 * @brief Unbinds this texture.
	 */
	void unbind() const;

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

private:
	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	bool m_Loaded;
};