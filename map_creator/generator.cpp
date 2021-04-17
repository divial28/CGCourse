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
            scene[stringID(i, j, 0)] = 'g';
    
    doBSP({0, 0, w, h}, true);
}

void Generator::doBSP(Rect rect, bool horizontal)
{
    //horizontal = rand() % 2;
    if(horizontal && rect.w > 4*minSide)
    {
        int division = minSide + (rand() % (rect.w - 2*minSide));
        doBSP({rect.x, rect.z, division, rect.h}, !horizontal);
        doBSP({rect.x + division, rect.z, rect.w - division, rect.h}, !horizontal);
    }
    else if(!horizontal && rect.h > 4*minSide)
    {
        int division = minSide + (rand() % (rect.h - 2*minSide));
        doBSP({rect.x, rect.z, rect.w, division}, !horizontal);
        doBSP({rect.x, rect.z + division, rect.w, rect.h - division}, !horizontal);
    }
    else
        placeRoads(rect);
}

void Generator::placeRoads(Rect rect)
{
    for(int i = rect.x; i < rect.x + rect.w; i++)
    {
        scene[stringID(i, rect.z, 0)] = '#';
        scene[stringID(i, rect.z+rect.h-1, 0)] = '#';
    }
    for(int i = rect.z; i < rect.z + rect.h; i++)
    {
        scene[stringID(rect.x, i, 0)] = '#';
        scene[stringID(rect.x+rect.w-1, i, 0)] = '#';
    }
}

const sf::Color& Generator::getBlock(int i, int j, int k)
{
    auto block = scene.find(stringID(i, j, k));
    
    if(block != scene.end())
    {
        return colors.at(block->second);
    }
    else
        return colors.at('0');
}

const std::string Generator::stringID(int i, int j, int k)
{
    return (std::to_string(j) + "," + std::to_string(k) + "," + std::to_string(i));
}