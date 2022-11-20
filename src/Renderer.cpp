#include "Renderer.h"

#include<iostream>

//Error Handling
//Enable below to exit if error occurs
#define EXIT_ON_GL_ERROR
void GLCheckError(const char* function, const char* file, int line)
{
    //std::cout << "Got into Error func " << std::endl;
    GLenum error = 0;
    bool is_error = false;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        is_error = true;
        switch (error)
        {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:
            std::cout << "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:
            std::cout << "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        case GL_CONTEXT_LOST:
            std::cout << "GL_CONTEXT_LOST"; break;
        case GL_TABLE_TOO_LARGE:
            std::cout << "GL_TABLE_TOO_LARGE"; break;
        default:
            std::cout << "Unknown error code " << error;
        }
        std::cout << " encountered at function '" << function
            << "' (" << file << ":" << line << ")" << std::endl;
    }
    #ifdef EXIT_ON_GL_ERROR
    if (is_error) {
        __debugbreak();
    }
    #endif
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT); GCE
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr); GCE
    //GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
