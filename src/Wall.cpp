#include "pch.h"
#include "Wall.h"
#include <imgui/imgui.h>
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexBufferLayout.h"

Wall::Wall(glm::vec3 _p1, glm::vec3 _p2, float h, float w) : p1(_p1), p2(_p2), height(h), thickness(w)
{
    std::vector<glm::vec3> geometry = getGeometry();
    vbo = new VertexBuffer(geometry.data(), geometry.size() * sizeof(glm::vec3));
    va = new VertexArray();
    VertexBufferLayout layout;

    std::vector<uint32_t> indicies = getIndicies();

    ib = new IndexBuffer(indicies.data(), indicies.size());
    layout.Push<float>(3); //pos
    layout.Push<float>(3); //color

    va->addBuffer(*vbo, layout);

    vbo->unbind();
    va->unbind();
    ib->unbind();
}

Wall::Wall(float p1_x, float p1_y, float p1_z, float p2_x, float p2_y, float p2_z, float h, float w) :
    p1(glm::vec3(p1_x, p1_y, p1_z)), p2(glm::vec3(p2_x, p2_y, p2_z)), height(h), thickness(w)
{
    std::vector<glm::vec3> geometry = getGeometry();
    vbo = new VertexBuffer(geometry.data(), geometry.size() * sizeof(glm::vec3));
    va = new VertexArray();
    VertexBufferLayout layout;

    std::vector<uint32_t> indicies = getIndicies();

    ib = new IndexBuffer(indicies.data(), indicies.size());
    layout.Push<float>(3); //pos
    layout.Push<float>(3); //color

    va->addBuffer(*vbo, layout);

    vbo->unbind();
    va->unbind();
    ib->unbind();
}

std::vector<glm::vec3> Wall::getGeometry()
{
    std::vector<glm::vec3> geometry;

    geometry.push_back(p1);
    geometry.push_back(glm::vec3(1.0f, 1.0f, 1.0f));

    geometry.push_back(glm::vec3(p1.x, p1.y + height, p1.z));
    geometry.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

    geometry.push_back(p2);
    geometry.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    
    geometry.push_back(glm::vec3(p2.x, p2.y + height, p2.z));
    geometry.push_back(glm::vec3(0.0f, 1.0f, 1.0f));

    return geometry;
}

std::vector<uint32_t> Wall::getIndicies()
{
    std::vector<uint32_t> indices = { 0, 1, 2, 0, 1, 3 }; // Indeksy dla pojedynczej œciany (przyk³adowe)
    return indices;
}

void Wall::drawImGui()
{
    ImGui::Begin("Wall 1");

    ImGui::DragFloat("Height", &height);
    ImGui::DragFloat("Thickness", &thickness);
    ImGui::DragFloat3("P1", &p1[0]);
    ImGui::DragFloat3("P2", &p2[0]);

    ImGui::End();
}

void Wall::draw(const Renderer& rend, const Shader& shader)
{
    std::vector<glm::vec3> geometry = getGeometry();
    vbo->updateData(geometry.data(), geometry.size() * sizeof(glm::vec3));
    //for (int i = 0; i < geometry.size(); i++)
    //{
    //    std::cout << geometry[i].x << ", " << geometry[i].y << ", " << geometry[i].z << std::endl;
    //}
    //std::cout << "------------------------\n";

    rend.draw(*va, *ib, shader);
}