#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "tile.h"
#include "layer.h"

class Generator;

class Net
{
public:
    Net();

    Net(int x, int y, int w, int h, int size, int spacer);

    void draw(sf::RenderWindow * window, Net * parent);
    void moveTiles(sf::Vector2f& shift);

    void handleMouseEvent(sf::Event * ev);

    bool isBound(int x, int y);

    void setCurrentLayer(std::string layerID);

    void setLayer(int i, int j);

    sf::Vector2f getPos();

private:
    int x;  //координаты на экране
    int y;
    int w;  //размер сетки в тайлах
    int h;
    int wl; //размер сетки в слоях
    int hl;
    int ls; //ls
    int ts; //tile size
    int s;  //spacer

    int focus;

    std::string currentLayer;

    sf::RectangleShape focusShape;

    std::vector<sf::RectangleShape> blocks;
    std::vector<Layer*> layers;

    //Net * parent;
};