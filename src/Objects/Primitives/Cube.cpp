#include "pch.h"
#include "Cube.h"
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Cube::Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr) : Object(pos, rot, sz, clr)
{
    init();
}

Cube::Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type) : Object(pos, rot, sz, clr, type)
{
    init();
}

void Cube::init()
{
    indices = {
        //Top
        0,1,2,
        0,2,3,
        //Bottom
        4,5,6,
        4,6,7,
        //Left
        8,9,10,
        8,10,11,
        //Right
        12,13,14,
        12,14,15,
        //Front
        16,17,18,
        16,18,19,
        //Back
        20,21,22,
        20,22,23
    };
    
    vertices = {
        //Position                //Normals
        -size.x, -size.y, -size.z, 0.0f,  0.0f, -1.0f,
        +size.x, -size.y, -size.z, 0.0f,  0.0f, -1.0f,
        +size.x, +size.y, -size.z, 0.0f,  0.0f, -1.0f,
        -size.x, +size.y, -size.z, 0.0f,  0.0f, -1.0f,

        -size.x, -size.y, +size.z, 0.0f,  0.0f, 1.0f,
        +size.x, -size.y, +size.z, 0.0f,  0.0f, 1.0f,
        +size.x, +size.y, +size.z, 0.0f,  0.0f, 1.0f,
        -size.x, +size.y, +size.z, 0.0f,  0.0f, 1.0f,
        
        -size.x, +size.y, +size.z, -1.0f,  0.0f, 0.0f,
        -size.x, +size.y, -size.z, -1.0f,  0.0f, 0.0f,
        -size.x, -size.y, -size.z, -1.0f,  0.0f, 0.0f,
        -size.x, -size.y, +size.z, -1.0f,  0.0f, 0.0f,
        
        +size.x, +size.y, +size.z, 1.0f,  0.0f, 0.0f,
        +size.x, +size.y, -size.z, 1.0f,  0.0f, 0.0f,
        +size.x, -size.y, -size.z, 1.0f,  0.0f, 0.0f,
        +size.x, -size.y, +size.z, 1.0f,  0.0f, 0.0f,
        
        -size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f,
        +size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f,
        +size.x, -size.y, +size.z, 0.0f, -1.0f, 0.0f,
        -size.x, -size.y, +size.z, 0.0f, -1.0f, 0.0f,
        
        -size.x, +size.y, -size.z, 0.0f, 1.0f, 0.0f,
        +size.x, +size.y, -size.z, 0.0f, 1.0f, 0.0f,
        +size.x, +size.y, +size.z, 0.0f, 1.0f, 0.0f,
        -size.x, +size.y, +size.z, 0.0f, 1.0f, 0.0f
    };
    
    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    va = new VertexArray();
    VertexBufferLayout layout;
    ib = new IndexBuffer(indices.data(), indices.size());
    layout.Push<float>(3); //pos
    layout.Push<float>(3); //normals

    va->addBuffer(*vb, layout);

    va->unbind();
    ib->unbind();
    vb->unbind();
}

void Cube::draw(const Shader& shader, const Renderer& renderer)
{
	renderer.draw(*va, *ib, shader);
}