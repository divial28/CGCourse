#pragma once

#include "Model.h"

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class ModelManager
{
    public:
        ModelManager(const char * path);
        ~ModelManager();
        void addTransform(glm::mat4 transform);
        void setupBuffer();
        void draw(Shader& shader);

    private:

        std::vector<glm::mat4> transforms;
        GLuint  transformsBuffer;
        Model   model;
};