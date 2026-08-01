#include "TextureManager.h"
#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/stb_image.h"

unsigned int TextureManager::loadTexture(const std::string& filepath) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Voxel art looks best with nearest-neighbor filtering (no blurry textures)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Repeat texture if coordinates > 1.0 (though our atlas uses 0.0-1.0)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // OpenGL expects the 0.0 coordinate on the Y-axis to be on the bottom side of the image,
    // but images usually have 0.0 at the top. stb_image can flip it for us.
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Optional: generates smaller versions of the texture
    } else {
        std::cerr << "[TextureManager] Failed to load texture: " << filepath << std::endl;
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
    stbi_image_free(data);

    return textureID;
}

void TextureManager::bindTexture(unsigned int textureID) {
    glBindTexture(GL_TEXTURE_2D, textureID);
}
