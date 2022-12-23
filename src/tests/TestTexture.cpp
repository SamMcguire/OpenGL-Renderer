#include "TestTexture.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"
#include "Texture.h"

#include <iostream>

namespace test
{
	TestTexture::TestTexture()
		: m_vao(), m_Ortho(sam::mat4f::Ortho(0, 1500, 0, 500, -10.0f, 10.0f)),
		m_Move(), m_Translation(0.0f), m_mvp(), m_Shader("res/shaders/Texture.shader"), 
		m_ibo(m_Indices, 6), m_vbo(m_Positions, 4 * 4 * sizeof(float)), m_Texture("res/textures/test.png")
	{
		//Enabling blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GCE;
		glEnable(GL_BLEND); GCE;

		//Setting vertex array layout(attributes)
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_vao.AddBuffer(m_vbo, layout);
		
		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
		m_Shader.SetUniform1i("u_Texture", 0);//Value is texture slot
	}
	TestTexture::~TestTexture()
	{
	}
	void TestTexture::OnUpdate(float deltaTime)
	{
		m_Move << 1, 0, 0, m_Translation,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
			
		m_mvp = m_Ortho * m_Move;
	}
	void TestTexture::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GCE;
		glClear(GL_COLOR_BUFFER_BIT); GCE;

		m_Texture.Bind();

		Renderer renderer;
		//Issuing a draw call
        m_Shader.Bind();
        m_Shader.SetUniform4f("u_Color", 0.9f, 0.9f, 0.8f, 1.0f); 
        m_Shader.SetUniformMat4f("u_MVP", m_mvp);
        renderer.Draw(m_vao, m_ibo, m_Shader);
	}
	void TestTexture::OnImGuiRender()
	{
		ImGui::Begin("Movement!");
		ImGui::SliderFloat("XTranslation", &m_Translation, -800.0f, 800.0f);            
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}