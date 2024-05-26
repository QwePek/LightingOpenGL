#pragma once

#include "glm/glm.hpp"

enum class direction { FORWARD, BACKWARD, LEFT, RIGHT, LEANRIGHT, LEANLEFT };
class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch, float roll);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, float roll);

	void processInput(direction dir, double dt);
	void processMouse(double xPos, double yPos);
	void processZoom(double xOffset, double yOffset);

	glm::mat4 getProjectionMatrix(glm::vec2 windowSize);
	glm::mat4 getViewMatrix();

	//Getters
	float getSpeed() const { return speed; };
	float getDefaultSpeed() const { return defaultSpeed; };
	float getMouseSensitivity() const { return mouseSensitivity; };
	glm::vec3 getPosition() const { return position; };

	//Setters
	void setSpeed(float sp) { if (sp <= 0) { std::cout << "Cannot set speed < 0!\n"; return; }; speed = sp; };
	void setMouseSensitivity(float sens) { if (sens <= 0) { std::cout << "Cannot set sensitivity < 0!\n"; return; }; mouseSensitivity = sens; };
	void turnOffMouseMovement() { mouseOn = false; };
	void turnOnMouseMovement() { mouseOn = true; };

	void drawImGui();
private:
	void recalculateCameraVectors();

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);    //z+ to do ty³u jest

	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 worldUp = up;

	//View matrix
	glm::mat4 view;

	//Projection matrix
	glm::mat4 projection;

	glm::mat4 rollMat;

	glm::vec3 tmpVec = glm::vec3(1.0f);
	//Mouse
	float lastX = 500, lastY = 400; //Middle of our screen 1000x800


	float yaw = -90.0f; //Patrzenie w boki
	float pitch = 0.0f; //Patrzenie w gore
	float roll = 0.0f;  //Obracanie sie jak beczka

	float speed = 3.0f;
	float defaultSpeed = 3.0f;
	float mouseSensitivity = 0.1f;
	float zoom = 45.0f;

	bool mouseOn = true;
};