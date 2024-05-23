#include "pch.h"
#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Sphere::Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec2 size, glm::vec3 clr) 
    : radius(radius), stacks(size.x), sectors(size.y)
{
    init(pos, rot, radius, clr, Material::None);
}

Sphere::Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec2 size, glm::vec3 clr, Material::Type type) 
    : radius(radius), stacks(size.x), sectors(size.y)
{
    init(pos, rot, radius, clr, type);
}

void Sphere::init(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec3 clr, Material::Type type)
{
    transform = Transform(pos, rot, glm::vec3(radius, radius, radius));
    std::vector<Vertex> initVerts;
    std::vector<uint32_t> initIndic;

    generateSphereVerticesAndIndices(initVerts, initIndic);
    meshes.emplace_back(initVerts, initIndic, clr, type);
}

//Sphere generation help article - https://www.songho.ca/opengl/gl_sphere.html
void Sphere::recalculateVertices()
{
    std::vector<Vertex> updateVerts;
    std::vector<uint32_t> updateIndic;

    generateSphereVerticesAndIndices(updateVerts, updateIndic);
    //First cause it is primitive type
    meshes[0].newVertices(updateVerts);
    meshes[0].newIndices(updateIndic);
}

void Sphere::generateSphereVerticesAndIndices(std::vector<Vertex>& verRef, std::vector<uint32_t>& indRef)
{
    float inverseLength = 1.0f / radius;

    float sectorStepF = 2 * M_PI / sectors;
    float stackStepF = M_PI / stacks;
    verRef.clear();
    verRef.reserve(stacks * sectors);

    for (int stackStep = 0; stackStep <= stacks; stackStep++)
    {
        float beta = M_PI_2 - stackStep * stackStepF;

        float xy = radius * cos(beta);
        float z = radius * sin(beta);
        for (int sectorStep = 0; sectorStep <= sectors; sectorStep++)
        {
            float alpha = sectorStep * sectorStepF;

            float x = xy * cos(alpha);
            float y = xy * sin(alpha);
            float xNormal = x * inverseLength;
            float yNormal = y * inverseLength;
            float zNormal = z * inverseLength;;
            float s = (float)sectorStep / sectors;
            float t = (float)stackStep / stacks;
            verRef.emplace_back(x, y, z, xNormal, yNormal, zNormal, s, t);
        }
    }

    //Indicies
    indRef.clear();
    indRef.reserve(stacks * sectors);
    for (int stackStep = 0; stackStep < stacks; stackStep++)
    {
        int i1 = stackStep * (sectors + 1);
        int i2 = i1 + (sectors + 1);

        for (int sectorStep = 0; sectorStep < sectors; sectorStep++)
        {
            if (stackStep != 0) {
                indRef.push_back(i1);
                indRef.push_back(i2);
                indRef.push_back(i1 + 1);
            }

            if (stackStep != stacks + 1) {
                indRef.push_back(i1 + 1);
                indRef.push_back(i2);
                indRef.push_back(i2 + 1);
            }

            i1++;
            i2++;
        }
    }
}
