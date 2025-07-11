#include <Texture.hpp>

Texture::Texture(int target, int size)
: m_ID(new uint[size]), m_target(target), m_size(size) {
    for(int i = 0; i < size; i++)
        glGenTextures(1, &m_ID[i]);

    stbi_set_flip_vertically_on_load(true);
}

Texture::~Texture() {
    for(int i = 0; i < m_size; i++)
        glDeleteTextures(1, &m_ID[i]);
    delete[] m_ID;
}

void Texture::loadImage(int index, const char* filepath) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(m_target, m_ID[index]);

    int width, height, nrChannels;
    uchar* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if(!data) {
        std::println("Failed to load image texture!");
    } else {
        glTexImage2D(m_target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(m_target);
    }

    stbi_image_free(data);
    glBindTexture(m_target, 0);
}

void Texture::setParameter(int index, int name, int value) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(m_target, m_ID[index]);
    glTexParameteri(m_target, name, value);
    glBindTexture(m_target, 0);
}

void Texture::use(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(m_target, m_ID[index]);
}
