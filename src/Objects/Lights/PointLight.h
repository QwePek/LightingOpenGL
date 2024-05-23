#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(glm::vec3 pos, float distance);

	void draw(const Shader& shader, const Renderer& renderer);
	void drawImGUI(uint32_t additionalID);

	void setLightDistance(float dist);
	inline float getConstant() { return constant; }
	inline float getLinear() { return linear; }
	inline float getQuadratic() { return quadratic; }

private:
	void init();

	float coveringDistance;
	float constant;
	float linear;
	float quadratic;
};