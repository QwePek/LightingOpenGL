#pragma once
#include "Components/Transform/Transform.h"
#include "Components/Mesh/Mesh.h"

class Object
{
public:
	Object() {}
	void draw(const Shader& shader, const Renderer& renderer) {
		for (Mesh& m : meshes)
			m.draw(shader, renderer);
	}

	Transform transform;
	std::vector<Mesh> meshes;
};