#pragma once

#include <string>

#include "Vroom/Render/Abstraction/Texture2D.h"

namespace vrm
{

class ImageTexture : public Texture2D
{
public:
	
	/**
	 * @brief Constructs a ImageTexture object and loads it from system.
	 * @param path Image file path.
	 */
	ImageTexture(const std::string& path);

	/**
	 * @brief Constructs an empty ImageTexture object.
	 * ImageTexture won't be valid; use ImageTexture::loadFromFile(const std::string&) for loading an image in the texture.
	 */
	ImageTexture();

	/**
	 * @brief Releases GPU memory.
	 */
	virtual ~ImageTexture();

	/**
	 * @brief Loads the texture as an image.
	 * @param path Image file path.
	 * @return true If loaded successfuly.
	 * @return false Otherwise.
	 */
	bool loadFromFile(const std::string& path);

	/**
	 * @brief Check if texture is loaded.
	 * @return true If loaded.
	 * @return false Otherwise.
	 */
	inline bool isLoaded() const { return m_Loaded; }

private:
	int m_BPP;
	bool m_Loaded;
};

} // namespace vrm