#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

class Generator;

class Net
{
public:
    Net();

    Net(int x, int y, int w, int h, int size, int spacer);

    void draw(sf::RenderWindow * window);

    void setBlock(int i, int j, sf::Color color);

private:
    int x;  //координаты на экране
    int y;
    int w;  //размер сетки в тайлах
    int h;
    int ts; //tile size
    int s;  //spacer

    std::vector<sf::RectangleShape> blocks;
    //Net * parent;
};