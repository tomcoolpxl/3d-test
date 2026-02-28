#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

static std::string readFile(const std::string& path)
{
    std::ifstream f(path);
    if (!f.is_open()) {
        std::fprintf(stderr, "Shader file not found: %s\n", path.c_str());
        std::exit(1);
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compileShader(GLenum type, const std::string& src)
{
    GLuint shader = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(shader, 1, &c, nullptr);
    glCompileShader(shader);

    GLint ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Shader compile error:\n%s\n", log);
        std::exit(1);
    }
    return shader;
}

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{
    GLuint vert = compileShader(GL_VERTEX_SHADER,   readFile(vertPath));
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, readFile(fragPath));

    m_program = glCreateProgram();
    glAttachShader(m_program, vert);
    glAttachShader(m_program, frag);
    glLinkProgram(m_program);

    GLint ok;
    glGetProgramiv(m_program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(m_program, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Shader link error:\n%s\n", log);
        std::exit(1);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::use() const
{
    glUseProgram(m_program);
}

void Shader::setMat4(const char* name, const glm::mat4& m) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setVec3(const char* name, const glm::vec3& v) const
{
    glUniform3fv(glGetUniformLocation(m_program, name), 1, glm::value_ptr(v));
}

void Shader::setFloat(const char* name, float f) const
{
    glUniform1f(glGetUniformLocation(m_program, name), f);
}
