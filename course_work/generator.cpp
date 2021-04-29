#include "generator.h"

Block::Block()
    : id('0'), model(1.0f), frontLights(0.0f), backLights(0.0f)
{

}

Block::Block(char id, glm::mat4 model)
    : id(id), model(model), frontLights(0.4f), backLights(0.4f)
{

} 

Generator::Generator(int w, int h, int minSide)
    :w(w), h(h), minSide(minSide), log("../log")
{
    srand(time(0));
    generateMap();
    
    for(int i = 0; i < torchPos.size(); i++)
    {
        calculateLight(torchPos[i].x, torchPos[i].y, torchPos[i].z, 0, DALL, MNONE);
    }

    std::cout << scene.size() << std::endl;
}

Generator::~Generator()
{
    log.close();
}

void Generator::generateMap()
{
    for(int i = 0; i < h; i++)
        for(int j = 0; j < w; j++)
            setBlock('g', j, 0, i);
    
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
            setBlock(fillBlock, j, yLayer, i);
    
    fillPerimeter(rect, yLayer, borderBlock);
}

void Generator::fillPerimeter(Rect rect, int yLayer, char blockID)
{
    for(int i = rect.x; i < rect.x + rect.w; i++)
    {
        setBlock(blockID, i, yLayer, rect.z);
        setBlock(blockID, i, yLayer, rect.z+rect.h-1);
    }
    for(int i = rect.z; i < rect.z + rect.h; i++)
    {
        setBlock(blockID, rect.x, yLayer, i);
        setBlock(blockID, rect.x+rect.w-1, yLayer, i);
    }
}

void Generator::fillCorners(Rect rect, int yLayer, char blockID)
{
    setBlock(blockID, rect.x, yLayer, rect.z);
    setBlock(blockID, rect.x + rect.w-1, yLayer, rect.z);
    setBlock(blockID, rect.x, yLayer, rect.z + rect.h-1);
    setBlock(blockID, rect.x + rect.w-1, yLayer, rect.z + rect.h-1);
}

void Generator::copyPerimeter(Rect rect, int src, int dest)
{
    for(int i = rect.x; i < rect.x + rect.w; i++)
    {
        setBlock(getBlock(i, src, rect.z), i, dest, rect.z);
        setBlock(getBlock(i, src, rect.z+rect.h-1), i, dest, rect.z+rect.h-1);
    }
    for(int i = rect.z; i < rect.z + rect.h; i++)
    {
        setBlock(getBlock(rect.x, src, i), rect.x, dest, i);
        setBlock(getBlock(rect.x+rect.w-1, src, i), rect.x+rect.w-1, dest, i);
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
    float rotation = 0;
    int side;

    while(!doorChosen)
    {
        side = rand()%4;
        if(sides[side])
        {
            if(side == TOP || side == BOTTOM)
            {
                doorX = rect.x + 2 + rand()%(rect.w-4);
                doorZ = (side == TOP) ? rect.z : rect.z + rect.h-1;

                //check windows
                int winSize = 0;
                while(getBlock(doorX-1-winSize++, 2, doorZ) == 'w');
                if(winSize > 3)
                    setBlock('#', doorX-1, 2, doorZ);
                else
                {
                    for(int i = 0; i < winSize; setBlock('#', doorX-1-i++, 2, doorZ));
                        setBlock('0', doorX-1-winSize, 2, doorZ+1);
                        setBlock('0', doorX-1-winSize, 2, doorZ-1);
                }
                winSize = 0;
                while(getBlock(doorX+1+winSize++, 2, doorZ) == 'w');
                if(winSize > 3)
                    setBlock('#', doorX+1, 2, doorZ);
                else
                {
                    for(int i = 0; i < winSize; setBlock('#', doorX+1+i++, 2, doorZ));
                    setBlock('0', doorX+1+winSize, 2, doorZ+1);
                    setBlock('0', doorX+1+winSize, 2, doorZ-1);
                }

                //roads
                int dir = (side == TOP) ? -1 : 1;
                int z = doorZ + dir;
                while(getBlock(doorX, 0, z) != 'c')
                {
                    setBlock('c', doorX, 0, z);
                    z += dir;
                }

                //torchs
                setBlock('0', doorX, 3, doorZ-1);
                setBlock('0', doorX, 3, doorZ+1);
                setupTorchs(doorX-1, 2, doorZ, true);
                setupTorchs(doorX+1, 2, doorZ, true);
            }
            else if(side == LEFT || side == RIGHT)
            {
                doorZ = rect.z + 2 + rand()%(rect.h-4);
                doorX = (side == LEFT) ? rect.x : rect.x + rect.w-1;

                //check windows
                int winSize = 0;
                while(getBlock(doorX, 2, doorZ-1-winSize++) == 'w');
                if(winSize > 3)
                    setBlock('#', doorX, 2, doorZ-1);
                else
                {
                    for(int i = 0; i < winSize; setBlock('#', doorX, 2, doorZ-1-i++));
                    setBlock('0', doorX-1, 3, doorZ-1-winSize);
                    setBlock('0', doorX+1, 3, doorZ-1-winSize);
                }
                winSize = 0;
                while(getBlock(doorX, 2, doorZ+1+winSize++) == 'w');
                if(winSize > 3)
                    setBlock('#', doorX, 2, doorZ+1);
                else
                {
                    for(int i = 0; i < winSize; setBlock('#', doorX, 2, doorZ+1+i++));
                    setBlock('0', doorX-1, 3, doorZ+1+winSize);
                    setBlock('0', doorX+1, 3, doorZ+1+winSize);
                }

                //roads
                int dir = (side == LEFT) ? -1 : 1;
                int x = doorX + dir;
                while(getBlock(x, 0, doorZ) != 'c')
                {
                    setBlock('c', x, 0, doorZ);
                    x += dir;
                }

                //torchs
                setBlock('0', doorX-1, 3, doorZ);
                setBlock('0', doorX+1, 3, doorZ);
                setupTorchs(doorX, 2, doorZ-1, false);
                setupTorchs(doorX, 2, doorZ+1, false);
            }
            doorChosen = true;
        }
    }
    copyPerimeter(rect, 2, 3);
    
    rotation = -90 + 90*side;
    glm::mat4 model(1.0f);
    if(side & 1)
        model = glm::translate(model, glm::vec3(doorX, 1, (side == 1) ? doorZ-0.5 : doorZ+0.5));
    else
        model = glm::translate(model, glm::vec3(side ? doorX+0.32 : doorX-0.32, 1, doorZ));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    
    scene[stringID(doorX, 1, doorZ)] = Block('d', model);

    setBlock('0', doorX, 2, doorZ);
    setBlock('#', doorX, 3, doorZ);
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

        setupTorchs(front-1, layer, rect.z, true);
        setupTorchs(back+1, layer, rect.z, true);
        setupTorchs(front-1, layer, rect.z+rect.h-1, true);
        setupTorchs(back+1, layer, rect.z+rect.h-1, true);

        for(int i = 0; i < winSize; i++)
        {   
            if(abs(front - back) < 2 && i >= 3  || back < front)
                break;

            setBlock('w', front, layer, rect.z);
            setBlock('w', back, layer, rect.z);
            setBlock('w', front, layer, rect.z+rect.h-1);
            setBlock('w', back, layer, rect.z+rect.h-1);
            front++;
            back--;
        }
        setupTorchs(front, layer, rect.z, true);
        setupTorchs(back, layer, rect.z, true);
        setupTorchs(front, layer, rect.z+rect.h-1, true);
        setupTorchs(back, layer, rect.z+rect.h-1, true);


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

        setupTorchs(rect.x, layer, front-1, false);
        setupTorchs(rect.x, layer, back+1, false);
        setupTorchs(rect.x+rect.w-1, layer, front-1, false);
        setupTorchs(rect.x+rect.w-1, layer, back+1, false);

        for(int i = 0; i < winSize; i++)
        {   
            if(abs(front - back) < 2 && i >= 3  || back < front)
                break;

            setBlock('w', rect.x, layer, front);
            setBlock('w', rect.x, layer, back);
            setBlock('w', rect.x + rect.w-1, layer, front);
            setBlock('w', rect.x + rect.w-1, layer, back);
            front++;
            back--;
        }
        setupTorchs(rect.x, layer, front, false);
        setupTorchs(rect.x, layer, back, false);
        setupTorchs(rect.x+rect.w-1, layer, front, false);
        setupTorchs(rect.x+rect.w-1, layer, back, false);

        int space =  1 + rand()%3;
        front += space;
        back -= space;
    }
}

void Generator::setupTorchs(int x, int y, int z, bool horizontal)
{
    if(getBlock(x, y, z) == '#')
    {
        if(horizontal)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(x, y+1, z-0.55));
            model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(1.0, 0.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            scene[stringID(x, y+1, z-1)] = Block('!', model);
            torchPos.push_back({x, y+1, z-1});

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x, y+1, z+0.55));
            model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            scene[stringID(x, y+1, z+1)] = Block('!', model);
            torchPos.push_back({x, y+1, z+1});
        }
        else
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(x-0.55, y+1, z));
            model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            scene[stringID(x-1, y+1, z)] = Block('!', model);
            torchPos.push_back({x-1, y+1, z});

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x+0.55, y+1, z));
            model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            scene[stringID(x+1, y+1, z)] = Block('!', model);
            torchPos.push_back({x+1, y+1, z});
        }
    }
}

void Generator::calculateLight(int x, int y, int z, int depth, int dir, int forbidden_mov)
{   
    float factor = 1 - depth*0.1;

    if(dir == DALL)
    {
        scene[stringID(x, y, z)].backLights = glm::vec3(factor);
        scene[stringID(x, y, z)].frontLights = glm::vec3(factor);
    }
    else if(scene.find(stringID(x, y, z)) != scene.end())
    {
        if(dir > 0 && scene[stringID(x, y, z)].frontLights[dir-1] < factor)
        {
            scene[stringID(x, y, z)].frontLights[dir-1] = factor;
            return;
        }
        else if(dir < 0 && scene[stringID(x, y, z)].backLights[-dir-1] < factor)
        {
            scene[stringID(x, y, z)].backLights[-dir-1] = factor;
            return;
        }      
    }
    
    if(depth > 5)    
        return;

    if(-dir != DLEFT) calculateLight(x-1, y, z, depth+1, DLEFT, 0);
    if(-dir != DRIGHT) calculateLight(x+1, y, z, depth+1, DRIGHT, 0);
    if(-dir != DBOT) calculateLight(x, y-1, z, depth+1, DBOT, 0);
    if(-dir != DTOP) calculateLight(x, y+1, z, depth+1, DTOP, 0);
    if(-dir != DBACK) calculateLight(x, y, z-1, depth+1, DBACK, 0);
    if(-dir != DFRONT) calculateLight(x, y, z+1, depth+1, DFRONT, 0);
}

void Generator::setBlock(char id, int x, int y, int z, float rotation)
{   
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(x, y, z));
    if(rotation)
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    
    scene[stringID(x, y, z)] = Block(id, model);
}

const char Generator::getBlock(int x, int y, int z) const
{
    auto block = scene.find(stringID(x, y, z));
    
    if(block != scene.end())
        return block->second.id;
    else
        return '0';
}

const std::string Generator::stringID(int x, int y, int z) const
{
    return (std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z));
}

void Generator::getCoords(const std::string& key, int* x, int* y, int* z)
{
    int p1 = key.find_first_of(',');
    int p2 = key.find_last_of(',');

    *x = std::atoi(key.substr(0, p1).c_str());
    *y = std::atoi(key.substr(p1+1, p2-p1-1).c_str());
    *z = std::atoi(key.substr(p2+1, key.length()-p1).c_str());
}