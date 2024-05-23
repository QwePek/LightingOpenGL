#include "pch.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBufferLayout.h"
#include "Rendering/Texture.h"
#include "Camera.h"
#include "Wall.h"
#include "Objects/Primitives/Cube.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/DirectionalLight.h"
#include "Utils/Random.h"
#include "Objects/Primitives/Sphere.h"
#include "Objects/Primitives/Primitives.h"

glm::vec2 windowSize = glm::vec2(1024, 768);

Camera cam(glm::vec3(-4.0f, 4.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -45.0f, 0.0f);
bool firstMouse = true;
float lastX = windowSize.x / 2.0f;
float lastY = windowSize.y / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

enum shaderType { Phong, DepthBuffer, PBR };
shaderType currentShaderType = shaderType::Phong;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.processInput(direction::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.processInput(direction::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.processInput(direction::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.processInput(direction::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.processInput(direction::LEANLEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.processInput(direction::LEANRIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        cam.turnOffMouseMovement();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        cam.turnOnMouseMovement();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        currentShaderType = shaderType::Phong;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        currentShaderType = shaderType::DepthBuffer;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        currentShaderType = shaderType::PBR;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    float xp = static_cast<float>(xPos);
    float yp = static_cast<float>(yPos);

    if (firstMouse)
    {
        lastX = xp;
        lastY = yp;
        firstMouse = false;
    }

    float xoffset = xp - lastX;
    float yoffset = lastY - yp; // reversed since y-coordinates go from bottom to top

    lastX = xp;
    lastY = yp;

    cam.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    cam.processZoom(xOffset, yOffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{   
    srand(time(NULL));
    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfwInit()" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowSize.x, windowSize.y, "App", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;

    glViewport(0, 0, windowSize.x, windowSize.y);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cam.turnOffMouseMovement();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //imgui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextAlign, ImVec2(0.5f, 0.5f));


    //------------------
    Renderer renderer;
    Shader lightShader("src/Rendering/Shaders/lightShader.shader");
    Shader myShader("src/Rendering/Shaders/shader.shader");
    Shader depthShader("src/Rendering/Shaders/depthShader.shader");
    myShader.bind();

    std::unique_ptr<Light> directionalLight = std::make_unique<DirectionalLight>(glm::vec3(0.4f, -1.0f, 0.05f));
    std::vector<std::unique_ptr<Light>> pointLights;
    int numOfLights = 2;
    for (int i = 0; i < numOfLights; i++) {
        float distance = Utils::Random::generateRandomNumber(1, 100);
        pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(0.0f, -5.0f + 10.0f * i, 0.0f), distance));
    }

    std::vector<std::unique_ptr<Object>> worldObjects;
    int cubeCount = 100;
    int minX = 10;
    int minY = 10;
    int minZ = 10;
    worldObjects.reserve(cubeCount);

    for (int x = 0; x < cubeCount; x++)
    {
        float r = Utils::Random::generateRandomNumber(0.0, 1.0f);
        float g = Utils::Random::generateRandomNumber(0.0, 1.0f);
        float b = Utils::Random::generateRandomNumber(0.0, 1.0f);

        float sx = Utils::Random::generateRandomNumber(1.0f, 1.0f);
        float sy = Utils::Random::generateRandomNumber(1.0f, 1.0f);
        float sz = Utils::Random::generateRandomNumber(1.0f, 1.0f);

        float xPos = Utils::Random::generateRandomNumber(-minX, minX);
        float yPos = Utils::Random::generateRandomNumber(-minY, minY);
        float zPos = Utils::Random::generateRandomNumber(-minZ, minZ);
        int mat = Utils::Random::generateRandomNumber(2, Material::MATERIAL_SIZE);

        int primitive = Utils::Random::generateRandomNumber(0, Primitives::Models::MODELS_COUNT);
        std::unique_ptr<Object> obj;
        switch (primitive)
        {
            case Primitives::Models::Cube: 
                obj = std::make_unique<Cube>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(sx, sy, sz), glm::vec3(r, g, b), static_cast<Material::Type>(mat)); 
                break;
            case Primitives::Models::Sphere: 
                float radius = Utils::Random::generateRandomNumber(0.5f, 2.0f);
                int size = Utils::Random::generateRandomNumber(6, 30);

                obj = std::make_unique<Sphere>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f, 0.0f, 0.0f),
                    radius, glm::vec2(size, size), glm::vec3(r, g, b), static_cast<Material::Type>(mat));
                break;
        }

        worldObjects.push_back(std::move(obj));
    }

    glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    Texture tx("res\\textures\\MetalContainer.png");
    Texture tx1("res\\textures\\MetalContainer_specular.png");

    myShader.unbind();
    lightShader.unbind();
    depthShader.unbind();

    while (!glfwWindowShouldClose(window))
    {
        float currFrame = static_cast<float>(glfwGetTime());
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        renderer.clear();

        //ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            //cam.drawImGui();
            directionalLight->drawImGUI();
            for (int i = 0; i < pointLights.size(); i++) {
                pointLights[i]->drawImGUI(i);
            }

            ImGui::Begin("Scene settings");
            ImGui::DragFloat3("Size", &size[0]);
            ImGui::DragFloat3("Rotation", &rotation[0]);
            ImGui::End();
        }
        processInput(window);

        switch (currentShaderType)
        {
            case Phong:
                //Light drawing
                lightShader.bind();
                for (int i = 0; i < pointLights.size(); i++)
                {
                    lightShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                    lightShader.setUniformMat4f("view", cam.getViewMatrix());
                    lightShader.setUniformMat4f("model", pointLights[i]->getModelMatrix());
                    lightShader.setUniformVec3f("lightColor", pointLights[i]->getColor());
                    pointLights[i]->draw(lightShader, renderer);
                }
                lightShader.unbind();
                //Light drawing */

                myShader.bind();

                myShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                myShader.setUniformMat4f("view", cam.getViewMatrix());
                myShader.setUniformVec3f("viewPos", cam.getPosition());

                //Directional light
                myShader.setUniformVec3f("dirLight.direction", static_cast<DirectionalLight*>(directionalLight.get())->getDirection());
                myShader.setUniformVec3f("dirLight.ambient", directionalLight->getAmbient());
                myShader.setUniformVec3f("dirLight.diffuse", directionalLight->getDiffuse());
                myShader.setUniformVec3f("dirLight.specular", directionalLight->getSpecular());

                //Point light
                for (int i = 0; i < pointLights.size(); i++) {
                    myShader.setUniformVec3f("pointLight[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
                    myShader.setUniformVec3f("pointLight[" + std::to_string(i) + "].ambient", pointLights[i]->getAmbient());
                    myShader.setUniformVec3f("pointLight[" + std::to_string(i) + "].diffuse", pointLights[i]->getDiffuse());
                    myShader.setUniformVec3f("pointLight[" + std::to_string(i) + "].specular", pointLights[i]->getSpecular());
                    myShader.setUniform1f("pointLight[" + std::to_string(i) + "].constant", dynamic_cast<PointLight*>(pointLights[i].get())->getConstant());
                    myShader.setUniform1f("pointLight[" + std::to_string(i) + "].linear", dynamic_cast<PointLight*>(pointLights[i].get())->getLinear());
                    myShader.setUniform1f("pointLight[" + std::to_string(i) + "].quadratic", dynamic_cast<PointLight*>(pointLights[i].get())->getQuadratic());
                }
                break;

            case DepthBuffer:
                depthShader.bind();
                depthShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                depthShader.setUniformMat4f("view", cam.getViewMatrix());
                break;

            case PBR:

                break;
        }

        for (auto& obj : worldObjects)
        {
            obj->transform.setSize(size);
            obj->transform.setRotation(rotation);

            switch (currentShaderType)
            {
            case Phong:
                tx.bind(0);
                tx1.bind(1);
                myShader.setUniform1i("material.diffuse", 0);
                myShader.setUniform1i("material.specular", 1); //Assigning texture number of specular map

                //TODO doadac to do draw :)
                myShader.setUniform1f("material.shineness", obj->meshes[0].getShininess());
                myShader.setUniformMat4f("model", obj->transform.getModelMatrix());
                obj->draw(myShader, renderer);
                break;

            case DepthBuffer:
                depthShader.setUniformMat4f("model", obj->transform.getModelMatrix());
                obj->draw(depthShader, renderer);
                break;
            }
        }

        myShader.unbind();
        tx.unbind();
        tx1.unbind();
        //Objects drawing */

        //End Draw
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}