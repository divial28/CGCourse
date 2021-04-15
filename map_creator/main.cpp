#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "layer.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "Map Creator", sf::Style::Default);

    int s = 15;     //spacer
    int ls = 2;     //layer spacer
    int ts = 20;    //tile size
    int w = 20;
    int h = 20;

    std::vector<Layer> vLayers;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            vLayers.emplace_back(s*(j+1) + w*ts*j + w*ls*j, s*(i+1) + h*ts*i + h*ls*i, w, h, ts, ls);
        }
    }

    std::vector<sf::Color> vColors = {sf::Color(0x22, 0xa8, 0x03)       //земля с травой
                                     ,sf::Color(0x52, 0x52, 0x52)       //камень
                                     ,sf::Color(0xe8, 0xe1, 0x56)       //доски
                                     ,sf::Color(0x7d, 0x40, 0x2a)       //бревна
                                     ,sf::Color(0xcc, 0x81, 0x00)       //дверь
                                     ,sf::Color(0x4d, 0xf9, 0xff)       //стекло
                                     ,sf::Color(0xd1, 0xca, 0x00)};     //лестница

    int key = 0;

    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            
            if(event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseMoved)
            {
                for(int i = 0; i < vLayers.size(); i++)
                    vLayers[i].handleMouseEvent(&event);
            }
            
            if(event.type == sf::Event::KeyPressed)
            {  
                key = event.key.code-27;
                if(key >= 0 && key < vColors.size())
                    for(int i = 0; i < vLayers.size(); i++)
                        vLayers[i].setCurrentColor(vColors[key]);
            }
        }
        window.clear(sf::Color(0x8a, 0x8a, 0x8a));

        for(int i = 0; i < vLayers.size(); i++)
        {
            vLayers[i].draw(&window);
        }

        //swap buffers
        window.display();
    }

    return 0;
}