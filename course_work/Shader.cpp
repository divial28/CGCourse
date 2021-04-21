#include "Shader.h"

Shader::Shader(Camera* camera, const GLchar* vertexPath, const GLchar* fragmentPath)
    : camera(camera)
    , viewName("view")
    , projectionName("projection")
{
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, readSource(vertexPath), NULL);
    glCompileShader(vertexShader);
    checkShader(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, readSource(fragmentPath), NULL);
    glCompileShader(fragmentShader);
    checkShader(fragmentShader, "FRAGMENT");

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{

}

void Shader::Use()
{
    glUseProgram(program);
    uniform(viewName.c_str(), camera->View());
    uniform(projectionName.c_str(), camera->Projection());
}

const GLchar* const* Shader::readSource(const GLchar* sourcePath)
{
    std::ifstream in(sourcePath);
    if(!in)
    {
        std::cout << "cant open file: " << sourcePath << std::endl;
    }
    
    source.assign((std::istreambuf_iterator<char>(in)),
                 std::istreambuf_iterator<char>());
    sourcePtr = source.c_str();

    return &sourcePtr;
}

void Shader::checkShader(GLuint shader, const GLchar* shaderType)
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::checkProgram(GLuint program)
{
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}