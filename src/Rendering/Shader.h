#pragma once

#include <iostream>
#include <unordered_map>
#include "glm\glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const char* filePath);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform1i(const std::string& name, int v0);
    void setUnfiformMat4f(const std::string& name, const glm::mat4& matrix);

    uint32_t getUniformLocation(const std::string& name);


private:
    ShaderProgramSource parseShader(const char* filePath);
    uint32_t compileShader(uint32_t type, const std::string& src);

    uint32_t programID;
    std::unordered_map<std::string, uint32_t> uniformLocationCache;
};

