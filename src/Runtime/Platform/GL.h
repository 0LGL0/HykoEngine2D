#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace HKCR {
    inline void loadGLFunctions() { // get OpenGL functions
        [[maybe_unused]] static const std::nullptr_t glF = [] {
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            return nullptr;
        }();
    }

    void setGLLog(const GLFWerrorfun& func);

    template<typename T>
    inline GLuint getOpenGLType() {
        if (std::is_same<T, GLbyte>::value) return GL_BYTE;
        else if (std::is_same<T, GLubyte>::value) return GL_UNSIGNED_BYTE;
        else if (std::is_same<T, GLshort>::value) return GL_SHORT;
        else if (std::is_same<T, GLushort>::value) return GL_UNSIGNED_SHORT;
        else if (std::is_same<T, GLint>::value) return GL_INT;
        else if (std::is_same<T, GLuint>::value) return GL_UNSIGNED_INT;
        else if (std::is_same<T, GLfloat>::value) return GL_FLOAT;

        return 0;
    }
}