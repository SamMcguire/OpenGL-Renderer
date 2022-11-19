#include "Shader.h"

#include<iostream>
#include<fstream>
#include<sstream>

#include "Renderer.h"




Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
        char* message = (char*)(_malloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }


    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    //If location has already been found, then return it!
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    //Otherwise find location
    GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
 
    m_UniformLocationCache[name] = location;
    return location;
}
