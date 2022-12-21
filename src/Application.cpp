#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <samath/samath.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"  
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1500, 500, "Coolest Tab", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); 

	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;


	Renderer renderer;
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		io.Fonts->AddFontDefault();

		test::TestClearColor test;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			test.OnUpdate(0.0f);
			test.OnRender();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		
			test.OnImGuiRender();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

//int main()
//{
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    //window = glfwCreateWindow(640, 480, "Coolest Tab", NULL, NULL);
//    window = glfwCreateWindow(1500, 500, "Coolest Tab", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;  
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    glfwSwapInterval(1); //Matches frame rate to refresh rate 
//
//    if (glewInit() != GLEW_OK)
//        return -1;
//
//    std::cout << glGetString(GL_VERSION) << std::endl;
//
//    {
//        //pos_x, pos_y, tex_x, tex_y
//        /*float positions[16] = {
//            -0.5, -0.5, 0.0f, 0.0f,
//            0.5, -0.5, 1.0f, 0.0f,
//            0.5, 0.5, 1.0f, 1.0f,
//            -0.5, 0.5, 0.0f, 1.0f
//        };*/
//        float positions[16] = {
//            600, 100, 0.0f, 0.0f,
//            900, 100, 1.0f, 0.0f,
//            900, 400, 1.0f, 1.0f,
//            600, 400, 0.0f, 1.0f
//        };
//
//        unsigned int indices[] = {
//            0,1,2,
//            2,3,0
//        };
//
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GCE;
//        glEnable(GL_BLEND); GCE;
//
//        //Defining vertex buffers
//        VertexArray va;
//        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
//        //Setting vertex array layout(attributes)
//        VertexBufferLayout layout;
//        layout.Push<float>(2);
//        layout.Push<float>(2);
//        va.AddBuffer(vb, layout);
//
//        //index buffer object
//        IndexBuffer ib(indices, 6);
//
//        //sam::mat4f ortho = sam::mat4f::Ortho(-2, 2, -2, 2, -10.0f, 10.0f);
//        sam::mat4f ortho = sam::mat4f::Ortho(0, 1500, 0, 500, -10.0f, 10.0f);
//        //sam::mat4f ortho = sam::mat4f::GetOrtho(-2.0f,2.0f,-1.5f,1.5f,-1.0,1.0);
//
//        Texture texture("res/textures/test.png");
//        texture.Bind();
//
//        Shader shader("res/shaders/Texture.shader");
//        shader.Bind();
//        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);   
//        shader.SetUniform1i("u_Texture", 0);//Value is texture slot
//
//        sam::mat4f move;
//        float translation = 0.0f;
//        
//        //Need to fix it so can do below
//        //shader.SetUniformMat4f("u_MVP", rotation * ortho);
//
//        Renderer renderer;
//
//        // Setup Dear ImGui context
//        IMGUI_CHECKVERSION();
//        ImGui::CreateContext();
//        ImGuiIO& io = ImGui::GetIO(); (void)io;
//        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//        // Setup Dear ImGui style
//        ImGui::StyleColorsDark();
//        //ImGui::StyleColorsLight();
//
//        // Setup Platform/Renderer backends
//        ImGui_ImplGlfw_InitForOpenGL(window, true);
//        ImGui_ImplOpenGL3_Init("#version 330");
//        io.Fonts->AddFontDefault();
//
//        float r = 0.0f;
//        float increment = 0.05f;
//        float img_co = 0;
//        float* img_col = &img_co;
//        // Our state
//        bool show_demo_window = true;
//        bool show_another_window = false;
//        /* Loop until the user closes the window */
//        while (!glfwWindowShouldClose(window))
//        {
//            /* Render here */
//            renderer.Clear();
//            ImGui_ImplOpenGL3_NewFrame();
//            ImGui_ImplGlfw_NewFrame();
//            ImGui::NewFrame();
//
//            move << 1, 0, 0, translation,
//                0, 1, 0, 0,
//                0, 0, 1, 0,
//                0, 0, 0, 1;
//
//            sam::mat4f result = ortho * move; //* move;
//
//
//            //Issuing a draw call
//            //nullptr because index buffer is bound
//            shader.Bind();
//            shader.SetUniform4f("u_Color", 0.3f, r, 0.8f, 1.0f); //img_col[0], img_col[1], img_col[2], img_col[3]);
//            shader.SetUniformMat4f("u_MVP", result);
//            renderer.Draw(va, ib, shader);
//            //GLDrawArrays because we dont have an index buffer
//            //glDrawArrays(GL_TRIANGLES, 0, 3);
//
//            if (r > 1.0f)
//                increment = -0.05f;
//            else if (r < 0.0f)
//                increment = 0.05f;
//            r += increment;
//
//            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//            {
//                static int counter = 0;
//
//                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//                ImGui::Checkbox("Another Window", &show_another_window);
//
//                ImGui::SliderFloat("float", &translation, -800.0f, 800.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//                ImGui::ColorEdit3("clear color", (float*)&img_col); // Edit 3 floats representing a color
//
//                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//                    counter++;
//                ImGui::SameLine();
//                ImGui::Text("counter = %d", counter);
//
//                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//                ImGui::End();
//            }
//
//            ImGui::Render();
//            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//            /* Swap front and back buffers */
//            glfwSwapBuffers(window);
//
//            /* Poll for and process events */
//            glfwPollEvents();
//        }
//    }
//    // Cleanup
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwTerminate();
//    return 0;
//}