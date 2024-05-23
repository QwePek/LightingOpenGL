#include "pch.h"
#include "Light.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Light::Light(const glm::vec3& p, const glm::vec3& clr) : position(p), color(clr), size(1.0f, 1.0f, 1.0f)
{
    initLight();
}

Light::~Light()
{
    delete vb;
    delete va;
    delete ib;

    delete material;
}

void Light::initLight()
{
    material = new Material::Material(Material::Custom);
    material->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    material->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    material->specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::mat4 Light::getModelMatrix()
{
    return glm::translate(glm::mat4(1.0f), position);
}