#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "net.h"
#include "layer.h"
#include "tile.h"

class Generator
{
private:
    void parseColors();
    void parseLayers();
    void parseTiles();

public:
    
    static std::map<std::string, Layer*> layers;
    static std::map<char, sf::Color> colors;
    
    Generator();

};