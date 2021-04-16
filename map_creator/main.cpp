#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "net.h"
#include "tile.h"
#include "generator.h"
#include "bar.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1800, 1000), "Map Creator", sf::Style::Default);

    int s = 10;     //spacer
    int ls = 1;     //layer spacer
    int ts = 5;    //tile size
    int w = 45;
    int h = 45;

    Generator generator;

    Bar bar(1700, 50, 50);

    std::vector<Net*> vLayers;
    std::vector<Tile> vTiles;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            vLayers.push_back(new Net(s*(j+1) + w*ts*j + w*ls*j, s*(i+1) + h*ts*i + h*ls*i, w, h, ts, ls));
        }
    }

    

    std::vector<std::string> layers = {"N", "G", "F", "CF", "SF", "#", "D", "C#", "W"
                                      , "RL", "RM", "RH", "CRL", "CRM", "RSC", "RC"};

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
                    vLayers[i]->handleMouseEvent(&event);
            }
            
            if(event.type == sf::Event::KeyPressed)
            {  
                key = event.key.code-27;
                if(key >= 0 && key < layers.size())
                    for(int i = 0; i < layers.size(); i++)
                        vLayers[i]->setCurrentLayer(layers[key]);
            }
        }
        window.clear(sf::Color(0x8a, 0x8a, 0x8a));

        Net* parent = 0;
        for(int i = 0; i < vLayers.size(); i++)
        {
            vLayers[i]->draw(&window, parent);
            parent = vLayers[i];
        }
        
        bar.draw(&window);

        //swap buffers
        window.display();
    }

    return 0;
}