#include "pch.h"
#include "Cube.h"
#include "../../Rendering/VertexBufferLayout.h"

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
        -1, -1,  1, color.r, color.g, color.b, //0
         1, -1,  1, color.r, color.g, color.b, //1
        -1,  1,  1, color.r, color.g, color.b, //2
         1,  1,  1, color.r, color.g, color.b, //3
        -1, -1, -1, color.r, color.g, color.b, //4
         1, -1, -1, color.r, color.g, color.b, //5
        -1,  1, -1, color.r, color.g, color.b, //6
         1,  1, -1, color.r, color.g, color.b, //7
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

void Cube::draw(const Shader& shader, const Renderer& renderer)
{
	renderer.draw(*va, *ib, shader);
}