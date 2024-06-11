#include "pch.h"
#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Sphere::Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::u32vec2 size, glm::vec3 clr) 
    : radius(radius), stacks(size.x), sectors(size.y)
{
    init(pos, rot, radius, clr, Material::None);
}

Sphere::Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::u32vec2 size, glm::vec3 clr, Material::Type type) 
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
    model->meshes.emplace_back(initVerts, initIndic, clr, type);
}

//Sphere generation help article - https://www.songho.ca/opengl/gl_sphere.html
void Sphere::recalculateVertices()
{
    std::vector<Vertex> updateVerts;
    std::vector<uint32_t> updateIndic;

    generateSphereVerticesAndIndices(updateVerts, updateIndic);
    //First cause it is primitive type
    model->meshes[0].newVertices(updateVerts);
    model->meshes[0].newIndices(updateIndic);
}

void Sphere::generateSphereVerticesAndIndices(std::vector<Vertex>& verRef, std::vector<uint32_t>& indRef)
{
    float inverseLength = 1.0f / radius;

    double sectorStepF = 2 * M_PI / sectors;
    double stackStepF = M_PI / stacks;
    verRef.clear();
    verRef.reserve(stacks * sectors);

    for (uint32_t stackStep = 0; stackStep <= stacks; stackStep++)
    {
        double beta = M_PI_2 - stackStep * stackStepF;

        double xy = radius * cos(beta);
        double z = radius * sin(beta);
        for (uint32_t sectorStep = 0; sectorStep <= sectors; sectorStep++)
        {
            double alpha = sectorStep * sectorStepF;

            double x = xy * cos(alpha);
            double y = xy * sin(alpha);
            double xNormal = x * inverseLength;
            double yNormal = y * inverseLength;
            double zNormal = z * inverseLength;;
            double s = (double)sectorStep / sectors;
            double t = (double)stackStep / stacks;
            verRef.emplace_back(x, y, z, xNormal, yNormal, zNormal, s, t, 0.0f, 0.0f, 0.0f); //Tangets losowe :(
        }
    }

    //Indicies
    indRef.clear();
    indRef.reserve(stacks * sectors);
    for (uint32_t stackStep = 0; stackStep < stacks; stackStep++)
    {
        int i1 = stackStep * (sectors + 1);
        int i2 = i1 + (sectors + 1);

        for (uint32_t sectorStep = 0; sectorStep < sectors; sectorStep++)
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

    //Obliczanie tangent
    for (unsigned int i = 0; i < indRef.size(); i += 3) {
        Vertex& v0 = verRef[indRef[i]];
        Vertex& v1 = verRef[indRef[i + 1]];
        Vertex& v2 = verRef[indRef[i + 2]];

        glm::vec3 edge1 = v1.position - v0.position;
        glm::vec3 edge2 = v2.position - v0.position;
        glm::vec2 deltaUV1 = v1.txCoord - v0.txCoord;
        glm::vec2 deltaUV2 = v2.txCoord - v0.txCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);

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

    // Normalizacja wynikowych tangentów i bitangentów dla ka¿dego wierzcho³ka
    for (unsigned int i = 0; i < verRef.size(); ++i) {
        verRef[i].tangents = glm::normalize(verRef[i].tangents);
        //vertices[i].Bitangent = glm::normalize(vertices[i].Bitangent);
    }
}
