#include "pch.h"
#include "Object.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Object::~Object()
{
    delete vb;
    delete va;
    delete ib;

    delete material;
}

void Object::initMaterial(Material::Type type)
{
    material = new Material::Material(type);
}

glm::mat4 Object::getModelMatrix()
{
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);
    modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::scale(modelMat, size);

    return modelMat;
}