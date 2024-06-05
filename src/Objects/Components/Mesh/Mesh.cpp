#include "pch.h"
#include "Mesh.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "../../../Rendering/Texture.h"

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i) : color(1.0f, 1.0f, 1.0f)
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

Mesh::Mesh(const std::vector<Vertex> v, const std::vector<uint32_t> i, const std::vector<TextureData> t, const glm::vec3& clr, Material::Type type)
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

void Mesh::addTexture(const TextureData& txData)
{
    for (auto& tx : textures) {
        if (tx.id == txData.id && tx.type == txData.type) {
            std::cout << "Texture is the same, not added!" << std::endl;
            return;
        }
    }

    textures.emplace_back(txData.id, txData.type, txData.txRef);
}

void Mesh::removeTexture(const TextureData& txData)
{
    for (int i = 0; i < textures.size(); i++) {
        if (textures[i].id == txData.id && textures[i].type == txData.type) {
            textures.erase(textures.begin() + i);
            return;
        }
    }

    std::cout << "Cannot remove texture, it does not exist!" << std::endl;
}

void Mesh::draw(Shader& shader, const Renderer& renderer)
{
    switch (shader.getType())
    {
    case BlinnPhong:
    case Phong:
        int matDiffID = 0;
        int matSpecID = 0;
        std::string str;
        shader.setUniform1f("material0.shineness", getShininess());
        
        for (auto& tx : textures)
        {
            tx.txRef->bind(tx.id);
            switch (tx.type)
            {
            case Diffuse: 
                shader.setUniform1f("material" + std::to_string(matDiffID++) + ".diffuse", tx.id); //Assigning texture number of specular map
                break;
            case Specular: 
                shader.setUniform1f("material" + std::to_string(matSpecID++) + ".specular", tx.id); //Assigning texture number of specular map
                break;
            }
        }
        break;
    }

    renderer.draw(*va, *ib, shader);
    for (auto& tx : textures)
        tx.txRef->unbind();
}