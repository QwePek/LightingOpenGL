#pragma once
#include "glm/glm.hpp"
#include "../../Rendering/Renderer.h"
#include "../Object.h"
#include "../Material.h"

class Cube : public Object
{
public:
	Cube() { init(); }
	Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr);
	Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type);

	void draw(const Shader& shader, const Renderer& renderer);

private:
	void init();
};