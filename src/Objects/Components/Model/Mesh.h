#pragma once
#include "glm/glm.hpp"
#include "../../../Rendering/Renderer.h"
#include "Material.h"
#include "../../../Rendering/VertexBufferLayout.h"
#include "../../../Rendering/Texture.h"

struct Vertex {
	Vertex(const glm::vec3& p, const glm::vec3& n, const glm::vec2& txC, const glm::vec3& tan) : position(p), normal(n), txCoord(txC), tangents(tan) { }
	Vertex(float x, float y, float z, float nx, float ny, float nz, float txCoordX, float txCoordY, float tanX, float tanY, float tanZ) : position(x, y, z), 
		normal(nx, ny, nz), txCoord(txCoordX, txCoordY), tangents(tanX, tanY, tanZ) { }
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 txCoord;
	glm::vec3 tangents;
};

class Mesh
{
public:
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices);
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices, const std::vector<std::shared_ptr<Texture>> textures);
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices, const glm::vec3& clr);
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices, const glm::vec3& clr, Material::Type type);
	Mesh(const std::vector<Vertex> vertices, const std::vector<uint32_t> indices, 
		const std::vector<std::shared_ptr<Texture>> textures, const glm::vec3& clr, Material::Type type);
	~Mesh();
	
	//Creates new id with new indices
	void newIndices(const std::vector<uint32_t> i);

	//Creates new vb with new vertices
	void newVertices(const std::vector<Vertex> v);
	
	//Adds new texture
	void addTexture(std::shared_ptr<Texture> tx);
	void addTexture(const std::string& path, TextureType type);
	//void removeTexture(const TextureData& txData);
	void clearAllTextures() { textures.clear(); }

	//Updates current vb with new vertices - indices stay the same
	void updateVertices(const std::vector<Vertex> v);

	void draw(Shader& shader, const Renderer& renderer);

	virtual void setColor(const glm::vec3& clr) { color = clr; };

	//Material properties
	virtual Material::Material& getMaterial() const { return *material; }
	inline virtual glm::vec3 getColor() { return color; };
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
	std::vector<std::shared_ptr<Texture>> textures;

	VertexBuffer* vb = nullptr;
	VertexArray* va = nullptr;
	IndexBuffer* ib = nullptr;
	VertexBufferLayout layout;
};

