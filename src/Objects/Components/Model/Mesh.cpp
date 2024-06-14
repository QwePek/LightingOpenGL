#include "pch.h"
#include "Mesh.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "../../../Rendering/Texture.h"

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i) : color(1.0f, 1.0f, 1.0f), vertices(v), indices(i)
{
    material = new Material::Material(Material::Type::None);
    initMesh();
}

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i, const std::vector<std::shared_ptr<Texture>> t) 
    : vertices(v), indices(i), textures(t)
{
    material = new Material::Material(Material::Type::None);
    initMesh();
}

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i, const glm::vec3& clr) : color(clr), vertices(v), indices(i)
{
    material = new Material::Material(Material::Type::None);
    initMesh();
}

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i, const glm::vec3& clr, Material::Type type) : color(clr), vertices(v), indices(i)
{
    material = new Material::Material(type);
    initMesh();
}

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i, const std::vector<std::shared_ptr<Texture>> t, const glm::vec3& clr, Material::Type type)
    : color(clr), vertices(v), indices(i), textures(t)
{
    material = new Material::Material(type);
    initMesh();
}

Mesh::~Mesh()
{
    delete vb;
    delete va;
    delete ib;

    delete material;
}

void Mesh::initMesh()
{
    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    va = new VertexArray();
    ib = new IndexBuffer(indices.data(), indices.size());
    layout.reset();
    layout.Push<float>(3); //pos
    layout.Push<float>(3); //normals
    layout.Push<float>(2); //text coords
    layout.Push<float>(3); //tangents

    va->addBuffer(*vb, layout);

    va->unbind();
    ib->unbind();
    vb->unbind();
}

void Mesh::updateVertices(const std::vector<Vertex> v)
{
    vertices = v;
    vb->updateData(vertices.data(), vertices.size() * sizeof(Vertex));
}

void Mesh::newVertices(const std::vector<Vertex> v)
{
    vertices = v;
    delete vb;
    delete va;
    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    va = new VertexArray();

    va->addBuffer(*vb, layout);

    vb->unbind();
    va->unbind();
}

void Mesh::newIndices(const std::vector<uint32_t> i)
{
    indices = i;
    delete ib;
    ib = new IndexBuffer(indices.data(), indices.size());

    ib->unbind();
}

void Mesh::addTexture(std::shared_ptr<Texture> tx)
{
    textures.push_back(tx);
}

void Mesh::addTexture(const std::string& path, TextureType type)
{
    textures.push_back(std::make_shared<Texture>(path, type));
}

void Mesh::draw(Shader& shader, const Renderer& renderer)
{
    uint32_t matID[7] = { };
    std::string str;
    switch (shader.getType())
    {
    case BlinnPhongNormalMapping:
    case BlinnPhong:
    case Phong:
        shader.setUniform1f("material0.shineness", getShininess());
        
        for (uint32_t i = 0; i < textures.size(); i++)
        {
            textures[i]->bind(i);
            switch (textures[i]->getType())
            {
            case Diffuse:
                shader.setUniform1i("material" + std::to_string(matID[0]++) + ".diffuse", i);
                break;
            case Specular:
                shader.setUniform1i("material" + std::to_string(matID[1]++) + ".specular", i);
                break;
            case Normal:
                if (shader.getType() == BlinnPhongNormalMapping)
                    shader.setUniform1i("material" + std::to_string(matID[2]++) + ".normalMap", i);
                break;
            }
        }
        break;

    case PBR:
        for (uint32_t i = 0; i < textures.size(); i++)
        {
            textures[i]->bind(i);
            switch (textures[i]->getType())
            {
            case Normal:
                //+3 BO SA IRRADIANCE I PREFILTERED I BRDF - POTEM TO OGARNIJ
                shader.setUniform1i("material" + std::to_string(matID[2]++) + ".normalMap", i);
                break;
            case Albedo:
                shader.setUniform1i("material" + std::to_string(matID[3]++) + ".albedoMap", i);
                break;
            case AmbientOcclusion:
                shader.setUniform1i("material" + std::to_string(matID[4]++) + ".aoMap", i);
                break;
            case Metallic:
                shader.setUniform1i("material" + std::to_string(matID[5]++) + ".metallicMap", i );
                break;
            case Roughness:
                shader.setUniform1i("material" + std::to_string(matID[6]++) + ".roughnessMap", i);
                break;
            //Support for Phong materials
            case Diffuse:
                shader.setUniform1i("material" + std::to_string(matID[3]++) + ".albedoMap", i);
                break;
            case Specular:
                shader.setUniform1i("material" + std::to_string(matID[3]++) + ".metallicMap", i);
                break;
            }
        }
        break;
    }
    //glColor3f(0, 0, 1);
    //glBegin(GL_LINES);
    //for (int i = 0; i < indices.size(); i++) {
    //glColor3f(0, 0, 1);
    //    glm::vec3 p = vertices[indices[i]].position;
    //    glVertex3fv(&p.x);

    //    glm::vec3 o = glm::normalize(vertices[indices[i]].tangents);
    //    p += o * 0.1f;
    //    glVertex3fv(&p.x);

    //    glColor3f(1, 0, 0);
    //    p = vertices[indices[i]].position;
    //    glVertex3fv(&p.x);
    //    glm::vec3 t = glm::normalize(vertices[indices[i]].tangents);
    //    p += t * 0.1f;
    //    glVertex3fv(&p.x);
    //}
    //glEnd();

    renderer.draw(*va, *ib, shader);
    for (auto& tx : textures)
        tx->unbind();
}