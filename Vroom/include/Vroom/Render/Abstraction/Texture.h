#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	Texture();
	virtual ~Texture();

	bool loadFromFile(const std::string& path);
	inline bool isLoaded() const { return m_Loaded; }

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }

private:
	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	bool m_Loaded;
};