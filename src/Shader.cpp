#include "Shader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

#include "NameUtils.h"

GLenum Shader::ShaderTypeFromString(const std::string &type)
{
    if (type == "vertex") return GL_VERTEX_SHADER;

    if (type == "fragment") return GL_FRAGMENT_SHADER;

    return 0;
}

Shader::Shader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
{
    m_Name = name;
    m_OpenGLSourceCode[GL_VERTEX_SHADER] = vertexSrc;
    m_OpenGLSourceCode[GL_FRAGMENT_SHADER] = fragmentSrc;
}

Shader::Shader(const std::string &filePath)
{
    m_FilePath = filePath;
    m_Name = ParseNameFromPath(filePath);

    std::string source = ReadFile(filePath);
    Preprocess(source);
    CreateProgram();
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

std::string Shader::ReadFile(const std::string &filePath)
{
    std::ifstream sourceFile;
    sourceFile.open(filePath);

    std::string result;
    if (sourceFile)
    {
        sourceFile.seekg(0, std::ios::end);
        result.resize(sourceFile.tellg());

        sourceFile.seekg(0, std::ios::beg);
        sourceFile.read(&result[0], result.size());

        sourceFile.close();
    }

    return result;
}

void Shader::Preprocess(const std::string &source)
{
    std::string typeToken = "#type";

    std::stringstream ss(source);

    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        std::string shaderType;
        ss.seekg(pos);
        ss >> shaderType >> shaderType;

        size_t sourceBegPos = ss.tellg();
        std::string curSource;

        size_t sourceEndPos = source.find(typeToken, sourceBegPos);

        if (sourceEndPos == std::string::npos)
        {
            curSource.resize(source.size() - sourceBegPos);
            ss.read(&curSource[0], source.size() - sourceBegPos);
        }
        else
        {
            curSource.resize(sourceEndPos - sourceBegPos);
            ss.read(&curSource[0], sourceEndPos - sourceBegPos);
        }

        m_OpenGLSourceCode[ShaderTypeFromString(shaderType)] = curSource;

        pos = sourceEndPos;
    }

}

void Shader::CreateProgram()
{
    unsigned int ProgramID = glCreateProgram();
    std::vector<unsigned int> shaderIDs;
    shaderIDs.reserve(m_OpenGLSourceCode.size());

    GLint Result = GL_FALSE;
    int InfoLogLength;

    for (auto &kv : m_OpenGLSourceCode)
    {
        unsigned int shaderID = glCreateShader(kv.first);

        char const * sourcePointer = kv.second.c_str();
        glShaderSource(shaderID, 1, &sourcePointer , nullptr);
        glCompileShader(shaderID);

        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> shaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shaderID, InfoLogLength, nullptr, &shaderErrorMessage[0]);
            printf("%s\n", &shaderErrorMessage[0]);
        }

        shaderIDs.emplace_back(shaderID);
    }

    for (auto shaderID : shaderIDs)
    {
        glAttachShader(ProgramID, shaderID);
    }

    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    for (auto shaderID : shaderIDs)
    {
        glDetachShader(ProgramID, shaderID);
        glDeleteShader(shaderID);
    }

    m_RendererID = ProgramID;

}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetInt(const std::string &name, const int &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, 1, &value);
}

void Shader::SetIntArray(const std::string &name, int *values, unsigned int count)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
}

void Shader::SetFloat(const std::string &name, const float &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1fv(location, 1, &value);
}

void Shader::SetFloat2(const std::string &name, const glm::vec2 &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::SetFloat3(const std::string &name, const glm::vec3 &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetFloat4(const std::string &name, const glm::vec4 &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
