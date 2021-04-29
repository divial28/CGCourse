#include "modelmanager.h"
#include "generator.h"

ModelManager::ModelManager(const char* path)
    : model(path)
{
    log.open("../managerLog");
}

ModelManager::~ModelManager()
{
    log.close();
}

void ModelManager::addAttribs(Block& block)
{
    transforms.push_back(block.model);
    lights.push_back(block.frontLights);
    lights.push_back(block.backLights);
}

void ModelManager::setupBuffer()
{
    GLuint VAO = model.getVAO();
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &lightsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, lightsBuffer);
    glBufferData(GL_ARRAY_BUFFER, lights.size() * sizeof(glm::vec3), lights.data(), GL_STATIC_READ);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)*2, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)*2, (void*)(sizeof(glm::vec3)));
    
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);

    glGenBuffers(1, &transformsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, transformsBuffer);
    glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_READ);

    // vertex attributes
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glEnableVertexAttribArray(7); 
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(8); 
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);

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