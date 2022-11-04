#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include<string>
#include<sstream>


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    //Below is just a pointer to null terminated immutable array
    //Same as doing source[0]
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //Same as doing message[length]. C++ cant allocate variable length to stack. 
        char* message = (char*)(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }


    return id;
}

//Static so other files can't access this func
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program_id = glCreateProgram();
    unsigned int vs_id = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs_id = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);

    return program_id;
}


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

    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float triangle_positions[6] = {
        -0.5f, -0.5f,
        -0.0f, 0.5f,
        0.5f, -0.5f
    };
    //Defining vertex buffers
    unsigned int buffer_id; //could just name buffer
    glGenBuffers(1, &buffer_id); //1 is how many buffers. &(passes in pointer)
    //Selecting buffer. GL_ARRAY_BUFFER is purpose of buffer.
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), triangle_positions, GL_STATIC_DRAW);

    //Setting attributes
    //if 8 then do (const void*)8
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    ShaderProgramSource src = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(src.VertexSource, src.FragmentSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Issuing a draw call
        //GLDrawArrays because we dont have an index buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}