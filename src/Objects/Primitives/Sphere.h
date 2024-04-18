#pragma once
#include "glm/glm.hpp"
#include "../../Rendering/Renderer.h"
#include "../Object.h"
#include "../Material.h"

class Sphere : public Object
{
public:
	Sphere() { init(); }
	Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec2 size, glm::vec3 clr);
	Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec2 size, glm::vec3 clr, Material::Type type);

	void draw(const Shader& shader, const Renderer& renderer);

	inline void setRadius(float r) { radius = r; recalculateVertices(); }
	inline void setStack(uint16_t st) { stacks = st; recalculateVertices(); }
	inline void setSectors(uint16_t se) { sectors = se; recalculateVertices(); }

	inline float setRadius() { return radius; }
	inline uint16_t setStack() { return stacks; }
	inline uint16_t setSectors() { return sectors; }

private:
	void init();
	void recalculateVertices();

	float radius = 1.0f;
	uint16_t stacks = 2, sectors = 2;
};