#pragma once
#include <GL/glew.h>
#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}

		//ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() {};

	void reset() { stride = 0; length = 0; elements.clear(); }

	template<typename T>
	void Push(unsigned int count) = delete;

	template<>
	void Push<float>(unsigned int count)
	{
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
		length += count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
		length += count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
		length += count;
	}

	inline const std::vector<VertexBufferElement> getElements() const& { return elements; };
	inline uint32_t getStride() const { return stride; };
	inline size_t getLength() { return length; }

private:
	std::vector<VertexBufferElement> elements;
	uint32_t stride = 0;
	uint32_t length = 0;
};