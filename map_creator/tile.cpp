#include "tile.h"

Tile::Tile(int x, int y, int texID)
    : x(x), y(y), texID(texID)
{

}

void Tile::draw()
{

}

void Tile::setPos(int i, int j)
{
    x = j;
    y = i;
}

void Tile::setTexture(int newTex)
{
    texID = newTex;
}