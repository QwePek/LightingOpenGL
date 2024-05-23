#include "pch.h"
#include "Cube.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "../Components/Transform/Transform.h"

Cube::Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr)
{
    init(pos, rot, sz, clr, Material::None);
}

Cube::Cube(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type)
{
    init(pos, rot, sz, clr, type);
}

void Cube::init(glm::vec3 pos, glm::vec3 rot, glm::vec3 sz, glm::vec3 clr, Material::Type type)
{
    transform = Transform(pos, rot, sz);
    glm::vec3 size = transform.getSize();

    std::vector<uint32_t>indices = {
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
    
    std::vector<Vertex> vertices = {
        //Position                //Normals          //Texture coords
        Vertex(-size.x, -size.y, -size.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, -size.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
        Vertex(+size.x, +size.y, -size.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
        Vertex(-size.x, +size.y, -size.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),

        Vertex(-size.x, -size.y, +size.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, +size.z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),
        Vertex(+size.x, +size.y, +size.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
        Vertex(-size.x, +size.y, +size.z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),

        Vertex(-size.x, +size.y, +size.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        Vertex(-size.x, +size.y, -size.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(-size.x, -size.y, -size.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vertex(-size.x, -size.y, +size.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        
        Vertex(+size.x, +size.y, +size.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, -size.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
        Vertex(+size.x, -size.y, -size.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vertex(+size.x, -size.y, +size.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),

        Vertex(-size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
        Vertex(+size.x, -size.y, +size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
        Vertex(-size.x, -size.y, +size.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),

        Vertex(-size.x, +size.y, -size.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, -size.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),
        Vertex(+size.x, +size.y, +size.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
        Vertex(-size.x, +size.y, +size.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f)
    };

    if (meshes.size() != 0)
    {
        meshes[0].newVertices(vertices);
        meshes[0].newIndices(indices);
    }
    else
        meshes.emplace_back(vertices, indices, clr, type);
}