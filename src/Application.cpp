#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include<string>
#include<sstream>

#define ASSERT(x) if (!(x)) __debugbreak(); 
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    //Willl loop until glGetError() is 0
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "):"
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

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
    GLCall(unsigned int id = glCreateShader(type));
    //Below is just a pointer to null terminated immutable array
    //Same as doing source[0]
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    
    //Error Handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //Same as doing message[length]. C++ cant allocate variable length to stack. 
        char* message = (char*)(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }


    return id;
}

//Static so other files can't access this func
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program_id = glCreateProgram());
    unsigned int vs_id = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs_id = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program_id, vs_id));
    GLCall(glAttachShader(program_id, fs_id));
    GLCall(glLinkProgram(program_id));
    GLCall(glValidateProgram(program_id));

    GLCall(glDeleteShader(vs_id));
    GLCall(glDeleteShader(fs_id));

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

    glfwSwapInterval(1); //Matches frame rate to refresh rate 

    if (glewInit() != GLEW_OK)
        return -1;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float triangle_positions[8] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned int vao_id;//Vertex array object
    GLCall(glGenVertexArrays(1, &vao_id));
    GLCall(glBindVertexArray(vao_id));

    //Defining vertex buffers
    unsigned int buffer_id; //could just name buffer
    GLCall(glGenBuffers(1, &buffer_id)); //1 is how many buffers. &(passes in pointer)
    //Selecting buffer. GL_ARRAY_BUFFER is purpose of buffer.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), triangle_positions, GL_STATIC_DRAW));

    //Setting attributes
    //if 8 then do (const void*)8
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    GLCall(glEnableVertexAttribArray(0));

    //index buffer object
    unsigned int ibo_id; //could just name buffer
    GLCall(glGenBuffers(1, &ibo_id)); //1 is how many buffers. &(passes in pointer)
    //Selecting buffer. GL_ARRAY_BUFFER is purpose of buffer.
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource src = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(src.VertexSource, src.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Colour"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        //Issuing a draw call
        //nullptr because index buffer is bound
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        GLCall(glBindVertexArray(vao_id));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
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

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}