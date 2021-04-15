#pragma once

#include <SFML/Graphics.hpp>

class Tile
{
public:
    Tile(int x = 0, int y = 0, int texID = 0);

    void draw();
    
    void setPos(int i, int j);
    
    void setTexture(int newTex);

private:
    int x;  //логические координаты на слое
    int y;
    //int size;   //размер в пикселях на экране
    //int z; further implementation
    int texID;
};