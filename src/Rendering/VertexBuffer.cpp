#include "pch.h"
#include "VertexBuffer.h"

#include "GL/glew.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
	rendererId = 0;
	GLCall(glGenBuffers(1, &rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));

	//Gl_STATIC_DRAW zaklada ze raz tylko zmienimy dane i bedziemy je uzywali czesto
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(uint32_t size)
{
	rendererId = 0;
	GLCall(glGenBuffers(1, &rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));

	//Gl_DYNAMIC_DRAW zaklada ze wiele razy zmienimy dane i bedziemy je uzywali czesto
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::updateData(const void* data, uint32_t size)
{
	bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}