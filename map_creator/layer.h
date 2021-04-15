#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "tile.h"

class Layer
{
public:
    Layer();

    Layer(int x, int y, int w, int h, int size, int spacer);

    void draw(sf::RenderWindow * window);

    void handleMouseEvent(sf::Event * ev);

    bool isBound(int x, int y);

    void setCurrentColor(sf::Color& color);

private:
    int x;  //координаты на экране
    int y;
    int w;  //размер сетки в тайлах
    int h;
    int ts; //tile size
    int s;  //spacer

    int focus;

    sf::Color   currentColor;

    sf::RectangleShape focusShape;

    std::vector<sf::RectangleShape> vTiles;
};