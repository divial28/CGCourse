#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

typedef std::map<std::string, char> Scene;

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

    const char getBlock(int i, int j, int k) const;

    Scene& getScene() { return scene; }

    static void getCoords(const std::string& key, int* x, int* y, int* z);

private:
    void generateMap();

    void doBSP(Rect rect, bool horizontal);

    void fillFloor(Rect rect, int yLayer, char fillBlock, char borderBlock);

    void fillPerimeter(Rect rect, int yLayer, char blockID);

    void fillCorners(Rect rect, int yLayer, char blockID);

    void copyPerimeter(Rect rect, int src, int dest); 

    void setupHouse(Rect rect);

    void setupDoor(Rect rect, bool* sides);

    void setupWindows(Rect rect, int stage);

    const std::string stringID(int i, int j, int k) const;

private:

    int w;  //ширина генерируемого поля
    int h;  //длина генерируемого поля
    int minSide;    //минимальная сторона прямоугольника

    Scene scene;  //итоговая генерируемая сцена
    std::map<std::string, Rect> houses;


    std::ofstream log;
    std::ofstream log2;
};