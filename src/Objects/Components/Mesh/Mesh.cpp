#include "pch.h"
#include "Mesh.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

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

void Mesh::draw(const Shader& shader, const Renderer& renderer)
{
    renderer.draw(*va, *ib, shader);
}