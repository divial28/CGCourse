#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "net.h"
#include "generator.h"

//TODO: add BSP
//TODO: add add roads
//TODO: add doors
//TODO: add higher stages

int main()
{
    sf::RenderWindow window(sf::VideoMode(1800, 1000), "Map Creator", sf::Style::Default);

    int s = 10;     //spacer
    int ls = 1;     //layer spacer
    int ts = 3;    //tile size
    int w = 200;
    int h = 200;
    int minSide = 10;

    Generator generator(w, h, minSide);

    std::vector<Net*> nets;

    for(int i = 0; i < 1; i++)
    {
        for(int j = 0; j < 1; j++)
        {
            nets.push_back(new Net(s*(j+1) + w*ts*j + w*ls*j, s*(i+1) + h*ts*i + h*ls*i, w, h, ts, ls));
        }
    }

    int blockCount = w*h;
    for(int k = 0; k < nets.size(); k++)
    {
        for(int i = 0; i < h; i++)
            for(int j = 0; j < w; j++)
                nets[k]->setBlock(i, j, generator.getBlock(i, j, k));
    }

    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            
        }
        window.clear(sf::Color(0x8a, 0x8a, 0x8a));

        for(int i = 0; i < nets.size(); i++)
            nets[i]->draw(&window);

        //swap buffers
        window.display();
    }

    return 0;
}