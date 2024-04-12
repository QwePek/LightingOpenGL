#include "pch.h"
#include "IndexBuffer.h"

#include "GL/glew.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const uint32_t* data, const uint32_t count)
{
	ASSERT(sizeof(uint32_t) == sizeof(GLuint)); //To poniewaz na roznych kompach sa rozne rozmiary

	GLCall(glGenBuffers(1, &rendererId));
	bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));

	indiciesCount = count;
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}