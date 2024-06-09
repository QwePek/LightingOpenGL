#include "pch.h"
#include "Model.h"
#include "stb_image/stb_image.h"

void Model::loadModel(const std::string& pathStr)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(pathStr, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERORR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.parent_path();
	meshes.reserve(scene->mNumMeshes);
	processNode(scene->mRootNode, scene);

	std::cout << "Model loaded successfully: " << pathStr << std::endl;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<std::shared_ptr<Texture>> textures;
	
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		vertices.clear(); indices.clear(); textures.clear();

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, vertices, indices, textures);
		meshes.emplace_back(vertices, indices, textures);
	}
	
	for (uint32_t i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, 
	std::vector<std::shared_ptr<Texture>>& textures)
{
	//Vertices
	vertices.reserve(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec2 textureCoords(0.0f, 0.0f);
		if (mesh->mTextureCoords[0]) {
			textureCoords.x = mesh->mTextureCoords[0][i].x;
			textureCoords.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.emplace_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
			glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z), textureCoords);
	}

	//Indices
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, TextureType::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, TextureType::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		std::string fullTexturePath = directory.string() + "/";
		fullTexturePath.append(str.C_Str());

		for (uint32_t j = 0; j < texturesLoaded.size(); j++)
		{
			if (fullTexturePath == texturesLoaded[j]->getPath()) //std::strcmp(texturesLoaded[j]->getPath().data(), str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true;
				break;
			}
		}

		if (skip)
			continue;

		//std::shared_ptr<Texture> teksturka = std::make_shared<Texture>(str.C_Str(), typeName);
		std::shared_ptr<Texture> teksturka = std::make_shared<Texture>(fullTexturePath, typeName);
		textures.push_back(teksturka);
		texturesLoaded.push_back(teksturka);
	}

	return textures;
}