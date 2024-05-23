#pragma once
#include "../Object.h"

class Cube : public Object
{
public:
	Cube() {
		init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f), Material::Type::None);
	}
	Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr);
	Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type);

	void drawImGUI(uint32_t additionalID) { };

private:
	void init(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type);
};