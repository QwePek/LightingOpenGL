#include "pch.h"
#include "Texture.h"

#include <GL/glew.h>
#include "stb_image/stb_image.h"

#include "Renderer.h"

Texture::Texture(const std::string& path)
	: texturePath(path), texID(0), textureBuffer(nullptr), width(0), height(0), numOfChannels(0)
{
	stbi_set_flip_vertically_on_load(true);
	textureBuffer = stbi_load(texturePath.c_str(), &width, &height, &numOfChannels, 4);

	GLCall(glGenTextures(1, &texID));
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
	glGenerateMipmap(GL_TEXTURE_2D);

	if (textureBuffer)
		stbi_image_free(textureBuffer);
	else
		std::cout << "FAILED TO LOAD TEXTURE!" << std::endl;

	unbind();
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &texID));
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