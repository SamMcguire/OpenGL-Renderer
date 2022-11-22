#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filepath)
	: m_RendererID(0), m_FilePath(filepath), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	//By default opengl starts at bottom left corner, but normally loading images is from 
	//top right, so image would be upside down!
	stbi_set_flip_vertically_on_load(1); 
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);


	glGenTextures(1, &m_RendererID); GCE;
	glBindTexture(GL_TEXTURE_2D, m_RendererID); GCE;

	//Setting texture parameter(should look at later, basically how to resize and display texutre)
	//MIN_FILTER is minimization filter, and we are setting it to linear minimize
	//These are necassary parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GCE;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GCE;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); GCE;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); GCE;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer); GCE;
	glBindTexture(GL_TEXTURE_2D, 0); GCE;

	//Clearing memory of texture(might want to save tho?)
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID); GCE;
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot); GCE;
	glBindTexture(GL_TEXTURE_2D, m_RendererID); GCE;
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0); GCE;
}
