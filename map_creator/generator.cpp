#include "generator.h"

Generator::Generator(int w, int h, int minSide)
    :w(w), h(h), minSide(minSide)
{
    log.open("log");
    log2.open("log2");
    srand(time(0));
    parseColors();
    generateMap();
}

Generator::~Generator()
{
    log.close();
    log2.close();
}

void Generator::parseColors()
{
    std::ifstream in("../colors");
    if(!in)
    {
        std::cout << "cant open colors file" << std::endl;
        return;
    }
    std::string s;
    while(std::getline(in, s))
    {
        sf::Color color;
        char id = s.at(0);

        std::getline(in, s);
        std::stringstream ss(s);
        ss >> s; color.r = std::stoi(s);
        ss >> s; color.g = std::stoi(s);
        ss >> s; color.b = std::stoi(s);
        ss >> s; color.a = std::stoi(s);

        colors[id] = color;
    }
}

void Generator::generateMap()
{
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            scene[stringID(j, 0, i)] = 'g';
    
    doBSP({0, 0, w, h}, true);
}

void Generator::doBSP(Rect rect, bool horizontal)
{
    //horizontal = rand() % 2;

    bool isTerminal = true;
    if(horizontal && rect.w > 3*minSide)
    {
        isTerminal = false;
        int division = minSide + (rand() % (rect.w - 2*minSide));
        doBSP({rect.x, rect.z, division, rect.h}, !horizontal);
        doBSP({rect.x + division, rect.z, rect.w - division, rect.h}, !horizontal);
    }

    if(!horizontal && rect.h > 3*minSide)
    {
        isTerminal = false;
        int division = minSide + (rand() % (rect.h - 2*minSide));
        doBSP({rect.x, rect.z, rect.w, division}, !horizontal);
        doBSP({rect.x, rect.z + division, rect.w, rect.h - division}, !horizontal);
    }

    if(isTerminal)
    {
        //setup roads
        fillPerimeter(rect, 0, 'c');

        setupHouse(rect);
    }
        
}

void Generator::fillFloor(Rect rect, int yLayer, char fillBlock, char borderBlock)
{
    for(int i = rect.z; i < rect.z + rect.h; i++)
        for(int j = rect.x; j < rect.x + rect.w; j++)
            scene[stringID(j, yLayer, i)] = fillBlock;
    
    fillPerimeter(rect, yLayer, borderBlock);
}

void Generator::fillPerimeter(Rect rect, int yLayer, char blockID)
{
    for(int i = rect.x; i < rect.x + rect.w; i++)
    {
        scene[stringID(i, yLayer, rect.z)] = blockID;
        scene[stringID(i, yLayer, rect.z+rect.h-1)] = blockID;
    }
    for(int i = rect.z; i < rect.z + rect.h; i++)
    {
        scene[stringID(rect.x, yLayer, i)] = blockID;
        scene[stringID(rect.x+rect.w-1, yLayer, i)] = blockID;
    }
}

void Generator::fillCorners(Rect rect, int yLayer, char blockID)
{
    scene[stringID(rect.x, yLayer, rect.z)] = blockID;
    scene[stringID(rect.x + rect.w-1, yLayer, rect.z)] = blockID;
    scene[stringID(rect.x, yLayer, rect.z + rect.h-1)] = blockID;
    scene[stringID(rect.x + rect.w-1, yLayer, rect.z + rect.h-1)] = blockID;
}

void Generator::copyPerimeter(Rect rect, int src, int dest)
{
    for(int i = rect.x; i < rect.x + rect.w; i++)
    {
        scene[stringID(i, dest, rect.z)] = scene.at(stringID(i, src, rect.z));
        scene[stringID(i, dest, rect.z+rect.h-1)] = scene.at(stringID(i, src, rect.z+rect.h-1));
    }
    for(int i = rect.z; i < rect.z + rect.h; i++)
    {
        scene[stringID(rect.x, dest, i)] = scene.at(stringID(rect.x, src, i));
        scene[stringID(rect.x+rect.w-1, dest, i)] = scene.at(stringID(rect.x+rect.w-1, src, i));
    }
}

void Generator::setupHouse(Rect rect)
{
    //set valid sides
    bool sides[4] = {true, true, true, true};
    if(rect.x == 0) sides[LEFT] = false;
    if(rect.x + rect.w == w) sides[RIGHT] = false;
    if(rect.z == 0) sides[TOP] = false;
    if(rect.z + rect.h == h) sides[BOTTOM] = false;

    //shrink house
    rect.x+=2; rect.w-=4;
    rect.z+=2; rect.h-=4;
    while(rand()%10 < 7 && rect.w > minSide)
    {
        rect.x++; rect.w-=2;
    }    
    while(rand()%10 < 7 && rect.h > minSide)
    {
        rect.z++; rect.h-=2;
    }

    int stages = 1 + rand() % 5;
    for (int i = 0; i < stages; i++)
    {
        fillFloor(rect, i*5, '#', '@');
        fillPerimeter(rect, i*5 + 1, '#');
        fillCorners(rect, i*5 + 1, '@');
        copyPerimeter(rect, i*5 + 1, i*5 + 2);
        setupWindows(rect, i*5 + 2);
        copyPerimeter(rect, i*5 + 2, i*5 + 3);
        copyPerimeter(rect, i*5 + 1, i*5 + 4);
    }
    fillFloor(rect, stages*5, '#', '@');
    setupDoor(rect, sides);
    
}

void Generator::setupDoor(Rect rect, bool* sides)
{
    bool doorChosen = false;
    int doorX;
    int doorZ;
    while(!doorChosen)
    {
        int side = rand()%4;
        if(sides[side])
        {
            if(side == TOP || side == BOTTOM)
            {
                doorX = rect.x + 2 + rand()%(rect.w-4);
                doorZ = (side == TOP) ? rect.z : rect.z + rect.h-1;
                
                //check windows
                int winSize = 0;
                while(scene[stringID(doorX-1-winSize++, 2, doorZ)] == 'w');
                if(winSize > 3)
                    scene[stringID(doorX-1, 2, doorZ)] = '#';
                else
                    for(int i = 0; i < winSize; scene[stringID(doorX-1-i++, 2, doorZ)] = '#');
                winSize = 0;
                while(scene[stringID(doorX+1+winSize++, 2, doorZ)] == 'w');
                if(winSize > 3)
                    scene[stringID(doorX+1, 2, doorZ)] = '#';
                else
                    for(int i = 0; i < winSize; scene[stringID(doorX+1+i++, 2, doorZ)] = '#');

                int dir = (side == TOP) ? -1 : 1;
                int z = doorZ + dir;
                while(scene[stringID(doorX, 0, z)] != 'c')
                {
                    scene[stringID(doorX, 0, z)] = 'c';
                    z += dir;
                }
            }
            else if(side == LEFT || side == RIGHT)
            {
                doorZ = rect.z + 2 + rand()%(rect.h-4);
                doorX = (side == LEFT) ? rect.x : rect.x + rect.w-1;

                //check windows
                int winSize = 0;
                while(scene[stringID(doorX, 2, doorZ-1-winSize++)] == 'w');
                if(winSize > 3)
                    scene[stringID(doorX, 2, doorZ-1)] = '#';
                else
                    for(int i = 0; i < winSize; scene[stringID(doorX, 2, doorZ-1-i++)] = '#');
                winSize = 0;
                while(scene[stringID(doorX, 2, doorZ+1+winSize++)] == 'w');
                if(winSize > 3)
                    scene[stringID(doorX, 2, doorZ+1)] = '#';
                else
                    for(int i = 0; i < winSize; scene[stringID(doorX, 2, doorZ+1+i++)] = '#');

                int dir = (side == LEFT) ? -1 : 1;
                int x = doorX + dir;
                while(scene[stringID(x, 0, doorZ)] != 'c')
                {
                    scene[stringID(x, 0, doorZ)] = 'c';
                    x += dir;
                }
            }
            doorChosen = true;
        }
    }
    copyPerimeter(rect, 2, 3);
    scene[stringID(doorX, 1, doorZ)] = 'd';
    scene[stringID(doorX, 2, doorZ)] = '0';
    scene[stringID(doorX, 3, doorZ)] = '#';
}

void Generator::setupWindows(Rect rect, int layer)
{
    int front = rect.x + 2;
    int back = rect.x + rect.w-3;
    while(true)
    {
        int winSize = 3 + rand() % 3;
        while(winSize >= 3)
        {
            if(back - front < winSize/2)
                winSize--;
            else
                break;
        }

        if(winSize < 3)
            break;

        for(int i = 0; i < winSize; i++)
        {   
            if(abs(front - back) < 2 && i >= 3  || back < front)
                break;

            scene[stringID(front, layer, rect.z)] = 'w';
            scene[stringID(back, layer, rect.z)] = 'w';
            scene[stringID(front, layer, rect.z+rect.h-1)] = 'w';
            scene[stringID(back, layer, rect.z+rect.h-1)] = 'w';
            front++;
            back--;
        }
        int space =  1 + rand()%2;
        front += space;
        back -= space;
    }

    front = rect.z + 2;
    back = rect.z + rect.h-3;
    while(true)
    {
        int winSize = 3 + rand() % 3;
        while(winSize >= 3)
        {
            if(back - front < winSize/2)
                winSize--;
            else
                break;
        }

        if(winSize < 3)
            break;

        for(int i = 0; i < winSize; i++)
        {   
            if(abs(front - back) < 2 && i >= 3  || back < front)
                break;

            scene[stringID(rect.x, layer, front)] = 'w';
            scene[stringID(rect.x, layer, back)] = 'w';
            scene[stringID(rect.x + rect.w-1, layer, front)] = 'w';
            scene[stringID(rect.x + rect.w-1, layer, back)] = 'w';
            front++;
            back--;
        }
        int space =  1 + rand()%3;
        front += space;
        back -= space;
    }
}

const sf::Color& Generator::getBlock(int x, int y, int z)
{
    auto block = scene.find(stringID(x, y, z));
    
    if(block != scene.end())
    {
        return colors.at(block->second);
    }
    else
        return colors.at('0');
}

const std::string Generator::stringID(int x, int y, int z)
{
    return (std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z));
}