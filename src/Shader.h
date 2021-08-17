#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
    Shader(const std::string &filePath);
    ~Shader();

    std::string GetName() const { return m_Name; }

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string &name, const int &value);
    void SetIntArray(const std::string &name, int *values, unsigned int count);

    void SetFloat(const std::string &name, const float &value) ;
    void SetFloat2(const std::string &name, const glm::vec2 &value);
    void SetFloat3(const std::string &name, const glm::vec3 &value);
    void SetFloat4(const std::string &name, const glm::vec4 &value);

    void SetMat3(const std::string &name, const glm::mat3 &value);
    void SetMat4(const std::string &name, const glm::mat4 &value);

    static GLenum ShaderTypeFromString(const std::string &type);

private:

    std::string ReadFile(const std::string &filePath);
    void Preprocess(const std::string &source);
    void CreateProgram();


private:
    std::string m_FilePath;
    std::string m_Name;

    unsigned int m_RendererID;

    std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
};
