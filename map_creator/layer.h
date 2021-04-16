#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>


class Layer
{
public:
    char blocks[3][3];

    Layer(std::string blocksID);
    void rotate();
};