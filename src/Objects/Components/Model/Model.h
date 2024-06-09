#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	Model() { }
	Model(const std::string& _path) : path(_path) { loadModel(path.string()); }
	void draw(Shader& shader, const Renderer& renderer) {
		for (Mesh& m : meshes)
			m.draw(shader, renderer);
	}

	//DEBUG
	std::vector<Mesh> meshes;
private:
	void loadModel(const std::string& pathStr);
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices,
		std::vector<std::shared_ptr<Texture>>& textures);
	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, TextureType typeName);

	std::filesystem::path path;
	std::filesystem::path directory;
	std::vector<std::shared_ptr<Texture>> texturesLoaded;
};