#pragma once
#include "glm/glm.hpp"
#include "../Rendering/Renderer.h"
#include "Material.h"

class Object
{
public:
	Object() : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), size(glm::vec3(1.0f, 1.0f, 1.0f)),
		color(1.0f,1.0f,1.0f) { initMaterial(Material::Type::None); }
	Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr) : position(pos), rotation(rot), size(sz),
		color(clr) { initMaterial(Material::Type::None); }
	Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type) : position(pos), rotation(rot), size(sz),
		color(clr) { initMaterial(type); }
	~Object();

	virtual void draw(const Shader& shader, const Renderer& renderer) = 0;

	virtual void setPosition(glm::vec3 pos) { position = pos; };
	virtual void setRotation(glm::vec3 rot) { rotation = rot; };
	virtual void setSize(glm::vec3 sz) { size = sz; };
	virtual void setColor(glm::vec3 clr) { color = clr; };
	inline virtual glm::vec3 getPosition() { return position; };
	inline virtual glm::vec3 getRotation() { return rotation; };
	inline virtual glm::vec3 getSize() { return size; };
	inline virtual glm::vec3 getColor() { return color; };

	//Material properties
	virtual Material::Material& getMaterial() const { return *material; }
	inline virtual glm::vec3 getAmbient() { if (material == nullptr) return glm::vec3(0.0f); return material->ambient; }
	inline virtual glm::vec3 getDiffuse() { if (material == nullptr) return glm::vec3(0.0f); return material->diffuse; }
	inline virtual glm::vec3 getSpecular() { if (material == nullptr) return glm::vec3(0.0f); return material->specular; }
	inline virtual float getShininess() { if (material == nullptr) return 1; return material->shininess; }

	virtual glm::mat4 getModelMatrix();

protected:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	void initMaterial(Material::Type type);
	Material::Material* material = nullptr;

	//Draw variables
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	VertexBuffer* vb = nullptr;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;
};

