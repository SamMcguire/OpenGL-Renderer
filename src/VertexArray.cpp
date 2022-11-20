#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID); GCE
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID); GCE
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID); GCE
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0); GCE
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset); GCE
		glEnableVertexAttribArray(i); GCE
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
