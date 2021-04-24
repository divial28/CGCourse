#pragma once

#include "generator.h"
#include "modelmanager.h"

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <fstream>


class Scene
{
    public:
        Scene(const char* path);
        ~Scene();
        void draw(Shader& shader);

    private:
        std::map <char, ModelManager*> modelManagers;
        Generator * generator;
};