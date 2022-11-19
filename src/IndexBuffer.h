#pragma once
class IndexBuffer
{
private:
	unsigned int m_RendererID;//The ID of this buffer obj
	unsigned int m_Count;
public:
	//data is array containing all data
	//size is number of bytes the buffer takes
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

