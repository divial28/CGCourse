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
    enum SIDES 
    {
        LEFT = 0,
        TOP,
        RIGHT,
        BOTTOM
    };

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

    void fillFloor(Rect rect, int yLayer, char fillBlock, char borderBlock);

    void fillPerimeter(Rect rect, int yLayer, char blockID);

    void fillCorners(Rect rect, int yLayer, char blockID);

    void copyPerimeter(Rect rect, int src, int dest); 

    void setupHouse(Rect rect);

    void setupDoor(Rect rect, bool* sides);

    void setupWindows(Rect rect, int stage);

    const std::string stringID(int i, int j, int k);

private:

    int w;  //ширина генерируемого поля
    int h;  //длина генерируемого поля
    int minSide;    //минимальная сторона прямоугольника

    std::map<std::string, char> scene;  //итоговая генерируемая сцена
    std::map<char, sf::Color> colors;   //карта цветов
    std::map<std::string, Rect> houses;


    std::ofstream log;
    std::ofstream log2;
};