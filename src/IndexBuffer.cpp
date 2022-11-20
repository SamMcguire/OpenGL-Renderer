#include "IndexBuffer.h"

#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererID); GCE//1 is how many buffers. &(passes in pointer)
    //Selecting buffer. GL_ARRAY_BUFFER is purpose of buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); GCE
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW); GCE
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID); GCE
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); GCE
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GCE
}
