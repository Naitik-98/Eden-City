#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram();

    // Prevent copying because it manages an OpenGL resource (ID)
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    // Move semantics are fine if needed, but typically we construct and hold
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    // Load, compile and link vertex and fragment shaders
    bool load(const std::string& vertexPath, const std::string& fragmentPath);

    // Use this shader program
    void bind() const;
    void unbind() const;

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    GLuint getID() const { return m_ID; }

private:
    GLuint m_ID = 0;

    void checkCompileErrors(GLuint shader, std::string type);
};
