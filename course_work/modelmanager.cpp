#include "modelmanager.h"

ModelManager::ModelManager(const char* path)
    : model(path)
{

}

ModelManager::~ModelManager()
{
    
}

void ModelManager::addTransform(glm::mat4 transform)
{
    transforms.push_back(transform);
}

void ModelManager::setupBuffer()
{
    glGenBuffers(1, &transformsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, transformsBuffer);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

    GLuint VAO = model.getVAO();
    glBindVertexArray(VAO);
    // vertex attributes
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void ModelManager::draw(Shader& shader)
{   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model.getTexture());
    shader.uniform("material.texture_diffuse1", 0);
    glBindVertexArray(model.getVAO());
    glDrawElementsInstanced(GL_TRIANGLES, model.getIndicesCount(), GL_UNSIGNED_INT, 0, transforms.size());
}