#include "bar.h"
#include "generator.h"

Button::Button(int x, int y, int size)
    : x(x), y(y), size(size)
{

}

bool Button::isBound(int mx, int my)
{
    return (mx > x && mx < x + size && my > y && my < y + size);
}

void Button::draw(sf::RenderWindow* window)
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            window->draw(blocks[i][j]);
}

Bar::Bar(int x, int y, int bSize, bool horizontal)
    : x(x), y(y), bSize(bSize)
{
    size = Generator::layers.size();
    spacer = 10;
    
    int i = 0;
    for(auto layer : Generator::layers)
    {
        if(horizontal)
            buttons.emplace_back(x + i*(bSize+spacer), y);
        else
            buttons.emplace_back(x, y + i*(bSize+spacer));
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                char id = layer.second->blocks[j][k];
                sf::Color color = Generator::colors[id];
                buttons.back().blocks[j][k].setFillColor(color);
                buttons.back().blocks[j][k].setOutlineThickness(2);
                buttons.back().blocks[j][k].setOutlineColor(sf::Color::Black);
                int a = bSize/3;
                buttons.back().blocks[j][k].setSize(sf::Vector2f(a, a));
                buttons.back().blocks[j][k].setPosition(sf::Vector2f(x + k*a, y + j*a));
            }
        }
        i++;
    }
    focusShape.setSize(sf::Vector2f(bSize, bSize));
    focusShape.setOutlineColor(sf::Color::Red);
    focusShape.setOutlineThickness(3);
    focus = -1;
}

std::string handle(sf::Event* ev)
{
    return "G";
}

void Bar::draw(sf::RenderWindow* window)
{
    for(int i = 0; i < size; i++)
    {
        buttons[i].draw(window);
    }

    if(focus >= 0)
        window->draw(focusShape);
}