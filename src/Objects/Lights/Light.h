#pragma once
#include "glm/glm.hpp"
#include "../../Rendering/Renderer.h"
#include "../Components/Mesh/Material.h"
#include "../../Rendering/VertexBufferLayout.h"

struct LightVertex {
	LightVertex(const glm::vec3& p) : position(p) { }
	LightVertex(float x, float y, float z) : position(x, y, z) { }
	glm::vec3 position;
};

class Light
{
public:
	Light() : position(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), size(1.0f, 1.0f, 1.0f) { initLight(); }
	Light(const glm::vec3& p) : position(p), color(1.0f, 1.0f, 1.0f), size(1.0f, 1.0f, 1.0f) { initLight(); }
	Light(const glm::vec3& p, const glm::vec3& clr);
	~Light();

	virtual void draw(const Shader& shader, const Renderer& renderer) = 0;
	virtual void drawImGUI(uint32_t additionalID = 0) = 0;

	virtual void setPosition(const glm::vec3& pos) { position = pos; };
	virtual void setSize(const glm::vec3& s) { size = s; };
	virtual void setColor(const glm::vec3& clr) { material->ambient = clr; };
	virtual void setIntensity(float in) { intensity = in; };

	inline virtual glm::vec3 getPosition() { return position; };
	inline virtual glm::vec3 getSize() { return size; };
	inline virtual glm::vec3 getColor() { return color; };
	inline virtual float getIntensity() { return intensity; };

	inline glm::vec3 getAmbient() { return color * intensity * material->ambient; }
	inline glm::vec3 getDiffuse() { return intensity * material->diffuse; }
	inline glm::vec3 getSpecular() { return intensity * material->specular; }

	//Material properties
	virtual Material::Material& getMaterial() const { return *material; }

	virtual glm::mat4 getModelMatrix();

protected:
	virtual void initLight();

	float intensity = 1.0f;
	glm::vec3 position, color, size;
	Material::Material* material = nullptr;

	//Draw variables
	std::vector<LightVertex> vertices;
	std::vector<uint32_t> indices;

	VertexBuffer* vb = nullptr;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;
	VertexBufferLayout layout;
};