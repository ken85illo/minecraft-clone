#pragma once

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <string>

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    void use();

    // Uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 &matrix) const;
    void setVec3(const std::string &name, glm::vec3 vec) const;

private:
    uint32_t m_ID;
};
