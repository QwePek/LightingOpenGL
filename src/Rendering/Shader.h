#pragma once

#include <iostream>
#include <unordered_map>
#include "glm\glm.hpp"

enum shaderType { LightSh, Phong, DepthBuffer, BlinnPhong, PBR };

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const char* filePath, shaderType type);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1f(const std::string& name, float f0);
    void setUniform1i(const std::string& name, int v0);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void setUniformVec3f(const std::string& name, const glm::vec3& vector);

    uint32_t getUniformLocation(const std::string& name);
    shaderType getType() { return type; }

private:
    ShaderProgramSource parseShader(const char* filePath);
    uint32_t compileShader(uint32_t type, const std::string& src);

    shaderType type;
    uint32_t programID;
    std::unordered_map<std::string, uint32_t> uniformLocationCache;
};

