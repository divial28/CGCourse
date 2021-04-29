#pragma once

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Block
{
    char id;
    glm::mat4 model;
    glm::vec3 frontLights;
    glm::vec3 backLights;

    Block();
    Block(char id, glm::mat4 model);
    //Block(Block & other);
};

typedef std::map<std::string, Block> RawScene;

class Generator
{
    struct Pos
    {
        int x,y,z;
    };

    enum FORBIDDEN_MOVEMENT
    {
        MNONE = 0,
        MTOP    = 1 << 1,
        MBOT    = 1 << 5,
        MRIGHT  = 1 << 4,
        MLEFT   = 1 << 2,
        MFRONT  = 1 << 6,
        MBACK   = 1 << 0
    };

    enum DIRECTION
    {
        DTOP    = -2,
        DBOT    = 2,
        DRIGHT  = 1,
        DLEFT   = -1,
        DFRONT  = 3,
        DBACK   = -3,
        DALL     = 0
    };

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

    RawScene& getRawScene() { return scene; }

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

    void setupTorchs(int x, int y, int z, bool horizontal);

    void setBlock(char id, int x, int y, int z, float rotation = 0);    //rotation is in degrees

    const std::string stringID(int i, int j, int k) const;

    void calculateLight(int x, int y, int z, int depth, int dir, int movement);

private:

    int w;  //ширина генерируемого поля
    int h;  //длина генерируемого поля
    int minSide;    //минимальная сторона прямоугольника

    RawScene scene;  //итоговая генерируемая сцена
    std::map<std::string, Rect> houses;
    std::vector<Pos> torchPos;


    std::ofstream log;
    std::ofstream log2;
};