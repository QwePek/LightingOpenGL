#pragma once
#include "../Object.h"

class Sphere : public Object
{
public:
	Sphere() {
		init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), Material::Type::None);
	}
	Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::u32vec2 size, glm::vec3 clr);
	Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::u32vec2 size, glm::vec3 clr, Material::Type type);

	void drawImGUI(uint32_t additionalID) { };

	inline void setRadius(float r) { radius = r; recalculateVertices(); }
	inline void setStack(uint16_t st) { stacks = st; recalculateVertices(); }
	inline void setSectors(uint16_t se) { sectors = se; recalculateVertices(); }

	inline float setRadius() { return radius; }
	inline uint16_t setStack() { return stacks; }
	inline uint16_t setSectors() { return sectors; }

private:
	void init(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec3 clr, Material::Type type);
	void recalculateVertices();

	void generateSphereVerticesAndIndices(std::vector<Vertex>& vecRef, std::vector<uint32_t>& indRef);

	float radius = 1.0f;
	uint32_t stacks = 2, sectors = 2;
};