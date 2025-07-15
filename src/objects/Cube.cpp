#include "Cube.hpp"

Cube::Cube(int size, Shader* shader, const char* texturePath, Block type)
: m_size(size),
  m_shader(shader),
  m_VAO(new uint32_t[size]),
  m_VBO(new uint32_t[size]),
  m_EBO(new uint32_t[size]),
  m_properties(new Properties[size]) {

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

    glGenVertexArrays(size, m_VAO);
    glGenBuffers(size, m_VBO);
    glGenBuffers(size, m_EBO);

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


    for(int i = 0; i < size; i++) {
        m_properties[i] = Properties{
            .x = 0.0f,
            .y = 0.0f,
            .z = 0.0f,
            .width = 1.0f,
            .height = 1.0f,
            .depth = 1.0f,
            .rotX = 0.0f,
            .rotY = 0.0f,
            .rotZ = 0.0f,
        };

        glBindVertexArray(m_VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    m_texture = new Texture(GL_TEXTURE_2D, 1);
    m_texture->bind(0);
    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture->loadImage(texturePath);
}

Cube::~Cube() {
    glDeleteVertexArrays(m_size, m_VAO);
    glDeleteBuffers(m_size, m_VBO);
    glDeleteBuffers(m_size, m_EBO);
    delete[] m_VAO;
    delete[] m_VBO;
    delete[] m_EBO;
}

void Cube::render() {
    m_texture->bind(0);
    for(int i = 0; i < m_size; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,
        glm::vec3(m_properties[i].x, m_properties[i].y, m_properties[i].z));

        model = glm::rotate(
        model, glm::radians(m_properties[i].rotX), glm::vec3(1.0f, 0.0f, 0.0f));

        model = glm::rotate(
        model, glm::radians(m_properties[i].rotY), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::rotate(
        model, glm::radians(m_properties[i].rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(model,
        glm::vec3(m_properties[i].width, m_properties[i].height, m_properties[i].depth));

        m_shader->setMat4("model", model);
        glBindVertexArray(m_VAO[i]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }
}

void Cube::setPos(uint32_t index, float x, float y, float z) {
    m_properties[index].x = x;
    m_properties[index].y = y;
    m_properties[index].z = z;
}

void Cube::setPosX(uint32_t index, float x) {
    m_properties[index].x = x;
}

void Cube::setPosY(uint32_t index, float y) {
    m_properties[index].y = y;
}

void Cube::setPosZ(uint32_t index, float z) {
    m_properties[index].z = z;
}

void Cube::move(uint32_t index, float x, float y, float z) {
    m_properties[index].x += x;
    m_properties[index].y += y;
    m_properties[index].z += z;
}

void Cube::setScale(uint32_t index, float x, float y, float z) {
    m_properties[index].width = x;
    m_properties[index].height = y;
    m_properties[index].depth = z;
}

void Cube::setScaleX(uint32_t index, float x) {
    m_properties[index].width = x;
}

void Cube::setScaleY(uint32_t index, float y) {
    m_properties[index].height = y;
}

void Cube::setScaleZ(uint32_t index, float z) {
    m_properties[index].depth = z;
}

void Cube::scale(uint32_t index, float x, float y, float z) {
    m_properties[index].width *= x;
    m_properties[index].height *= y;
    m_properties[index].depth *= z;
}

void Cube::setRotation(uint32_t index, float x, float y, float z) {
    m_properties[index].rotX = x;
    m_properties[index].rotY = y;
    m_properties[index].rotZ = z;
}

void Cube::setRotationX(uint32_t index, float x) {
    m_properties[index].rotX = x;
}

void Cube::setRotationY(uint32_t index, float y) {
    m_properties[index].rotY = y;
}

void Cube::setRotationZ(uint32_t index, float z) {
    m_properties[index].rotZ = z;
}

void Cube::rotate(uint32_t index, float x, float y, float z) {
    m_properties[index].rotX *= x;
    m_properties[index].rotY *= y;
    m_properties[index].rotZ *= z;
}

uint32_t Cube::size() const {
    return m_size;
}

Cube::Properties& Cube::get(uint32_t index) const {
    return m_properties[index];
}
