#pragma once
#include "Components/Transform/Transform.h"
#include "Components/Model/Model.h"

class Object
{
public:
	Object() { model = new Model(); }
	Object(const std::string& modelPath) { model = new Model(modelPath); }
	~Object() { delete model; }
	void draw(Shader& shader, const Renderer& renderer) {
		shader.setUniformMat4f("model", transform.getModelMatrix());

		if (model != nullptr)
			model->draw(shader, renderer);
	}

	Transform transform;
	Model* model = nullptr;
};