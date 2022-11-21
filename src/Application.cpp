#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"  
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Coolest Tab", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;  
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); //Matches frame rate to refresh rate 

    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //pos_x, pos_y, tex_x, tex_y
        float positions[16] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GCE;
        glEnable(GL_BLEND); GCE;

        //Defining vertex buffers
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        //Setting vertex array layout(attributes)
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        //index buffer object
        IndexBuffer ib(indices, 6);


        Shader shader("res/shaders/Texture.shader");
        shader.Bind();
        shader.SetUniform4f("u_Colour", 0.2f, 0.3f, 0.8f, 1.0f);
        std::cout << "test";
        Texture texture("res/textures/puppy.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);//Value is texture slot

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            //Issuing a draw call
            //nullptr because index buffer is bound
            shader.Bind();
            shader.SetUniform4f("u_Colour", r, 0.3f, 0.8f, 1.0f);
            renderer.Draw(va, ib, shader);
            //GLDrawArrays because we dont have an index buffer
            //glDrawArrays(GL_TRIANGLES, 0, 3);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}