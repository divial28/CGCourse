#pragma once

#include "Model.h"

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <fstream>

struct Block;

class ModelManager
{
    public:
        ModelManager(const char * path);
        ~ModelManager();
        void addAttribs(Block& block);
        void setupBuffer();
        void draw(Shader& shader);

    private:

        std::vector<glm::mat4> transforms;
        std::vector<glm::vec3> lights;
        GLuint  transformsBuffer;
        GLuint  lightsBuffer;
        Model   model;

        std::ofstream log;
};