#pragma once

#include <string>

class TextureManager {
public:
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    // Loads a texture and returns its OpenGL texture ID.
    // Returns 0 on failure.
    unsigned int loadTexture(const std::string& filepath);

    // Binds a specific texture to GL_TEXTURE_2D
    void bindTexture(unsigned int textureID);

private:
    TextureManager() = default;
};
