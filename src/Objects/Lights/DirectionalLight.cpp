#include "pch.h"
#include "DirectionalLight.h"
#include <imgui/imgui.h>

void DirectionalLight::drawImGUI(uint32_t additionalID)
{
    ImGui::Begin("Directional light");
    
    ImGui::DragFloat3("Direction", &direction[0], 0.05f, -1.0f, 1.0f);
    ImGui::DragFloat("##Intensity", &intensity, 0.1f, 0.0f, 5.0f);
    ImGui::SameLine();
    ImGui::ColorEdit3("##Color", &color[0], ImGuiColorEditFlags_NoInputs);    

    ImGui::SeparatorText("Material properties");
    ImGui::DragFloat3("Ambient", &material->ambient[0], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat3("Diffuse", &material->diffuse[0], 0.05f, 0.0f, 1.0f);
    ImGui::DragFloat3("Specular", &material->specular[0], 0.05f, 0.0f, 1.0f);
    
    ImGui::End();
}