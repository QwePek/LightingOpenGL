#pragma once
#include <iostream>

enum TextureType { None, Diffuse, Specular, Normal, 
	Albedo, AmbientOcclusion, Metallic, Roughness, HDR};

class Texture
{
public:
	Texture(const std::string& path, TextureType type);
	~Texture();

	void bind(uint32_t slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }

	inline uint32_t getID() { return texID; }
	inline std::string getPath() { return path; }
	inline TextureType getType() { return type; }

private:
	bool loadHDRTexture();

	uint32_t texID;
	TextureType type;
	std::string path;

	unsigned char* textureBuffer;
	int width, height, numOfChannels;
};