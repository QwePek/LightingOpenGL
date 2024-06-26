#include "pch.h"
#include "Texture.h"

#include <GL/glew.h>
#include "stb_image/stb_image.h"

#include "Renderer.h"

Texture::Texture(const std::string& path, TextureType t)
	: path(path), type(t), texID(0), textureBuffer(nullptr), width(0), height(0), numOfChannels(0)
{
	if (type == TextureType::HDR) {
		loadHDRTexture();
		return;
	}

	std::cout << "Loading texture: " << path << std::endl;
	stbi_set_flip_vertically_on_load(true);
	textureBuffer = stbi_load(path.c_str(), &width, &height, &numOfChannels, 4);

	if (!textureBuffer) {
		std::cout << "FAILED TO LOAD TEXTURE!" << std::endl;
		return;
	}

	GLCall(glGenTextures(1, &texID));
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(textureBuffer);

	unbind();
}

bool Texture::loadHDRTexture()
{
	std::cout << "Loading HDR texture: " << path << std::endl;
	stbi_set_flip_vertically_on_load(true);
	float *data = stbi_loadf(path.c_str(), &width, &height, &numOfChannels, 0);

	if (!data) {
		std::cout << "FAILED TO LOAD HDR TEXTURE!" << std::endl;
		return false;
	}
	
	GLCall(glGenTextures(1, &texID));
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data));
	stbi_image_free(data);

	return true;
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &texID));
	std::cout << "Deleting texture: " << path << std::endl;
}

void Texture::bind(uint32_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));
}

void Texture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}