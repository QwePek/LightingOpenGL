#include "pch.h"
#include "Camera.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <imgui/imgui.h>

#include "Utils/ImGuiExtension.h"

Camera::Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float roll = 0.0f)
{
    this->position = pos;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;
    
    rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(roll), front);

    recalculateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float roll)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    this->roll = roll;

    rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(roll), front);

    recalculateCameraVectors();
}

void Camera::processInput(direction dir, double dt)
{
    //Cam movement
    const float camSpeed = speed * dt;

    if (dir == direction::FORWARD)
        position += front * camSpeed;

    if (dir == direction::BACKWARD)
        position -= front * camSpeed;

    if (dir == direction::RIGHT)
        position += right * camSpeed;

    if (dir == direction::LEFT)
        position -= right * camSpeed;
}

void Camera::processMouse(double xOffs, double yOffs)
{
    if (!mouseOn)
        return;

    xOffs *= mouseSensitivity;
    yOffs *= mouseSensitivity;

    yaw += xOffs;
    pitch += yOffs;

    recalculateCameraVectors();
}

void Camera::processZoom(double xOffset, double yOffset)
{
    zoom -= (float)yOffset;

    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

glm::mat4 Camera::getProjectionMatrix(glm::vec2 windowSize)
{
    projection = glm::perspective(glm::radians(zoom), windowSize.x / (float)windowSize.y, 0.1f, 100.0f);

    return projection;
}

glm::mat4 Camera::getViewMatrix()
{
    view = glm::lookAt(position, position + front, up);
    return view;
}

void Camera::recalculateCameraVectors()
{
    //Constrains
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::drawImGui()
{
    ImGui::Begin("Camera", 0, ImGuiWindowFlags_AlwaysAutoResize);

    {
        ImGui::PushID("Local");
        ImGui::SeparatorText("Local");
        ImGui::PushID("Rotation");
        ImGui::Text("Rotation:"); ImGui::SameLine();

        float pitchDelta = pitch;
        float yawDelta = yaw;
        float rollDelta = roll;
        ImGui::Drag_Float3("X", "Y", "Z", &yaw, &pitch, &roll, -180, 180, -90, 90, -180, 180, 1);
        pitchDelta -= pitch;
        yawDelta -= yaw;
        rollDelta -= roll;

        if (yawDelta != 0)
        {
            rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(yawDelta), glm::vec3(1.0f, 0.0f, 0.0f));
            front = glm::mat3(rollMat) * front;
            right = glm::mat3(rollMat) * right;
            up = glm::mat3(rollMat) * up;
        }
        if (pitchDelta != 0)
        {
            rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(pitchDelta), glm::vec3(0.0f,1.0f,0.0f));
            front = glm::mat3(rollMat) * front;
            right = glm::mat3(rollMat) * right;
            up = glm::mat3(rollMat) * up;
        }
        if (rollDelta != 0)
        {
            rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(rollDelta), glm::vec3(0.0f, 0.0f, 1.0f));
            front = glm::mat3(rollMat) * front;
            right = glm::mat3(rollMat) * right;
            up = glm::mat3(rollMat) * up;
        }
        ImGui::PopID();
        ImGui::PopID();

        ImGui::PushID("LocalCam");
        ImGui::SeparatorText("Local");
        ImGui::PushID("Rotation");
        ImGui::Text("Rotation:"); ImGui::SameLine();

        static float localyawDelta = 0.f;
        static float localpitchDelta = 0.f;
        static float localrollDelta = 0.f;
        ImGui::Drag_Float3("X", "Y", "Z", &localyawDelta, &localpitchDelta, &localrollDelta, -180, 180, -90, 90, -180, 180, 1);

        if (localyawDelta != 0)
        {
            rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(localyawDelta), front);
            front = glm::mat3(rollMat) * front;
            right = glm::mat3(rollMat) * right;
            up = glm::mat3(rollMat) * up;
        }
        if (localpitchDelta != 0)
        {
            rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(localpitchDelta), up);
            front = glm::mat3(rollMat) * front;
            right = glm::mat3(rollMat) * right;
            up = glm::mat3(rollMat) * up;
        }
        if (localrollDelta != 0)
        {
            rollMat = glm::rotate(glm::mat4(1.0f), glm::radians(localrollDelta), right);
            front = glm::mat3(rollMat) * front;
            right = glm::mat3(rollMat) * right;
            up = glm::mat3(rollMat) * up;
        }

        ImGui::PopID();
        ImGui::PopID();

        ImGui::Text(("Roll delta: " + std::to_string(rollDelta)).c_str());

        ImGui::SeparatorText("Matrixes");
        ImGui::Text(("Front (" + std::to_string(front.x) + ", " + std::to_string(front.y) + ", " + std::to_string(front.z) + ")").c_str());
        ImGui::Text(("Right (" + std::to_string(right.x) + ", " + std::to_string(right.y) + ", " + std::to_string(right.z) + ")").c_str());
        ImGui::Text(("Up (" + std::to_string(up.x) + ", " + std::to_string(up.y) + ", " + std::to_string(up.z) + ")").c_str());
    }

    ImGui::End();
}

