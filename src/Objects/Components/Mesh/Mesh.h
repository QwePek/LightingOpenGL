#pragma once
#include "glm/glm.hpp"
#include "../../../Rendering/Renderer.h"
#include "Material.h"
#include "../../../Rendering/VertexBufferLayout.h"

struct Vertex {
	Vertex(const glm::vec3& p, const glm::vec3& n, const glm::vec2& txC) : position(p), normal(n), txCoord(txC) { }
	Vertex(float x, float y, float z, float nx, float ny, float nz, float txCoordX, float txCoordY) : position(x, y, z), normal(nx, ny, nz), txCoord(txCoordX, txCoordY) { }
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 txCoord;
};

struct TextureData {
	uint32_t id;
	std::string type;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices);
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices, const glm::vec3& clr);
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices, const glm::vec3& clr, Material::Type type);
	~Mesh();
	
	//Creates new id with new indices
	void newIndices(const std::vector<uint32_t> i);

	//Creates new vb with new vertices
	void newVertices(const std::vector<Vertex> v);

	//Updates current vb with new vertices - indices stay the same
	void updateVertices(const std::vector<Vertex> v);

	void draw(const Shader& shader, const Renderer& renderer);

	virtual void setColor(const glm::vec3& clr) { color = clr; };
	inline virtual glm::vec3 getColor() { return color; };

	//Material properties
	virtual Material::Material& getMaterial() const { return *material; }
	inline virtual glm::vec3 getAmbient() { if (material == nullptr) return glm::vec3(0.0f); return material->ambient; }
	inline virtual glm::vec3 getDiffuse() { if (material == nullptr) return glm::vec3(0.0f); return material->diffuse; }
	inline virtual glm::vec3 getSpecular() { if (material == nullptr) return glm::vec3(0.0f); return material->specular; }
	inline virtual float getShininess() { if (material == nullptr) return 1; return material->shininess; }

protected:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	void initMesh();
	Material::Material* material = nullptr;

	//Draw variables
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<TextureData> textures;

	VertexBuffer* vb = nullptr;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;
	VertexBufferLayout layout;
};
