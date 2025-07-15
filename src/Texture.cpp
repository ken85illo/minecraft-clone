#include <Texture.hpp>

Texture::Texture(int target, int size)
: m_ID(new uint[size]), m_target(target), m_size(size) {
    glGenTextures(size, m_ID);

    stbi_set_flip_vertically_on_load(true);
}

Texture::~Texture() {
    glDeleteTextures(m_size, m_ID);
    delete[] m_ID;
}

void Texture::loadImage(const char* filepath) {
    int width, height, nrChannels;
    u_char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if(!data) {
        std::println("Failed to load image texture!");
    } else {
        glTexImage2D(m_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(m_target);
    }

    stbi_image_free(data);
}

void Texture::bind(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(m_target, m_ID[index]);
}

void Texture::unbind(int index) {
    glBindTexture(m_target, 0);
}

void Texture::setParameter(int name, int value) {
    glTexParameteri(m_target, name, value);
}
