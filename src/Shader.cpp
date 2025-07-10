#include "Shader.hpp"

#include <fstream>
#include <print>

void Shader::loadShader(const char* filepath, uint shader) {
    std::ifstream file(filepath);
    if(!file.is_open()) {
        std::println("Failed to initialize file!");
        return;
    }

    std::string result = std::string(
    std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>())
                         .c_str();

    const char* cstr = result.c_str();
    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::println("Failed to compile shader (see log output below)\n{}\n{}",
        filepath, infoLog);
        return;
    }

    glAttachShader(m_program, shader);
}

uint Shader::getProgram() const {
    return m_program;
}

void Shader::linkProgram() const {
    glLinkProgram(m_program);

    int success;
    char infoLog[512];
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
        std::println("Failed to link program (see log output below)\n{}", infoLog);
        return;
    }
}
