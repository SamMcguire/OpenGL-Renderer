#pragma once

#pragma once

#include "test.h"
#include "VertexArray.h"
#include "samath/samath.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

namespace test
{
	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_Positions[16] = {
						600, 100, 0.0f, 0.0f,
						900, 100, 1.0f, 0.0f,
						900, 400, 1.0f, 1.0f,
						600, 400, 0.0f, 1.0f };// pos, and tex pos
		unsigned int m_Indices[6] ={
						0,1,2,
						2,3,0 };

		Texture m_Texture;
		VertexArray m_vao;
		IndexBuffer m_ibo;
		VertexBuffer m_vbo;
		Shader m_Shader;
		
		sam::mat4f m_Ortho;
		sam::mat4f m_Move;
		float m_Translation;
		sam::mat4f m_mvp;
	};
}
