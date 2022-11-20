#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID); GCE //1 is how many buffers. &(passes in pointer)
    //Selecting buffer. GL_ARRAY_BUFFER is purpose of buffer.
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); GCE
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); GCE
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID); GCE
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); GCE
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0); GCE
}
