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

shaderType currentShaderType = shaderType::Phong;

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.setSpeed(cam.getDefaultSpeed() * 5);
    else
        cam.setSpeed(cam.getDefaultSpeed());


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
        currentShaderType = shaderType::BlinnPhong;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        currentShaderType = shaderType::BlinnPhongNormalMapping;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        currentShaderType = shaderType::PBR;
    }



    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        currentShaderType = shaderType::DepthBuffer;
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
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
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
    Shader lightShader("src/Rendering/Shaders/lightShader.shader", shaderType::LightSh);
    Shader phong("src/Rendering/Shaders/Phong.shader", shaderType::Phong);
    Shader depthShader("src/Rendering/Shaders/depthShader.shader", shaderType::DepthBuffer);
    Shader blinnPhong("src/Rendering/Shaders/BlinnPhong.shader", shaderType::BlinnPhong);
    Shader blinnPhongNormal("src/Rendering/Shaders/BlinnPhongNormalMapping.shader", shaderType::BlinnPhongNormalMapping);
    Shader pbrShader("src/Rendering/Shaders/PBR.shader", shaderType::PBR);
    Shader cubemapShader("src/Rendering/Shaders/Cubemap.shader", shaderType::Computation);
    Shader skyboxShader("src/Rendering/Shaders/Skybox.shader", shaderType::Cubemap);
    Shader irradianceShader("src/Rendering/Shaders/CubemapIrradiance.shader", shaderType::Computation);
    Shader prefilterShader("src/Rendering/Shaders/PBR/Prefilter.shader", shaderType::Computation);
    Shader brdfShader("src/Rendering/Shaders/PBR/brdf.shader", shaderType::Computation);

    //std::shared_ptr<Texture> tx = std::make_shared<Texture>("res\\textures\\MetalContainer.png", TextureType::Diffuse);
    //std::shared_ptr<Texture> tx1= std::make_shared<Texture>("res\\textures\\MetalContainer_specular.png", TextureType::Specular);

    std::shared_ptr<Texture> hdrTexture = std::make_shared<Texture>("res\\textures\\HDR\\pine_attic_8k.hdr", TextureType::HDR);
    std::shared_ptr<Texture> tx2 = std::make_shared<Texture>("res\\textures\\Wall2\\Diffuse.png", TextureType::Diffuse);
    std::shared_ptr<Texture> tx3 = std::make_shared<Texture>("res\\textures\\Wall2\\Normal.png", TextureType::Normal);

    phong.bind();

    std::unique_ptr<Light> directionalLight = std::make_unique<DirectionalLight>(glm::vec3(0.4f, -1.0f, 0.05f));
    bool dirLight = true;
    std::vector<std::unique_ptr<Light>> pointLights;
    pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(-10.0f,  10.0f, 10.0f), 100.0f));
    pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3( 10.0f,  10.0f, 10.0f), 100.0f));
    pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3(-10.0f, -10.0f, 10.0f), 100.0f));
    pointLights.emplace_back(std::make_unique<PointLight>(glm::vec3( 10.0f, -10.0f, 10.0f), 100.0f));
    int numOfLights = pointLights.size();

    int cubeCount = 0;
    std::vector<std::unique_ptr<Object>> worldObjects;
    worldObjects.reserve(cubeCount + 1);

    std::unique_ptr<Object> backpack = std::make_unique<Object>("res/models/backpack/backpack.obj");
    backpack->transform.position = glm::vec3(0, 2, 0);
    worldObjects.push_back(std::move(backpack));

    glm::i32vec3 minValues(50, 50, 50);
    for (int x = 0; x < cubeCount; x++)
    {
        float r = Utils::Random::generateRandomNumber(0.0, 1.0f);
        float g = Utils::Random::generateRandomNumber(0.0, 1.0f);
        float b = Utils::Random::generateRandomNumber(0.0, 1.0f);

        float sx = Utils::Random::generateRandomNumber(1.0f, 1.0f);
        float sy = Utils::Random::generateRandomNumber(1.0f, 1.0f);
        float sz = Utils::Random::generateRandomNumber(1.0f, 1.0f);

        float xPos = Utils::Random::generateRandomNumber(-minValues.x, minValues.x);
        float yPos = Utils::Random::generateRandomNumber(-minValues.y, minValues.y);
        float zPos = Utils::Random::generateRandomNumber(-minValues.z, minValues.z);
        int mat = (int)Utils::Random::generateRandomNumber(2, Material::MATERIAL_SIZE);

        int primitive = (int)Utils::Random::generateRandomNumber(0, Primitives::Models::MODELS_COUNT);
        std::unique_ptr<Object> obj;
        switch (primitive)
        {
            case Primitives::Models::Cube: 
                obj = std::make_unique<Cube>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(sx, sy, sz), glm::vec3(r, g, b), static_cast<Material::Type>(mat)); 
                break;
            case Primitives::Models::Sphere: 
                float radius = Utils::Random::generateRandomNumber(0.5f, 2.0f);
                int size = (int)Utils::Random::generateRandomNumber(6, 30);

                obj = std::make_unique<Sphere>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f, 0.0f, 0.0f),
                    radius, glm::vec2(size, size), glm::vec3(r, g, b), static_cast<Material::Type>(mat));
                break;
        }
        worldObjects.push_back(std::move(obj));
    }

    //Cubemap
    skyboxShader.bind();
    skyboxShader.setUniform1i("enviromentMap", 0);
    skyboxShader.unbind();

    uint32_t captureFBO, captureRBO;
    GLCall(glGenFramebuffers(1, &captureFBO));
    GLCall(glGenRenderbuffers(1, &captureRBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, captureRBO));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO));

    uint32_t envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (uint32_t i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
    };

    cubemapShader.bind();
    hdrTexture->bind(0);
    cubemapShader.setUniform1i("equirectangularMap", 0);
    cubemapShader.setUniformMat4f("projection", captureProjection);
    glViewport(0, 0, 512, 512);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
    for (uint32_t i = 0; i < 6; i++)
    {
        cubemapShader.setUniformMat4f("view", captureViews[i]);
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0));

        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        renderCube();
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    cubemapShader.unbind();


    //Irradiance
    uint32_t irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (uint32_t i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    irradianceShader.bind();
    irradianceShader.setUniform1i("enviromentMap", 0);
    irradianceShader.setUniformMat4f("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glViewport(0, 0, 32, 32);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
    for (uint32_t i = 0; i < 6; i++)
    {
        irradianceShader.setUniformMat4f("view", captureViews[i]);
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0));

        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        renderCube();
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    irradianceShader.unbind();
    //Irradiance */

    //Prefilter
    unsigned int prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


    prefilterShader.bind();
    prefilterShader.setUniform1i("enviromentMap", 0);
    prefilterShader.setUniformMat4f("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.setUniform1f("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.setUniformMat4f("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    prefilterShader.unbind();

    skyboxShader.bind();
    int scrW, scrH;
    glfwGetFramebufferSize(window, &scrW, &scrH);
    glViewport(0, 0, scrW, scrH);
    skyboxShader.unbind();
    //Cubemap */

    phong.unbind();
    blinnPhong.unbind();
    blinnPhongNormal.unbind();
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
            cam.drawImGui();
            static_cast<Cube*>(worldObjects[0].get())->drawImGUI(0);
            if(dirLight)
                directionalLight->drawImGUI();
            for (size_t i = 0; i < pointLights.size(); i++) {
                pointLights[i]->drawImGUI(i);
            }
        }
        processInput(window);

        if (currentShaderType != DepthBuffer) {
            //Light drawing
            lightShader.bind();
            for (size_t i = 0; i < pointLights.size(); i++)
            {
                lightShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                lightShader.setUniformMat4f("view", cam.getViewMatrix());
                lightShader.setUniformMat4f("model", pointLights[i]->getModelMatrix());
                lightShader.setUniformVec3f("lightColor", pointLights[i]->getColor());
                pointLights[i]->draw(lightShader, renderer);
            }
            lightShader.unbind();
            //Light drawing */
        }

        switch (currentShaderType)
        {
            case Phong:
                phong.bind();

                phong.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                phong.setUniformMat4f("view", cam.getViewMatrix());
                phong.setUniformVec3f("viewPos", cam.getPosition());

                //Directional light
                if (dirLight) {
                    phong.setUniformVec3f("dirLight.direction", static_cast<DirectionalLight*>(directionalLight.get())->getDirection());
                    phong.setUniformVec3f("dirLight.ambient", directionalLight->getAmbient());
                    phong.setUniformVec3f("dirLight.diffuse", directionalLight->getDiffuse());
                    phong.setUniformVec3f("dirLight.specular", directionalLight->getSpecular());
                }

                //Point light
                for (size_t i = 0; i < pointLights.size(); i++) {
                    phong.setUniformVec3f("pointLight[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
                    phong.setUniformVec3f("pointLight[" + std::to_string(i) + "].ambient", pointLights[i]->getAmbient());
                    phong.setUniformVec3f("pointLight[" + std::to_string(i) + "].diffuse", pointLights[i]->getDiffuse());
                    phong.setUniformVec3f("pointLight[" + std::to_string(i) + "].specular", pointLights[i]->getSpecular());
                    phong.setUniform1f("pointLight[" + std::to_string(i) + "].constant", dynamic_cast<PointLight*>(pointLights[i].get())->getConstant());
                    phong.setUniform1f("pointLight[" + std::to_string(i) + "].linear", dynamic_cast<PointLight*>(pointLights[i].get())->getLinear());
                    phong.setUniform1f("pointLight[" + std::to_string(i) + "].quadratic", dynamic_cast<PointLight*>(pointLights[i].get())->getQuadratic());
                }
                break;

            case DepthBuffer:
                depthShader.bind();
                depthShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                depthShader.setUniformMat4f("view", cam.getViewMatrix());
                break;

            case BlinnPhong:
                blinnPhong.bind();

                blinnPhong.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                blinnPhong.setUniformMat4f("view", cam.getViewMatrix());
                blinnPhong.setUniformVec3f("viewPos", cam.getPosition());

                //Directional light
                if (dirLight) {
                    blinnPhong.setUniformVec3f("dirLight.direction", static_cast<DirectionalLight*>(directionalLight.get())->getDirection());
                    blinnPhong.setUniformVec3f("dirLight.ambient", directionalLight->getAmbient());
                    blinnPhong.setUniformVec3f("dirLight.diffuse", directionalLight->getDiffuse());
                    blinnPhong.setUniformVec3f("dirLight.specular", directionalLight->getSpecular());
                }

                //Point light
                for (size_t i = 0; i < pointLights.size(); i++) {
                    blinnPhong.setUniformVec3f("pointLight[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
                    blinnPhong.setUniformVec3f("pointLight[" + std::to_string(i) + "].ambient", pointLights[i]->getAmbient());
                    blinnPhong.setUniformVec3f("pointLight[" + std::to_string(i) + "].diffuse", pointLights[i]->getDiffuse());
                    blinnPhong.setUniformVec3f("pointLight[" + std::to_string(i) + "].specular", pointLights[i]->getSpecular());
                    blinnPhong.setUniform1f("pointLight[" + std::to_string(i) + "].constant", dynamic_cast<PointLight*>(pointLights[i].get())->getConstant());
                    blinnPhong.setUniform1f("pointLight[" + std::to_string(i) + "].linear", dynamic_cast<PointLight*>(pointLights[i].get())->getLinear());
                    blinnPhong.setUniform1f("pointLight[" + std::to_string(i) + "].quadratic", dynamic_cast<PointLight*>(pointLights[i].get())->getQuadratic());
                }
                break;

            case BlinnPhongNormalMapping:
                blinnPhongNormal.bind();

                blinnPhongNormal.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                blinnPhongNormal.setUniformMat4f("view", cam.getViewMatrix());
                blinnPhongNormal.setUniformVec3f("viewPos", cam.getPosition());

                //Directional light
                if (dirLight) {
                    blinnPhongNormal.setUniformVec3f("dirLight.direction", static_cast<DirectionalLight*>(directionalLight.get())->getDirection());
                    blinnPhongNormal.setUniformVec3f("dirLight.ambient", directionalLight->getAmbient());
                    blinnPhongNormal.setUniformVec3f("dirLight.diffuse", directionalLight->getDiffuse());
                    blinnPhongNormal.setUniformVec3f("dirLight.specular", directionalLight->getSpecular());
                }

                //Point light
                for (size_t i = 0; i < pointLights.size(); i++) {
                    blinnPhongNormal.setUniformVec3f("lightPos[" + std::to_string(i) + "]", pointLights[i]->getPosition());
                    blinnPhongNormal.setUniformVec3f("pointLight[" + std::to_string(i) + "].ambient", pointLights[i]->getAmbient());
                    blinnPhongNormal.setUniformVec3f("pointLight[" + std::to_string(i) + "].diffuse", pointLights[i]->getDiffuse());
                    blinnPhongNormal.setUniformVec3f("pointLight[" + std::to_string(i) + "].specular", pointLights[i]->getSpecular());
                    blinnPhongNormal.setUniform1f("pointLight[" + std::to_string(i) + "].constant", dynamic_cast<PointLight*>(pointLights[i].get())->getConstant());
                    blinnPhongNormal.setUniform1f("pointLight[" + std::to_string(i) + "].linear", dynamic_cast<PointLight*>(pointLights[i].get())->getLinear());
                    blinnPhongNormal.setUniform1f("pointLight[" + std::to_string(i) + "].quadratic", dynamic_cast<PointLight*>(pointLights[i].get())->getQuadratic());
                }
                break;

            case PBR:
                pbrShader.bind();

                pbrShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
                pbrShader.setUniformMat4f("view", cam.getViewMatrix());
                pbrShader.setUniformVec3f("viewPos", cam.getPosition());

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
                //glActiveTexture(GL_TEXTURE1);
                //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
                //glActiveTexture(GL_TEXTURE2);
                //glBindTexture(GL_TEXTURE_CUBE_MAP, brdfLUTTexture);
                pbrShader.setUniform1i("irradianceMap", 0);
                //pbrShader.setUniform1i("prefilterMap", 1);
                //pbrShader.setUniform1i("brdfLUT", 2);

                //Directional light
                if (dirLight) {
                    pbrShader.setUniformVec3f("dirLight.direction", static_cast<DirectionalLight*>(directionalLight.get())->getDirection());
                    pbrShader.setUniformVec3f("dirLight.color", directionalLight->getAmbient());
                }

                //Point light
                for (size_t i = 0; i < pointLights.size(); i++) {
                    pbrShader.setUniformVec3f("pointLight[" + std::to_string(i) + "].position", pointLights[i]->getPosition());
                    pbrShader.setUniformVec3f("pointLight[" + std::to_string(i) + "].color", pointLights[i]->getAmbient() * 1500.0f);
                }

                pbrShader.unbind();
                break;
        }

        for (auto& obj : worldObjects)
        {
            switch (currentShaderType)
            {
            case Phong:
                obj->draw(phong, renderer);
                
                break;

            case DepthBuffer:
                obj->draw(depthShader, renderer);
                break;

            case BlinnPhong:
                obj->draw(blinnPhong, renderer);
                break;

            case BlinnPhongNormalMapping:
                obj->draw(blinnPhongNormal, renderer);
                break;

            case PBR:
                obj->draw(pbrShader, renderer);
                break;
            }
        }

        switch (currentShaderType)
        {
        case Phong: phong.unbind(); break;
        case DepthBuffer: depthShader.unbind(); break;
        case BlinnPhong: blinnPhong.unbind(); break;
        case BlinnPhongNormalMapping: blinnPhongNormal.unbind(); break;
        case PBR: pbrShader.unbind(); 
            //Cubemap rendering
            glDepthFunc(GL_LEQUAL);
            skyboxShader.bind();
            skyboxShader.setUniformMat4f("projection", cam.getProjectionMatrix(windowSize));
            skyboxShader.setUniformMat4f("view", cam.getViewMatrix());
            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
            renderCube();
            skyboxShader.unbind();
            glDepthFunc(GL_LESS); //Powrot do defaultowej wartosci
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            
            break;
        }     
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