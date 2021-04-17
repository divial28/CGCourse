#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "net.h"

class Generator
{
    struct Rect
    {
        int x;
        int z;
        int w;
        int h;
    };

public:
    Generator(int w, int h, int minSide);
    ~Generator();

    const sf::Color& getBlock(int i, int j, int k);

private:
    void parseColors();

    void generateMap();

    void doBSP(Rect rect, bool horizontal);

    void placeRoads(Rect rect)
;
    const std::string stringID(int i, int j, int k);

private:

    int w;  //ширина генерируемого поля
    int h;  //длина генерируемого поля
    int minSide;    //минимальная сторона прямоугольника

    std::map<std::string, char> scene;  //итоговая генерируемая сцена
    std::map<char, sf::Color> colors;   //карта цветов

    

    std::ofstream log;
    std::ofstream log2;
};