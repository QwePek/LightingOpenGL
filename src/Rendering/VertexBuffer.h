#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, uint32_t size);
	VertexBuffer(uint32_t size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void updateData(const void* data, uint32_t size);

private:
	uint32_t rendererId;
};