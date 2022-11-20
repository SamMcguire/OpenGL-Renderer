#pragma once
#include <GL/glew.h>

#include "IndexBuffer.h"  
#include "VertexArray.h"
#include "Shader.h"


void GLCheckError(const char* function, const char* file, int line);

#ifdef DEBUG
#define CHECK_GL_ERRORS
#endif

#ifdef CHECK_GL_ERRORS
    #define GCE GLCheckError(__FUNCTION__, __FILE__, __LINE__);
#else
    #define GCE
#endif // CHECK_GL_ERRORS

#define ASSERT(x) if (!(x)) __debugbreak(); 

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

