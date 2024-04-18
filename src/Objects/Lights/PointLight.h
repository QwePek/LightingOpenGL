#pragma once
#include "../Object.h"

class PointLight : public Object
{
public:
	PointLight() { init(); }
	PointLight(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr);

	void draw(const Shader& shader, const Renderer& renderer);

private:
	void init();
};