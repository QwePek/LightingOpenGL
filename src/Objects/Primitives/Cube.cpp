#include "pch.h"
#include "Cube.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "../Components/Transform/Transform.h"
#include <imgui/imgui.h>

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
        //Position                        //Normals          //Tx coords  //Tangents (losowe daje lol)
        Vertex(-size.x, -size.y, -size.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, -size.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, -size.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f),
        Vertex(-size.x, +size.y, -size.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f),

        Vertex(-size.x, -size.y, +size.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, +size.z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, +size.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f),
        Vertex(-size.x, +size.y, +size.z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f),

        Vertex(-size.x, +size.y, +size.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f),
        Vertex(-size.x, +size.y, -size.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f),
        Vertex(-size.x, -size.y, -size.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f),
        Vertex(-size.x, -size.y, +size.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f),

        Vertex(+size.x, +size.y, +size.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, -size.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, -size.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, +size.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f),

        Vertex(-size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, -size.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f),
        Vertex(+size.x, -size.y, +size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f),
        Vertex(-size.x, -size.y, +size.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f),

        Vertex(-size.x, +size.y, -size.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, -size.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f),
        Vertex(+size.x, +size.y, +size.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f),
        Vertex(-size.x, +size.y, +size.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f)
    };

    //Obliczanie bitangent
    for (unsigned int i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        glm::vec2 deltaUV1 = v1.txCoord - v0.txCoord;
        glm::vec2 deltaUV2 = v2.txCoord - v0.txCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent(f * (deltaUV2.y * edge1 - deltaUV1.y * edge2));

        v0.tangents += tangent;
        v1.tangents += tangent;
        v2.tangents += tangent;

        //glm::vec3 bitangent;
        //bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        //bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        //bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        //bitangent = glm::normalize(bitangent);

        //v0.Bitangent += bitangent;
        //v1.Bitangent += bitangent;
        //v2.Bitangent += bitangent;
    }

    if (model->meshes.size() != 0)
    {
        model->meshes[0].newVertices(vertices);
        model->meshes[0].newIndices(indices);
    }
    else
        model->meshes.emplace_back(vertices, indices, clr, type);
}

void Cube::drawImGUI(uint32_t additionalID)
{
    ImGui::Begin("Object");
    ImGui::DragFloat3("Position", &transform.position[0]);
    ImGui::End();
};