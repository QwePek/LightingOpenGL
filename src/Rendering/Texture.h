#pragma once
#include <iostream>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void bind(uint32_t slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

	inline uint32_t getTextureID() { return texID; }

private:
	uint32_t texID;
	std::string texturePath;

	unsigned char* textureBuffer;
	int width, height, numOfChannels;
};