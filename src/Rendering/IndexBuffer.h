#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t* data, const uint32_t count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline uint32_t getCount() const { return indiciesCount; }

private:
	uint32_t rendererId;
	uint32_t indiciesCount;
};