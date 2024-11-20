#pragma once

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
    virtual ~Texture2D();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    void create(int width, int height, Format format);

    inline unsigned int getRendererID() const { return m_RendererID; }

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
    unsigned int m_RendererID = 0;
	int m_Width = 0, m_Height = 0;
};

} // namespace vrm