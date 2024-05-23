#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 dir) : direction(dir) { }

	inline void setDirection(glm::vec3 dir) { direction = dir; }
	inline glm::vec3 getDirection() { return direction; }

	void draw(const Shader& shader, const Renderer& renderer) { }
	void drawImGUI(uint32_t additionalID);

private:
	glm::vec3 direction;
};