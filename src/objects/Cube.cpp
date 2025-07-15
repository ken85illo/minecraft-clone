#include "Cube.hpp"

Cube::Cube(Shader* shader, Texture* texture, uint32_t textureIndex, Block type = NONE)
: m_shader(shader),
  m_texture(texture),
  m_textureIndex(textureIndex),
  m_properties(Properties{
  .x = 0.0f,
  .y = 0.0f,
  .z = 0.0f,
  .width = 1.0f,
  .height = 1.0f,
  .depth = 1.0f,
  .rotX = 0.0f,
  .rotY = 0.0f,
  .rotZ = 0.0f,
  }) {

    float vertices[] = {
        // vertex           //tex coords
        // front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right

        // back face
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-right

        // top face
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right

        // bottom face
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right

        // right face
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-left
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // top-left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right

        // left face
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-left
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // top-right
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
    };

    uint32_t indices[] = {
        // front face
        0, 1, 2, //
        0, 2, 3, //

        // back face
        4, 5, 6, //
        4, 6, 7, //

        // top face
        8, 9, 10,  //
        8, 10, 11, //

        // bottom face
        12, 13, 14, //
        12, 14, 15, //

        // right face
        16, 17, 18, //
        16, 18, 19, //

        // left face
        20, 21, 22, //
        20, 22, 23, //
    };

    if(type != NONE) {
        m_atlas = new Atlas(16, 256, vertices);

        switch(type) {
        case NONE: break;
        case DIRT_BLOCK:
            m_atlas->map(DIRT_TEX, DIRT_TEX, DIRT_TEX, DIRT_TEX, DIRT_TEX, DIRT_TEX);
            break;
        case GRASS_BLOCK:
            m_atlas->map(GRASS_SIDE_TEX, GRASS_SIDE_TEX, GRASS_TOP_TEX,
            DIRT_TEX, GRASS_SIDE_TEX, GRASS_SIDE_TEX);
            break;
        }
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Cube::render() {
    m_texture->bind(m_textureIndex);

    glm::mat4 model = glm::mat4(1.0f);
    model =
    glm::translate(model, glm::vec3(m_properties.x, m_properties.y, m_properties.z));
    model =
    glm::rotate(model, glm::radians(m_properties.rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    model =
    glm::rotate(model, glm::radians(m_properties.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    model =
    glm::rotate(model, glm::radians(m_properties.rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model,
    glm::vec3(m_properties.width, m_properties.height, m_properties.depth));

    m_shader->setMat4("model", model);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Cube::setPos(float x, float y, float z) {
    m_properties.x = x;
    m_properties.y = y;
    m_properties.z = z;
}

void Cube::setPosX(float x) {
    m_properties.x = x;
}

void Cube::setPosY(float y) {
    m_properties.y = y;
}

void Cube::setPosZ(float z) {
    m_properties.z = z;
}

void Cube::move(float x, float y, float z) {
    m_properties.x += x;
    m_properties.y += y;
    m_properties.z += z;
}

void Cube::setScale(float x, float y, float z) {
    m_properties.width = x;
    m_properties.height = y;
    m_properties.depth = z;
}

void Cube::setScaleX(float x) {
    m_properties.width = x;
}

void Cube::setScaleY(float y) {
    m_properties.height = y;
}

void Cube::setScaleZ(float z) {
    m_properties.depth = z;
}

void Cube::scale(float x, float y, float z) {
    m_properties.width *= x;
    m_properties.height *= y;
    m_properties.depth *= z;
}

void Cube::setRotation(float x, float y, float z) {
    m_properties.rotX = x;
    m_properties.rotY = y;
    m_properties.rotZ = z;
}

void Cube::setRotationX(float x) {
    m_properties.rotX = x;
}

void Cube::setRotationY(float y) {
    m_properties.rotY = y;
}

void Cube::setRotationZ(float z) {
    m_properties.rotZ = z;
}

void Cube::rotate(float x, float y, float z) {
    m_properties.rotX *= x;
    m_properties.rotY *= y;
    m_properties.rotZ *= z;
}

Cube::Properties Cube::getProperty() const {
    return m_properties;
}
