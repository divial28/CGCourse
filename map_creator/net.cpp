#include "net.h"
#include "generator.h"

Net::Net()
{

}

Net::Net(int x, int y, int w, int h, int size, int spacer)
    : x(x), y(y), w(w), h(h), ts(size), s(spacer)
{
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            blocks.emplace_back(sf::Vector2f(size, size));
            blocks.back().setFillColor(sf::Color(0x0, 0x0, 0x0, 0x0));
            blocks.back().setOutlineColor(sf::Color::Black);
            blocks.back().setOutlineThickness(s);
            blocks.back().setPosition(sf::Vector2f(x + j*(size+s), y + i*(size+s) ));
        }
    }
}

void Net::draw(sf::RenderWindow * window)
{

    for(int i = 0; i < blocks.size(); i++)
    {
        window->draw(blocks[i]); 
    }
}

void Net::setBlock(int i, int j, sf::Color color)
{
    blocks[i*w + j].setFillColor(color);

}