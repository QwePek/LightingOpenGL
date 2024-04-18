#include "pch.h"
#include "PointLight.h"
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

PointLight::PointLight(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr) : Object(pos, rot, sz, clr)
{
    init();
}

void PointLight::init()
{
    indices = {
        //Top
        2, 6, 7,
        2, 3, 7,
        //Bottom
        0, 4, 5,
        0, 1, 5,
        //Left
        0, 2, 6,
        0, 4, 6,
        //Right
        1, 3, 7,
        1, 5, 7,
        //Front
        0, 2, 3,
        0, 1, 3,
        //Back
        4, 6, 7,
        4, 5, 7
    };

    vertices = {
        -size.x, -size.y,  size.z, //0
         size.x, -size.y,  size.z, //1
        -size.x,  size.y,  size.z, //2
         size.x,  size.y,  size.z, //3
        -size.x, -size.y, -size.z, //4
         size.x, -size.y, -size.z, //5
        -size.x,  size.y, -size.z, //6
         size.x,  size.y, -size.z, //7
    };

    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    va = new VertexArray();
    VertexBufferLayout layout;
    ib = new IndexBuffer(indices.data(), indices.size());
    layout.Push<float>(3); //pos

    va->addBuffer(*vb, layout);

    va->unbind();
    ib->unbind();
    vb->unbind();
}

void PointLight::draw(const Shader& shader, const Renderer& renderer)
{
    renderer.draw(*va, *ib, shader);
}