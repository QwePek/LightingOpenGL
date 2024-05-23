#include "pch.h"
#include "PointLight.h"
#include "../../Rendering/VertexBufferLayout.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <imgui/imgui.h>

PointLight::PointLight(glm::vec3 pos, float distance) : Light(pos)
{
    init();
    setLightDistance(distance);
}

void PointLight::init()
{
    indices = {
        //Top
        2, 6, 7,
        2, 3, 7,
        //Bottom
        0, 4, 5,
        0, 1, 5,
        //Left
        0, 2, 6,
        0, 4, 6,
        //Right
        1, 3, 7,
        1, 5, 7,
        //Front
        0, 2, 3,
        0, 1, 3,
        //Back
        4, 6, 7,
        4, 5, 7
    };

    vertices = {
        LightVertex(-size.x, -size.y,  size.z), //0
        LightVertex( size.x, -size.y,  size.z), //1
        LightVertex(-size.x,  size.y,  size.z), //2
        LightVertex( size.x,  size.y,  size.z), //3
        LightVertex(-size.x, -size.y, -size.z), //4
        LightVertex( size.x, -size.y, -size.z), //5
        LightVertex(-size.x,  size.y, -size.z), //6
        LightVertex( size.x,  size.y, -size.z), //7
    };

    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(LightVertex));
    va = new VertexArray();
    ib = new IndexBuffer(indices.data(), indices.size());
    layout.reset();
    layout.Push<float>(3); //pos

    va->addBuffer(*vb, layout);

    va->unbind();
    ib->unbind();
    vb->unbind();
}

void PointLight::setLightDistance(float dist)
{
    coveringDistance = dist;
    constant = 1.0f;

    if (coveringDistance <= 7) {
        linear = 0.7f;
        quadratic = 1.8f;
    }
    else if (coveringDistance <= 13) {
        linear = 0.35f;
        quadratic = 0.44f;
    }
    else if (coveringDistance <= 20) {
        linear = 0.22f;
        quadratic = 0.2f;
    }
    else if (coveringDistance <= 32) {
        linear = 0.14f;
        quadratic = 0.07f;
    }
    else if (coveringDistance <= 50) {
        linear = 0.09f;
        quadratic = 0.032f;
    }
    else if (coveringDistance <= 65) {
        linear = 0.07f;
        quadratic = 0.017f;
    }
    else if (coveringDistance <= 100) {
        linear = 0.045f;
        quadratic = 0.0075f;
    }
    else if (coveringDistance <= 160) {
        linear = 0.027f;
        quadratic = 0.0028f;
    }
    else if (coveringDistance <= 200) {
        linear = 0.022f;
        quadratic = 0.0019f;
    }
    else if (coveringDistance <= 325) {
        linear = 0.014f;
        quadratic = 0.0007f;
    }
    else if (coveringDistance <= 600) {
        linear = 0.007f;
        quadratic = 0.0002f;
    }
    else {
        linear = 0.0014f;
        quadratic = 0.000007f;
    }
}

void PointLight::draw(const Shader& shader, const Renderer& renderer)
{
    renderer.draw(*va, *ib, shader);
}

void PointLight::drawImGUI(uint32_t additionalID)
{
    ImGui::Begin(("Point light " + std::to_string(additionalID)).c_str());

    ImGui::DragFloat3("Position", &position[0]);
    ImGui::DragFloat("##Intensity", &intensity, 0.1f, 0.0f, 5.0f);
    ImGui::SameLine();
    ImGui::ColorEdit3("##Color", &color[0], ImGuiColorEditFlags_NoInputs);

    ImGui::SeparatorText("Point light properties");
    if (ImGui::DragFloat("Distance", &coveringDistance, 1.0f, 0.0f, 10000.0f))
        setLightDistance(coveringDistance);
    

    ImGui::SeparatorText("Material properties");
    ImGui::DragFloat3("Ambient", &material->ambient[0], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat3("Diffuse", &material->diffuse[0], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat3("Specular", &material->specular[0], 0.05f, 0.0f, 1.0f);

    ImGui::End();
}