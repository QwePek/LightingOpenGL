#pragma once
#include "glm/glm.hpp"
#include "../../Rendering/Renderer.h"

class Cube
{
public:
	Cube() : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), size(glm::vec3(1.0f, 1.0f, 1.0f)),
		color(glm::vec3(1.0f,1.0f,1.0f)){ init(); }
	Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr);
	~Cube();

	void draw(const Shader& shader, const Renderer& renderer);

	inline glm::vec3 getPosition() { return position; };
	inline glm::vec3 getRotation() { return rotation; };
	inline glm::vec3 getSize() { return size; };

private:
	void init();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 size;

	glm::vec3 color;

	//Draw variables
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	VertexBuffer* vb;
	VertexArray* va;
	IndexBuffer* ib;
};