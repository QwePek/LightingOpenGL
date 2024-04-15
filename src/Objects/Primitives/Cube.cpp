#include "pch.h"
#include "Cube.h"
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Cube::Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr) : position(pos), rotation(rot), size(sz), color(clr)
{
    init();
}

Cube::~Cube()
{
    delete va;
    delete ib;
}

void Cube::init()
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
        -size.x, -size.y,  size.z, color.r, color.g, color.b, //0
         size.x, -size.y,  size.z, color.r, color.g, color.b, //1
        -size.x,  size.y,  size.z, color.r, color.g, color.b, //2
         size.x,  size.y,  size.z, color.r, color.g, color.b, //3
        -size.x, -size.y, -size.z, color.r, color.g, color.b, //4
         size.x, -size.y, -size.z, color.r, color.g, color.b, //5
        -size.x,  size.y, -size.z, color.r, color.g, color.b, //6
         size.x,  size.y, -size.z, color.r, color.g, color.b, //7
    };
    
    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    va = new VertexArray();
    VertexBufferLayout layout;
    ib = new IndexBuffer(indices.data(), indices.size());
    layout.Push<float>(3); //pos
    layout.Push<float>(3); //color

    va->addBuffer(*vb, layout);

    va->unbind();
    ib->unbind();
    vb->unbind();
}

glm::mat4 Cube::getModelMatrix()
{
    glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), position);
    modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::scale(modelMat, size);

    return modelMat;
}

void Cube::draw(const Shader& shader, const Renderer& renderer)
{
	renderer.draw(*va, *ib, shader);
}