#pragma once

#include "Shader.hpp"
#include <cstdint>

// define global variables
namespace Engine {

extern int32_t windowWidth;
extern int32_t windowHeight;
extern float deltaTime;
extern Shader* worldShader;
extern Shader* lineShader;

}; // namespace Engine
