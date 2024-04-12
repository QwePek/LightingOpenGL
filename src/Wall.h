#pragma once
#include "glm/glm.hpp"

#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/Renderer.h"
#include "Rendering/IndexBuffer.h"

class Wall
{
public:
	Wall(glm::vec3 _p1, glm::vec3 _p2, float h, float w);
	Wall(float p1_x, float p1_y, float p1_z, float p2_x, float p2_y, float p2_z, float h, float w);

	std::vector<glm::vec3> getGeometry();
	std::vector<uint32_t> getIndicies();
	
	void drawImGui();
	void draw(const Renderer& rend, const Shader& shader);

	inline glm::vec3 getPosition() { return p1; };

private:
	glm::vec3 p1, p2;
	float thickness;
	float height;

	VertexBuffer* vbo;
	VertexArray* va;
	IndexBuffer* ib;
};