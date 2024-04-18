#include "pch.h"
#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

Sphere::Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec2 size, glm::vec3 clr) 
    : Object(pos, rot, glm::vec3(1.0f, 1.0f, 1.0f), clr), radius(radius), stacks(size.x), sectors(size.y)
{
    init();
}

Sphere::Sphere(glm::vec3 pos, glm::vec3 rot, float radius, glm::vec2 size, glm::vec3 clr, Material::Type type) 
    : Object(pos, rot, glm::vec3(1.0f, 1.0f, 1.0f), clr, type), stacks(size.x), sectors(size.y)
{
    init();
}

//Sphere generation help article - https://www.songho.ca/opengl/gl_sphere.html
void Sphere::init()
{
    recalculateVertices();
}

void Sphere::draw(const Shader& shader, const Renderer& renderer)
{
    renderer.draw(*va, *ib, shader);
}

void Sphere::recalculateVertices()
{
    indices.clear();
    vertices.clear();
    float inverseLength = 1.0f / radius;

    float sectorStepF = 2 * M_PI / sectors;
    float stackStepF = M_PI / stacks;

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

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            float xNormal = x * inverseLength;
            float yNormal = y * inverseLength;
            float zNormal = z * inverseLength;;

            vertices.push_back(xNormal);
            vertices.push_back(yNormal);
            vertices.push_back(zNormal);
        }
    }

    //for (int i = 0; i < vertices.size(); i++)
    //{
    //    if (i % 6 == 0)
    //        std::cout << std::endl;
    //    std::cout << vertices[i] << ", ";
    //}

    //Indicies
    indices.clear();
    for (int stackStep = 0; stackStep < stacks; stackStep++)
    {
        int i1 = stackStep * (sectors + 1);
        int i2 = i1 + (sectors + 1);

        for (int sectorStep = 0; sectorStep < sectors; sectorStep++)
        {
            if (stackStep != 0)
            {
                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i1 + 1);
            }

            if (stackStep != stacks + 1)
            {
                indices.push_back(i1 + 1);
                indices.push_back(i2);
                indices.push_back(i2 + 1);
            }

            i1++;
            i2++;
        }
    }

    if (vb != nullptr)
        delete vb;
    if (va != nullptr)
        delete va;
    if (ib != nullptr)
        delete ib;

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