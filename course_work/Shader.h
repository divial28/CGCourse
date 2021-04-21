#ifndef SHADER_H
#define SHADER_H

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Camera.h"

class Shader
{
public:
    Shader(Camera* cam, const GLchar* vertexPath, const GLchar* fragmentPath);
    
    ~Shader();

    void Use();

    GLuint getProgram() { return program; }

    void setViewTransformName(const char* name)
        { viewName = name; }

    void setProjectionTransformName(const char* name)
        { projectionName = name; }

    void uniform(const char* name, GLfloat value)
        { glUniform1f(glGetUniformLocation(program, name), value); }

    void uniform(const char* name, GLint textureIndex)
        { glUniform1i(glGetUniformLocation(program, name), textureIndex); }

    void uniform(const char* name, const glm::vec3& vector) 
        { glUniform3f(glGetUniformLocation(program, name), vector.x, vector.y, vector.z); }

    void uniform(const char* name, const glm::mat4& matrix, GLsizei count = 1, GLboolean transpose = GL_FALSE)
        { glUniformMatrix4fv(glGetUniformLocation(program, name), 1, transpose, glm::value_ptr(matrix)); }

private:
    const GLchar* const* readSource(const GLchar* sourcePath);

    void checkShader(GLuint shader, const GLchar* shaderType);

    void checkProgram(GLuint program);

private:
    GLuint          program;
    GLint           success;
    GLchar          infoLog[512];
    std::string     source;
    const GLchar    * sourcePtr;
    Camera          * camera;
    std::string     viewName;
    std::string     projectionName;
};

#endif