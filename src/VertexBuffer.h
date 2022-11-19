#pragma once
class VertexBuffer
{
private:
	unsigned int m_RendererID;//The ID of this buffer obj
public:
	//data is array containing all data
	//size is number of bytes the buffer takes
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};

