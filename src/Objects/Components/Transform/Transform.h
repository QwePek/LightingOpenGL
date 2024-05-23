#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

class Transform
{
public:
	Transform() : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), size(glm::vec3(1.0f, 1.0f, 1.0f)) { }
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz) : position(pos), rotation(rot), size(sz) { }

	virtual void setPosition(glm::vec3 pos) { position = pos; };
	virtual void setRotation(glm::vec3 rot) { rotation = rot; };
	virtual void setSize(glm::vec3 sz) { size = sz; };
	inline virtual glm::vec3 getPosition() { return position; };
	inline virtual glm::vec3 getRotation() { return rotation; };
	inline virtual glm::vec3 getSize() { return size; };

	glm::mat4 getModelMatrix() {
		glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);
		modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMat = glm::scale(modelMat, size);

		return modelMat;
	}

protected:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
};