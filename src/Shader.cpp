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
    glDeleteProgram(m_RendererID); GCE
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
    unsigned int id = glCreateShader(type); GCE
    //Below is just a pointer to null terminated immutable array
    //Same as doing source[0]
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); GCE
    glCompileShader(id); GCE

    //Error Handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); GCE
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); GCE
        //Same as doing message[length]. C++ cant allocate variable length to stack. 
        char* message = (char*)(_malloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message); GCE
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id); GCE
        return 0;
    }


    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program_id = glCreateProgram(); GCE
    unsigned int vs_id = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs_id = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program_id, vs_id); GCE
    glAttachShader(program_id, fs_id); GCE
    glLinkProgram(program_id); GCE
    glValidateProgram(program_id); GCE

    glDeleteShader(vs_id); GCE
    glDeleteShader(fs_id); GCE

    return program_id;
}


void Shader::Bind() const
{
    glUseProgram(m_RendererID); GCE
}

void Shader::Unbind() const
{
    glUseProgram(0); GCE
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3); GCE
}

int Shader::GetUniformLocation(const std::string& name)
{
    //If location has already been found, then return it!
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    //Otherwise find location
    unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()); GCE
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
 
    m_UniformLocationCache[name] = location;
    return location;
}
