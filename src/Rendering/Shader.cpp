#include "pch.h"
#include "Shader.h"

#include "GL/glew.h"

Shader::Shader(const char* filePath, shaderType shType) : type(shType)
{
    ShaderProgramSource src = parseShader(filePath);

    programID = glCreateProgram();

    uint32_t vertexShader = compileShader(GL_VERTEX_SHADER, src.VertexSource);
    uint32_t fragmentShader = compileShader(GL_FRAGMENT_SHADER, src.FragmentSource);

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    glLinkProgram(programID);

    glValidateProgram(programID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(programID);
}

ShaderProgramSource Shader::parseShader(const char* filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

uint32_t Shader::compileShader(uint32_t type, const std::string& src)
{
    uint32_t shader = glCreateShader(type);
    const char* srcChar = src.c_str();

    glShaderSource(shader, 1, &srcChar, 0);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        int len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* retMsg = (char*)alloca(len * sizeof(char));
        glGetShaderInfoLog(shader, len, &len, retMsg);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << retMsg << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}


void Shader::bind() const
{
    glUseProgram(programID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

uint32_t Shader::getUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];

    int loc = glGetUniformLocation(programID, name.c_str());
    if (loc == -1)
        std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;

    uniformLocationCache[name] = loc;
    return loc;
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform1i(const std::string& name, int v0)
{
    glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1f(const std::string& name, float f0)
{
    glUniform1f(getUniformLocation(name), f0);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformMat3f(const std::string& name, const glm::mat3& matrix)
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformVec3f(const std::string& name, const glm::vec3& vector)
{
    glUniform3f(getUniformLocation(name), vector[0], vector[1], vector[2]);
}